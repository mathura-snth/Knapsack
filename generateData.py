import random

# ---------------- Paramètres ----------------
MIN_ITEMS = 5                 # nombre minimum d'objets
MAX_ITEMS = 25                # nombre maximum d'objets (100 pour les figures 8 à 10)
ITEM_STEP = 1
NUM_INSTANCES_PER_SIZE = 10   # 10 pour n=25, 100 pour n=100 (figures 8 à 10)
MAX_CAPACITY = 200            # 400 pour la figure 10
MIN_CAPACITY = 50             # poids minimum M (question 7)
MAX_WEIGHT = 10               # 100 pour la figure 9
MAX_VALUE = 10

# True  : format de la question 7  -> capacité, n, M, (poids, valeur)*
# False : format de la question 1  -> capacité, n, (poids, valeur)*
AVEC_POIDS_MINIMUM = True
# --------------------------------------------

instances = []
for num_items in range(MIN_ITEMS, MAX_ITEMS + 1, ITEM_STEP):
    for _ in range(NUM_INSTANCES_PER_SIZE):
        capacity = random.randint(MAX_CAPACITY // 2, MAX_CAPACITY)
        mincap = random.randint(MIN_CAPACITY, MIN_CAPACITY * 2)
        weights = [random.randint(1, MAX_WEIGHT) for _ in range(num_items)]
        values = [random.randint(1, MAX_VALUE) for _ in range(num_items)]
        if AVEC_POIDS_MINIMUM:
            instance = [capacity, num_items, mincap]
        else:
            instance = [capacity, num_items]
        instance += [val for pair in zip(weights, values) for val in pair]
        instances.append(instance)

with open("instances.csv", "w") as f:
    f.write("\n//INSTANCES.CSV\n\n")
    for instance in instances:
        f.write(",".join(map(str, instance)) + "\n")

print(f"{len(instances)} instances écrites dans instances.csv")
