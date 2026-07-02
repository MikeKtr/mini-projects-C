#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>
#include <time.h>

double F(double x){
	return 4.0/(x*x + 1);
}

int main(int argc, char* argv[]){
	if(argc != 3){
		return -1;
	}
	
	double h = atof(argv[1]);
	int n = atoi(argv[2]);

	pid_t PID;
	int x = 0;
	for(int k = 1 ; k <= n ; k++){
		struct timespec start, end;
		int fds[k][2];
		clock_gettime(CLOCK_MONOTONIC, &start);
		for(int i = 0 ; i < k ; i++){
			x = pipe(fds[i]);
			PID = fork();
			if(PID == 0){
				double part_sum = 0;
				
				close(fds[i][0]);
				
				double a = (double)i / k;
				double b = (double)(i + 1) / k;
				double j;
				for(j = a ; j + h <= b ; j+=h){
					part_sum += F((j+j+h)/2) * h;
				}
				part_sum +=  F((j + b) / 2.0) * (b - j);
				
				x = write(fds[i][1],&part_sum,sizeof(double));
				exit(0);
			}
			else{
				close(fds[i][1]);
			}
		}

		for(int i = 0; i < k ; i++){
			wait(NULL);
		}

		double sum = 0;
		for(int i = 0 ; i < k ; i++){
			double part;
			x = read(fds[i][0],&part,sizeof(double));
			sum+=part;
			close(fds[i][0]);
		}
		clock_gettime(CLOCK_MONOTONIC, &end);
		double time_spent = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
		printf("Dla k = %d wynik calki to: %f \n", k, sum, time_spent);
	}
	printf("%d",x);
	return 0;

}

