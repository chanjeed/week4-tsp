Build@Mercari 2020 Week4 - Travelling Salesman PRoblem Challenges.

This is forked from [https://github.com/hayatoito/google-step-tsp-2016](https://github.com/hayatoito/google-step-tsp-2016).

## Solver

### Greedy (NN+2opt) (solver_greedy_opt2.c)
- Nearest Neighbor
- Optimization by adding 2opt method untill there is no intersection
- Each city be initial city once and choose the best route

### Greedy (NN+2opt+insert) (solver_greedy_opt2_insert.c)
- From Greedy (NN+2opt) more optimization by inserting one city into other position if the distance decrease 

### Greedy_prob (+2opt) (solver_greedy_prob.c)
- Randomly choose next city from 2 Nearest Neighbor according to ratio of inverse of distance from current city ( but divide distance to nearest neighbor by 2 to double probability to choose nearest neighbor) 
- Optimization by adding 2opt method untill there is no intersection
- Each city be initial city once and choose the best route
- I tried adding randomness to greedy by chossing next city from 2 to 5 Nearest Neighbor and found that this one is the best

### Genetic algorithm (solver_GA.py)

- I think Genetic algorithm is interesting and I would like to know the result. 
- I used code from this website [Evolution of a salesman: A complete genetic algorithm tutorial for Python](https://towardsdatascience.com/evolution-of-a-salesman-a-complete-genetic-algorithm-tutorial-for-python-6fe5d2b3ca35)
- I change constant mutation rate to adaptive mutation rate according to this paper [Solving TSP Problem with Improved Genetic Algorithm](https://aip.scitation.org/doi/pdf/10.1063/1.5039131)
- Initial population by random
- The result looks good for small number of cities but it is terrible when the number of city increased.

### Genetic algorithm from greedy (solver_GA_greedy.py)

- Change initial population from random to greedy (Nearest Neighbor) from each city


----

## Result

| Solver    | N = 5 | N = 8 | N = 16  | N = 64 | N = 128 | N = 512  | N = 2048 | Speed challenge|
| -------- | -------|--------|------- | ------- | -------|----------|--------- |----------------| 
| Greedy (NN+2opt+insert)  |**3291.62** | **3778.72** |**4494.42** |8604.04| 11629.90 | 22445.92  | **43149.49**  | 1027.75s|    
| Greedy (NN+2opt)  |3418.10| 3832.29 |5228.03 |8768.33| 12066.55 | 23701.76  | 45360.80  | **349.59s**|    
| Greedy_prob_insert (2NN+2opt+insert)  |**3291.62** | **3778.72** |**4494.42** |**8532.60**| **11008.36** | **21519.87**  | 44038.64  | 1335.00s|    
| Greedy_prob (2NN+2opt)  |**3291.62**| 3832.29    | **4494.42** | 9066.32  | 12023.92 | 23700.27  | 47997.69   | |  
| GA initial by greedy (500 generation) |   **3291.62**    | **3778.72**  |  4821.46| 9088.15|  12252.17| 176173.46  | 1106591.58  |      |   
| GA (500 generation) |   **3291.62**    | **3778.72**  |  **4494.42** | 14531.46|  29068.87| 250897.11 | 1181979.75 |      |                        



