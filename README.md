# Problème du sac à dos 0/1 : Conception d'algorithmes

Projet réalisé par SANTHALINGAM Mathura (12212867).
Université Sorbonne Paris Nord, Institut Galilée, 2024.

Ce projet implémente et compare plusieurs algorithmes pour le problème du sac à dos 0/1 : backtracking, programmation dynamique, variante avec poids minimum et algorithme glouton.

## Contenu

| Fichier | Rôle |
|---|---|
| `generateData.py` | Génère les instances dans `instances.csv` |
| `knapsack.c` | Implémente les algorithmes, mesure les temps et écrit les résultats |
| `plot_temps.py` | Trace les courbes de temps d'exécution moyens |
| `instances.csv` | Exemple d'instances (n de 5 à 25, 10 instances par taille) |

## Prérequis

- `gcc`
- Python 3 avec `pandas` et `matplotlib` (`pip install pandas matplotlib`)

## Utilisation

```bash
python generateData.py              # génère instances.csv
gcc knapsack.c -o knapsack -lm      # compile
./knapsack                          # résout et écrit average_times.csv et solutions.csv
python plot_temps.py                # affiche les courbes
```

## Format des fichiers

**`instances.csv`** : une instance par ligne, les lignes vides et les commentaires `//` sont ignorés.

```
W, n, M, p1, v1, p2, v2, ..., pn, vn
```

- `W` : capacité maximale du sac
- `n` : nombre d'objets
- `M` : poids minimum requis (exercice 4)
- `(pi, vi)` : poids et valeur de chaque objet

L'ancien format sans `M` (`W, n, p1, v1, ...`) est aussi lu par `knapsack.c` : `M` vaut alors 0. Pour le générer, mettre `AVEC_POIDS_MINIMUM = False` dans `generateData.py`.

**`average_times.csv`** : temps moyens (en secondes) par taille `n`.

```
n, BT1, BT2, DP, BT2bs, DPV, DPW, GL
```

**`solutions.csv`** : pour chaque instance, `n, W`, puis le couple (temps, valeur) de chaque algorithme dans le même ordre que ci-dessus.

## Algorithmes

| Colonne | Fonction | Description | Complexité temporelle |
|---|---|---|---|
| `BT1` | `knapsackBT1` | Backtracking avec variables globales | O(2^n) |
| `BT2` | `knapsackBT2` | Backtracking sans variables globales | O(2^n) |
| `BT2bs` | `knapsackBT2bs` | Backtracking avec élagage par borne supérieure (somme des valeurs restantes précalculée) | O(2^n) au pire, bien meilleur en pratique |
| `DP` | `knapsackDP` | Programmation dynamique basée sur le poids | O(nW) |
| `DPV` | `knapsackDP_Value` | Programmation dynamique basée sur la valeur : `dp[v]` = poids minimal pour atteindre `v` | O(nV), avec V la somme des valeurs |
| `DPW` | `knapsackDP_Weight` | Variante avec poids minimum : `dp[i][w]` = valeur max avec les `i` premiers objets pour un poids exactement `w`, résultat pris sur `w` dans `[M, C]` | O(nC) en temps et en espace |
| `GL` | `knapsack_tri` | Glouton : tri par rapport valeur/poids décroissant | O(n log n) en temps, O(n) en espace |

Le glouton n'est pas toujours optimal. Contre-exemple : `27,2,10,18,36,27,40` (W=27, n=2, M=10, objets (18,36) et (27,40)). Le glouton renvoie 36 alors que l'optimum est 40.

## Paramètres modifiables

Dans `generateData.py` :

- `MIN_ITEMS`, `MAX_ITEMS`, `ITEM_STEP` : tailles testées (par défaut 5 à 25)
- `NUM_INSTANCES_PER_SIZE` : instances par taille (10 par défaut, 100 pour les figures 8 à 10)
- `MAX_CAPACITY` : capacité max (200 par défaut, 400 pour la figure 10) ; la capacité est tirée dans `[MAX_CAPACITY//2, MAX_CAPACITY]`
- `MIN_CAPACITY` : sert à tirer le poids minimum `M` dans `[MIN_CAPACITY, 2*MIN_CAPACITY]`
- `MAX_WEIGHT`, `MAX_VALUE` : poids et valeurs max des objets (10 par défaut, `MAX_WEIGHT = 100` pour la figure 9)

Dans `knapsack.c` :

- `BT_MAX_N` (30 par défaut) : au-delà de cette taille, les trois backtracking ne sont pas exécutés (colonnes à 0), sinon ils ne terminent pas.
- Compiler avec `-DDEBUG` pour afficher le détail de `knapsackDP_Weight`.

Dans `plot_temps.py` :

- `AFFICHER` : liste des courbes à tracer, par exemple `['DP', 'DPV']`. Avec `None`, toutes les colonnes non nulles sont tracées.

## Remarques

- `knapsack.c` signale sur `stderr` toute incohérence entre les valeurs trouvées par DP, DPV et les backtracking.
- Les temps absolus dépendent de la machine. Seule l'allure des courbes est comparable : exponentielle pour les backtracking, quasi nulle pour la programmation dynamique et le backtracking avec borne.