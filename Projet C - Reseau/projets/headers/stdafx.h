#ifndef STDAFX_H
#define STDAFX_H

/** Librairies Standard **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <math.h>
#include <time.h>



/** -------------------------------
    ------- Libraire pThread ------
    -------------------------------

    Windows :
        Ne pas oublier de lier les fichiers
        -\pThread\lib\pthreadVC2.lib
        -\pThread\lib\libpthreadGC2.a
        Ne pas oublier de copier le fichier pthreadVC2.dll dans le dossier de l'executables
        Ajouter -lpthread a la comilation
    Linux :
        Ajouter -lpthread a la comilation

    -------------------------------
**/
#include <pthread.h>


/** -------------------------------
    ----- Fichier .h Du Projet ----
    ------------------------------- **/
#include "GestionReseau.h"
#include "ListeChainee.h"
#include "simSonde.h"
#include "utils.h"
#include "parserConfig.h"



/** -------------------------------
    -------- CONFIGURATION --------
    ------------------------------- **/

#define CONFG_PORT            23
#define HOST 		          INADDR_ANY
#define NB_MAX_CONN 	      5

#define CONFIG_PORT_SERVER      48000  // Define le port du routeur
#define CONFIG_PORT_ECOUTE      48000  // Define le port du routeur
#define CONFIG_PORT_COORD       49000  // Define le port du routeur

#define CONFIG_TEMPS_MESURE     20      // Definie le temps entre chaque mesure de température en secondes +/- un delais random
#define CONFIG_TEMPS_RAND       3       // Definir la limite max de temps aléatoire a ajouter ou retrancher au delais

#endif //STDAFX_H
