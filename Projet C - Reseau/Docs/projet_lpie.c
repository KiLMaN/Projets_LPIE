// mesure_temperature et standard_normal_distribution a utiliser dans le projet

void standard_normal_distribution(double *x1) 
{ 
	double v1,v2,r,tmp; 
	do 
	{ 
		v1 = drand48(); 
		v2 = drand48(); 
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


// Voici un exemple de code utilisant mesure_temperature(). 
// Remarquez quels sont des "includes" a faire. 


/*
#include <stdio.h>                    
#include <stdlib.h> 
#include <math.h>
#include <time.h> 

void standard_normal_distribution(double *x1) 
{ 
	double v1,v2,r,tmp; 
	do 
	{ 
		v1 = drand48(); 
		v2 = drand48(); 
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

int main() 
{
    double temperature;
    srand48(time(0));
    
    temperature = mesure_temperature();
    printf("%3.3f\n",temperature);
}
*/