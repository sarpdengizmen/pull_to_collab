/*
A c program that implements the k-measn algorithm using 
lloyd algorithm for updating the centroids.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

clock_t start, init, end, algorithm;
double cpu_time_used, init_time, algorithm_time, writing_time;

int getMinIndex(double arr[], int n);
int someExceedsTol(double arr[], int n, double tol);


int main(int argc, char *argv[]){
    /* Function takes two arguments first is the input file name 
    second is the data file name*/
    start = clock();
    
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
   
    
/*     // Initialize the centroids randomly from the data points
    srand(time(0)); // Seed the random number generator with time
    for (int i = 0; i < clusters; i++){
        int random = rand() % datapoints;
        for (int j = 0; j < dimensions; j++){
            centroids[i][j] = points[random][j];
        }
    } */

    // First centroid is chosen randomly
    srand(time(0)); // Seed the random number generator with time
    int random = rand() % datapoints;
    for (int j = 0; j < dimensions; j++){
            centroids[0][j] = points[random][j];
    }
    for (int i = 1; i < clusters; i++){ // For the rest of the centroids
        // Compute distances of all points to all centroids
        double* distances = calloc(datapoints, sizeof(double));
        for (int j = 0; j < datapoints; j++){ // For each point
            double min_distance = 1000000000;
            for (int k = 0; k < i; k++){ // For each already chosen centroid
                double distance = 0;
                for (int l = 0; l < dimensions; l++){
                    distance += pow(points[j][l] - centroids[k][l], 2); // Calculate the distance of the point to the centroid
                }
                if (distance < min_distance){
                    min_distance = distance;
                }
            }
            distances[j] = min_distance; // Stores the minimum distance of the point to the already chosen centroids
        }
        // Choose the next centroid which is the point with the maximum minimum distance
        int max_index = 0;  
        for (int j = 1; j < datapoints; j++){
            if (distances[j] > distances[max_index]){
                max_index = j;
            }
        }
        for (int j = 0; j < dimensions; j++){
            centroids[i][j] = points[max_index][j];
        }
        free(distances);
    }

    
    double d_centroid[clusters]; // Array to store change in centroids
    for (int i = 0; i < clusters; i++){
        d_centroid[i] = tolerance + 1;
    }

    int cluster_assignment[datapoints];
    double distance[clusters];    

    init = clock();

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
    algorithm = clock();
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
        fprintf(output_file, "%d %d ", i, cluster_assignment[i]);
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
    end = clock();
    algorithm_time = ((double) (algorithm - init)) / CLOCKS_PER_SEC * 1000;
    printf("Algorithm time: %f \n", algorithm_time);
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


