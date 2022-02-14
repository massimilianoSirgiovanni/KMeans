#include <stdio.h>
#include <math.h>
#include <time.h>
#include <omp.h>

#define n 1000 //Number of points
#define k 100 //Number of centroids
//#define DEBUG

typedef struct points{

    double x;
    double y;
    int cluster

}point;

int KMeans(point pts[], point centroids[]);
int MinCentroid(point Point, point centroids[]);
double EuclideanDistance(point a, point b);
double recomputesCentroidX(int centroid, point pts[], double def);
double recomputesCentroidY(int centroid, point pts[], double def);
int printClusters(int centroidID, point centroid, point pts[]);
point initializePoint(int a, int b);

int main (){

    double itime, ftime, exec_time;
    itime = omp_get_wtime();

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

    ftime = omp_get_wtime();
    exec_time = ftime - itime;
    printf("\n\nTime taken is %f", exec_time);


    return 0;

}

int KMeans(point pts[], point centroids[]){
    //Application of K-Means Algotiyhm
#ifdef DEBUG
    printf("STARTING K-MEANS...\n");
#endif

    //cluster results[k];

    //Initialization of the clusters
    int i;

    //for (i = 0; i<k; i++){
    //results[i].indexCluster = i;
    //results[i].centroid = centroids[i];
    //}

    int j, nearestCentroid;
    double x, y;
    int end = 0;

    while(end == 0){

        end = 1;
        //Reset the number of elements in each cluster
        //for (i = 0; i<k; i++){
        //results[i].indexElement = 0;
        //}

        //Iteration on all the points considered

        for(i = 0; i<n; i++){
#ifdef DEBUG
            printf("Verifing point %d...\n", i);
#endif
            nearestCentroid = MinCentroid(pts[i], centroids);
            //Append the point considered to "nearestCentroid" cluster
            pts[i].cluster = nearestCentroid;
            //results[nearestCentroid].elements[results[nearestCentroid].indexElement] = i;
            //results[nearestCentroid].indexElement += 1;
#ifdef DEBUG
            //printf("Point %d will go in %d cluster\n", i, nearestCentroid);
#endif
        }

        //Verify if centroid don't change
#ifdef DEBUG
        printf("\nRecomputing centroids...\n\n");
#endif

        for(j = 0; j < k; j++){
            x = recomputesCentroidX(j, pts, centroids[j].x);
            y = recomputesCentroidY(j, pts, centroids[j].y);
#ifdef DEBUG
            printf("Values for cluster %d's centroid: (%f, %f) --> (%f, %f)\n",j,  centroids[j].x, centroids[j].y,  x, y);
#endif
            if(x != centroids[j].x | y != centroids[j].y){
                end = 0;
                centroids[j].x = x;
                centroids[j].y = y;
            }

        }
#ifdef DEBUG
        printf("\n Iteration ending... \n\n");
#endif

    }



    //Print of the clusters found
    for(i = 0; i<k; i++){
        printClusters(i, centroids[i], pts);
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

double recomputesCentroidX(int centroid, point pts[], double def){
    //Recomputation of x parameter for centroids

    int i, count = 0;
    double x = 0;
    for(i = 0; i<n; i++){
        if(pts[i].cluster == centroid) {
            x = x + pts[i].x;
            count++;
        }

    }
    if (count == 0){
        return def;
    }
    return x/count;

}

double recomputesCentroidY(int centroid, point pts [], double def){
    //Recomputation of y parameter for centroids

    int i, count;
    double y = 0;
    for(i = 0; i<n; i++) {
        if (pts[i].cluster == centroid) {
            y = y + pts[i].y;
            count++;
        }

    }
    if (count == 0){
        return def;
    }

    return y / count;

}


int printClusters(int centroidID, point centroid, point pts[]){
    //Printing method for a Cluster

    printf("\n-----------------------------------\n");
    printf("Cluster number %d\n", centroidID);
    printf("    Centroid: (%f, %f)\n", centroid.x, centroid.y);
    printf("    Points in cluster: ");
    int i;
    for(i = 0; i < n; i++){
        if(pts[i].cluster == centroidID) {
            printf("(%f, %f) ", pts[i].x, pts[i].y);
        }
    }
    printf("\n-----------------------------------\n");

    return 0;

}

point initializePoint(int a, int b){
    point p = {a, b, -1};
    return p;
}

