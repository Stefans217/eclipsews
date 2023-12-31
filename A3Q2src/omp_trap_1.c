/* File:    omp_trap1.c
 * Purpose: Estimate definite integral (or area under curve) using trapezoidal 
 *          rule.
 *
 * Input:   a, b, n
 * Output:  estimate of integral from a to b of f(x)
 *          using n trapezoids.
 *
 * Compile: gcc -g -Wall -fopenmp -o omp_trap1 omp_trap1.c
 * Usage:   ./omp_trap1 <number of threads>
 *
 * Notes:   
 *   1.  The function f(x) is hardwired.
 *   2.  In this version, each thread explicitly computes the integral
 *       over its assigned subinterval, a critical directive is used
 *       for the global sum.
 *   3.  This version assumes that n is evenly divisible by the 
 *       number of threads
 *
 * IPP:  Section 5.2.1 (pp. 216 and ff.)
 */




/*
program results:
==============================================================
Default code
With n = 1200000 trapezoids, our estimate
of the integral from 1.000000 to 50000.000000 = 4.16666666666808e+013
finished in: 4.00ms
0.000000
==============================================================
Manual reduction code
With n = 1200000 trapezoids, our estimate
of the integral from 1.000000 to 50000.000000 = 4.16666666666808e+013
finished in: 4.000006ms
==============================================================
Reduction Code
With n = 1200000 trapezoids, our estimate
of the integral from 1.000000 to 50000.000000 = 4.16666666666808e+013
finished in: 4.00ms
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

void Usage(char* prog_name);
double f(double x);    /* Function we're integrating */
void Trap(double a, double b, int n, double* global_result_p);
double Trap1(double a, double b, int n);

int main(int argc, char* argv[]) {
   double  global_result = 0.0;  /* Store result in global_result */
   double  a, b;                 /* Left and right endpoints      */
   int     n;                    /* Total number of trapezoids    */
   int     thread_count;

   if (argc != 2) Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);
   printf("Enter a, b, and n\n"); fflush(stdout);
   scanf("%lf %lf %d", &a, &b, &n);
   if (n % thread_count != 0) Usage(argv[0]);

   double t = omp_get_wtime();
   //default code
#  pragma omp parallel num_threads(thread_count)
   {
	   Trap(a, b, n, &global_result);
   }
   t = 1000 * (omp_get_wtime() - t);

   printf("==============================================================\nDefault code\n");
   printf("With n = %d trapezoids, our estimate\n", n);
   printf("of the integral from %f to %f = %.14e\n", a, b, global_result);
   printf("finished in: %.2fms\n", t);

   //reset global result
   global_result = 0.0;

   t = omp_get_wtime();
   //manual reduction code
#  pragma omp parallel num_threads(thread_count) 
   {
	   double my_result = Trap1(a, b, n);
#  pragma omp critical
	   {
		   global_result += my_result;
	   }
   }
   t = 1000 * (omp_get_wtime() - t);

   printf("==============================================================\nManual reduction code\n");
   printf("With n = %d trapezoids, our estimate\n", n);
   printf("of the integral from %f to %f = %.14e\n", a, b, global_result);
   printf("finished in: %fms\n", t);

   //reset global result
   global_result = 0.0;

   t = omp_get_wtime();
   //reduction code
#pragma omp parallel num_threads(thread_count) reduction(+:global_result)
   {
	   double my_result = Trap1(a, b, n);
	   global_result += my_result;
   }
   t = 1000 * (omp_get_wtime() - t);

   printf("==============================================================\nReduction Code\n");
   printf("With n = %d trapezoids, our estimate\n", n);
   printf("of the integral from %f to %f = %.14e\n", a, b, global_result);
   printf("finished in: %.2fms\n", t);

   return 0;
}  /* main */

/*--------------------------------------------------------------------
 * Function:    Usage
 * Purpose:     Print command line for function and terminate
 * In arg:      prog_name
 */
void Usage(char* prog_name) {

   fprintf(stderr, "usage: %s <number of threads>\n", prog_name);
   fprintf(stderr, "   number of trapezoids must be evenly divisible by\n");
   fprintf(stderr, "   number of threads\n");
   exit(0);
}  /* Usage */

/*------------------------------------------------------------------
 * Function:    f
 * Purpose:     Compute value of function to be integrated
 * Input arg:   x
 * Return val:  f(x)
 */
double f(double x) {
   double return_val;

   return_val = x*x;
   return return_val;
}  /* f */

/*------------------------------------------------------------------
 * Function:    Trap
 * Purpose:     Use trapezoidal rule to estimate definite integral
 * Input args:  
 *    a: left endpoint
 *    b: right endpoint
 *    n: number of trapezoids
 * Output arg:
 *    integral:  estimate of integral from a to b of f(x)
 */
void Trap(double a, double b, int n, double* global_result_p) {
   double  h, x, my_result;
   double  local_a, local_b;
   int  i, local_n;
   int my_rank = omp_get_thread_num();
   int thread_count = omp_get_num_threads();

   h = (b-a)/n;
   local_n = n/thread_count;
   local_a = a + my_rank*local_n*h;
   local_b = local_a + local_n*h;
   my_result = (f(local_a) + f(local_b))/2.0;
   for (i = 1; i <= local_n-1; i++) {
     x = local_a + i*h;
     my_result += f(x);
   }
   my_result = my_result*h;
#pragma omp critical
   *global_result_p += my_result;
}  /* Trap */

//rewrote the function for the question.
double Trap1(double a, double b, int n) {
   double  h, x, my_result;
   double  local_a, local_b;
   int  i, local_n;
   int my_rank = omp_get_thread_num();
   int thread_count = omp_get_num_threads();

   h = (b-a)/n; 
   local_n = n/thread_count;  
   local_a = a + my_rank*local_n*h; 
   local_b = local_a + local_n*h; 
   my_result = (f(local_a) + f(local_b))/2.0; 
   for (i = 1; i <= local_n-1; i++) {
     x = local_a + i*h;
     my_result += f(x);
   }
   my_result = my_result*h; 

   return my_result;
}  /* Trap */
