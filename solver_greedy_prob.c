#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <float.h>
#include <math.h>
int n;

struct City
{
    double x;
    double y;
};

int random_city()
{
    return rand() % (n + 1);
}

int read_input(char filename[], struct City cities[])
{
    FILE *f = fopen(filename, "r");
    if (f == NULL)
    {
        fprintf(stderr, "Error reading file\n");
        exit(1);
    }
    int ret;
    int idx = 0;
    char bf1[4];
    double bf3, bf4;
    fscanf(f, "%s\n", &bf1);

    while ((ret = fscanf(f, "%lf,%lf\n", &bf3, &bf4)) != -1)
    {
        cities[idx].x = bf3;
        cities[idx].y = bf4;
        //printf("%d %d\n", idx, ret);
        //printf("%lf %lf\n", cities[idx].x, cities[idx].y);
        idx++;
    }
    // n = idx;

    fclose(f);
    printf("idx = %d\n", idx);
    return idx;
}

double distance(struct City city1, struct City city2)
{
    return sqrt(pow(city1.x - city2.x, 2) + pow(city1.y - city2.y, 2));
}

int print_cities(struct City cities[])
{
    int i;
    for (i = 0; i < n; i++)
    {
        printf("%lf %lf\n", cities[i].x, cities[i].y);
    }
}

void greedy(struct City cities[], int solution[])
{
    int i, j;
    double **dist;
    dist = (double **)malloc(n * sizeof(double *));

    for (int i = 0; i < n; i++)
        dist[i] = (double *)malloc(n * sizeof(double));

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            dist[i][j] = dist[j][i] = distance(cities[i], cities[j]);
        }
    }

    int current_city = random_city();

    bool visited[n];
    for (i = 0; i < n; i++)
    {
        visited[i] = false;
    }
    visited[current_city] = true;

    int solution_idx = 0;
    solution[solution_idx] = current_city;
    solution_idx++;

    double distance_from_current_city(int to)
    {
        return dist[current_city][to];
    }
    double min_dist_from_current, dist_from_current;
    int next_city;
    while (solution_idx < n)
    {
        min_dist_from_current = DBL_MAX;
        for (i = 0; i < n; i++)
        {
            if ((i != current_city) && (visited[i] == false))
            {
                dist_from_current = distance_from_current_city(i);
                if (dist_from_current < min_dist_from_current)
                {
                    min_dist_from_current = dist_from_current;
                    next_city = i;
                }
            }
        }

        solution[solution_idx] = next_city;
        solution_idx++;
        visited[next_city] = true;
        current_city = next_city;
    }
}

void print_solution(int solution[])
{
    int i;
    printf("index\n");
    for (i = 0; i < n; i++)
    {
        printf("%d\n", solution[i]);
    }
}

int main(int argc, char *argv[])
{
    char *input_file = argv[1];
    struct City *cities = (struct City *)malloc(2048 * sizeof(struct City));
    int i;

    n = read_input(input_file, cities);
    printf("finish read_input\n");
    printf("%d\n", n);
    int solution[n];
    greedy(cities, solution);
    printf("n = %d\n", n);
    print_solution(solution);
    return 0;
}