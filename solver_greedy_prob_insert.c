#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <float.h>
#include <math.h>
#include <time.h>
int n;

struct City
{
    double x;
    double y;
};

int random_city()
{
    return rand() % n;
}

double random_prob()
{
    return rand() / (double)RAND_MAX;
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

    fclose(f);
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

int pick_next_city(double d[], int c[])
{
    int i;
    double prob = random_prob();
    double offset = 0;
    double sum_prob = 0;
    //printf("prob = %lf\n", prob);

    for (i = 0; i < 2; i++)
    {
        sum_prob += 1.0 / d[i];
    }
    for (i = 0; i < 2; i++)
    {
        offset += (1 / d[i]) / sum_prob;
        //printf("offset = %lf\n", offset);
        if (prob < offset)
        {
            //printf("i = %d\n", i);
            return c[i];
        }
    }
}

void reverse(int path[], int x, int y)
{
    int temp[y - x + 1], temp_index = 0;
    int k;
    for (k = x; k <= y; k++)
    {
        //	printf("path[k] = %d\n",path[k]);
        temp[temp_index] = path[k];
        temp_index++;
    }

    temp_index--;
    for (k = x; k <= y; k++)
    {
        path[k] = temp[temp_index];
        temp_index--;
    }
}

int detect_intersect(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
{
    double ta = (x3 - x4) * (y1 - y3) + (y3 - y4) * (x3 - x1);
    double tb = (x3 - x4) * (y2 - y3) + (y3 - y4) * (x3 - x2);
    double tc = (x1 - x2) * (y3 - y1) + (y1 - y2) * (x1 - x3);
    double td = (x1 - x2) * (y4 - y1) + (y1 - y2) * (x1 - x4);
    return tc * td < 0 && ta * tb < 0;
}

void opt2(struct City cities[], int path[])
{
    int i, j;
    int count = 0;
    int have_improve = 1;
    while (have_improve)
    {
        have_improve = 0;
        for (i = 0; i < n - 2; i++)
        {
            for (j = i + 2; j < n; j++)
            {

                if (detect_intersect(cities[path[i]].x, cities[path[i]].y, cities[path[i + 1]].x, cities[path[i + 1]].y,
                                     cities[path[j]].x, cities[path[j]].y, cities[path[(j + 1) % n]].x, cities[path[(j + 1) % n]].y))
                {
                    count++;
                    reverse(path, i + 1, j);
                    have_improve = 1;
                }
            }
        }
    }
    //printf("found %d intersect\n", count);
}

void insert_city_right(int path[], int from, int to)
{
    // from : left, to : right
    int i;
    int tmp = path[from];
    for (i = from; i < to; i++)
    {
        path[i] = path[i + 1];
    }
    path[to] = tmp;
}

void insert_city_left(int path[], int from, int to)
{
    // from : right, to : left
    int i;
    int tmp = path[from];
    for (i = from; i > to + 1; i--)
    {
        path[i] = path[i - 1];
    }
    path[to + 1] = tmp;
}

void insert(double **dist, int path[])
{
    int i, j, k;
    int have_insert = 1;
    while (have_insert)
    {
        have_insert = 0;
        for (i = 1; i < n; i++)
        {
            for (j = 1; j < n; j++)
            {
                if (i == j || i == j + 1)
                {
                    continue;
                }
                if (dist[path[i - 1]][path[i]] + dist[path[i]][path[(i + 1) % n]] + dist[path[j]][path[(j + 1) % n]] > dist[path[i - 1]][path[(i + 1) % n]] + dist[path[j]][path[i]] + dist[path[i]][path[(j + 1) % n]])
                {
                    //insert i city between j,j+1 city

                    // printf("i = %d, j = %d\n", i, j);
                    // printf("before\n");
                    // for (k = 0; k < n; k++)
                    // {
                    // 	printf("%d\n", path[k]);
                    // }

                    have_insert = 1;
                    if (i < j)
                    {
                        insert_city_right(path, i, j);
                    }

                    else
                    {
                        insert_city_left(path, i, j);
                    }

                    // printf("after\n");
                    // for (k = 0; k < n; k++)
                    // {
                    // 	printf("%d\n", path[k]);
                    // }
                    // exit(1);
                }
            }
        }
    }
}

void make_distance(double **dist, struct City cities[])
{
    int i, j;

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            dist[i][j] = dist[j][i] = distance(cities[i], cities[j]);
        }
    }
}

void greedy(double **dist, int solution[], int current_city)
{
    int i, j;

    //int current_city = random_city();
    //printf("current = %d\n", current_city);

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
    double first, second, third, fourth, fifth;
    int c1, c2, c3, c4, c5;
    while (solution_idx < n)
    {

        if (solution_idx + 2 < n)
        {

            first = second = DBL_MAX;

            c1 = c2 = -1;
            for (i = 0; i < n; i++)
            {

                if ((i != current_city) && (visited[i] == false))
                {
                    dist_from_current = distance_from_current_city(i);

                    if (dist_from_current < first)
                    {

                        second = first;
                        first = dist_from_current;
                        c2 = c1;
                        c1 = i;
                    }
                    else if (dist_from_current < second)
                    {

                        second = dist_from_current;
                        c2 = i;
                    }
                }
            }

            int c[2] = {c1, c2};
            double d[2] = {first / 2.0, second};
            next_city = pick_next_city(d, c);
        }
        else
        {
            // <5 cities left
            // get nearest city
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

double find_sum_distance(double **dist, int solution[])
{
    int i;
    double sum_distance = 0;
    for (i = 1; i < n; i++)
    {

        sum_distance += dist[solution[i - 1]][solution[i]];
    }
    sum_distance += dist[solution[i - 1]][solution[0]];
    //printf("sum distance = %lf\n", sum_distance);
    return sum_distance;
}

int main(int argc, char *argv[])
{
    char *input_file = argv[1];
    struct City *cities = (struct City *)malloc(2048 * sizeof(struct City));
    int i, j;

    srand(time(0));

    n = read_input(input_file, cities);

    //printf("finish read_input\n");
    int solution[n];
    int best_solution[n];
    double best_sum_distance = DBL_MAX, sum_distance;
    double **dist;
    dist = (double **)malloc(n * sizeof(double *));

    for (int i = 0; i < n; i++)
        dist[i] = (double *)malloc(n * sizeof(double));
    make_distance(dist, cities);
    int MAX_SOLUTION = n;
    for (i = 0; i < MAX_SOLUTION; i++)
    {
        greedy(dist, solution, i % n);
        //print_solution(solution);
        opt2(cities, solution);
        insert(dist, solution);
        sum_distance = find_sum_distance(dist, solution);
        if (sum_distance < best_sum_distance)
        {
            for (j = 0; j < n; j++)
            {
                best_solution[j] = solution[j];
            }
            best_sum_distance = sum_distance;
        }
    }

    print_solution(best_solution);
    //printf("best sum distance = %lf\n", best_sum_distance);
    return 0;
}