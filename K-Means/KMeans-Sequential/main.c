#include <stdio.h>
#include <math.h>
#include <time.h>

#define n 700 //Number of points
#define k 150 //Number of centroids
//#define DEBUG

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
int MinCentroid(point Point, point centroids[]);
double EuclideanDistance(point a, point b);
double recomputesCentroidX(cluster cl);
double recomputesCentroidY(cluster cl);
int printClusters(cluster cl);
point initializePoint(int a, int b);

int main (){

    clock_t begin = clock();

    //Point initialization
    int p, q;
    point points [n];
    point centroids [k];

    clock_t PointInitialization = clock();

    for (p = 0; p<n; p++) {

        points[p] = initializePoint(p, fabs(n - p));
    }

    for (q = 0; q<k; q++){
        centroids[q] = initializePoint(q * k % (n + 1), (int) fabs(n - q * k) % (n + 1));
    }

    ///////////////////////////////////////////////////

    printf("//////////////////START EXECUTION///////////////\n\n");
    KMeans(points, centroids); //Apply K-Means Algorithm to the points
    printf("\n////////////////////////////////////////////////////\n");

    clock_t end = clock();
    double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;

    printf("time spent: %f\n", time_spent);


    return 0;

}

int KMeans(point pts[], point centroids[]){
    //Application of K-Means Algotiyhm
#ifdef DEBUG
    printf("STARTING K-MEANS...\n");
#endif

    cluster results[k];

    //Initialization of the clusters
    int i;

    for (i = 0; i<k; i++){
        results[i].indexCluster = i;
        results[i].centroid = centroids[i];
    }

    int j, nearestCentroid;
    double x, y;
    int end = 0;

    while(end == 0){

        end = 1;
        //Reset the number of elements in each cluster
        for (i = 0; i<k; i++){
            results[i].indexElement = 0;
        }

        //Iteration on all the points considered

        for(i = 0; i<n; i++){
#ifdef DEBUG
            printf("Verifing point %d...\n", i);
#endif
            nearestCentroid = MinCentroid(pts[i], centroids);
            //Append the point considered to "nearestCentroid" cluster
            results[nearestCentroid].elements[results[nearestCentroid].indexElement] = pts[i];
            results[nearestCentroid].indexElement += 1;
#ifdef DEBUG
            //printf("Point %d will go in %d cluster\n", i, nearestCentroid);
#endif
        }

        //Verify if centroid don't change
#ifdef DEBUG
        printf("\nRecomputing centroids...\n\n");
#endif

        for(j = 0; j < k; j++){
            x = recomputesCentroidX(results[j]);
            y = recomputesCentroidY(results[j]);
#ifdef DEBUG
            printf("Values for cluster %d's centroid: (%f, %f) --> (%f, %f)\n",j,  results[j].centroid.x, results[j].centroid.y,  x, y);
#endif
            if(results[j].indexElement != 0 & (x != results[j].centroid.x | y != results[j].centroid.y)){
                end = 0;
                results[j].centroid.x = x;
                results[j].centroid.y = y;
            }

        }
#ifdef DEBUG
        printf("\n Iteration ending... \n\n");
#endif
        for (i = 0; i < k; i++) {
            centroids[i] = results[i].centroid;
        }

    }



    //Print of the clusters found
    for(i = 0; i<k; i++){
        printClusters(results[i]);
    }

    return 0;

}

int MinCentroid(point Point, point centroids[]){
    int j, nearestCentroid = 0;
    double distance, min = EuclideanDistance(Point, centroids[0]);
    //Iteration on the first centroid done (Needed to assign a value to the min variable)
    for(j = 1; j<k; j++) {
        //Iteration on the others centroids
        distance = EuclideanDistance(Point, centroids[j]);
        if (distance < min) {
            min = distance;
            nearestCentroid = j;
        }

    }

    //printf("minValue: %f\n", min);

    return nearestCentroid;
}

double EuclideanDistance(point  a, point b){
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

point initializePoint(int a, int b){
    point p = {a, b};
    return p;
}

