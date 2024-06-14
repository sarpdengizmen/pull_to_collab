/*
A c program that implements the k-measn algorithm using 
lloyd algorithm for updating the centroids.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

int getMinIndex(double arr[], int n);
double average(double arr[], int n);
int someExceedsTol(double arr[], int n, double tol);


int main(int argc, char *argv[]){
    /* Function takes two arguments first is the input file name 
    second is the data file name*/
    
    FILE *input_file = fopen(argv[1], "r");

    char line[20];
    int datapoints;
    int dimensions;
    int clusters;
    double tolerance;

    while (fscanf(input_file, "%s", line) != EOF){
        if (strcmp(line, "[NUMBER_OF_POINTS]") == 0){
            fscanf(input_file, "%d", &datapoints);
        }
        else if (strcmp(line, "[DATA_DIMENSION]") == 0){
            fscanf(input_file, "%d", &dimensions);
        }
        else if (strcmp(line, "[NUMBER_OF_CLUSTERS]") == 0){
            fscanf(input_file, "%d", &clusters);
        }
        else if (strcmp(line, "[TOLERANCE]") == 0){
            fscanf(input_file, "%lf", &tolerance);
        }
    }
    fclose(input_file); // Close the file
    printf("Number of datapoints: %d\n", datapoints);
    printf("Number of dimensions: %d\n", dimensions);
    printf("Number of clusters: %d\n", clusters);
    printf("Tolerance: %.5lf\n\n", tolerance);

    double points[datapoints][dimensions]; // Array to store the data points

    // Read the data file and store the data points in the array
    FILE *data_file = fopen(argv[2], "r");
    
    for (int i = 0; i < datapoints; i++){
        for (int j = 0; j < dimensions; j++){
            fscanf(data_file, "%lf", &points[i][j]);
        }
    }
    fclose(data_file); // Close the file

    double centroids[clusters][dimensions]; // Array to store the centroids
   
    
    // Initialize the centroids randomly from the data points
    srand(time(0)); // Seed the random number generator with time
    for (int i = 0; i < clusters; i++){
        int random = rand() % datapoints;
        for (int j = 0; j < dimensions; j++){
            centroids[i][j] = points[random][j];
        }
    }

    
    double d_centroid[clusters]; // Array to store change in centroids
    for (int i = 0; i < clusters; i++){
        d_centroid[i] = tolerance + 1;
    }

    int cluster_assignment[datapoints];
    double distance[clusters];    

    while (someExceedsTol(d_centroid, clusters, tolerance)==1){
        // Assign each data point to a cluster
        for (int i = 0; i < datapoints; i++){
            for (int j = 0; j < clusters; j++){
                distance[j] = 0;
                for (int k = 0; k < dimensions; k++){
                    distance[j] += pow(points[i][k] - centroids[j][k], 2);
                }
            }
            cluster_assignment[i] = getMinIndex(distance, clusters);
        }
        double new_centroids[clusters][dimensions]; // Array to store the new centroids
        // Update the centroids
        for (int i = 0; i < clusters; i++){
            int count = 0;
            for (int j = 0; j < datapoints; j++){
                if (cluster_assignment[j]==i){
                    count++;
                    for (int k = 0; k < dimensions; k++){
                        new_centroids[i][k] += points[j][k];
                    }
                }
            }

            // Average the sums of cluster points for the new centroid locations
            for (int k = 0; k < dimensions; k++){
                new_centroids[i][k] /= count;
                if (count == 0){
                    printf("Cluster %d has no points\n", i);
                }
            }
        }

        // Calculate the change in centroids and assign the new centroids to the old centroids
        for (int i = 0; i < clusters; i++){
            d_centroid[i] = 0;
            for (int j = 0; j < dimensions; j++){
                d_centroid[i] += pow(centroids[i][j] - new_centroids[i][j], 2);
            }
            for (int j = 0; j < dimensions; j++){
                centroids[i][j] = new_centroids[i][j];
            }
        }
    }
    int cluster_count[clusters];
    // Print the final centroids
    for (int i = 0; i < clusters; i++){
        cluster_count[i] = 0;
        for (int k = 0; k < datapoints; k++){
            if (cluster_assignment[k] == i){
                cluster_count[i]++;
            }
        }
        printf("(%d of %d) points are in the cluster %d with centroid (", cluster_count[i], datapoints, i);
        for (int j = 0; j < dimensions; j++){
            printf("%.5lf ", centroids[i][j]);
        }
        printf(")\n");
    }

    // Create output file and write the cluster assignments
    FILE *output_file = fopen("output.dat", "w");
    for (int i = 0; i < datapoints; i++){
        fprintf(output_file, "%d ", cluster_assignment[i]);
        for (int j = 0; j < dimensions; j++){
            fprintf(output_file, "%.5lf ", points[i][j]);
        }
        fprintf(output_file, "\n");
        //fprintf(output_file, "%d %.5lf %.5lf\n", cluster_assignment[i], points[i][0], points[i][1]);
    }
    fclose(output_file); // Close the file
    // Create centroids file and write the centroids
    FILE *centroids_file = fopen("centroids.dat", "w");
    for (int i = 0; i < clusters; i++){
        for (int j = 0; j < dimensions; j++){
            fprintf(centroids_file, "%.5lf ", centroids[i][j]);
        }
        fprintf(centroids_file, "\n");
    }
    fclose(centroids_file); // Close the file

    return 0;
}

int getMinIndex(double arr[], int n){
    int min_index = 0;
    for (int i = 1; i < n; i++){
        if (arr[i] < arr[min_index]){
            min_index = i;
        }
    }
    return min_index;
}
int someExceedsTol(double arr[], int n, double tol){
    for (int i = 0; i < n; i++){
        if (arr[i] > tol){
            return 1;
        }
    }
    return 0;
}


