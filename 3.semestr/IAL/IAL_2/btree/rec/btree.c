/*
 * Binární vyhledávací strom — rekurzivní varianta
 *
 * S využitím datových typů ze souboru btree.h a připravených koster funkcí
 * implementujte binární vyhledávací strom pomocí rekurze.
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci. 
 */
void bst_init(bst_node_t **tree) { // ukazatel tree bude ukazovat na NULL
  *tree = NULL;
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * hodnotu daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 * 
 * Funkci implementujte rekurzivně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  if (tree == NULL) { // strom je prazdny
    return false;
  }
  if (tree->key == key) { // klice se shoduji v koreni
    *value = tree->value;
    return true;
  }
  else { // klice korene a hledany klic se neshoduji
    if(bst_search(tree->left, key, value)) // rekurzivne volam funkci na leveho syna
    {
      return true;
    }
    if (bst_search(tree->right, key, value)) // a praveho syna
    {
      return true;
    }
    return false;
  }
}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší. 
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  if ((*tree) == NULL) { // prvni prvek ve stromu
    (*tree) = malloc(sizeof(bst_node_t));
    (*tree)->key = key;
    (*tree)->value = value;
    (*tree)->right = NULL;
    (*tree)->left = NULL;
  }
  else if ((*tree)->key == key) { // existujici prvek -> prepiseme value
    (*tree)->value = value;
  }
  else if ((*tree)->key > key) { // klic je mensi nez klic prvku -> jdeme doleva
    if ((*tree)->left == NULL) { // nema leveho syna -> novy prvek
      (*tree)->left = malloc(sizeof(bst_node_t));
      (*tree)->left->key = key;
      (*tree)->left->value = value;
      (*tree)->left->left = NULL;
      (*tree)->left->right = NULL;
    }
    else { //ma leveho syna -> rekurze 
      bst_insert(&((*tree)->left), key, value);
    }
  }
  else { // klic je vetsi nez klic prvku, to same v blede modrem akorat doprava
    if ((*tree)->right == NULL) {
      (*tree)->right = malloc(sizeof(bst_node_t));
      (*tree)->right->key = key;
      (*tree)->right->value = value;
      (*tree)->right->right = NULL;
      (*tree)->right->left = NULL;
    }
    else {
      bst_insert(&((*tree)->right), key, value);
    }
  }
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 * 
 * Klíč a hodnota uzlu target budou nahrazeny klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 * 
 * Tato pomocná funkce bude využitá při implementaci funkce bst_delete.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  if ((*tree)->right == NULL) { // nasli jsme nejpravejsi prvek -> prekopirujeme hodnoty tohoto prvku do target 
    target->key = (*tree)->key;
    target->value = (*tree)->value;
    if ((*tree)->left == NULL) { //je to list
      free(*tree); // uvolnime nejpravejsi prvek leveho podstromu targetu
      *tree = NULL; 
    }
    else { // ma leveho syna -> prepiseme hodnoty leveho syna do aktualniho prvku i adresy na jeho podstromy a smazeme puvodniho leveho syna naseho prvku
      (*tree)->key = (*tree)->left->key;
      (*tree)->value = (*tree)->left->value;
      bst_node_t *node = (*tree)->left;
      (*tree)->right = (*tree)->left->right;
      (*tree)->left = (*tree)->left->left;
      free(node);
    }
  }
  else { // ma praveho syna -> rekurze
    bst_replace_by_rightmost(target, &(*tree)->right);
  }
}

/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 * 
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 * 
 * Funkci implementujte rekurzivně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
  if ((*tree) != NULL) { // predany strom neni prazdny
    if ((*tree)->key == key) { // klice se shoduji
      if ((*tree)->left != NULL && (*tree)->right != NULL) { // ma oba syny -> viz funkce bst_replace_by_rightmost
        bst_replace_by_rightmost((*tree), &(*tree)->left);
      } 
      else if ((*tree)->left != NULL && (*tree)->right == NULL) { // ma leveho syna -> nakopirujeme hodnoty i adresy a smazeme puvodniho leveho syna
        (*tree)->key = (*tree)->left->key;
        (*tree)->value = (*tree)->left->value;
        bst_node_t *node_del = (*tree)->left;
        (*tree)->right = (*tree)->left->right;
        (*tree)->left = (*tree)->left->left;
        free(node_del);
      }
      else if (((*tree)->left == NULL && (*tree)->right != NULL)){ // ma praveho syna -> v blede modrym to samy akorat right <=> left
        (*tree)->key = (*tree)->right->key;
        (*tree)->value = (*tree)->right->value;
        bst_node_t *node_del = (*tree)->right;
        (*tree)->left = (*tree)->right->left;
        (*tree)->right = (*tree)->right->right;
        free(node_del);
      }
      else { // je to list -> smazeme
        free(*tree);
        (*tree) = NULL;
      }
    }
    else { // klice se neshoduji -> rekurze
      bst_delete(&(*tree)->left, key);
      bst_delete(&(*tree)->right, key);
    }
  }
}

/*
 * Zrušení celého stromu.
 * 
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po 
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených 
 * uzlů.
 * 
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
  if (*tree != NULL) { // predany strom neni prazdny
    bst_dispose(&(*tree)->left); // rekurzivne doleva
    bst_dispose(&(*tree)->right); // rekurzivne doprava (pri vraceni se z leve casti -> projde vse)
    free(*tree); // uvolnime prvek
    (*tree) = NULL;
  }
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
  if (tree != NULL) { // predany strom neni prazdny
    bst_add_node_to_items(tree, items); // pri prvnim vstupu do prvku ho zapiseme do items
    bst_preorder(tree->left, items); // rekurze doleva
    bst_preorder(tree->right, items); // rekurze doprava
  }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
  if (tree != NULL) { // predany strom neni prazdny
    bst_inorder(tree->left, items); // rekurze doleva 
    bst_add_node_to_items(tree, items); // pri vyreseni leveho podstromu zapiseme prvek do items
    bst_inorder(tree->right, items); // rekurze doprava
  }
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
  if (tree != NULL) { // predany strom neni prazdny
    bst_postorder(tree->left, items); // rekurze doleva
    bst_postorder(tree->right, items); // rekurze doprava
    bst_add_node_to_items(tree, items); // az pri vyreseni praveho podstromu zapiseme prvek do items
  }
}
