/*
 ============================================================================
 Assignment 2
 Start Eclipse, create an OpenMP project,
 ============================================================================
 */
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "test.h"
#include <time.h>

/*
 * C macro used to supress printing if not required.
 */
#define pprint


/* Q1 [12 marks]
    A, B, C: the three vectors
    size: number of elements in a vector

    Do not create additional variables in the function 
    (hint: use pointers and pointer expressions)

    Marking: +6 for the function
*/
void addVec(int* C, int* A, int* B, int size)
{


	for(int i = 0; i < size; i++){
		C[i] = A[i] + B[i];
	}

	
}



/* Q1 Unit testing 
    Test with two vectors each having 50 million integers, all initialized to 0 (hint: calloc)
    Allocate memory for A, B, and C within this function
    Print the first 10 elements of C
    Print the total time to complete the execution
    Mention the results as a comment

    Marking: +5 for the test code (2 marks for error checking)
*/

/*
testq1() results:
C[0]: 0
C[1]: 0
C[2]: 0
C[3]: 0
C[4]: 0
C[5]: 0
C[6]: 0
C[7]: 0
C[8]: 0
C[9]: 0
Time taken = 99.00ms
 */

static char *  testq1 (){
	int n;
	n = 50000000;
	int * A = (int *)calloc(n, sizeof(int));
	int * B = (int *)calloc(n, sizeof(int));
	int * C = (int *)calloc(n, sizeof(int));

	for(int i = 0; i < n; i++){
		A[i] = 0;
		B[i] = 0;
		C[i] = 0;
	}

	if(A == NULL || B == NULL || C == NULL)
		printf("Memory Allocation unsuccessful");

	double t = clock();

	addVec(C,A,B,n);

	t = clock() - t;

#ifdef pprint
	for(int i = 0; i < 10; i++)
		printf("C[%d]: %d\n", i, C[i]);
	printf("Time taken = %.2fms\n", t);

#endif
	/*
	 * Check values in c
	 */
	return 0;
}
/*   
    Comment your resutls for the test run 
    Marks: +1 for the comment
*/

// ================================================================================================

/* Q2 [12 marks]
    Repeat question Q1 with a function that receives pointers to A and B, allocates memory
    space for C, and returns a pointer to the memory holding the computed output.

    Should return NULL if memory allocation for C fails
    You can only create two local variables of the type int* within the function

    Marks: +6 for the function (2 for error checking)
 */
int* addVec2(int* A, int* B, int size){
	
	int* C = (int*)calloc(size, sizeof(int));

	for(int i = 0; i < size; i++){
		C[i] = A[i] + B[i];
	}

	return C;
}

/* Q2 Unit testing 
    Test code should produce output similar to the sample runs in Q1

    Marking: +5 for the test code(2 for error checking)
*/
static char *  testq2 (){
	
	int n;
	n = 50000000;
	int * A = (int *)calloc(n, sizeof(int));
	int * B = (int *)calloc(n, sizeof(int));

	double t = clock();
	int* C = addVec2(A, B, n);
	t = clock() - t;
	for(int i = 0; i < 10; i++){
		printf("C[%d]: %d\n", i, C[i]);
	}
	printf("Time taken = %.2fms\n", t);


	return 0;
}
/*   
    Comment your resutls for the test run (Q2)
    Marks: +1 for the comment
*/

// ================================================================================================

/* Q3 [4 marks]
    Build and run the code given below. 
    Check everything works as expected, if not fix as appropriate.

    Marking: +4 for running code
 */
void q3() {
   int numThreads, tid;
  /* This creates a team of threads; each thread has own copy of variables */
  #pragma omp parallel private(numThreads, tid)
  {
    tid = omp_get_thread_num();
    printf("Hello World from thread number %d\n", tid);
    /* The following is executed by the master thread only (tid=0) */
    if (tid == 0) {
      numThreads = omp_get_num_threads();
      printf("Number of threads is %d\n", numThreads);
  }
  }
}

// ================================================================================================

/* Q4 [20 marks]
    Write the sequential code for the function.
 */
int* vecCreate (int size)
{

}

/* 
    Write the OpenMP implementation of the function
    (Hint: Use #pragma omp parallel num_threads( num_thread ))
*/
int* vecCreateOpenMP(int size, int num_thread)
{

}

/* Q4 Unit tests
 * Unit test for both functions
 * 50-million element vector (and 4 threads for the second function)
 */
static char *  testq4 (){
	int n;
	n = 50000000;
	/*
	 * call vecCreate
	 */

	/*
	 * Check allocation of o/p
	 */



#ifdef pprint
	/*
	 * Print first and last 10 elements of o/p
	 */


#endif
	/*
	 * Check elements in o/p
	 */

	/*
	 * Call vecCreateOpenMP(n,4)
	 */
	/*
	 * Check allocation
	 */


#ifdef pprint
	/*
	 * Print first and last 10 elements of o/p
	 */


#endif
	/*
		 * Check elements in o/p
		 */

	return 0;
}


/*
 * Unit test for both functions
 * 100000 element vector (and 3 threads for the second function)
 */

static char *  testq4_2 (){
	int n;
	n = 100000;
	/* vecCreate
	 * all checks as in earlier test
	 */

	/*
	 *
	 * int nT = 3;
	*  int * b = vecCreateOpenMP(n,nT);
	   print if number of threads not divisible
	   all checks as in earlier test
	 */

	return 0;
}


/*
 * Unit test for both functions
 * 40000000 element vector (and 3 threads for the second function)
 */

static char *  testq4_3 (){
	int n;
	n = 40000000;
	/* vecCreate
		 * all checks as in earlier test
		 */

	/* vecCreateOpenMP
		 * all checks as in earlier test
		 * int * b = vecCreateOpenMP(n,4);
		 */


	return 0;
}

// ================================================================================================

static char * all_tests() {

	run_test(testq1);
	run_test(testq2);
	run_test(testq4);
	/* First 3 tests pass */
	/* Run without next two functions once then add them*/

	/* change order of next to functions and see errors */
	run_test(testq4_2);
	run_test(testq4_3);

	return 0;
 }

 int main (int argc, char *argv[]) {
    
    
    char *result = all_tests();
	if (result != 0) {
	   printf("%s\n", result);
	}
	else {
	   printf("ALL TESTS PASSED\n");
	}
	printf("Tests run: %d\n", tests_run);
	q3();
        return 0;

}
