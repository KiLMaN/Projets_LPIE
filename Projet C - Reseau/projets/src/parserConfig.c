#include "stdafx.h"

FILE * ChargerFichierConfig(char * pathFichier)
{
    FILE * fichierConf = NULL;
    fichierConf = fopen(pathFichier, "r");

    if (fichierConf != NULL)
    {
        printf("Ouverture du fichier '%s'\n",pathFichier);
        return fichierConf;
    }
    else
    {
        printf("Impossible d'ouvrir le fichier %s\n",pathFichier);
        return NULL;
    }
}

int getValue(FILE * fichier ,char * var,char * retVal)
{
    char chaineLue[TAILLE_LECTURE_MAX] = "", id[100] = "";
    char * posChar , * posChar2;
    if(var == NULL || retVal == NULL)
        return -1;

    if(fichier != NULL)
    {
        /* Rembobine le fichier au debut */
        rewind(fichier);

        // Boucle de lecture des caractères un à un
        while (fgets(chaineLue, TAILLE_LECTURE_MAX, fichier) != NULL) // On lit le fichier tant qu'on ne reçoit pas d'erreur (NULL)
        {
            /* Recherche le caractere '=' dans la ligne lue */
            posChar=strchr(chaineLue,SIGNE_EGAL);
            while (posChar!=NULL)
            {
                memset(id,'\0',sizeof(id));
                /* Copie l'identifiant de la variable dans id */
                int positionChar = posChar-chaineLue+1;
                strncpy ( id, chaineLue, positionChar-1 );

                /* Test si c'est la variable que l'on veux */
                if(strcmp(var,id) == 0)
                {
                    /* Copie dans le buffer de sortie */
                    strncpy ( retVal, posChar+1,strlen(posChar) );

                    /* Suppresion des \n en fin de ligne */
                    posChar2=strchr(retVal,'\n');
                    while (posChar2!=NULL)
                    {
                        *posChar2 = '\0';
                        posChar2=strchr(posChar2+1,'\n');
                    }
                    return 1;
                }

                /* Si pas bon id passe a la ligne suivante */
                posChar=strchr(posChar+1,SIGNE_EGAL);
            }
        }
        return 0;
    }
    else
    {
        return -1;
    }
}


int FermerFichierconfig(FILE * fichier)
{
    if (fichier != NULL)
    {
        fclose(fichier);
        return 0;
    }
    else
    {
        return -1;
    }
}
