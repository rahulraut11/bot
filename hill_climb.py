import math, random
from itertools import combinations

coords = {'W':(0,0),'A':(2,6),'B':(5,2),'C':(6,7),
          'D':(8,3),'E':(1,4),'F':(7,6),'G':(3,1)}

def dist(a, b):
    return math.dist(coords[a], coords[b])

def cost(route):
    path = ['W'] + route + ['W']
    return sum(dist(path[i], path[i+1]) for i in range(len(path)-1))

def hill_climbing(route):
    current, cur_cost, iters = route[:], cost(route), 0
    print("Initial:", current, round(cur_cost, 3))
    while True:
        best, best_cost = None, cur_cost
        for i, j in combinations(range(len(current)), 2):
            nb = current[:]
            nb[i], nb[j] = nb[j], nb[i]
            c = cost(nb)
            if c < best_cost:
                best, best_cost = nb, c
        if best is None:          # no improving neighbour
            break
        current, cur_cost = best, best_cost
        iters += 1
        print(f"Iter {iters}: {current}  cost = {cur_cost:.3f}")
    return current, cur_cost, iters

start = list("ABCDEFG")
random.shuffle(start)
final, final_cost, n = hill_climbing(start)
print("Final:", final, round(final_cost, 3), "| Iterations:", n)
