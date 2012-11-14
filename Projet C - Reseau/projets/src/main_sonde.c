#include "stdafx.h"


int main()
{

    initReseau();
    time_t LastSend = 0; // Heure du last send
    MySRand(time(0)); // Init du random
    double temperature = 0; // Stocke la température
    int sleepTime = 0;//(CONFIG_TEMPS_MESURE/5 * 1000 ) ;
    char * buffer = malloc(80);
    char Heure[80];
    char ip[20] = "127.0.0.1";
    char strConfig[20];
    int port = 8002 ;
    int intervalEnvoi = 20;
    int ID = 0;
    MySocket SocketRouteur = creerSocketUdpClient();


    if( SocketRouteur > 0)
    {

        /* Recuperation des valeurs du fichier de config */
        FILE * ConfigFile  = ChargerFichierConfig("config_sonde.txt");
        int ret;
        if((ret = getValue(ConfigFile,"IP",ip)) != 1)
        {
            printf("Erreur dans la lecture de la configuration IP ! [%i]",ret);
            return -1;
        }

        if((ret = getValue(ConfigFile,"PORT",strConfig)) != 1)
        {
            printf("Erreur dans la lecture de la configuration PORT! [%i]",ret);
            return -1;
        }
        port = atoi(strConfig);

         if((ret = getValue(ConfigFile,"INTERVAL",strConfig)) != 1)
        {
            printf("Erreur dans la lecture de la configuration INTERVAL! [%i]",ret);
            return -1;
        }
        intervalEnvoi = atoi(strConfig);

        if((ret = getValue(ConfigFile,"ID",strConfig)) != 1)
        {
            printf("Erreur dans la lecture de la configuration ID! [%i]",ret);
            return -1;
        }
        ID = atoi(strConfig);


        printf("Sonde ID : %i\n",ID);
        printf("Routeur : %s:%i\n",ip,port);
        printf("Delais : %i\n",intervalEnvoi);
        printf("\n");

        sleepTime = intervalEnvoi * 1000 -1;

        while(1)
        {
            /* Calcul du temps d'envoi */
            int NextSend = intervalEnvoi + LastSend;
            if(NextSend <= time(NULL))
            {

                getStrTime(Heure);
                LastSend = time(NULL);

                /* Simulation d'une température */
                temperature = mesure_temperature();

                /* Creatuion du message à envoyer */
                sprintf(buffer,"T,%i,%f,E",ID,temperature);

                printf("%s Envoi d'une mesure  : %f ->",Heure,temperature);

                int ret = sendUdpMessageTo(SocketRouteur , buffer , ip , port );
                if(ret == -1)
                {
                    printf(" Erreur lors de l'envoi du message ! (%i)\n",ret);
                }
                else
                {
                    printf(" OK \n");
                }
                fflush(stdout);

            }
            MySleep(sleepTime);
        }
    }

    free(buffer);
    deInitReseau();
    return 0;
}
