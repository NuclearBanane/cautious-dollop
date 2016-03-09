#include <pthread.h>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <map>
#include <semaphore.h>  /* Semaphore */

//smallest to first wire.
std::vector<sem_t> mutex;
std::vector<int> wire;
std::map<int,sem_t*> wires_in_use;

void * Comparator(void * data){
	int *wires = (int *) data;
	sem_t task, wire1, wire2;
	sem_init(&task,0,1);
	sem_wait(&task);
	wire1 = *wires_in_use[wires[0]];
	wires_in_use[wires[0]] = &task;
	wire2 = *wires_in_use[wires[1]];
	wires_in_use[wires[1]] = &task;
	printf("	Waiting on %d for %d, %d \n", wire[wires[0]],wire[wires[0]], wire[wires[1]]);
	sem_wait(&wire1);
	printf("	Waiting on %d for %d, %d \n", wire[wires[1]],wire[wires[0]], wire[wires[1]]);
	sem_wait(&wire2);
	printf("	Running %d, %d \n", wire[wires[0]], wire[wires[1]]);
	if(wire[wires[0]] > wire[wires[1]]){
		int temp = wire[wires[0]];
		wire[wires[0]] = wire[wires[1]];
		wire[wires[0]] = temp;
		printf("	Crossing %d, %d \n", wire[wires[0]],wire[wires[1]]);
	} else {
		printf("	Leaving %d, %d \n", wire[wires[0]],wire[wires[1]]);
	}
	sem_post(&task);
}

int main(){
	std::ifstream network;
	network.open("net.txt");
	std::ifstream inputs;
	inputs.open("values");

	std::string output;
	pthread_t *threads;
	int wires = -1;
	int comparators = -1;
	if (network.is_open()){
		if(!network.eof()){
			getline(network,output);
			char* outter = const_cast<char*>(output.c_str());
			outter = strtok(outter, " ");
			wires = std::stoi(outter);
			comparators = std::stoi(strtok (NULL, " "));
			printf("Wires : %d - Comparators : %d\n", wires, comparators);
			/*if(inputs.is_open()){
				for(int i = 0; !inputs.eof();i++){
					getline(network,output);
					printf("%s\n", output.c_str());
					char* outter = const_cast<char*>(output.c_str());
					wire.push_back(std::stoi(outter));
				}
			}*/
			for(int i =0; i < wires; i++){
				sem_t t;
				sem_init(&t,0,1);
				wire.push_back(i);
				wires_in_use[i] = &t;
			}
			threads = new pthread_t[comparators];
		} else {
			return 1;
		}
		int i = 0;
		while (!network.eof()) {
			getline(network,output);
			if(output.c_str()[0]=='\0'){
				printf("End of file \n");
				break;
			}
			char* outter = const_cast<char*>(output.c_str());
			int wires[2];
			outter = strtok(outter, " ");
			wires[0] = std::stoi(outter);
			wires[1] = std::stoi(strtok (NULL, " "));
			printf("Launching thread %d, %d\n",wires[0],wires[1]);
			pthread_create(&threads[i], NULL, Comparator, (void *)wires);
			i++;
		}
	}
	if(comparators > 0)
		for(int i = 0; i<comparators; i++)
			pthread_join(threads[i],NULL);
	for(int i = 0; i<wire.size();i++)
		printf("%d\n", wire[i]);
	//network.close();
	return 0;
}
