#include "stdafx.h"
typedef struct t_ConfigCoordinateur
{
    int portEcoute;
}ConfigCoordinateur;
ConfigCoordinateur g_Config;


void * waitForClient (void * pData);
void * sendMoyenneCoordinateur(void * pData);

/*ListeChainee * ListeTemperature;
ListeChainee * ListeCompteur;*/

MySocket SocketEcoute;
MySocket SocketCoord;

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
    //SocketCoord = creerSocketUdpClient();

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

  // pthread_create(&Thread_SendMoyenne,0,sendMoyenneCoordinateur,(void *)&SocketCoord);

    while(1)
        MySleep(1000);

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
        char * ip = ( char * ) malloc(sizeof(char) * 20);
        sprintf ( ip, "%s", inet_ntoa (exp_addr.sin_addr));
        int port = ntohs(exp_addr.sin_port);

       printf("Message recu : %s:%i => (%i)[%s]\n",ip,port,n,buff);


//        int idSonde = -1;
//        float temperatureSonde = -1.0;
//        /* Le protocole doit être "T,<ID>,<TEMP>,E" */
//
//        if(*buff == 'T' && *(buff + strlen(buff)-1 ) == 'E')
//        {
//            char * posVirgule, * nextVirgule;
//            posVirgule=strchr(buff,',');
//
//            if(posVirgule!=NULL)
//            {
//                nextVirgule=strchr(posVirgule+1,',');
//                if(nextVirgule!=NULL)
//                {
//                    char id[5] = "";
//                    strncpy(id,posVirgule+1,(nextVirgule-posVirgule) -1 );
//                    idSonde = atoi(id);
//
//                    posVirgule = nextVirgule;
//                    nextVirgule=strchr(posVirgule+1,',');
//                    if(nextVirgule!=NULL)
//                    {
//                        char temperature[20] = "";
//                        strncpy(temperature,posVirgule+1,(nextVirgule-posVirgule) -1 );
//                        temperatureSonde = atof(temperature);
//                    }
//                }
//            }
//        }
//        else
//        {
//            /* Si le protocole n'est pas bon on passe en affichant un message */
//            printf("PROTOCOL_ERROR !\n");
//            continue;
//        }
//
//        if(temperatureSonde != -1.0 && idSonde != -1)
//        {
//            float * ElementTemp = getValueAtID(ListeTemperature,idSonde);
//            int * ElementCompteur = getValueAtID(ListeCompteur,idSonde);
//            if(ElementTemp == NULL)
//            {
//                float * DataListeTemp = malloc(sizeof (float));
//                int * DataListeCpt = malloc(sizeof (int));
//                *DataListeCpt = 1;
//                *DataListeTemp = temperatureSonde;
//                ListeTemperature = AddElementListeID( DataListeTemp, ListeTemperature,0,idSonde);
//
//                ListeCompteur = AddElementListeID( DataListeCpt, ListeCompteur,0,idSonde);
//            }
//            else
//            {
//                *ElementTemp += temperatureSonde;
//                *ElementCompteur += 1;
//            }
//
//        }
        free(ip);
    }
    return NULL;
}
