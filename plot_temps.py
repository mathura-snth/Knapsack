import pandas as pd
import matplotlib.pyplot as plt

csv_file_path = 'average_times.csv'

# Colonnes dans l'ordre d'écriture de knapsack.c
df = pd.read_csv(csv_file_path, header=None, comment='/', skipinitialspace=True,
                 names=['Taille', 'BT1', 'BT2', 'DP', 'BT2bs', 'DPV', 'DPW', 'GL'])

# Courbes à afficher (None = toutes celles dont les temps ne sont pas tous nuls).
# Exemple pour la figure 7 : AFFICHER = ['DP', 'DPV']
AFFICHER = None

courbes = {
    'BT1':   ('Backtracking 1', 'o'),
    'BT2':   ('Backtracking 2', 's'),
    'BT2bs': ('Backtracking BS 2', 's'),
    'DP':    ('Programmation Dynamique', '^'),
    'DPV':   ('Programmation Dynamique Value', '^'),
    'DPW':   ('Programmation Dynamique Weight', '^'),
    'GL':    ('Glouton', 'x'),
}

# Préparation du graphique pour comparer les temps d'exécution des algorithmes
plt.figure(figsize=(10, 6))
for col, (label, marker) in courbes.items():
    if AFFICHER is None:
        if (df[col] == 0).all():
            continue
    elif col not in AFFICHER:
        continue
    plt.plot(df['Taille'], df[col], label=label, marker=marker)

plt.xlabel("Taille du problème (nombre d'objets)")
plt.ylabel("Temps d'exécution moyen (secondes)")
plt.title("Comparaison des temps d'exécution des algorithmes pour le problème du sac à dos")
plt.legend()
plt.grid(True)
plt.show()
