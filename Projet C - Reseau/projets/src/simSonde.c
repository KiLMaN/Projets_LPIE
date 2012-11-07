#include "stdafx.h"
#include <math.h>
void standard_normal_distribution(double *x1)
{
    double v1,v2,r,tmp;
    do
    {
        v1 = MyDrand();
        v2 = MyDrand();
        r=v1*v1+v2*v2;
    }
    while((r==0)||(r>=1));

    tmp = sqrt(-2*log(r)/r);
    *x1 = v1*tmp;
}

double mesure_temperature()
{
    double mean = 48;
    double variance = 1;
    double temperature, x1;

    standard_normal_distribution(&x1);
    temperature = mean+variance*x1;

    return temperature;
}

