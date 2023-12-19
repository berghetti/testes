#include <stdlib.h>
#include <stdio.h>
#include <math.h>

double r; // rand [0,1]

double sample_pareto(double alpha, double xm) {
    double u = r; //(double)rand() / RAND_MAX; // Uniform random number [0,1]
    return xm / pow(1 - u, 1.0 / alpha);
}

double sample_paretopy(double alpha, double xm) {
    double u = 1.0 - r; //(double)rand() / RAND_MAX; // Uniform random number [0,1]
    return  xm * pow(u, -1.0 / alpha);
}


double sample_lognormal(double mu, double sigma) {
    double u1 = ((double)rand() / RAND_MAX); // Uniform random number [0,1]
    double u2 = ((double)rand() / RAND_MAX); // Uniform random number [0,1]

    double z = sqrt(-2.0 * log(u1)) * cos(2 * M_PI * u2);

    return exp(mu + sigma * z);
}


int main()
{
    srand(42);

    double rate_per_queue = 1000; //packets per second
    
    double mean = (1.0/rate_per_queue) * 1000000.0;
    double alpha = 1.0 + mean / (mean - 1.0);   
    double xm = mean * (alpha - 1) / (alpha); // xm = mean / 2

    printf("alpha: %lf xm %lf mean: %lf\n", alpha, xm, mean);

    //printf("rand: %f\n", (double)rand() / RAND_MAX);
    
    //printf("Pareto:\n");
    //for(int i = 0; i < 10; i++) {
    //    r = (double)rand()/RAND_MAX;
    //    printf(" fabricio  %f\n", sample_pareto(alpha, xm));    
    //    printf(" python    %f\n", sample_paretopy(alpha, xm));    
    //}
    
    double sigma = sqrt(2*(log(mean) - log(mean/2)));
    double u = log(mean) - (sigma*sigma)/2;

    printf("sigma %lf mu %lf\n", sigma, u);
    
    printf("Lognormal:\n");
    for(int i = 0; i < 10; i++) {
        printf("  %f\n", sample_lognormal(u, sigma));    
    }

    return 0;
}
