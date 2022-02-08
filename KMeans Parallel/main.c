#include <stdio.h>
#include <math.h>
#include <time.h>
#ifdef _OPENMP
#include <omp.h> // for OpenMP library functions
#endif

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
point initializePoint(int a, int b);

int main (){

    clock_t begin = clock();


    //Point initialization

    point p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29;

//Centroid initialization
    point k0, k1, k2, k3, k4;

#pragma omp parallel
    {
#pragma omp sections
        {
#pragma omp section
        {
            p0 = initializePoint(0, 4);
            p1 = initializePoint(3, 5);
            p2 = initializePoint(5, 5);
            p3 = initializePoint(6, 4);
            p4 = initializePoint(7, 3);

            printf("Thread number %d completed section 0\n", omp_get_thread_num());
        }
#pragma omp section
        {
            p5 = initializePoint(6, 2);
            p6 = initializePoint(2, 3);
            p7 = initializePoint(8, 4);
            p8 = initializePoint(3, 3);
            p9 = initializePoint(2, 6);
            printf("Thread number %d completed section 1\n", omp_get_thread_num());
        }
#pragma omp section
        {
            p10 = initializePoint(3, 2);
            p11 = initializePoint(2, 9);
            p12 = initializePoint(7, 3);
            p13 = initializePoint(1, 2);
            p14 = initializePoint(5, 9);
            printf("Thread number %d completed section 2\n", omp_get_thread_num());
        }
#pragma omp section
        {
            p15 = initializePoint(9, 6);
            p16 = initializePoint(2, 4);
            p17 = initializePoint(8, 8);
            p18 = initializePoint(3, 1);
            p19 = initializePoint(2, 7);
            printf("Thread number %d completed section 3\n", omp_get_thread_num());
        }

#pragma omp section
        {
            p20 = initializePoint(10, 3);
            p21 = initializePoint(4, 2);
            p22 = initializePoint(6, 6);
            p23 = initializePoint(1, 5);
            p24 = initializePoint(8, 9);
            printf("Thread number %d completed section 4\n", omp_get_thread_num());
        }

#pragma omp section
        {
            p25 = initializePoint(8, 6);
            p26 = initializePoint(4, 7);
            p27 = initializePoint(6, 5);
            p28 = initializePoint(6, 10);
            p29 = initializePoint(9, 4);
            printf("Thread number %d completed section 5\n", omp_get_thread_num());
        }

#pragma omp section
        {
            k0 = initializePoint(4, 2);
            k1 = initializePoint(7, 6);
            k2 = initializePoint(1, 8);
            k3 = initializePoint(10, 7);
            k4 = initializePoint(5, 3);
            printf("Thread number %d completed section 6\n", omp_get_thread_num());
        }
    }
}

    point pts[n] = {p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28, p29};
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

///////////////////////////////////////////////////////////////////////

int KMeans(point pts[], point centroids[]){
    //Application of K-Means Algotiyhm

    cluster results[k];

    //Initialization of the clusters
    int i;

#pragma omp parallel for
        for (i = 0; i < k; i++) {
            results[i].indexCluster = i;
            results[i].centroid = centroids[i];
        }

    int j, nearestCentroid;
    double min, distance, x, y;
    int end = 0;

    while(end == 0){

        end = 1;
        omp_lock_t lock;
        omp_init_lock(&lock);

#pragma omp parallel
        {

            //Reset the number of elements in each cluster
#pragma omp for
            for (i = 0; i < k; i++) {
                results[i].indexElement = 0;
            }
            //Implicit barrier (needed because in the other loop is used the variable indexElement


            //Iteration on all the points considered

#pragma omp for schedule(dynamic) private(min, nearestCentroid, distance)
            for (i = 0; i < n; i++) {
                //Iteration on the first centroid (Needed to assign a value to the min variable)
                omp_set_lock(&lock);
                min = EuclideanDistance(pts[i], results[0].centroid);
                omp_unset_lock(&lock);
                nearestCentroid = 0;
                //Iteration on the others centroids
                for (j = 1; j < k; j++) {
                    distance = EuclideanDistance(pts[i], results[j].centroid);
                    if (distance < min) {
                        min = distance;
                        nearestCentroid = j;
                        //The purpose of this second for loop is to return a single cluster in which to put the node.
                        // Parallelizing it would risk getting different results for each thread
                    }

                }
                //Append the point considered to "nearestCentroid" cluster
#pragma omp critical(UpdareCluster)
                {
                    results[nearestCentroid].elements[results[nearestCentroid].indexElement] = pts[i];
                    results[nearestCentroid].indexElement = results[nearestCentroid].indexElement + 1;
                }
            }
            //Implicit barrier

            //Verify if centroid don't change

#pragma omp for private(x, y)
            for (j = 0; j < k; j++) {
                x = recomputesCentroidX(results[j]);
                y = recomputesCentroidY(results[j]);
                if (x != results[j].centroid.x | y != results[j].centroid.y) {
                    end = 0;
                    results[j].centroid.x = x;
                    results[j].centroid.y = y;
                }

            }

        }
        omp_destroy_lock(&lock);


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
    for (i = 0; i < cl.indexElement; i++) {
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
#pragma omp parallel for
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

