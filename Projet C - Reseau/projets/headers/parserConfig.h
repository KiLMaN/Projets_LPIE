#ifndef PARSER_CONFIG_H
#define PARSER_CONFIG_H

#define TAILLE_LECTURE_MAX 100
#define SIGNE_EGAL '='

//FILE * g_fichierConf;

FILE * ChargerFichierConfig(char * pathFichier);
int getValue(FILE * fichier ,char * var,char * retVal);
int FermerFichierconfig(FILE * fichier);


#endif // PARSER_CONFIG_H
