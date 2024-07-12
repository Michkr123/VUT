/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/*           Daniel Dolejška, září 2022                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

bool error_flag;
bool solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error(void) {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = true;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) { // vyreseno viz prezetace, nastavime vsechny hondoty na NULL
	list->activeElement = NULL;
	list->firstElement = NULL;
	list->lastElement = NULL;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) { // dokud neni seznam prazdny, mazeme pvni prvek
	while (list->firstElement != NULL) { // pouzita funkce DLL_DeleteFirst - vysvetlena nize :)
		if (list->activeElement == list->firstElement) { 
			list->activeElement = NULL;
		}
		if (list->firstElement == list->lastElement) { 
			free(list->firstElement);
			list->firstElement = NULL;
			list->lastElement = NULL;
		}
		else { 
			list->firstElement = list->firstElement->nextElement; 
			free(list->firstElement->previousElement);
			list->firstElement->previousElement = NULL;
		}
	}
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, int data ) {
	DLLElementPtr newElemPtr = malloc(sizeof(struct DLLElement)); // alokace noveho prvku a ulozeni ukazatele na nej
	if (newElemPtr == NULL) { // alokace selhala
		DLL_Error();
	}
	else { 
		newElemPtr->data = data; // vlozime do nej data
		newElemPtr->nextElement = list->firstElement; // po nasem novem prvnim prvku seznamu bude nasledovat stavajici prvni prvek
		newElemPtr->previousElement = NULL; // pred nim zadny jiny prvek neni
		if (list->firstElement != NULL) { // seznam ma prvni prvek -> novy prvni prvek je predchazejici stavajiciho prvniho prvku 
			list->firstElement->previousElement = newElemPtr;
		}
		else { // seznam nema prvni prvek -> seznam je prazdny -> novy prvni prvek je zaroven i posledni prvek
			list->lastElement = newElemPtr;
		}
		list->firstElement = newElemPtr;
	}
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, int data ) { // viz DLL_InsertFrist, akorat vsechny last <-> first a previous <-> next 
	DLLElementPtr newElemPtr = malloc(sizeof(struct DLLElement));
	if (newElemPtr == NULL) {
		DLL_Error();
	}
	else {
		newElemPtr->data = data;
		newElemPtr->previousElement= list->lastElement;
		newElemPtr->nextElement = NULL;
		if (list->lastElement != NULL) {
			list->lastElement->nextElement = newElemPtr;
		}
		else {
			list->firstElement = newElemPtr;
		}
		list->lastElement = newElemPtr;
	}
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {
	list->activeElement = list->firstElement;
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {
	list->activeElement = list->lastElement;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, int *dataPtr ) {
	if (list->firstElement == NULL) {
		DLL_Error();
	}
	else {
		*dataPtr = list->firstElement->data; // vracime ukazatel na data prvniho prvku seznamu
	}
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, int *dataPtr ) {
	if (list->lastElement == NULL) {
		DLL_Error();
	}
	else {
			*dataPtr = list->lastElement->data;
	}	
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {
	if (list->firstElement != NULL) {
		if (list->activeElement == list->firstElement) { // zrusime akrivitu, jeli prvni prvek aktivni
			list->activeElement = NULL;
		}
		if (list->firstElement == list->lastElement) {  // ma-li tento seznam jen jeden prvek, smazeme tento prvek a seznam bude prazdny
			free(list->firstElement);
			list->firstElement = NULL;
			list->lastElement = NULL;
		}
		else { 
			list->firstElement = list->firstElement->nextElement; // prvnim prvkem se stava druhy prvek a prvni smazeme
			free(list->firstElement->previousElement);
			list->firstElement->previousElement = NULL;
		}
	}
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {
	if (list->firstElement != NULL) { // viz DLL_DeleteFirst
		if (list->activeElement == list->lastElement) {
			list->activeElement = NULL;
		}
		if (list->firstElement == list->lastElement) { 
			free(list->lastElement);
			list->firstElement = NULL;
			list->lastElement = NULL;
		}
		else {
			list->lastElement = list->lastElement->previousElement;
			free(list->lastElement->nextElement);
			list->lastElement->nextElement = NULL;
		}
	}
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {
	if (list->activeElement != NULL) {
		if (list->activeElement != list->lastElement) { // jeli nastaveny aktivni prvek a neni zaroven posledni
			DLLElementPtr newElemPtr = list->activeElement->nextElement; // ulozime si pointer na mazany prvek
			list->activeElement->nextElement = newElemPtr->nextElement; // premostime se pres mazany prvek
			if (newElemPtr == list->lastElement) { // pokud za mazanym prvkem nic neni, aktivni se stava poslednim
				list->lastElement = list->activeElement;
			} 
			else { // navazeme zpetne prvek za mazanym na aktivni
				newElemPtr->nextElement->previousElement = list->activeElement;
			}
			free(newElemPtr); // uvolnime mazany prvek
		}
	}
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) { // viz DLL_DeleteAfter
	if (list->activeElement != NULL) { 
		if (list->activeElement != list->firstElement) {
			DLLElementPtr newElemPtr = list->activeElement->previousElement;
			list->activeElement->previousElement = newElemPtr->previousElement;
			if (newElemPtr == list->firstElement) {
				list->firstElement = list->activeElement;
			} 
			else {
				newElemPtr->previousElement->nextElement = list->activeElement;
			}
			free(newElemPtr);
		}
	}
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter( DLList *list, int data ) {
	if(list->activeElement != NULL) { 
		DLLElementPtr newElemPtr = malloc(sizeof(struct DLLElement));
		newElemPtr->data = data;
		newElemPtr->nextElement = list->activeElement->nextElement; // novy ukazuje za aktivni prvek
		newElemPtr->previousElement = list->activeElement; // zpetne ukazuje na aktivni prvek
		list->activeElement->nextElement = newElemPtr; // aktivni ukazuje na novy

		if (list->activeElement == list->lastElement) { // pokud byl aktivni posledni -> novy je posledni
			list->lastElement = newElemPtr;
		}
		else {
			newElemPtr->nextElement->previousElement = newElemPtr; // puvodne nasledujici za aktivnim bude zpetne ukazovat na novy
		} 
	}
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, int data ) { // viz DLL_InsertAfter
	if(list->activeElement != NULL) {
		DLLElementPtr newElemPtr = malloc(sizeof(struct DLLElement));
		newElemPtr->data = data;
		newElemPtr->previousElement = list->activeElement->previousElement;
		newElemPtr->nextElement = list->activeElement;
		list->activeElement->previousElement = newElemPtr;

		if (list->activeElement == list->firstElement) {
			list->firstElement = newElemPtr;
		}
		else {
			newElemPtr->previousElement->nextElement = newElemPtr;
		}
	}
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, int *dataPtr ) {
	if (list->activeElement == NULL) {
		DLL_Error();
	}
	else {
		*dataPtr = list->activeElement->data; // vrati ukazatel na data aktivniho prvku
	}
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, int data ) {
	if (list->activeElement != NULL) {
		list->activeElement->data = data; // prepise data aktivniho prvku
	}
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {
	if (list->activeElement != NULL) {
		if (list->activeElement == list->lastElement) { // jeli aktivni posledni prvek, aktivita se ztraci
			list->activeElement = NULL;
		}
		else {
			list->activeElement = list->activeElement->nextElement; // jinak se posune na nasledujiciho
		}
	}
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) { // viz o funkci vyse
	if (list->activeElement != NULL) {
		if (list->activeElement == list->firstElement) {
			list->activeElement = NULL;
		}
		else {
			list->activeElement = list->activeElement->previousElement;
		}
	}
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive( DLList *list ) {
	return list->activeElement != NULL? true : false; // tercialni operator pro zjisteni nastaveni aktivity prvku
}

/* Konec c206.c */
