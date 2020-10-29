#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <omp.h>

// gcc main.c -o main -lpthread

int somat = 0;
int vetor[1500];
int controle_contagem = 0;
double temp_ini, temp_fim, tempt;

pthread_t newthread4;
pthread_mutex_t newthread5 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutexattr_t mutexattr_prioinherit;
int mutex_protocol;

void* soma1(void *valor){
	temp_ini = (double) clock();
	int i = 0;
	long aux = (long)valor;
	
	pthread_mutexattr_init(&mutexattr_prioinherit);
	pthread_mutexattr_getprotocol(&mutexattr_prioinherit, &mutex_protocol);
	if (mutex_protocol != PTHREAD_PRIO_INHERIT) { 	
	    pthread_mutexattr_setprotocol(&mutexattr_prioinherit, PTHREAD_PRIO_INHERIT); 	
	}
		
	#pragma omp parallel
	{
		while(controle_contagem == 1){
			pthread_mutex_lock(&newthread5);
			if(somat < 1500){
				sleep(0.001);
				i = rand() % 10+1;
				vetor[somat] = i;
				somat = somat + 1;
				printf("Produto de Número %d passando na esteira %ld\n", somat, aux);
			}else{
				controle_contagem = 0;
			}
			pthread_mutex_unlock(&newthread5);
		}
	}
	
	temp_fim = (double) clock();
	tempt = (double) temp_fim - temp_ini;
	pthread_exit(NULL);
}

void* controle(void * arg){
	pthread_t newthread1, newthread2, newthread3;
	int aux = 0, create;
	long num;

	pthread_mutex_init(&newthread5, NULL);
	while(1){
		if(controle_contagem == 0){
			controle_contagem = 1;
			num = 1;
			create = pthread_create(&newthread1, NULL, soma1, (void *)num);
			num = 2;
			create = pthread_create(&newthread2, NULL, soma1, (void *)num);
			num = 3;
			create = pthread_create(&newthread3, NULL, soma1, (void *)num);
		}
	
		pthread_join(newthread1, NULL);
		pthread_join(newthread2, NULL);
		pthread_join(newthread3, NULL);
	
				
		for(int a = 0; a<1500; a++){
			aux = aux + vetor[a];
		}
				
		printf("Soma total nas esteiras: %d\n", somat);
		printf("Soma dos pesos: %d\n", aux);
		printf("Tempo de execução função soma1: %.1lf\n", tempt);
		sleep(5);
		aux = 0;
		somat = 0;
		controle_contagem = 0;
	}
	return NULL;
}

int main(){
	srand(time(NULL));
	
	pthread_create(&newthread4, NULL, controle, NULL);
	
	pthread_join(newthread4, NULL);
}
