import math, random

coords = {'W':(0,0),'A':(2,6),'B':(5,2),'C':(6,7),
          'D':(8,3),'E':(1,4),'F':(7,6),'G':(3,1)}

def dist(a, b):
    return math.dist(coords[a], coords[b])

def cost(route):
    path = ['W'] + route + ['W']
    return sum(dist(path[i], path[i+1]) for i in range(len(path)-1))

def simulated_annealing(route, T=100, alpha=0.95, T_min=0.1):
    current, cur_cost = route[:], cost(route)
    best, best_cost = current[:], cur_cost
    iters = 0
    print("Initial:", current, round(cur_cost, 3))

    while T >= T_min:
        iters += 1

        # random neighbour: swap two random positions
        i, j = random.sample(range(len(current)), 2)
        nb = current[:]
        nb[i], nb[j] = nb[j], nb[i]

        new_cost = cost(nb)
        dE = new_cost - cur_cost

        if dE < 0:
            accept, note = True, "better"
        else:
            P = math.exp(-dE / T)
            r = random.random()
            accept = r < P
            note = f"worse, P={P:.4f}, r={r:.4f}"

        if accept:
            current, cur_cost = nb, new_cost
            if cur_cost < best_cost:
                best, best_cost = current[:], cur_cost

        print(f"Iter {iters:3d} | T={T:8.4f} | cost={cur_cost:7.3f} | "
              f"{'ACCEPTED' if accept else 'REJECTED'} ({note})")

        T *= alpha

    return best, best_cost, iters

start = list("ABCDEFG")
random.shuffle(start)
best, best_cost, n = simulated_annealing(start)
print("Best route:", best, "| Best cost:", round(best_cost, 3))
print("Iterations:", n)
