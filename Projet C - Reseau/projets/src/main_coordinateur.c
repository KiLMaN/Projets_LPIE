#include "stdafx.h"

#define SEUIL_MAX_TEMP 50 // degreés ( a partir de quelle température est affiché une alarme )
#define SEUIL_DELAIS_SONDE 2 // secondes ( combien de temps attend une mesure d'une sonde au maximum)

typedef struct t_ConfigCoordinateur
{
    int portEcoute;
}ConfigCoordinateur;
ConfigCoordinateur g_Config;

typedef struct t_StructSonde
{
    float Moyenne;
    int id;
    int id_routeur;
    int AlerteAffichee;
    int NouvelleVal; // Flag pour savoir si une nouvelle mesure a été envoyée depuis l'affichage de la derniere
    int TimeLastVal; // Heure d'arrive d'une nouvelle mesure envoyée
}TypeSonde;


void * waitForClient (void * pData);

ListeChainee * ListeSondes;

MySocket SocketEcoute;

int main()
{
  initReseau();

    char strConfig[20];

    /* Recuperation des valeurs du fichier de config */
    FILE * ConfigFile  = ChargerFichierConfig("config_coordinateur.txt");
    int ret;

    if((ret = getValue(ConfigFile,"PORT",strConfig)) != 1)
    {
        printf("Erreur dans la lecture de la configuration PORT! [%i]",ret);
        return -1;
    }
    g_Config.portEcoute = atoi(strConfig);


    printf("Coordinateur : \n");
    printf("Ecoute sur : %i\n", g_Config.portEcoute);
    printf("\n");


    SocketEcoute = creerSocketUdpServer ( g_Config.portEcoute);

    if(SocketEcoute > 0)
    {
        printf("Creation de la socket d'ecoute sur le port %i reussie !\n", g_Config.portEcoute);
    }
    else
    {
        printf("Erreur lors de la creation de la socket ! %i",SocketEcoute);
        return -1;
    }

    pthread_t Thread_waitForClient ;//, Thread_SendMoyenne;
    pthread_create(&Thread_waitForClient,0,waitForClient,(void *)&SocketEcoute);


    while(1)
    {
        ListeChainee * elemSonde = ListeSondes;
        int b_MesuresOk = 1;
        while(elemSonde != NULL)
        {
            TypeSonde * sonde = ((TypeSonde *)elemSonde->data);
            if(sonde->NouvelleVal == 0 && (sonde->TimeLastVal > (time(NULL) - SEUIL_DELAIS_SONDE ))) // Si on a pas de mesure et que la derniere date d'apres le seuil
            {
               // printf("Attente");
                b_MesuresOk = 0; // On attend encore
            }
            elemSonde = elemSonde->next;
        }

        if(b_MesuresOk)
        {
            elemSonde = ListeSondes;
            while(elemSonde != NULL)
            {
                TypeSonde * sonde = ((TypeSonde *)elemSonde->data);
                sonde->NouvelleVal = 0;
                if(sonde->Moyenne > SEUIL_MAX_TEMP && sonde->AlerteAffichee == 0)
                {
                    char Time[10];
                    getStrTime(Time);
                    sonde->AlerteAffichee = 1;
                    printf("%s ATTENTION sur la sonde: %i, routeur: %i, temperature: %3.3f\n",Time,sonde->id,sonde->id_routeur,sonde->Moyenne);

                }
                elemSonde = elemSonde->next;
            }
        }
        MySleep(1000);
    }

    deInitReseau();
    return 0;
}

void * waitForClient (void * pData)
{
    MySocket sock = *((MySocket *)pData); // Recuperation de la socket
    char buff[50];
    int n ;
     struct sockaddr_in exp_addr;
    memset (&exp_addr,0, sizeof exp_addr) ;
    int exp_lenth= sizeof(exp_addr);


    while (1) // Boucle d'écoute
    {
        memset (&buff,0, sizeof(buff)) ;
        //buff[sizeof(buff)-1] = '\0';
        //memset (&buff,0, strlen(buff)) ;
        n = recvfrom (sock, buff, sizeof(buff),0,(struct sockaddr *)&exp_addr,(socklen_t *)&exp_lenth);

        /* Recuperation des informations sur l'envoyeur */
      /* char * ip = ( char * ) malloc(sizeof(char) * 20);
        sprintf ( ip, "%s", inet_ntoa (exp_addr.sin_addr));
        int port = ntohs(exp_addr.sin_port);

       printf("Message recu : %s:%i => (%i)[%s]\n",ip,port,n,buff);*/


        int idSonde = -1;
        int idRouteur = -1;
        float temperatureSonde = -1.0;
        /* Le protocole doit être "M,<ID_ROUTEUR>,<ID_SONDE>,<TEMP>,E" */

        if(*buff == 'M' && *(buff + strlen(buff)-1 ) == 'E') // Le debut et la fin sont M et E
        {
            char * posVirgule, * nextVirgule;
            posVirgule=strchr(buff,',');

            if(posVirgule!=NULL)
            {
                nextVirgule=strchr(posVirgule+1,',');
                if(nextVirgule!=NULL)
                {
                    char strId[5] = "";
                    strncpy(strId,posVirgule+1,(nextVirgule-posVirgule) -1 );
                    idRouteur = atoi(strId);

                    posVirgule = nextVirgule;
                    nextVirgule=strchr(posVirgule+1,',');
                    if(nextVirgule!=NULL)
                    {
                        memset(strId,'0',sizeof(strId));
                        strncpy(strId,posVirgule+1,(nextVirgule-posVirgule) -1 );
                        idSonde = atoi(strId);

                        posVirgule = nextVirgule;
                        nextVirgule=strchr(posVirgule+1,',');
                        if(nextVirgule!=NULL)
                        {
                            char temperature[20] = "";
                            strncpy(temperature,posVirgule+1,(nextVirgule-posVirgule) -1 );
                            temperatureSonde = atof(temperature);
                        }
                    }

                }
            }
        }
        else
        {
            /* Si le protocole n'est pas bon on passe en affichant un message */
            printf("PROTOCOL_ERROR !\n");
            continue;
        }

        if(temperatureSonde != -1.0 && idSonde != -1 && idRouteur != -1)
        {

            TypeSonde * Sonde = getValueAtID(ListeSondes,idSonde);
            if(Sonde == NULL)
            {
                TypeSonde * ElementSonde = malloc(sizeof (TypeSonde));

                ElementSonde->id = idSonde;
                ElementSonde->id_routeur = idRouteur;
                ElementSonde->Moyenne = temperatureSonde;
                ElementSonde->AlerteAffichee = 0;
                ElementSonde->NouvelleVal=1;
                ElementSonde->TimeLastVal=time(NULL);
                ListeSondes = AddElementListeID(ElementSonde,ListeSondes,0,idSonde);
            }
            else
            {
               /* Sonde->id = idSonde;
                ElementSonde->id_routeur = idRouteur;*/
                Sonde->AlerteAffichee = 0;
                Sonde->NouvelleVal=1;
                Sonde->TimeLastVal=time(NULL);
                Sonde->Moyenne = temperatureSonde; // Mise a jour de la température moyenne
            }

        }
       // free(ip);
    }
    return NULL;
}
