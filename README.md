Build@Mercari 2020 Week4 - Travelling Salesman PRoblem Challenges.

This is forked from [https://github.com/hayatoito/google-step-tsp-2016](https://github.com/hayatoito/google-step-tsp-2016).

## Solver

### Greedy_prob (solver_greedy_prob.c)

- 

### GA (solver_GA.py)

- Genetic algorithm
- Adaptive mutation rate


----

## Result

| Solver    | N = 5 | N = 8 | N = 16  | N = 64 | N = 128 | N = 512  | N = 2048 | Speed challenge|
| -------- | -------|--------|------- | ------- | -------|----------|--------- |----------------|
| GA  |   3291.62    | 3778.72  |  4494.42 | 14531.46|  29068.87| 250897.11 | 1181979.75 |      |       
| Greedy_opt2  |3418.10| 3832.29 |5290.21 |9821.65| 12066.55 | 23877.36  | 45557.88  | |    
| Greedy_prob (+opt2)  |3418.10| 3832.29 |4926.7 |9642.21| 13888.62 | 27526.37  | 55356.14  | |                      



