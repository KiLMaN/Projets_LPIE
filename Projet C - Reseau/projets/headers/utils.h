#ifndef UTILS_H
#define UTILS_H


char * getStrTime(char * buffer);

/* Fait fonctionner les fonction de rand sur windows */
void MySRand( long seed );
double MyDrand( void );
int MyRand(int Min , int Max);
void MySleep(long delay);

#endif // UTILS_H
