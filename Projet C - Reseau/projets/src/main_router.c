#include "stdafx.h"

typedef struct t_ConfigRouteur
{
    char ipCoordinateur[20];
    int portCoordinateur;
    int portEcoute;
    int intervalEnvoi;
    int idRouteur;
}ConfigRouteur;

ConfigRouteur g_Config;


void * waitForClient (void * pData);
void * sendMoyenneCoordinateur(void * pData);

ListeChainee * ListeTemperature;
ListeChainee * ListeCompteur;

MySocket SocketEcoute;
MySocket SocketCoord;

int main()
{
    initReseau();

    char strConfig[20];

    /* Recuperation des valeurs du fichier de config */
    FILE * ConfigFile  = ChargerFichierConfig("config_routeur.txt");
    int ret;
    if((ret = getValue(ConfigFile,"IP_COORD",g_Config.ipCoordinateur)) != 1)
    {
        printf("Erreur dans la lecture de la configuration IP_COORD ! [%i]",ret);
        return -1;
    }

    if((ret = getValue(ConfigFile,"PORT_COORD",strConfig)) != 1)
    {
        printf("Erreur dans la lecture de la configuration PORT_COORD! [%i]",ret);
        return -1;
    }
    g_Config.portCoordinateur = atoi(strConfig);

    if((ret = getValue(ConfigFile,"PORT_ECOUTE",strConfig)) != 1)
    {
        printf("Erreur dans la lecture de la configuration PORT_ECOUTE! [%i]",ret);
        return -1;
    }
    g_Config.portEcoute = atoi(strConfig);

    if((ret = getValue(ConfigFile,"INTERVAL",strConfig)) != 1)
    {
        printf("Erreur dans la lecture de la configuration INTERVAL! [%i]",ret);
        return -1;
    }
     g_Config.intervalEnvoi = atoi(strConfig);

    if((ret = getValue(ConfigFile,"ID",strConfig)) != 1)
    {
        printf("Erreur dans la lecture de la configuration ID! [%i]",ret);
        return -1;
    }
    g_Config.idRouteur = atoi(strConfig);


    printf("Routeur ID : %i\n", g_Config.idRouteur);
    printf("Ecoute sur : %i\n", g_Config.portEcoute);
    printf("Coordinateur : %s:%i\n",g_Config.ipCoordinateur,g_Config.portCoordinateur);
    printf("Delais : %i s\n",g_Config.intervalEnvoi);
    printf("\n");


    SocketEcoute = creerSocketUdpServer ( g_Config.portEcoute);
    SocketCoord = creerSocketUdpClient();

    if(SocketEcoute > 0)
    {
        printf("Creation de la socket d'ecoute sur le port %i reussie !\n", g_Config.portEcoute);
    }
    else
    {
        printf("Erreur lors de la creation de la socket ! %i",SocketEcoute);
        return -1;
    }

    pthread_t Thread_waitForClient , Thread_SendMoyenne;
    pthread_create(&Thread_waitForClient,0,waitForClient,(void *)&SocketEcoute);

    pthread_create(&Thread_SendMoyenne,0,sendMoyenneCoordinateur,(void *)&SocketCoord);

    while(1)
        MySleep(1000);

    deInitReseau();
    return 0;
}


/**
    * Thread d'attente de messages udp contenant la température provenant des sondes
**/
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
        /*char * ip = ( char * ) malloc(sizeof(char) * 20);
        sprintf ( ip, "%s", inet_ntoa (exp_addr.sin_addr));
        int port = ntohs(exp_addr.sin_port);*/

       // printf("Message recu : %s:%i => (%i)[%s]\n",ip,port,n,buff);


        int idSonde = -1;
        float temperatureSonde = -1.0;
        /* Le protocole doit être "T,<ID>,<TEMP>,E" */

        if(*buff == 'T' && *(buff + strlen(buff)-1 ) == 'E')
        {
            char * posVirgule, * nextVirgule;
            posVirgule=strchr(buff,',');

            if(posVirgule!=NULL)
            {
                nextVirgule=strchr(posVirgule+1,',');
                if(nextVirgule!=NULL)
                {
                    char id[5] = "";
                    strncpy(id,posVirgule+1,(nextVirgule-posVirgule) -1 );
                    idSonde = atoi(id);

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
        else
        {
            /* Si le protocole n'est pas bon on passe en affichant un message */
            printf("PROTOCOL_ERROR !\n");
            continue;
        }

        if(temperatureSonde != -1.0 && idSonde != -1)
        {
            float * ElementTemp = getValueAtID(ListeTemperature,idSonde);
            int * ElementCompteur = getValueAtID(ListeCompteur,idSonde);
            if(ElementTemp == NULL)
            {
                float * DataListeTemp = malloc(sizeof (float));
                int * DataListeCpt = malloc(sizeof (int));
                *DataListeCpt = 1;
                *DataListeTemp = temperatureSonde;
                ListeTemperature = AddElementListeID( DataListeTemp, ListeTemperature,0,idSonde);

                ListeCompteur = AddElementListeID( DataListeCpt, ListeCompteur,0,idSonde);
            }
            else
            {
                *ElementTemp += temperatureSonde;
                *ElementCompteur += 1;
            }

        }
       // free(ip);
    }
    return NULL;
}

void * sendMoyenneCoordinateur(void * pData)
{
    MySocket sock = *((MySocket *)pData); // Recuperation de la socket
    while(1)
    {
         MySleep(10000);
    // printf("Calcul des moyennes ! \n");

    int nbSondes = LengthListe(ListeTemperature);
    if(nbSondes != LengthListe(ListeCompteur))
    {
        printf("Erreur : les listes ne sont pas synchronisées ! \n");
    }

    int i = 0;
    while(i < nbSondes)
    {
        // TODO : AJOUTER MUTEX PROTECTION */
        float * DataListeTemp = getValueAt(ListeTemperature,i);
        int * DataListeCpt = getValueAt(ListeCompteur,i);

float MoyenneSonde = -1.0;
        if(*DataListeCpt != 0)
            MoyenneSonde = *DataListeTemp / *DataListeCpt;


        *DataListeTemp = 0;
        *DataListeCpt = 0;
        // TODO : END MUTEX PROTECTION */

        printf("Moyenne pour la sonde %i : %f | Envoi -> ",getIDAt(ListeCompteur,i),MoyenneSonde);

        char buffer[80];
        sprintf(buffer,"M,%i,%f,E",i,MoyenneSonde);
        int ret = sendUdpMessageTo(sock , buffer , g_Config.ipCoordinateur ,  g_Config.portCoordinateur );
        if(ret == -1)
        {
            printf(" Erreur lors de l'envoi du message ! (%i)\n",ret);
        }
        else
        {
            printf(" OK \n");
        }


        i++;
    }



    }
    return NULL;
}
