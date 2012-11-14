#ifndef LISTE_CHAINEE_H
#define LISTE_CHAINEE_H

typedef struct t_listeChainee
{
    int identifiant;
    void * data;
    struct t_listeChainee * next;
} ListeChainee;

ListeChainee * CreateListe(void * pData );
ListeChainee * AddElementListe(void * pData , ListeChainee * pListe,int position);
ListeChainee * AddElementListeID(void * pData , ListeChainee * pListe,int position,int id);

ListeChainee * deleteElementListe(ListeChainee * pListe,int pos);
ListeChainee * deleteAllElements(ListeChainee * pListe);

int getIDAt(ListeChainee * pListe, int index);
void * getValueAtID(ListeChainee * pListe, int id);
void * getValueAt(ListeChainee * pListe, int index);
int LengthListe(ListeChainee * pListe);
void afficherListe(ListeChainee * pListe);


#endif //LISTE_CHAINEE_H
