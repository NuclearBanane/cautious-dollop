// Controlling when a thread will execute compared against it's siblings
// examples has pthreads launched in a for loop.
// Without explicitly stating it, they will work in which ever order the OS sees fit
// However our implementation intends they run in a specific ordr

#include <pthread.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <map>
#include <semaphore.h>

struct task {
	uint n;
	pthread_t *tid;
};

int fib(uint i){
	return (i==1 || i ==0) ? i : fib(i-1) + fib(i-2);
}

void *Fib(void *data){
	task *i = ((task *) data);
	uint p = i->n;
	printf("Thread %d : Getting fib of %d\n", i->n, i->n);
	i->n = fib(i->n);
	printf("Thread %d : got fib of %d\n", p, i->n);
	pthread_exit(NULL);
}

int main(){
	pthread_t threads[10];
	task t[10];
	for(uint i = 0; i < 10; i++){
		t[i].n = i;
		t[i].tid = (i+1 < 10) ? &threads[i+1] : &threads[10] ;
		printf("Main : Requesting fib of %d\n", t[i].n);
		pthread_create(&threads[i], NULL, Fib, (void *)&t[i]);
	}
	for(uint i = 0; i < 10; i++){
		pthread_join(threads[i],NULL);
		printf("Main : Thread %d - Returned %d\n",i, t[i].n);
	}
	pthread_exit(NULL);
}
