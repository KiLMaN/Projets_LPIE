#ifndef STDAFX_H
#define STDAFX_H

/** Librairies Standard **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>


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



/** -------------------------------
    -------- CONFIGURATION --------
    ------------------------------- **/

#define CONFG_PORT            23
#define HOST 		    INADDR_ANY
#define NB_MAX_CONN 	5

#endif //STDAFX_H
