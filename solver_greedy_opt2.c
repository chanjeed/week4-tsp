#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
int n;
double distance_matrix[2048][2048];
int answer_path[2048], answer_path_best[2048];
double sum_distance = 0;

struct POINT
{
	double x;
	double y;
};

double calculate_distance(double x1, double y1, double x2, double y2)
{
	double dx = x1 - x2;
	double dy = y1 - y2;
	return sqrt(dx * dx + dy * dy);
}

void check_distance_matrix()
{
	int i, j;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			printf("%lf ", distance_matrix[i][j]);
		}
		printf("\n");
	}
}

void make_distance_matrix(struct POINT point[])
{
	int i, j;

	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			distance_matrix[i][j] = calculate_distance(point[i].x, point[i].y, point[j].x, point[j].y);
		}
	}
}

int find_min_next(int visited[], int current_city)
{
	double min_distance = 9999999;
	int min_city;
	int next_city;
	for (next_city = 0; next_city < n; next_city++)
	{
		//printf("distance = %lf\n",distance_matrix[current_city][next_city]);
		if (distance_matrix[current_city][next_city] < min_distance && visited[next_city] == 0)
		{

			min_distance = distance_matrix[current_city][next_city];
			min_city = next_city;
		}
	}

	return min_city;
}

int detect_intersect(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
{
	double ta = (x3 - x4) * (y1 - y3) + (y3 - y4) * (x3 - x1);
	double tb = (x3 - x4) * (y2 - y3) + (y3 - y4) * (x3 - x2);
	double tc = (x1 - x2) * (y3 - y1) + (y1 - y2) * (x1 - x3);
	double td = (x1 - x2) * (y4 - y1) + (y1 - y2) * (x1 - x4);
	return tc * td < 0 && ta * tb < 0;
}

void find_sum_distance()
{
	int i;
	sum_distance = 0;
	for (i = 1; i < n; i++)
	{

		sum_distance += distance_matrix[answer_path[i - 1]][answer_path[i]];
	}
	sum_distance += distance_matrix[answer_path[i - 1]][answer_path[0]];
}

void reverse(int path[], int begin, int end)
{
	int tmp;
	int k;
	while (begin < end)
	{
		tmp = path[begin];
		path[begin] = path[end];
		begin++;
		path[end] = tmp;
		end--;
	}
}
void improve(struct POINT point[], int path[])
{
	int i, j;
	int count = 0;
	int have_improve = 1;
	while (have_improve)
	{
		have_improve = 0;
		for (i = 0; i < n - 1; i++)
		{
			for (j = i + 2; j < n - 1; j++)
			{

				if (detect_intersect(point[path[i]].x, point[path[i]].y, point[path[i + 1]].x, point[path[i + 1]].y,
									 point[path[j]].x, point[path[j]].y, point[path[j + 1]].x, point[path[j + 1]].y))
				{
					count++;
					reverse(path, i + 1, j);
					have_improve = 1;
				}
			}
		}
		// improve between last city and first city
		for (i = 1; i < n - 2; i++)
		{
			if (detect_intersect(point[path[i]].x, point[path[i]].y, point[path[i + 1]].x, point[path[i + 1]].y,
								 point[path[n - 1]].x, point[path[n - 1]].y, point[path[0]].x, point[path[0]].y))
			{
				count++;
				reverse(path, i + 1, n - 1);
				have_improve = 1;
			}
		}
	}
	//printf("found %d intersect\n", count);
}
void copy_path()
{
	int i;
	for (i = 0; i < n; i++)
	{
		answer_path_best[i] = answer_path[i];
	}
}
void tsp(struct POINT point[], int start_city)
{
	int current_city, next_city;
	int visited[n], i;
	int index_path = 0;
	for (i = 0; i < n; i++)
	{
		visited[i] = 0;
	}
	visited[start_city] = 1; //Marked that insert in Set
	next_city = start_city;
	answer_path[0] = start_city;
	index_path += 1;
	while (index_path < n)
	{
		current_city = next_city;
		next_city = find_min_next(visited, current_city);
		//printf("next city = %d\n",next_city);
		visited[next_city] = 1;
		answer_path[index_path] = next_city;
		index_path += 1;
	}

	improve(point, answer_path);
}

void output()
{
	int i;

	printf("index\n");
	for (i = 0; i < n; i++)
	{
		printf("%d\n", answer_path[i]);
	}
}

void input(char file_name[])
{
	int ret;
	double x, y;
	int i = 0;
	FILE *f = fopen(file_name, "r");
	//printf("open file\n");
	struct POINT point[2048];
	char buffer[4];
	fscanf(f, "%s\n", buffer);
	//printf("%s\n", buffer);

	char line[100];
	while (1)
	{
		ret = fscanf(f, "%lf,%lf\n", &x, &y);
		//printf("%d\n", ret);
		if (ret == -1)
			break; //end of file
		//printf("%lf,%lf\n", x, y);
		point[i].x = x;
		point[i].y = y;
		i++;
	}
	n = i;
	fclose(f);
	//printf("finish read file\n");
	make_distance_matrix(point);
	//printf("finish make distance_matrix\n");
	//check_distance_matrix();
	double min_sum_distance = 999999;
	for (i = 0; i < n; i++)
	{
		tsp(point, i);
		find_sum_distance();
		if (sum_distance < min_sum_distance)
		{
			min_sum_distance = sum_distance;
			copy_path();
		}
	}
	//printf("Best distance = %lf\n", min_sum_distance);
}

int main(int argc, char *argv[])
{

	char *input_file = argv[1];
	char output_file[50];
	int i;
	input(input_file);

	output();

	return 0;
}