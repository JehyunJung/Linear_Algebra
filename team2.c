#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define MAX_ARRAY_INDEX 100  //maximum array index
#define NUMBER_OF_CENTER 3   //number of centroids



typedef struct point {  //structure of a data(block)
	int date;
	int time;
	double amount;
	int position;
};

struct point array[MAX_ARRAY_INDEX]; //structure array
int prev_array[MAX_ARRAY_INDEX];     //previous cluster array 
double center[NUMBER_OF_CENTER][2] = { { 3.5,0.7 },{5.0,0.0}, {11.0,6.0} }; // random centroids

void point_init(int n);  //initializing function ( n is number of blocks)
double distance(int i, int n);   //calculating  distance of point and centroid(i is block index, n is number of blocks)  
int no_change_check(int n);      //checking  whether modifications have occured, comparing with previous clusters
void change_center(int n);       //reseting centroids (n is number of blocks)
void print_k_means();             //printing k_means
void change_point_position(int n);   //changing cluster 
void print_point(int n);         //print blocks

int main() {

	FILE *fp;
	int n;
	int i;
	int temp_date;
	int temp_time;
	float temp_amount;


	if ((fp = fopen("input.txt", "r")) == 0) {
		printf("file reading failure");
		exit(1);
	}

	fscanf(fp, "%d", &n);

	for (i = 0; i < n; i++)
	{
		fscanf(fp, "%d", &temp_date);
		fscanf(fp, "%d", &temp_time);
		fscanf(fp, "%f", &temp_amount);
		array[i].date = temp_date;
		array[i].time = temp_time;
		array[i].amount = temp_amount;
	}

	point_init(n);

	do
	{
		change_center(n);
		change_point_position(n);
	} while (no_change_check(n));

	print_k_means();
	print_point(n);
	fclose(fp);
	return 0;
}

void point_init(int n) {
	int i, j;
	double min;
	int	min_index;

	for (i = 0; i < n; i++) {
		min = distance(i, 0);
		min_index = 0;
		for (j = 1; j < NUMBER_OF_CENTER; j++) {
			if (min > distance(i, j)) {
				min = distance(i, j);
				min_index = j;
			}
		}
		array[i].position = min_index;
		prev_array[i] = min_index;
	}
}

double distance(int i, int n)
{
	return pow((center[n][0] - array[i].time), 2) + pow((center[n][1] - array[i].amount), 2);
}

int no_change_check(int n) {
	int i;
	for (i = 0; i < n; i++) {
		if (array[i].position != prev_array[i])
			return 1;
	}
	return 0;
}

void change_center(int n)
{
	int i, j;
	int count[NUMBER_OF_CENTER] = { 0 };
	double save[NUMBER_OF_CENTER][2] = { 0.0 };
	for (i = 0; i < NUMBER_OF_CENTER; i++) {
		for (j = 0; j < 2; j++) {
			save[i][j] = center[i][j];
			center[i][j] = 0;
		}
	}
	for (j = 0; j < NUMBER_OF_CENTER; j++) {
		for (i = 0; i < n; i++)
		{
			if (array[i].position == j) {
				center[j][0] += array[i].time;
				center[j][1] += array[i].amount;
				count[j]++;
			}
		}
	}

	for (i = 0; i < NUMBER_OF_CENTER; i++) {
		center[i][0] = center[i][0] / count[i];
		center[i][1] = center[i][1] / count[i];
	}

	for (i = 0; i < NUMBER_OF_CENTER; i++)
	{
		if (count[i] == 0) {
			center[i][0] = save[i][0];
			center[i][1] = save[i][1];
		}
	}
}

void print_k_means() {
	int i;
	for(i=0;i< NUMBER_OF_CENTER;i++)
		printf("cluster %d mean : (%lf, %lf)\n", i,center[i][0], center[i][1]);
}

void change_point_position(int n) {
	int i,j;
	double min;
	int min_index;
	for (i = 0; i < n; i++)
		prev_array[i] = array[i].position;

	for (i = 0; i < n; i++) {
		min = distance(i, 0);
		min_index = 0;
		for (j = 1; j < NUMBER_OF_CENTER; j++) {
			if (min > distance(i, j)) {
				min = distance(i, j);
				min_index = j;
			}
		}
		array[i].position = min_index;
	}
}

void print_point(int n) {
	int i,j;
	int count = 0;
	FILE *fp1;
	FILE *fp2;

	fp1 = fopen("output1.txt", "w");
	fp2 = fopen("output2.txt", "w");

	for (j = 0; j < NUMBER_OF_CENTER; j++) {
		printf("\nElements of cluster %d\n", j);
		for (i = 0; i < n; i++) {
			if (array[i].position == j) {
				printf("(%3d , %.3lf)", array[i].time, array[i].amount);
				fprintf(fp1, "%3d\n", array[i].time);
				fprintf(fp2, "%.3lf\n", array[i].amount);
				count++;
			}
			if (count == 4) {
				printf("\n");
				count = 0;
			}
		}
	}
	fclose(fp1);
	fclose(fp2);
}


