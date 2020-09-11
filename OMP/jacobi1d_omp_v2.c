/* Include benchmark-specific header */
#include "jacobi-1d.h"
double bench_t_start, bench_t_end;

static
double rtclock()
{
    struct timeval Tp; 
	/* structure including fields:
       long tv_sec - seconds since Jan. 1, 1970 
       long tv_usec - microseconds */
    int stat;
    stat = gettimeofday (&Tp, NULL); /* get current time and data in seconds and mircroseconds (fill fields of struct Tp) */
    if (stat != 0) /* 0 - succcess, (-1) - failure */
        printf ("Error return from gettimeofday: %d", stat);
    return (Tp.tv_sec + Tp.tv_usec * 1.0e-6);
}

void bench_timer_start()
{
    bench_t_start = rtclock (); /* remember time of start */
}

void bench_timer_stop()
{
    bench_t_end = rtclock (); /* remember time of end */
}

void bench_timer_print()
{
    printf ("Time in seconds = %0.6lf\n", bench_t_end - bench_t_start); /* print runtime */
}

/* initialiating of an array */
static
void init_array (int n,
   double A[n],
   double B[n])
{
    int i;

    for (i = 0; i < n; i++) {
        A[i] = ((double) i + 2) / n;
        B[i] = ((double) i + 3) / n;
    }
}

/* printing of an array */
static
void print_array(int n, double A[n])
{
    int i;

    fprintf(stderr, "==BEGIN DUMP_ARRAYS==\n");
    fprintf(stderr, "begin dump: %s", "A");
    for (i = 0; i < n; i++)
    {
        if (i % 20 == 0) fprintf(stderr, "\n");
        fprintf(stderr, "%0.2lf ", A[i]);
    }
    fprintf(stderr, "\nend   dump: %s\n", "A");
    fprintf(stderr, "==END   DUMP_ARRAYS==\n");
}


static
void kernel_jacobi_1d(int tsteps,
    int n,
    double A[n],
    double B[n])
{
    int t, i;
    double C[4];
    double *D = (double*)malloc((n) * sizeof(double));
    D[0] = A[0];
    D[n - 1] = A[n - 1];
    double *F;
    double temp1 = 0, temp2 = 0, temp3 = 0;
    for(t = 0; t < tsteps; t++)
    { 
        C[0] = A[2];
        C[1] = A[3];
        C[2] = A[n - 3];
        C[3] = A[n - 4];
        temp1 = temp2 = A[0];
        temp3 = A[1];
        for(i = 2; i < n - 2; i++)
        {
            temp1 = temp2; 
            temp2 = temp3; 
            temp3 = A[i];
            A[i] = (0.33333 * 0.33333) * (temp1 + 2 * temp2 + 3 * A[i] + 2 * A[i + 1] + A[i + 2]);
            B[i] = 0.33333 * (temp2 + temp3 + A [i + 1]);
        }
        A[1] = 0.33333 * (B[0] + 0.33333 * (A[0] + 2 * A[1] + 2 * C[0] + C[1]));
        A[n - 2] = 0.33333 * (B[n - 1] + 0.33333 * (2 * A[n - 2] + 2 * C[2] + C[3] + A[n - 1]));  
    }
}

int main(int argc, char** argv)
{
    int n = N;
    int tsteps = TSTEPS;
    double (*A)[n]; A = (double(*)[n])malloc ((n) * sizeof(double));
    double (*B)[n]; B = (double(*)[n])malloc ((n) * sizeof(double));

    double sum = 0;

    init_array (n, *A, *B);
        
    bench_timer_start();

    kernel_jacobi_1d(tsteps, n, *A, *B);
    
    bench_timer_stop();
    //bench_timer_print();
    sum = bench_t_end - bench_t_start;
    
    printf("%d,%d,%f\n", n, tsteps, sum);
    //if (argc > 42 && ! strcmp(argv[0], "")) print_array(n, *A);

    free((void*)A);
    free((void*)B);
    return 0;
}
