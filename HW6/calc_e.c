#include <stdlib.h>
#include <stdio.h>
#include <omp.h>


double calculation_e_simple(int n_iter, int N)
{
    // Расчет числа e.
    
    double exp = 0;
    double e;
    long f;
    
    for (int j = 0; j < N; j++)
    {
        e = 0;
        
        for (int n = 0; n < n_iter; n++)
        {
            f = 1;
            
            for (int i = 2; i < n + 1; i++)
            {
                f *= i;
            }
            
            e += 1.0 / f;
        }
            
        exp += e;
    }
    
    return exp / N;
}


long factorial(int n)
{
    long f = 1;
    
    #pragma omp parallel for shared(f)
    for (int i = 2; i < n + 1; i++)
    {
        f *= i;
    }
    
    return f;
}

double calc_e_one_time(int n_iter)
{
    double e = 0;
    long f;
    
    #pragma omp parallel for private(f) reduction(+:e)
    for (int n = 0; n < n_iter; n++)
    {
        f = factorial(n);
        e += 1.0 / f;
    }
    
    return e;
}


double calculation_e_omp(int n_iter, int N)
{
    // Расчет числа e.
    
    double exp = 0;
    double e;
    long f;
    
    omp_set_num_threads(8);
    
    /*#pragma omp parallel shared(exp, N, n_iter)
    {
        #pragma omp for private(e) reduction(+:exp)
        for (int j = 0; j < N; j++)
        {
            double e = calc_e_one_time(n_iter);            

            exp += e;
        }
    }*/
    
    #pragma omp parallel shared(exp, N, n_iter)
    {
        #pragma omp for private(e) reduction(+:exp)
        for (int j = 0; j < N; j++)
        {
            e = 0;
            
            for (int n = 0; n < n_iter; n++)
            {
                f = 1;
                
                for (int i = 2; i < n + 1; i++)
                {
                    f *= i;
                }
                
                e += 1.0 / f;
            }
                
            exp += e;
        }
    }
    
    return exp / N;
}


void main()
{
    double e = calculation_e_omp(20, 100);
    printf("%f\n", e);
}
