/* YOUR FILE-HEADER COMMENT HERE 
@Author Sean H Mahoney (smahone1@umbc.edu)
*This file is for project 1
*
*compile with:    make  
* 
*version: 1.0
*due tuesday 5/7/2019
*/


/*
starting version 2
parrelelization

deal with all unneccery comments.

*/




//#include <omp.h>
//#include <stdio.h>
#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include<stdlib.h>
#include <string.h>

//#include <stdio.h>
//#define MAXSIZE 1000
#include<time.h>


#include <ctype.h>
//
  
#include <stdbool.h>

//false


#define NUM_PEGS 4
#define NUM_COLORS 10
//10
//6

#define TOTAL  10000
//10000
//numbers 0-9999




//
#define MAXSIZE  5
#define SEQUENCE 5

#define OUTCOMES 14


//prints result of checknum for guess,and process guess
int debug1 = 0;
//suggested guess
int debug2 = 0;
//after still valid
int debug3 = 0;

//sequence
int debug4 = 1;
//parrelel
int debug5 = 1;

/*
 *
 *Function takes a string pointer and a number, and changes the string to correspond to a 4 digit value of the number e.x. 7 is "0007" 
 */

void asChars(char* asString, int number){
	if(number < 10){
		sprintf(asString, "000%d", number);	
	}
	else if(number < 100){
		sprintf(asString, "00%d", number);
	}
	else if(number < 1000){
		sprintf(asString, "0%d", number);
	}
	else {
		sprintf(asString, "%d", number);
	}
}

/*
 *returns a unique value for every possible value of red and white. 
 *inverse of printResult()
 //outcomes= 14
 */
//outcomes = 14
//max size = 5
int toIndex(int red, int white){
	if (red == 1 || red == 2){
		return (4*red + white + 1);
	}
	else if (red == (MAXSIZE-2)){
		//the red
		return (OUTCOMES-2);
	}
	else if (red == (MAXSIZE-1)){
		return (OUTCOMES-1);
	}
	else{
		return white;
	}
}
/*
BBB≠BW
BB≠BW
5,6,7,8
9,10,11
12
13
0,1,2,3,4

*/

/*
 *checkNums returns an int corelating to the red / white value of theGuess compared to the Secret. 
 *
 *
 */


int checkNums(char* theSecret, char* theGuess){
	int numRed = 0;
	int numWhite = 0;
	int i;
	char tempSecret[MAXSIZE];
	char tempGuess[MAXSIZE];	
	strncpy(tempSecret, theSecret, MAXSIZE);
	strncpy(tempGuess, theGuess, MAXSIZE);
	for(i = 0; i < (MAXSIZE-1); i++){
                if(tempSecret[i] == tempGuess[i]){
                        numRed++;
			tempGuess[i]= 'x';
			tempSecret[i] = 'y';
                }

        }
	int j;
	for(i = 0; i < (MAXSIZE-1); i++){
		for(j = 0; j < (MAXSIZE -1); j++){
			if(tempGuess[i] == tempSecret[j]){
				tempGuess[i] = 'x';
				tempSecret[j] = 'y';
				numWhite++;
			}
		}
	}

	return toIndex(numRed, numWhite);
}

/*
 *Array[] is an array of numbers where the index correspons to a guess value. The value stored at the index is either 0 or 1. 1 corresponds to a 
 *number that is still a possibility in Knuth's Algorithm. the string guess and the int guessResult correspond to the last guess and it's unique index value as determined by toIndex()
 *For every number that is currently still valid, the function checks to see if it is a possible value for the secret number if not it is marked as a 0
 */
 //NEED TO CORRECT- currently correct... I think
void stillValid(int* array, char* guess, int guessResult){
	int i = 0;
	char value[(MAXSIZE-1)];
	int i2;
	bool inPlay = false;
	bool possible;
	int checkVal;
	//Total 1296
	for (i = 0; i < TOTAL; i++){
		//printf("i is %d\n", i);
		//inPlay = true;
		i2=i;
		asChars(value, i2);
		//printf("i is %d\n", i);
		//this value of i is still present in the array that keeps track of which answers are still possible
		inPlay = (array[i] == 1);
		//printf("value is %s\n", value);
		//printf("array is %d\n", array[i]);
		//printf("Guess =  %s\n", guess);
		//Guess =  1111
		if(atoi(value) == atoi(guess)){
			//printf("value is %s\n", value);
			//printf("Guess =  %s\n", guess);
			//stringcmp compares the length
			inPlay = false;
			array[i] = 0;
			//possible = false;
		}

		possible = false;
		
		if (inPlay){
			//comparing current i, to the guess
			//value vs the guess
			//index vs guess with guess=/=value
			checkVal = checkNums(value, guess);
			//printf("check val is %d\n", checkVal);
			possible = (checkVal == guessResult);
			
			//if currentResponse != checkCode(currentCode, set[index]
		}
		//printf("inPlay %d and possible %d \n", inPlay, possible);
		//if not in play && possible
		if(!possible){
			if(inPlay){
			//still elimiating 9876
			array[i] = 0;
			//printf("array is now %d and index is %d\n", array[i], i);
			}
		}

	}
}
/*
void pruneCodes(vector<vector<int>> 
				&set, vector<int> currentCode, 
				string currentResponse) {

    int index;
    vector<vector<int>>::iterator it = set.begin();

    while (it != set.end()) {
        index = distance(set.begin(), it);

        if (currentResponse != checkCode(currentCode, set[index])) {
            it = set.erase(set.begin() + index);
        } else {
            it++;
        }
    }
}
*/

/*
 *playSelf implements Knuth's algorithm to solve the Mastermind game. Function takes pointers to arrays for the remaining valid numbers and the last guess.
 *The algorithm allocates an array large enough to hold an integer for every possible outcome of every possible guess.
 *The algorithm compares every possible guess (numbers 0-9999) to every remaining possible value. In doing so an array is created which details the possible outcomes of every guess. 
 The data for each outcome details how many numbers of those remaining will still be valid if that is the return value. 
 *The algorithm uses that information to choose the next guess based on what number will guarantee to remove the MOST values from the current list of numbers.
 * see also wikipedia -- Mastermind
 * 
 */
void playSelf(char* guess, int* remaining){
	int* allPossible[OUTCOMES];
	int j;
	//outcomes = 14
	//Total = all possible combinations, ether 1329ish or 0-9999
	//[14][10000]
	for (j = 0; j < OUTCOMES; j ++){
		allPossible[j] = malloc(TOTAL * sizeof(int));
	}
	int i;
	int results;
	char toString[MAXSIZE];
	//setting up double array, i equal position, j is outcomes (the unique value return by index(red white)
	//initializing
	//each diget, 
	//outcomes = 14
	for(i = 0; i < OUTCOMES; i++){
                for(j = 0; j < TOTAL; j++){ 
					allPossible[i][j] = 0;
			}
        }	
	//form 0-4 digits, and 0-9999?, set index to unique value representing if the given code would give a combination of black white etc
	for(i = 0; i < TOTAL; i++){
		//check if this value is still their.
		if(remaining[i] == 1){
			
			//j<10,000
			for(j = 0; j < TOTAL; j++){
				asChars(toString, j);
				char remainingStr[(MAXSIZE-1)];
				asChars(remainingStr, i);
				results = checkNums(remainingStr,toString);
				allPossible[results][j]++;
				//sets allPossible[results][j]== 1;
			}
		}
	}
	int minMax = TOTAL;
	int nextGuess;
	for (j = 0; j < TOTAL; j++){
		int maxValue = 0;
		int index= 0;
		for (i = 0; i < OUTCOMES; i++){
			if (allPossible[i][j] >= maxValue){
				maxValue = allPossible[i][j];
				index = j;	
			}
		}
		if(maxValue < minMax || (maxValue == minMax && allPossible[(OUTCOMES -1)][index] == 1)){
			minMax = maxValue;
			nextGuess = index;
		} 
	}
	for(j =0; j < OUTCOMES; j++){
		free(allPossible[j]);
	}
	asChars(guess, nextGuess);
	if(debug2){
		printf("Suggested Guess: %s\n", guess);
	}
	//return nextGuess;
}


/*
 *takes a unique value corresponding to a red/ white number and prints the red / white value to the screen.
 *inverse of toIndex()
 *
 */

void printResult(int index){
	if ( index < MAXSIZE ) {
		printf("%d white\n", index);
	}
	else if ( index < 9 ) {
		printf("1 red %d white\n", (index-5));
	}
	else if ( index < 12) {
		printf("2 red %d white\n", (index-9));
	}
	else if (index == (OUTCOMES-2)){
		printf("3 red\n");
	}
	else {
		printf("4 red\n");
	}
}


int main(int argc, char **argv)
{
	



		
	/**-------------------**/
    //double t1; // Timing variable	
    //t1 = omp_get_wtime();
    //float total; //total time	

	//time in seconds t
    time_t seconds;
	time_t diffrence_seconds;
	//Seconds since January 1, 1970
    time_t seconds2;
	time_t diffrence_seconds2;	
	time_t seconds3;
	time_t diffrence_seconds3;	
	time_t seconds4;
	time_t diffrence_seconds4;	
    seconds = time(NULL);
    seconds2 = time(NULL);
    seconds3 = time(NULL);
    seconds4 = time(NULL);	
	
	int i = 0;
	int guess2 = 1122;
	//9876
	//the above takes around 6 guesses.
	char secretNum[MAXSIZE];
	char secretNum2[MAXSIZE];
	char secretNum3[MAXSIZE];
	char secretNum4[MAXSIZE];	
	char theGuess[MAXSIZE];
	char theGuess2[MAXSIZE];
	char theGuess3[MAXSIZE];
	char theGuess4[MAXSIZE];
	//char theGuess2[MAXSIZE];
	int temp;

	int numGuess=0;
	int numGuess2=0;
	int numGuess3=0;
	int numGuess4=0;
	//int* remaining[MAXSIZE];
	//int* allPossible[OUTCOMES];
	int j;
	//outcomes = 14
	//Total = all possible combinations, ether 1329ish or 0-9999
	//[14][10000]
	//for (j = 0; j < MAXSIZE; j ++){		remaining[j] = malloc(TOTAL * sizeof(int));	}

	



	
	srand(time(0));



	
	/****/
	//mpi stuff
	/**/
	int myid, numprocs,  dest, tag;	
	//source, , largest
	//int data[MAXSIZE];
	//int  result=0;
	//x, low, high, my_result=0,
	//char fn[255];
	//FILE *fp;	
	
	//Need this for the recieve
	//MPI_Status status;
    dest = 0;         /* define the process that computes the final result */
    tag = 123;        /* set the tag to identify this particular job. I.e the processes identify each other */

	//these might need to be moved...
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);/* get number of processes */
	MPI_Comm_rank(MPI_COMM_WORLD,&myid);/* get current process id */
	
	
	int* remaining = malloc(TOTAL * sizeof(int));
	int* remaining2 = malloc(TOTAL * sizeof(int));
	int* remaining3 = malloc(TOTAL * sizeof(int));
	int* remaining4 = malloc(TOTAL * sizeof(int));
	if (myid == 0){  /* Open input file and initialize data */
		//strcpy(fn, "rand_data.txt");
		//if ((fp = fopen(fn,"r")) == NULL) {
		//	printf("Cannot open the input file: %s\n\n", fn);
		//	exit(1);
		for(j=0;j < MAXSIZE; j++){
			for(i =0;i < TOTAL; i++){
			remaining[i] = 1;
			remaining2[i] = 1;
			remaining3[i] = 1;
			remaining4[i] = 1;
			//printf("remaining is now %d\n", remaining[i]);
			}	
		}
		//MPI_Bcast(data, MAXSIZE, MPI_INT, 0, MPI_COMM_WORLD); /* broadcast data */ 
		////x = MAXSIZE/numprocs; /* Add my portion of data */
		////low = myid * x;
		////high = low + x;
		
		//for(i = low; i < high; i++) my_result += data[i];
		//printf("partial sum %d from %d\n", my_result, myid); /* Compute global sum */
		////for(i = 0; i < MAXSIZE; i++) fscanf(fp,"%d", &data[i]);
		
	}else if(myid == 1){
		for(j=0;j < MAXSIZE; j++){
			for(i =0;i < TOTAL; i++){
			remaining2[i] = 1;
			//printf("remaining is now %d\n", remaining[i]);
			}	
		}
		
	}else if(myid == 2){
		
			for(i =0;i < TOTAL; i++){
			remaining3[i] = 1;
			
			}
	}else if(myid == 3){
		
			for(i =0;i < TOTAL; i++){
			remaining4[i] = 1;
			
			}
	}
	
	if (myid == 0 || myid == 1||myid == 2||myid == 3){ 	
	//||myid == 4
	//setting guess to 0012 was messing it up
	//	printf("checking guess %04d .\n", guess);
	//debug test 1
	//for debug one to work num pegs must be set to 4;
	temp = rand() % TOTAL;
	asChars(secretNum, temp);
	temp = rand() % TOTAL;
	asChars(secretNum2, temp);
	temp = rand() % TOTAL;
	asChars(secretNum3, temp);
	temp = rand() % TOTAL;
	asChars(secretNum4, temp);
	
	asChars(theGuess, guess2);
	asChars(theGuess2, guess2);
	asChars(theGuess3, guess2);
	asChars(theGuess4, guess2);
	//strncpy(*(theGuess), *(guess2), MAXSIZE);
	printf("The Guess: %s %s %s %s\n", theGuess, theGuess2, theGuess3,theGuess4);
	printf("The Secret is: %s %s %s %s\n", secretNum,secretNum2,secretNum3,secretNum4);
	//printf("\n");
	}else if(myid == 4){	}
	


	//***** ****//

if(debug4){
	if(myid == 0) {	
		int first = 1;
		while(first){
			numGuess++;
			//numGuess2++;
			//numGuess
			//numGuess
			int result = checkNums(secretNum, theGuess);
			if(debug1){
				printResult(result);
			}
			if(result == (OUTCOMES -1)){
				if( numGuess == 1){
					diffrence_seconds = time(NULL) - seconds;
					//total = omp_get_wtime() - t1;
					fprintf(stdout, "You win!  It took you %d guess. this took %ld seconds for %s\n", numGuess,diffrence_seconds,secretNum);
				}
				else {
					diffrence_seconds = time(NULL) - seconds;
					fprintf(stdout, "You win!  It took you %d guesses. this took %ld seconds for %s\n", numGuess,diffrence_seconds,secretNum);
					//eventually convert to average guessess.
				}
				//first = 0;
				break;
			}
			
			//THIS RIGHT HERE IS WHERE IT STOPS, THE
			//Never mind, i fixed it.
			
			stillValid(remaining, theGuess, result);
			if(debug3){
				printf("after stillvalid\n");
			}
			//This one runs, and sujjests a file all right, the same one, over and over.
			//will continue to do so till still valid works.
			if(first){
				playSelf(theGuess, remaining);
			}
			//probally should just test a new one.
			//printf("Main funtion Suggested Guess: %s\n", theGuess);
			//theGuess = theGuess2;
			//first = 0;
		}
	}else if(myid == 1){
		int second = 1;
		seconds2 = time(NULL);
		while(second){
			numGuess2++;
			int result2 = checkNums(secretNum2, theGuess2);
			if(debug1){
				printResult(result2);
			}
			if(result2 == (OUTCOMES -1)){
				if( numGuess2 == 1){
					diffrence_seconds2 = time(NULL) - seconds2;
					
					fprintf(stdout, "You win!  It took you %d guess. this took %ld seconds for %s\n", numGuess2,diffrence_seconds2,secretNum2);
				}
				else {
					diffrence_seconds2 = time(NULL) - seconds2;
					fprintf(stdout, "You win!  It took you %d guesses. this took %ld seconds for %s\n", numGuess2,diffrence_seconds2,secretNum2);
					
				}
				//first = 0;
				break;
			}
			
			stillValid(remaining2, theGuess2, result2);
			if(debug3){
				printf("after stillvalid\n");
			}
			//This one runs, and sujjests a file all right, the same one, over and over.
			//will continue to do so till still valid works.
			if(second){
				playSelf(theGuess2, remaining2);
			}
		}
		
	}else if(myid == 2){
		int third = 1;
	    seconds3 = time(NULL);

		while(third){
			numGuess3++;
			int result3 = checkNums(secretNum3, theGuess3);
			if(debug1){
				printResult(result3);
			}
			if(result3 == (OUTCOMES -1)){
				if( numGuess3 == 1){
					diffrence_seconds3 = time(NULL) - seconds3;
					
					fprintf(stdout, "You win!  It took you %d guess. this took %ld seconds for %s\n", numGuess3,diffrence_seconds3,secretNum3);
				}
				else {
					diffrence_seconds3 = time(NULL) - seconds3;
					fprintf(stdout, "You win!  It took you %d guesses. this took %ld seconds for %s\n", numGuess3,diffrence_seconds3,secretNum3);
					
				}
				//first = 0;
				break;
			}
			
			stillValid(remaining3, theGuess3, result3);
			if(debug3){
				printf("after stillvalid\n");
			}
			//This one runs, and sujjests a file all right, the same one, over and over.
			//will continue to do so till still valid works.
			if(third){
				playSelf(theGuess3, remaining3);
			}
		}
	}else if(myid == 3){
		int fourth = 1;
	    seconds4 = time(NULL);	

		while(fourth){
			numGuess4++;
			int result4 = checkNums(secretNum4, theGuess4);
			if(debug1){
				printResult(result4);
			}
			if(result4 == (OUTCOMES -1)){
				if( numGuess4 == 1){
					diffrence_seconds4 = time(NULL) - seconds4;
					
					fprintf(stdout, "You win!  It took you %d guess. this took %ld seconds for %s\n", numGuess4,diffrence_seconds4,secretNum4);
				}
				else {
					diffrence_seconds4 = time(NULL) - seconds4;
					fprintf(stdout, "You win!  It took you %d guesses. this took %ld seconds for %s\n", numGuess4,diffrence_seconds4,secretNum4);
					
				}
				//first = 0;
				break;
			}
			
			stillValid(remaining4, theGuess4, result4);
			if(debug3){
				printf("after stillvalid\n");
			}
			//This one runs, and sujjests a file all right, the same one, over and over.
			//will continue to do so till still valid works.
			if(fourth){
				playSelf(theGuess4, remaining4);
			}
		}
	}				
	
}	
///////////////////////////////////////////////////////////////
//parrelel
if(debug5){	


}	


////////////////////////////////////		
	MPI_Finalize();
	if(myid==0){
		free(remaining);
	}
	if(myid==1){
		free(remaining2);
	}
	if(myid==2){
	free(remaining3);
	}
	if(myid == 3){
	free(remaining4);
	}
	return 0;
	
}
