//Daniel Herbert
//251137828
//CS 3305A Assignment 3
//Due Date: October 25th 2022
//All include statements
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
//Initializing global variables
int port[2];
int status;
int sizeOfString;
int subSize;

//Function executed by Thread 1 to perform subtraction
void *subtract(void *thread_id) {
        int threadID = 1;
        char reading[50];
	//Thread 1 reads X and Y from pipe
        read(port[0], &reading, sizeOfString);
	//Split what was read from pipe by delimiter and cast to integers
	const char s[2] = "-";
	char *num1;
	char *num2;
	num1 = strtok(reading, s);
	num2 = strtok(NULL, "-");
	int castNum1 = atoi(num1);
	int castNum2 = atoi(num2);
	//X - Y
	int sub = castNum1 - castNum2;
	printf("thread (TID %d) reads X = %d and Y = %d from the pipe\n", threadID, castNum1, castNum2);
	char pipeSub[50];
	snprintf(pipeSub, 10, "%d", sub);
	subSize = strlen(pipeSub);
	//Thread 1 writes S into pipe
	write(port[1], pipeSub, subSize);
	printf("thread (TID %d) writes X-Y = %d to the pipe\n", threadID, sub);


}

//Function executed by Thread 2 to determine if number is prime or not
void *prime_check(void *thread_id) {
        int threadID = 2;
	char reading2[50];
	//Thread 2 reads S from pipe
	read(port[0], &reading2, subSize);
	reading2[subSize] = '\0';
	printf("thread (TID %d) reads X-Y = %s from the pipe\n", threadID, reading2);
	int castSub = atoi(reading2);
	//Thread 2 determines if number read from pipe is prime or not
	int i, m, check;

	m = castSub / 2;

	for (i = 2; i <= m; i++) {
		if (castSub % i == 0) {
			check = 1;
			break;	
		}	

	}

	if (check == 0) {
		printf("thread (TID %d) identifed that %d is a prime number\n", threadID, castSub);
	}
	else {
		printf("thread (TID %d) identified that %d is NOT a prime number\n", threadID, castSub);
	}
	//Thread 2 writes S back into pipe
	write(port[1], reading2, subSize);
	printf("thread (TID %d) writes %s to the pipe\n", threadID, reading2);
} 

//Function executed by Thread 3 to reverse a number
void *reverse_num(void *thread_id) {
        int threadID = 3;
	char reading3[50];
	//Thread 3 reads S from pipe
	read(port[0], &reading3, subSize);
	reading3[subSize] = '\0';
	printf("thread (TID %d) reads X-Y = %s from the pipe\n", threadID, reading3);
	int reverse = 0, remainder;
	int castNum = atoi(reading3);
	//Thread 3 reverses number
	while (castNum != 0) {
		remainder = castNum % 10;
		reverse = reverse * 10 + remainder;
		castNum /= 10;


	}

	printf("thread (TID %d) reversed number is %d\n", threadID, reverse);


}

int main(int argc, char *argv[]) {
	//Checking if correct number of command line arguments are passed
	if (argc < 3) {
                printf("Not enough arguments provided.\n");
                exit(1);
        }

        if (argc > 3) {
                printf("Too many arguments provided.\n");
		exit(1);
	}
 
	int numArg;
	char * arg1;
	char * arg2;
	arg1 = argv[1];
        arg2 = argv[2];
	//Casting command line arguments from strings to integers
	int castArg1 = atoi(arg1);
	int castArg2 = atoi(arg2);
	pid_t parentID;
	pthread_t thread_1, thread_2, thread_3;
	arg1 = argv[1];
	arg2 = argv[2];
	//Checking if Argument 1 is larger than Argument 2
	if (castArg1 < castArg2) {
		printf("Argument 1 must be larger than Argument 2.\n");
		exit(1);
	}
	//Checking if both Arguments are greater than 0
	if (castArg1 < 0 || castArg2 < 0) {
		printf("Both arguments must be greater than 0.\n");
		exit(1);
	}
	//Creating pipe
	status = pipe(port);
	if (pipe(port) < 0) {
        	perror("pipe error");
        	exit(0);
	}
	//Parent print statements
	parentID = getpid();
	printf("parent (PID %d) receives X = %d and Y = %d from the user\n", parentID, castArg1, castArg2);
	sizeOfString = strlen(arg1) + strlen(arg2) + 1;
	char conc[100];
	//Concatenating two command line arguments with delimiter between before writing into pipe
	strcpy(conc, argv[1]);
	strcat(conc, "-");
	strcat(conc, argv[2]);
	//Parent writing X and Y into pipe
	write(port[1], conc,  sizeOfString);
	int i = 1, j = 2, k = 3;
	printf("parent (PID %d) writes X = %d and Y = %d to the pipe\n", parentID, castArg1, castArg2);	
	//Creating Thread 1
	pthread_create(&thread_1, NULL, subtract, &i);
	pthread_join(thread_1, NULL);
	//Creating Thread 2
	pthread_create(&thread_2, NULL, prime_check, &j);
	pthread_join(thread_2, NULL);
	//Creating Thread 3
	pthread_create(&thread_3, NULL, reverse_num, &k);
	pthread_join(thread_3, NULL);


}

