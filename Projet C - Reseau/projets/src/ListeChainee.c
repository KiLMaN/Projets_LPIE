/**
    Auteur : Gaulupeau Brice
    Gestion d'une liste chainée de pointeurs,
    Cette liste peut tout contenir.
**/
#include "stdafx.h"



/**
    CreateListe
    Creer une liste n'ayant pas de suivant contenant le pointeur sur la donnée
    Retourne la nouvelle liste
**/
ListeChainee * CreateListe(void * pData)
{
    ListeChainee * pListe = (ListeChainee *) malloc(sizeof(ListeChainee));
    pListe->identifiant = 0;
    pListe->data = pData;
    pListe->next = NULL;
    return pListe;
}

/**
    AddElementListe
    Ajoute un element a la liste déjà éxistante et a la position voulue
    Si la position <= 0 alors l'element est ajouter en premier
    Si la position > taille de la liste alors l'element est ajouter a la fin
    Retourne la nouvelle liste
**/
ListeChainee * AddElementListeID(void * pData , ListeChainee * pListe,int position,int id)
{

    /* Si la liste est vide : On créer le premier element */
    if(pListe == NULL || position <= 0)
    {
        ListeChainee * pElement = CreateListe(pData);
        pElement->next = pListe;
        pElement->identifiant = id;
        return pElement;
    }
    else
    {
        /* Parcours la liste et cherche l'emplacement */
        int i = 0;
        ListeChainee * pElement = pListe;
        while(i+1 < position && pElement->next != NULL)
        {
            pElement = pElement->next;
            i++;
        }
        ListeChainee * pNewElem = CreateListe(pData);
        pNewElem->next = pElement->next;
        pElement->next = pNewElem;
        pElement->identifiant = id;
        return pListe;
    }
}
/**
    AddElementListe
    Ajoute un element a la liste déjà éxistante et a la position voulue
    Si la position <= 0 alors l'element est ajouter en premier
    Si la position > taille de la liste alors l'element est ajouter a la fin
    Retourne la nouvelle liste
**/
ListeChainee * AddElementListe(void * pData , ListeChainee * pListe,int position)
{
    return AddElementListeID(pData , pListe, position,0);
}

ListeChainee * deleteElementListe(ListeChainee * pListe,int pos)
{
    if(pListe == NULL) // Liste vide
        return NULL;
    else if(pos < 0 || pos > LengthListe(pListe)) // Si on demande la suppression en dehors de la taille de la liste
        return pListe;
    else if(pos == 0) // Suppression du premier element
    {
        ListeChainee * OldElem = pListe;
        pListe = pListe->next;
        free(OldElem);
        OldElem = NULL;
        return pListe;
    }
    else
    {
        /* Parcours la liste et cherche l'emplacement */
        int i = 0;
        ListeChainee * ElementListe = pListe;
        while(i+1 < pos)
        {
            ElementListe = ElementListe->next;
            i++;
        }
        // On obtient l'élement précédant celui a supprimer

        // On eleve le pointeur sur l'élement a supprimer
        ListeChainee * OldElem = ElementListe->next;
        ElementListe->next = OldElem->next;
        // On supprime l'élement
        free(OldElem);
        OldElem = NULL;
        return pListe;
    }

}


/**
    LengthListe
    Retourne le nombre d'éléments de la liste
**/
int LengthListe(ListeChainee * pListe)
{
    int i = 0;
    ListeChainee * elem = pListe;
    while(elem != NULL)
    {
        i++;
        elem = elem->next;
    }
    return i;
}



ListeChainee * deleteAllElements(ListeChainee * pListe)
{
    while(LengthListe(pListe) != 0)
        pListe = deleteElementListe(pListe,0);
    return pListe;
}

int getIDAt(ListeChainee * pListe, int index)
{
    if(LengthListe(pListe) < index || index < 0)
        return -1;


    ListeChainee * ElementListe = pListe;
    while(index--)
    {
        ElementListe = ElementListe->next;
    }
    return ElementListe->identifiant;
}

void * getValueAt(ListeChainee * pListe, int index)
{
    if(LengthListe(pListe) < index || index < 0)
        return NULL;


    ListeChainee * ElementListe = pListe;
    while(index--)
    {
        ElementListe = ElementListe->next;
    }
    return ElementListe->data;
}
void * getValueAtID(ListeChainee * pListe, int id)
{
    if(LengthListe(pListe) == 0)
        return NULL;


    ListeChainee * ElementListe = pListe;
    while(ElementListe != NULL)
    {
        if(ElementListe->identifiant == id)
            return ElementListe->data;

        ElementListe = ElementListe->next;
    }
    return NULL;
}

void afficherListe(ListeChainee * pListe)
{
    printf("Affichage de la liste : (%i elements) Uniquement des pointeurs !\n",LengthListe(pListe));
    ListeChainee * elem = pListe;
    while(elem != NULL)
    {
        // if( typeof *elem->data )
        printf("Valeur : %p \n",elem->data);
        ///printf("Valeur : %i \n",*(int *)elem->data);
        elem = elem->next;
    }
}
