/*
 * Tabulka s rozptýlenými položkami
 *
 * S využitím datových typů ze souboru hashtable.h a připravených koster
 * funkcí implementujte tabulku s rozptýlenými položkami s explicitně
 * zretězenými synonymy.
 *
 * Při implementaci uvažujte velikost tabulky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptylovací funkce která přidělí zadanému klíči index z intervalu
 * <0,HT_SIZE-1>. Ideální rozptylovací funkce by měla rozprostírat klíče
 * rovnoměrně po všech indexech. Zamyslete sa nad kvalitou zvolené funkce.
 */
int get_hash(char *key) { 
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializace tabulky — zavolá sa před prvním použitím tabulky.
 */
void ht_init(ht_table_t *table) { // nastavi do vsech ukazatelu tabulky NULL hodnoty, jelikoz neobsahuje zadne prvky
  for (int i = 0; i < HT_SIZE; i++) {
    (*table)[i] = NULL;
  }
}

/*
 * Vyhledání prvku v tabulce.
 *
 * V případě úspěchu vrací ukazatel na nalezený prvek; v opačném případě vrací
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
  int index = get_hash(key); // zjistime hesh daneho klice, zjistime pod kterym odkazem se nachazi nas prvek
  ht_item_t *item = (*table)[index];
  while (item != NULL) {  // dokud ukazatel neukazuje na NULL, ukazuje-li na NULL -> neni hodnota nalezena a vraci NULL
    if (item->key == key) { // a klice se shoduji, vraci ukazatel na prvek
      return item;
    }
    else {  // dalsi prvek
      item = item->next;
    }
  }
  return NULL;
}

/*
 * Vložení nového prvku do tabulky.
 *
 * Pokud prvek s daným klíčem už v tabulce existuje, nahraďte jeho hodnotu.
 *
 * Při implementaci využijte funkci ht_search. Pri vkládání prvku do seznamu
 * synonym zvolte nejefektivnější možnost a vložte prvek na začátek seznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
  int index = get_hash(key); // zjistime hash
  ht_item_t *item = ht_search(table, key); // vyhledame v tabulce
  if (item == NULL) { // neexistuje -> vlozime
    item = malloc(sizeof(ht_item_t));
    item->key = key;
    item->value = value;
    item->next = (*table)[index]; // vlozeni prvku na zacatek seznamu
    (*table)[index] = item;
  }
  else { // existuje -> zmenime hodnoty key a value
    item->key = key;
    item->value = value;
  }
}

/*
 * Získání hodnoty z tabulky.
 *
 * V případě úspěchu vrací funkce ukazatel na hodnotu prvku, v opačném
 * případě hodnotu NULL.
 *
 * Při implementaci využijte funkci ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
  ht_item_t *item = ht_search(table, key); // vyhledame v tabulce
  if (item != NULL) { // nalezeno -> vratime ukazatel na hodnotu prvku
    return &(item->value);
  }
  return NULL;
}

/*
 * Smazání prvku z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje přiřazené k danému prvku.
 * Pokud prvek neexistuje, funkce nedělá nic.
 *
 * Při implementaci NEPOUŽÍVEJTE funkci ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
  //if (*key != '*') { //! PROC jsem to udelal?
    int index = get_hash(key); // zjistime hash
    ht_item_t *item = (*table)[index]; 
    ht_item_t *item_previous;
    while (item->key != key) { // dokud se klice neshoduji (item - shoduji se klice, item_prev je predchozi prvek)
      item_previous = item;
      item = item->next;
    }
    //item->value = 0; 
    //item->key = NULL;
    if (item == (*table)[index]) { // prvek se shodnym klicem je prvnim prvkem -> prvni prvek = nasledujici prvek
      (*table)[index] = item->next;
    }
    else {
      item_previous->next = item->next; // neni prvnim prvkem -> provedeme "premosteni"
    }
    free(item);
  //}
}

/*
 * Smazání všech prvků z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje a uvede tabulku do stavu po 
 * inicializaci.
 */
void ht_delete_all(ht_table_t *table) { // projdeme vsechny ukazatele a dokud neukazuji na null, tak smazeme prvek (automaticke posouvani prvniho prvku nam zajisti smazani vsech prvku)
  for (int i = 0; i < HT_SIZE; i++) {
    while ((*table)[i] != NULL) {
      ht_delete(table, (*table)[i]->key);
    }
  }
}
