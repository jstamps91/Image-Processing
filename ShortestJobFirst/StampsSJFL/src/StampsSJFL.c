/*
 * Project: StampsSJFL
 * File: StampsSJFL.c (Program that simulates Shortest-Job First/SJFL Algorithm).
 *
 *  Created on: November 30, 2019
 *  Author: Joshua Stamps
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[]) {

	//Define object of type FILE.
	FILE *fp;
	char arr[100];

	//Get file name from command-line argument
	strcpy(arr, argv[1]);

	//Open the file.
	fp = fopen(arr, "r");

	int no_proc; //number of processes
	int no_ticks; //number of ticks
	int pid = 0; //process ID

	//Read the data from the file.
	fscanf(fp, "%d", &no_ticks);
	fscanf(fp, "%d", &no_proc);

	int *tau = (int*) malloc(no_proc * sizeof(int));
	int *alpha = (int*) malloc(no_proc * sizeof(int));
	int *proc[no_proc];
	int i = 0;

	for (i = 0; i < no_proc; i++) {
		
		proc[i] = (int*) malloc(no_ticks * sizeof(int));
	}

	//allocate memory for current process
	int *s_curr_proc = (int*) malloc(no_proc * sizeof(int));
	int *s_proc_no = (int*) malloc(no_proc * sizeof(int));

	int process = 0; //process number
	int *tau_time[no_proc];
	i = 0;

	for (i = 0; i < no_proc; i++) {
		
		tau_time[i] = (int*) malloc(no_ticks * sizeof(int));
	}

	while (process < no_proc) {
		
		int cur_tau;
		float cur_alpha;

		fscanf(fp, "%d", &pid);
		fscanf(fp, "%d", &cur_tau);
		fscanf(fp, "%f", &cur_alpha);

		tau[process] = cur_tau;
		alpha[process] = cur_alpha;
		int k = 0;

		for (k = 0; k < no_ticks; k++) {

			int p_time;

			fscanf(fp, "%d", &p_time);

			proc[process][k] = p_time;

			if (k == 0) {

				tau_time[process][k] = cur_tau;
			}
			else {

				tau_time[process][k] = (cur_alpha * proc[process][k - 1]
						+ ((1 - cur_alpha) * tau_time[process][k - 1]));
			}
		}
		process++;
	}

	int *proc1[no_proc];

	for (i = 0; i < no_proc; i++) {

		proc1[i] = (int*) malloc(no_ticks * sizeof(int));
	}

	int j = 0;

	for (i = 0; i < no_proc; i++) {

		for (j = 0; j < no_ticks; j++) {

			proc1[i][j] = proc[i][j];
		}
	}
	fclose(fp);

	int l = 0;
	int tot_time = 0;
	int ta_time = 0;
	int wait_time = 0;

	//Shortest job first simulation
	printf("--Shortest-Job-First--\n");

	//SJF algorithm
	for (l = 0; l < no_ticks; l++) {

		printf("\nSimulating %dth tick of process @ time %d: \n", l, tot_time);

		int tick_time = 0;
		int p;
		int q = 0;
		
		for (p = 0; p < no_proc; p++) {

			int small = 100;
			int small_id = 0;

			for (q = 0; q < no_proc; q++) {

				if (small > proc[q][l]) {

					small = proc[q][l];
					small_id = q;
				}
			}

			s_curr_proc[p] = small;
			s_proc_no[p] = small_id;
			proc[small_id][l] = 100;
		}

		int z = 0;

		for (z = 0; z < no_proc; z++) {

			printf(" Process %d took %d\n", s_proc_no[z], s_curr_proc[z]);

			ta_time = ta_time + tick_time + s_curr_proc[z];
			tot_time = tot_time + s_curr_proc[z];
			wait_time = wait_time + tick_time;
			tick_time = tick_time + s_curr_proc[z];
		}
	}

	printf("\nTurnaround time: %d\n", ta_time);
	printf("Waiting time: %d\n\n", wait_time);

	tot_time = 0;
	ta_time = 0;
	wait_time = 0;

	int est_err = 0;//initialize estimated error to 0

	//Shortest job first live simulation
	printf("--Shortest-Job-First Live--\n");

	for (l = 0; l < no_ticks; l++) {

		printf("\nSimulating %dth tick of process @ time %d: \n", l, tot_time);

		int tick_time = 0;
		int p;
		int q = 0;

		for (p = 0; p < no_proc; p++) {

			int small = 100;
			int small_id = 0;

			for (q = 0; q < no_proc; q++) {

				if (small > tau_time[q][l]) {

					small = tau_time[q][l];
					small_id = q;
				}
			}

			s_curr_proc[p] = small;
			s_proc_no[p] = small_id;
			tau_time[small_id][l] = 100;
		}

		int z = 0;

		for (z = 0; z < no_proc; z++) {

			int cur_error = 0; //initialize estimated error to 0

			printf(" Process %d was estimated for %d but took %d\n",
					s_proc_no[z], s_curr_proc[z], proc1[s_proc_no[z]][l]);

			if (proc1[s_proc_no[z]][l] > s_curr_proc[z]) {

				cur_error = proc1[s_proc_no[z]][l] - s_curr_proc[z];
			}
			else {

				cur_error = s_curr_proc[z] - proc1[s_proc_no[z]][l];
			}

			ta_time = ta_time + tick_time + proc1[s_proc_no[z]][l];
			tot_time = tot_time + proc1[s_proc_no[z]][l];
			wait_time = wait_time + tick_time;
			tick_time = tick_time + proc1[s_proc_no[z]][l];
			est_err = est_err + cur_error;
		}
	}

	printf("\nTurnaround time: %d \n", ta_time);
	printf("Waiting time: %d \n", wait_time);
	printf("Estimation Error: %d\n", est_err);


	return 0;

}
