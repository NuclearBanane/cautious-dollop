// Controlling when a thread will execute compared against it's siblings
// examples has pthreads launched in a for loop.
// Without explicitly stating it, they will work in which ever order the OS sees fit
// However our implementation intends they run in a specific ordr

#include <pthread.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string.h>

struct task {
	int n;
	pthread_t *tid;
};

int fib(int i){
	return (i==1 || i ==0) ? i : fib(i-1) + fib(i-2);
}

void *Fib(void *data){
	task *i = ((task *) data);
	if(pthread_equal(*(i->tid),pthread_self())==0){
		pthread_join(*(i->tid),NULL);
	}
	int p = i->n;
	printf("Thread %d : Getting fib of %d\n", i->n, i->n);
	i->n = fib(i->n);
	printf("Thread %d : got fib of %d\n", p, i->n);
	pthread_exit(NULL);
}

int main(){
	pthread_t threads[10];
	task t[10];
	for(int i = 0; i < 10; i++){
		t[i].n = i;
		t[i].tid = (i!=0) ? &threads[i-1] : &threads[0] ;
		printf("Main : Requesting fib of %d\n", t[i].n);
		pthread_create(&threads[i], NULL, Fib, (void *)&t[i]);
	}
	pthread_join(threads[9],NULL);
	for(int i = 0; i < 10; i++)
		printf("Main : fib of %d is %d\n",i,t[i].n);
	return 0;	
}
