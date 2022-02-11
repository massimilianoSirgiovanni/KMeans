#include <stdio.h>
#include <math.h>
#include <time.h>

#define n 30 //Number of points
#define k 5 //Number of centroids

typedef struct Point{

        double x;
        double y;

} point;

typedef struct Cluster{

        int indexCluster;
        point centroid;
        point elements[n]; //Set of points that are contained in the cluster
        int indexElement; //Number of points in the cluster

} cluster;

int KMeans(point pts[], point centroids[]);
double EuclideanDistance(point a, point b);
double recomputesCentroidX(cluster cl);
double recomputesCentroidY(cluster cl);
int printClusters(cluster cl);

int main (){

    clock_t begin = clock();



    //Point initialization

    point p0 = {0, 4};
    point p1 = {3, 5};
    point p2 = {5, 5};
    point p3 = {6, 4};
    point p4 = {7, 3};
    point p5 = {6, 2};
    point p6 = {2, 3};
    point p7 = {8, 4};
    point p8 = {3, 3};
    point p9 = {2, 6};
    point p10 = {3, 2};
    point p11 = {2, 9};
    point p12 = {7, 3};
    point p13 = {1, 2};
    point p14 = {5, 9};
    point p15 = {9, 6};
    point p16 = {2, 4};
    point p17 = {8, 8};
    point p18 = {3, 1};
    point p19 = {2, 7};
    point p20 = {10, 3};
    point p21 = {4, 2};
    point p22 = {6, 6};
    point p23 = {1, 5};
    point p24 = {8, 9};
    point p25 = {8, 6};
    point p26 = {4, 7};
    point p27 = {6, 5};
    point p28 = {6, 10};
    point p29 = {9, 4};

    point pts[n] = {p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29};

    //Centroid initialization
    point k0 = {4, 2};
    point k1 = {7, 6};
    point k2 = {1, 8};
    point k3 = {10, 7};
    point k4 = {5, 3};


    point centroids[k] = {k0, k1, k2, k3, k4};

    ///////////////////////////////////////////////////

    int i;
    for (i=0; i<5; i++) {
        printf("\n//////////////////EXECUTION NUMBER %d///////////////\n", i);
        KMeans(pts, centroids); //Apply K-Means Algorithm to the points
        printf("\n////////////////////////////////////////////////////\n");
    }

    clock_t end = clock();
    double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;

    printf("time spent: %f\n", time_spent);


    return 0;

}

int KMeans(point pts[], point centroids[]){
    //Application of K-Means Algotiyhm

    cluster results[k];

    //Initialization of the clusters
    int i;
    for (i = 0; i<k; i++){
        results[i].indexCluster = i;
        results[i].centroid = centroids[i];
    }

    int j, nearestCentroid;
    double min, distance, x, y;
    int end = 0;

    while(end == 0){

        //Reset the number of elements in each cluster
        for (i = 0; i<k; i++){
            results[i].indexElement = 0;
        }

        //Iteration on all the points considered
        for(i = 0; i<n; i++){
            //Iteration on the first centroid (Needed to assign a value to the min variable)
            min = EuclideanDistance(pts[i], results[0].centroid);
            nearestCentroid = 0;
            //Iteration on the others centroids
            for(j = 1; j<k; j++){
                distance = EuclideanDistance(pts[i], results[j].centroid);
                if(distance < min){
                    min = distance;
                    nearestCentroid = j;
                }
            }
            //Append the point considered to "nearestCentroid" cluster
            results[nearestCentroid].elements[results[nearestCentroid].indexElement] = pts[i];
            results[nearestCentroid].indexElement = results[nearestCentroid].indexElement + 1;
        }

        //Verify if centroid don't change
        end = 1;
        for(j = 0; j < k; j++){
            x = recomputesCentroidX(results[j]);
            y = recomputesCentroidY(results[j]);
            if(x != results[j].centroid.x | y != results[j].centroid.y){
                end = 0;
                results[j].centroid.x = x;
                results[j].centroid.y = y;
            }

        }
    }

    //Print of the clusters found
    for(i = 0; i<k; i++){
        printClusters(results[i]);
    }

    return 0;

}

double EuclideanDistance(point a, point b){
    //Compute Euclidean between two points

    return sqrt(pow((a.x - b.x),2) + pow((a.y - b.y), 2));

}

double recomputesCentroidX(cluster cl){
    //Recomputation of x parameter for centroids

    int i;
    double x = 0;

    for(i = 0; i<cl.indexElement; i++){
        x = x + cl.elements[i].x;
    }

    return x/cl.indexElement;

}

double recomputesCentroidY(cluster cl){
    //Recomputation of y parameter for centroids

    int i;
    double y = 0;

    for(i = 0; i<cl.indexElement; i++){
        y = y + cl.elements[i].y;
    }

    return y/cl.indexElement;

}


int printClusters(cluster cl){
    //Printing method for a Cluster

    printf("\n-----------------------------------\n");
    printf("Cluster number %d\n", cl.indexCluster);
    printf("    Centroid: (%f, %f)\n", cl.centroid.x, cl.centroid.y);
    printf("    Points in cluster: ");
    int i;
    for(i = 0; i < cl.indexElement; i++){
        printf("(%f, %f) ", cl.elements[i].x, cl.elements[i].y);
    }
    printf("\n-----------------------------------\n");

    return 0;

}

