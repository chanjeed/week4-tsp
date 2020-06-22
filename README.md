Build@Mercari 2020 Week4 - Travelling Salesman PRoblem Challenges.

This is forked from [https://github.com/hayatoito/google-step-tsp-2016](https://github.com/hayatoito/google-step-tsp-2016).

## Solver

### Greedy (+2opt) (solver_greedy_opt.c)
- Nearest Neighbor
- Optimization by adding 2opt method untill there is no intersection
- Each city be initial city once and choose the best route


### Greedy_prob (+2opt) (solver_greedy_prob.c)
- Randomly choose next city from 5 Nearest Neighbor according to ratio of inverse of distance from current city ( but divide distance to nearest neighbor by 2 to double probability to choose nearest neighbor) 
- Optimization by adding 2opt method untill there is no intersection
- Each city be initial city once and choose the best route
- I try adding randomness to greedy but the result is worse

### Genetic algorithm (solver_GA.py)

- I think Genetic algorithm is interesting and I would like to know the result. 
- I used code from this website [Evolution of a salesman: A complete genetic algorithm tutorial for Python](https://towardsdatascience.com/evolution-of-a-salesman-a-complete-genetic-algorithm-tutorial-for-python-6fe5d2b3ca35)
- I change constant mutation rate to adaptive mutation rate
- The result looks good for small number of cities but it is terrible when the number of city increased.


----

## Result

| Solver    | N = 5 | N = 8 | N = 16  | N = 64 | N = 128 | N = 512  | N = 2048 | Speed challenge|
| -------- | -------|--------|------- | ------- | -------|----------|--------- |----------------| 
| Greedy (+2opt)  |3418.10| 3832.29 |5228.03 |**8768.33**| **12066.55** | **23649.93**  | **45443.88**  | 349.59s|    
| Greedy_prob (+2opt2)  |3418.10| 3778.72    | 4664.52 |9252.3  | 12019.74 | 24721.15 | 50036.93   | |    
| GA (500 generation) |   **3291.62**    | **3778.72**  |  **4494.42** | 14531.46|  29068.87| 250897.11 | 1181979.75 |      |                        



