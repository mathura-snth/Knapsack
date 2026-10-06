#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>

#define MAX_N      1000
#define MAX_LINE   16384
#define BT_MAX_N   30     /* au-delà, les backtracking sont ignorés (temps 0) */

typedef struct {
    int weight;
    int value;
} Item;

static inline int max(int a, int b) { return a > b ? a : b; }
static inline int min(int a, int b) { return a < b ? a : b; }

/* ------------------------------------------------------------------ */
/* Backtracking 1 : AVEC variables globales                           */
/* ------------------------------------------------------------------ */
static Item *g_items;
static int g_n, g_bestValue;

static void knapsackBT1Rec(int idx, int W, int currentValue) {
    if (idx == g_n || W == 0) {
        if (currentValue > g_bestValue)
            g_bestValue = currentValue;
        return;
    }
    // Ne pas inclure l'objet idx
    knapsackBT1Rec(idx + 1, W, currentValue);
    // Inclure l'objet idx si le poids le permet
    if (g_items[idx].weight <= W)
        knapsackBT1Rec(idx + 1, W - g_items[idx].weight,
                       currentValue + g_items[idx].value);
}

int knapsackBT1(Item items[], int n, int W) {
    g_items = items;
    g_n = n;
    g_bestValue = 0;
    knapsackBT1Rec(0, W, 0);
    return g_bestValue;
}

/* ------------------------------------------------------------------ */
/* Backtracking 2 : SANS variables globales                           */
/* ------------------------------------------------------------------ */
int knapsackBTUtil(Item items[], int n, int W, int idx, int currentValue,
                   int *bestValue) {
    if (idx == n || W == 0) {
        if (currentValue > *bestValue)
            *bestValue = currentValue;
        return *bestValue;
    }
    // Ne pas inclure l'objet idx
    knapsackBTUtil(items, n, W, idx + 1, currentValue, bestValue);
    // Inclure l'objet idx si le poids le permet
    if (items[idx].weight <= W) {
        knapsackBTUtil(items, n, W - items[idx].weight, idx + 1,
                       currentValue + items[idx].value, bestValue);
    }
    return *bestValue;
}

int knapsackBT2(Item items[], int n, int W) {
    int bestValue = 0;
    return knapsackBTUtil(items, n, W, 0, 0, &bestValue);
}

/* ------------------------------------------------------------------ */
/* Question 4 : Backtracking 2 avec borne supérieure précalculée      */
/* ------------------------------------------------------------------ */
// Algorithme de backtracking sans variables globales AVEC BORNE SUP
int knapsackBTUtilbs(Item items[], int n, int W, int idx, int currentValue,
                     int *bestValue, int *bs) {
    if (idx == n || W == 0) {
        if (currentValue > *bestValue)
            *bestValue = currentValue;
        return *bestValue;
    }
    if (bs[idx] + currentValue < *bestValue)
        return *bestValue;

    // Ne pas inclure l'objet idx
    knapsackBTUtilbs(items, n, W, idx + 1, currentValue, bestValue, bs);

    // Inclure l'objet idx si le poids le permet
    if (items[idx].weight <= W) {
        knapsackBTUtilbs(items, n, W - items[idx].weight, idx + 1,
                         currentValue + items[idx].value, bestValue, bs);
    }
    return *bestValue;
}

int knapsackBT2bs(Item items[], int n, int W) {
    int bestValue = 0;
    int i;
    int bs[n];
    bs[n - 1] = items[n - 1].value;   // bs quand il ne reste que le dernier élément
    // précalcul
    for (i = n - 2; i >= 0; i--) {
        bs[i] = bs[i + 1] + items[i].value;
    }
    /* Autre manière :
    for (i = 2; i < n + 1; i++) {
        bs[n - i] = bs[n - i + 1] + items[n - i].value;
    }
    */
    return knapsackBTUtilbs(items, n, W, 0, 0, &bestValue, bs);
}

/* ------------------------------------------------------------------ */
/* Programmation dynamique classique (basée sur le poids)             */
/* ------------------------------------------------------------------ */
int knapsackDP(Item items[], int n, int W) {
    int dp[W + 1];
    for (int w = 0; w <= W; w++)
        dp[w] = 0;
    for (int i = 0; i < n; i++) {
        for (int w = W; w >= items[i].weight; w--) {
            dp[w] = max(dp[w], dp[w - items[i].weight] + items[i].value);
        }
    }
    return dp[W];
}

/* ------------------------------------------------------------------ */
/* Question 6 : Programmation dynamique basée sur la valeur           */
/* dp[v] = poids minimal nécessaire pour atteindre la valeur v        */
/* ------------------------------------------------------------------ */
int knapsackDP_Value(Item items[], int n, int W) {
    int V = 0;
    for (int j = 0; j < n; j++) {
        V += items[j].value;
    }
    int dp[V + 1];
    int k = V;
    while (k > 0) {
        dp[k] = INT_MAX;
        k--;
    }
    dp[0] = 0;
    for (int i = 0; i < n; i++) {
        for (int v = V; v >= items[i].value; v--) {
            if (dp[v - items[i].value] != INT_MAX)
                dp[v] = min(dp[v], dp[v - items[i].value] + items[i].weight);
        }
    }
    for (int i = V; i > 0; i--) {
        if (dp[i] <= W) return i;
    }
    return 0;
}

/* ------------------------------------------------------------------ */
/* Question 7 : sac à dos avec poids minimum M et capacité C          */
/* dp[i][w] = valeur max avec les i premiers objets, poids EXACT w    */
/* (0 = poids non atteignable, valable car toutes les valeurs sont >0)*/
/* ------------------------------------------------------------------ */
int knapsackDP_Weight(Item items[], int n, int C, int M) {
    int dp[n + 1][C + 1];
#ifdef DEBUG
    fprintf(stderr, "C = %d, M = %d, n = %d\n", C, M, n);
#endif
    for (int w = 0; w <= C; w++) {
        dp[0][w] = 0;
    }
    for (int w = 0; w <= n; w++) {
        dp[w][0] = 0;
    }

    for (int i = 1; i <= n; ++i) {
        for (int w = 1; w <= C; ++w) {
            if (items[i - 1].weight <= C) {
                if (w < items[i - 1].weight) {
                    dp[i][w] = dp[i - 1][w];
                } else if (w == items[i - 1].weight) {
                    dp[i][w] = max(dp[i - 1][w], items[i - 1].value);
                } else if (dp[i - 1][w - items[i - 1].weight] != 0) {
                    dp[i][w] = max(dp[i - 1][w],
                                   dp[i - 1][w - items[i - 1].weight] + items[i - 1].value);
                } else {
                    dp[i][w] = dp[i - 1][w];
                }
            } else {
                dp[i][w] = dp[i - 1][w];
            }
#ifdef DEBUG
            printf("dp[%d][%d] = %d\n", i, w, dp[i][w]);
#endif
        }
    }

    int max_w = 0;
    for (int m = M; m <= C; m++) {
        if (dp[n][m] > max_w) {
            max_w = dp[n][m];
        }
    }
#ifdef DEBUG
    fprintf(stderr, "max_w = %d \n", max_w);
#endif
    return max_w;
}

/* ------------------------------------------------------------------ */
/* Question 8 : algorithme glouton (tri par rapport valeur/poids)     */
/* ------------------------------------------------------------------ */
// ordre décroissant de value/weight (produit en croix : pas de division entière)
int compare(const void *a, const void *b) {
    const Item *i = (const Item *)a;
    const Item *j = (const Item *)b;
    long gauche = (long)(*i).value * (*j).weight;
    long droite = (long)(*j).value * (*i).weight;
    if (gauche > droite) return -1;
    if (gauche < droite) return 1;
    return 0;
}

int knapsack_tri(Item items[], int n, int C) {
    qsort(items, (size_t)n, sizeof(Item), compare);
    int v = 0;
    int w = C;
    for (int i = 0; i < n; i++) {
        if (items[i].weight <= w) {
            v += items[i].value;
            w -= items[i].weight;
        }
    }
    return v;
}

/* ------------------------------------------------------------------ */
/* main                                                               */
/* ------------------------------------------------------------------ */
int main() {
    FILE *fp  = fopen("instances.csv", "r");
    FILE *out = fopen("average_times.csv", "w");
    FILE *sol = fopen("solutions.csv", "w");
    if (!fp || !out || !sol) {
        perror("ouverture des fichiers");
        return 1;
    }
    fprintf(out, "\n\n//AVERAGE_TIMES.CSV\n\n");
    fprintf(sol, "\n\n//SOLUTIONS.CSV (n, W, puis temps et valeur de BT1, BT2, DP, BT2bs, DPV, DPW, GLOUTON)\n\n");

    double sumTimeBT1 = 0, sumTimeBT2 = 0, sumTimeDP = 0, sumTimeBT2bs = 0,
           sumTimeDPV = 0, sumTimeDPW = 0, sumTimeGL = 0;
    int count = 0, currentN = -1;

    static char line[MAX_LINE];
    static int vals[2 * MAX_N + 10];
    static Item items[MAX_N], copie[MAX_N];
    clock_t start, end;

    while (fgets(line, MAX_LINE, fp)) {
        // on ignore les lignes vides et les commentaires (//...)
        const char *p = line;
        while (*p == ' ' || *p == '\t') p++;
        if (*p < '0' || *p > '9') continue;

        int cnt = 0;
        char *token = strtok(line, ", \t\r\n");
        while (token && cnt < 2 * MAX_N + 10) {
            vals[cnt++] = atoi(token);
            token = strtok(NULL, ", \t\r\n");
        }
        int W = vals[0], n = vals[1], M, off;
        if (cnt == 2 + 2 * n)      { M = 0;        off = 2; }  // format de la question 1
        else if (cnt == 3 + 2 * n) { M = vals[2];  off = 3; }  // format de la question 7
        else { fprintf(stderr, "Ligne ignorée (format invalide)\n"); continue; }

        for (int i = 0; i < n; i++) {
            items[i].weight = vals[off + 2 * i];
            items[i].value  = vals[off + 2 * i + 1];
        }

        if (n != currentN) {
            if (count > 0) {
                fprintf(out, "%d, %f, %f, %f, %f, %f, %f, %f\n", currentN,
                        sumTimeBT1 / count, sumTimeBT2 / count, sumTimeDP / count,
                        sumTimeBT2bs / count, sumTimeDPV / count,
                        sumTimeDPW / count, sumTimeGL / count);
            }
            currentN = n;
            sumTimeBT1 = sumTimeBT2 = sumTimeDP = sumTimeBT2bs = sumTimeDPV =
                sumTimeDPW = sumTimeGL = 0;
            count = 0;
        }

        double timeBT1 = 0, timeBT2 = 0, timeBT2bs = 0;
        int maxValBT1 = 0, maxValBT2 = 0, maxValBT2bs = 0;

        if (n <= BT_MAX_N) {
            start = clock();
            maxValBT1 = knapsackBT1(items, n, W);
            end = clock();
            timeBT1 = (double)(end - start) / CLOCKS_PER_SEC;
            sumTimeBT1 += timeBT1;

            start = clock();
            maxValBT2 = knapsackBT2(items, n, W);
            end = clock();
            timeBT2 = (double)(end - start) / CLOCKS_PER_SEC;
            sumTimeBT2 += timeBT2;

            start = clock();
            maxValBT2bs = knapsackBT2bs(items, n, W);
            end = clock();
            timeBT2bs = (double)(end - start) / CLOCKS_PER_SEC;
            sumTimeBT2bs += timeBT2bs;
        }

        start = clock();
        int maxValDP = knapsackDP(items, n, W);
        end = clock();
        double timeDP = (double)(end - start) / CLOCKS_PER_SEC;
        sumTimeDP += timeDP;

        start = clock();
        int maxValDPV = knapsackDP_Value(items, n, W);
        end = clock();
        double timeDPV = (double)(end - start) / CLOCKS_PER_SEC;
        sumTimeDPV += timeDPV;

        start = clock();
        int maxValDPW = knapsackDP_Weight(items, n, W, M);
        end = clock();
        double timeDPW = (double)(end - start) / CLOCKS_PER_SEC;
        sumTimeDPW += timeDPW;

        // glouton : sur une copie, car qsort modifie l'ordre des objets
        memcpy(copie, items, n * sizeof(Item));
        start = clock();
        int maxValGL = knapsack_tri(copie, n, W);
        end = clock();
        double timeGL = (double)(end - start) / CLOCKS_PER_SEC;
        sumTimeGL += timeGL;

        if (maxValDP != maxValDPV ||
            (n <= BT_MAX_N && (maxValDP != maxValBT1 || maxValDP != maxValBT2 ||
                               maxValDP != maxValBT2bs)))
            fprintf(stderr, "Incohérence : n=%d W=%d DP=%d DPV=%d BT1=%d BT2=%d BT2bs=%d\n",
                    n, W, maxValDP, maxValDPV, maxValBT1, maxValBT2, maxValBT2bs);

        count++;
        fprintf(sol, "%d, %d, %f, %d, %f, %d, %f, %d, %f, %d, %f, %d, %f, %d, %f, %d\n",
                n, W, timeBT1, maxValBT1, timeBT2, maxValBT2, timeDP, maxValDP,
                timeBT2bs, maxValBT2bs, timeDPV, maxValDPV, timeDPW, maxValDPW,
                timeGL, maxValGL);
    }
    if (count > 0) {
        fprintf(out, "%d, %f, %f, %f, %f, %f, %f, %f\n", currentN,
                sumTimeBT1 / count, sumTimeBT2 / count, sumTimeDP / count,
                sumTimeBT2bs / count, sumTimeDPV / count,
                sumTimeDPW / count, sumTimeGL / count);
    }

    fclose(fp);
    fclose(out);
    fclose(sol);
    return 0;
}
