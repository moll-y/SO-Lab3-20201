/**
 * @defgroup   SAXPY saxpy
 *
 * @brief      This file implements an iterative saxpy operation
 * 
 * @param[in] <-p> {vector size} 
 * @param[in] <-s> {seed}
 * @param[in] <-n> {number of threads to create} 
 * @param[in] <-i> {maximum itertions} 
 *
 * @author     Danny Munera
 * @date       2020
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>

sem_t s;

double *X, *Y, *Y_avgs, a;
int p;

void * thread_a (void *arg)
{
    int *buf, min, max, i;

    buf = (int *) arg;
    min = buf[0];
    max = buf[1];
	for(; min < max; min++){
        for(i = 0; i < p; i++){
            Y[i] = Y[i] + a * X[i];
            Y_avgs[min] += Y[i];
            //printf("Y[%d] = %f\n", i, Y[i]);
            sem_post(&s);
        }
        Y_avgs[min] = Y_avgs[min] / p;
	}
    return NULL;
}

void * thread_b (void *arg)
{
    int *buf, min, max, i;

    buf = (int *) arg;
    min = buf[0];
    max = buf[1];
	for(; min < max; min++){
        for(i = 0; i < p; i++){
            sem_wait(&s);
            Y[i] = Y[i] + a * X[i];
            Y_avgs[min] += Y[i]; 
            //printf("Y[%d] = %f\n", i, Y[i]);
        }
        Y_avgs[min] = Y_avgs[min] / p;
	}
    return NULL;
}


int main(int argc, char* argv[]){
	// Variables to obtain command line parameters
	unsigned int seed = 1;
  	int n_threads = 2;
  	int max_iters = 10000;
    int i;
	// Variables to get execution time
	struct timeval t_start, t_end;
	double exec_time;

  	p = 100;

	// Getting input values
	int opt;
	while((opt = getopt(argc, argv, ":p:s:n:i:")) != -1){  
		switch(opt){  
			case 'p':  
			printf("vector size: %s\n", optarg);
			p = strtol(optarg, NULL, 10);
			assert(p > 0 && p <= 2147483647);
			break;  
			case 's':  
			printf("seed: %s\n", optarg);
			seed = strtol(optarg, NULL, 10);
			break;
			case 'n':  
			printf("threads number: %s\n", optarg);
			n_threads = strtol(optarg, NULL, 10);
			break;  
			case 'i':  
			printf("max. iterations: %s\n", optarg);
			max_iters = strtol(optarg, NULL, 10);
			break;  
			case ':':  
			printf("option -%c needs a value\n", optopt);  
			break;  
			case '?':  
			fprintf(stderr, "Usage: %s [-p <vector size>] [-s <seed>] [-n <threads number>]\n", argv[0]);
			exit(EXIT_FAILURE);
		}  
	}  
	srand(seed);

	printf("p = %d, seed = %d, n_threads = %d, max_iters = %d\n", \
	 p, seed, n_threads, max_iters);	

	// initializing data
	X = (double*) malloc(sizeof(double) * p);
	Y = (double*) malloc(sizeof(double) * p);
	Y_avgs = (double*) malloc(sizeof(double) * max_iters);

	for(i = 0; i < p; i++){
		X[i] = (double)rand() / RAND_MAX;
		Y[i] = (double)rand() / RAND_MAX;
	}
	for(i = 0; i < max_iters; i++){
		Y_avgs[i] = 0.0;
	}
	a = (double)rand() / RAND_MAX;

#ifdef DEBUG
	printf("vector X= [ ");
	for(i = 0; i < p-1; i++){
		printf("%f, ",X[i]);
	}
	printf("%f ]\n",X[p-1]);

	printf("vector Y= [ ");
	for(i = 0; i < p-1; i++){
		printf("%f, ", Y[i]);
	}
	printf("%f ]\n", Y[p-1]);

	printf("a= %f \n", a);	
#endif

	/*
	 *	Function to parallelize 
	 */

    pthread_t t1, t2;
    int *buf, m;

    sem_init (&s, 0, 0);
    m = max_iters / n_threads;

	gettimeofday(&t_start, NULL);
	//SAXPY iterative SAXPY mfunction

    buf = (int *) malloc(sizeof(int)*2);
    buf[0] = 0;
    buf[1] = m;
    pthread_create (&t1, NULL, thread_a, (void*) buf);

    buf = (int *) malloc(sizeof(int)*2);
    buf[0] = m;
    buf[1] = 2*m;
    pthread_create (&t2, NULL, thread_b, (void*) buf);

    pthread_join (t1, NULL);
    pthread_join (t2, NULL);

	gettimeofday(&t_end, NULL);

#ifdef DEBUG
	printf("RES: final vector Y= [ ");
	for(i = 0; i < p-1; i++){
		printf("%f, ", Y[i]);
	}
	printf("%f ]\n", Y[p-1]);
#endif
	
	// Computing execution time
	exec_time = (t_end.tv_sec - t_start.tv_sec) * 1000.0;  // sec to ms
	exec_time += (t_end.tv_usec - t_start.tv_usec) / 1000.0; // us to ms
	printf("Execution time: %f ms \n", exec_time);
	printf("Last 3 values of Y: %f, %f, %f \n", Y[p-3], Y[p-2], Y[p-1]);
	printf("Last 3 values of Y_avgs: %f, %f, %f \n", Y_avgs[max_iters-3], Y_avgs[max_iters-2], Y_avgs[max_iters-1]);
	return 0;
}	
