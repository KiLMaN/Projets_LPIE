#include "stdafx.h"


char * getStrTime(char * buffer)
{
    time_t rawtime;
    struct tm * timeinfo;
    //char buffer [80];

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    strftime (buffer,80,"[%H:%M:%S]",timeinfo);
    return buffer;
}


void MySRand( long seed )
{
#ifdef WIN32
    srand(seed);
#else
    srand48(seed);
#endif
}
double MyDrand( void )
{
#ifdef WIN32
    return ((double) (rand())) /rand();
#else
    return drand48();
#endif
}

int MyRand(int Min , int Max)
{
    return rand()%(Max-Min) + Min;
}

void MySleep(long delay)
{
#ifdef __WIN32__
    Sleep(delay);
#else
    usleep(delay * 1000);
#endif
}
