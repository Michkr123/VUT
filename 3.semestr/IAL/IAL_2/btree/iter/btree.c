/*
 * Binární vyhledávací strom — iterativní varianta
 *
 * S využitím datových typů ze souboru btree.h, zásobníku ze souboru stack.h 
 * a připravených koster funkcí implementujte binární vyhledávací 
 * strom bez použití rekurze.
 */

#include "../btree.h"
#include "stack.h"
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
  (*tree) = NULL;
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * hodnotu daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 * 
 * Funkci implementujte iterativně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  bst_node_t *node = tree;
  while (node != NULL) { // dokud node je odlisne od NULL
    if (node->key == key) { // shodne klice -> return true a vraceni hodnoty prvku
      *value = node->value;
      return true;
    }
    else if (node->key > key) { // hledany klic je mensi -> jdeme doleva 
      node = node->left;
    }
    else { // hledany klic je vetsi -> jdeme doprava
      node = node->right;
    }
  }
  return false; // nenasli jsme shodny klic
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
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  bst_node_t *node = *tree;
  bst_node_t *prev_node = NULL;
  while (node != NULL) { // funkce bst_search, hleda existenci prvku s danym klicem
    if (node->key == key) { // nalezen -> prepiseme hodnotu prvku
      node->value = value;
      break;
    }
    else if (node->key > key) {
      prev_node = node;
      node = node->left;
    }
    else {
      prev_node = node;
      node = node->right;
    }
  }
  if (node == NULL) { // nebyl nalezen -> vlozime novy prvek
    node = malloc(sizeof(bst_node_t));
    node->key = key;
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    if (prev_node == NULL) { // je uplne prvnim prvkem stromu
      (*tree) = node;
    }
    else { // ve stromu existuje nejaky prvek
      if (prev_node->key > key) { // levy syn (klic je mensi)
        prev_node->left = node;
      }
      else { //pravy syn (klic je vetsi)
        prev_node->right = node;
      }
    }
  }
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 * 
 * Klíč a hodnota uzlu target budou nahrazené klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 * 
 * Tato pomocná funkce bude využita při implementaci funkce bst_delete.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  bst_node_t *node = *tree;
  bst_node_t *prev_node = NULL;
  while (node->right != NULL) { // najdeme nejpravejsi prvek stromu
    prev_node = node;
    node = node->right;
  }
  if (node->left != NULL) { // ma leveho syna, premostime se pres aktualni prvek
    if (prev_node != NULL) { // neni hned za targetem
      prev_node->right = node->left;
    }
    else { // je hned za targetem 
      target->left = node->left;
    }
  }
  else { // je list
    if (prev_node != NULL) { // neni hned za targetem -> musi ukazovat na NULL
      prev_node->right = NULL;
    }
    else { // neni hned za targetem -> tez musi ukazovat na NULL
      target->left = NULL;
    }
  }
  target->key = node->key; // nahradime key a value a uvolnime prvek
  target->value = node->value;
  free(node);
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
 * Funkci implementujte iterativně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
  bst_node_t *node = *tree;
  bst_node_t *prev_node = NULL;
  while (node != NULL) { // dokud neni node NULL
    if (node->key == key) { // nalezeno
      if (node->left != NULL && node->right != NULL) { // ma oba syny
        bst_replace_by_rightmost(node, &node->left); // nahradime nejpravejsim leveho syna targetu
      }
      else if (node->left != NULL && node->right == NULL) { // ma leveho syna
        if (prev_node != NULL) { // neni prvim prvkem
          if (prev_node->key > key) { // premostime se pres mazany prvek
            prev_node->left = node->left;
          }
          else {
            prev_node->right = node->left;
          }
        }
        else { // je prvnim prvkem 
          *tree = node->left;
        }
        free(node); // uvolnime mazany prvek
      }
      else if (node->left == NULL && node->right != NULL) { // ma praveho syna
        if (prev_node != NULL) { // neni prvnim prvkem
          if (prev_node->key > key) { // premostime se pres mazany prvek
            prev_node->left = node->right;
          }
          else { 
            prev_node->right = node->right;
          }
        }
        else { // je prvnim prvkem
          *tree = node->right;
        }
        free(node); // uvolnime mazany prvek
      }
      else { //je list
        if (prev_node != NULL) { // neni prvnim prvkem
          if (prev_node->key > key) { // predchozi bude ukazovat na NULL
            prev_node->left = NULL;
          }
          else {
            prev_node->right = NULL;
          } 
        }
        else { // je prvnim prvkem
          *tree = NULL;
        }
        free(node); // uvolnime mazany prvek
      }
      node = NULL;
    }
    else if (node->key > key) { // nenalezeno a hledany klic je mensi, ulozime si predchozi a projdeme dalsi
      prev_node = node;
      node = node->left;
    }
    else { // nenalezeno a hledany klic je vetsi, -||-
      prev_node = node;
      node = node->right;
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
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
  stack_bst_t *stack = malloc(sizeof(stack_bst_t));
  stack_bst_init(stack);
  bst_node_t *node = *tree;
  bool flag = true;
  if (node == NULL) { // strom je prazdny
    flag = false;
  }
  while (flag) {
    while (node != NULL) { // projdeme vsechny leve syny od korene az k nejvic levemu
      stack_bst_push(stack, node); // pushneme vsechny tyto prvky na stack -> zanoruji se -> pushnu si prvek na stack, at se muzu k nemu vratit
      node = node->left;
    }
    node = stack_bst_pop(stack); // nacteme si posledni prvek ze stacku
    if (node->right != NULL) { // ma praveho syna
      stack_bst_push(stack, node); // opet pushneme na stack
      node = node->right; // ulozime si syna
    }
    else { // je to list
      free(node); // uvolnime prvek
      if (stack->top == -1) { // stack je prazdny -> koncime mazani
        flag = false;
        *tree = NULL;
      }
      else { // stack neni prazdny
        node = stack_bst_pop(stack);; // nacteme dalsu prvek
        if (node->left != NULL) { // smazeme odkaz na levy (jiz smazany prvek)
          node->left = NULL;
        }
        else { // pokud ale je odkaz na leveho syna nastaven na NULL, tak byl mazany prvek pravy syn -> pravy odkaz nastavime na NULL
          node->right = NULL;
        }
      }
    }
  }
  free(stack); // uvolnime stack
}

/*
 * Pomocná funkce pro iterativní preorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu.
 * Nad zpracovanými uzly zavolá bst_add_node_to_items a uloží je do zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit, bst_items_t *items) {
  bst_node_t *node = tree;
  while(node != NULL) { // jdeme doleva a vsechny prvky pushujeme na stack (budeme se na ne vracet) a rovnou si je zapiseme na vystup
    bst_add_node_to_items(node, items);
    stack_bst_push(to_visit, node);
    node = node->left;
  }
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_preorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
  stack_bst_t *stack = malloc(sizeof(stack_bst_t));
  stack_bst_init(stack);
  bst_node_t *node = tree;
  bool flag = true;
  while(flag) { 
    while (node != NULL) { // dokud je node odlisny od NULL
      bst_leftmost_preorder(node, stack, items); // viz funkce bst_leftmost_preorder
      node = stack_bst_pop(stack); // nacteni prvku ze stacku
      while (node != NULL) { // dokud je node odlisny od NULL
        if (node->right != NULL) { //ma praveho syna -> prejdeme do nej
          node = node->right;
        }
        else {  //nema zadneho syna 
          if(stack->top == -1) { // zasobnik je prazdny -> koncime pruchod
            flag = false;
            node = NULL;
            break;
          }
          node =stack_bst_pop(stack); // nacteni ze stacku
          node = node->right; // jdeme do praveho syna a znovu projdmeme prvni while
          break;
        }
      }
    }
  }
  free(stack); // uvolnime stack
}

/*
 * Pomocná funkce pro iterativní inorder.
 * 
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
  bst_node_t *node = tree;
  while(node != NULL) { // projedeme vsechny leve syny od korene po list a pushneme je na stack
    stack_bst_push(to_visit, node);
    node = node->left;
  }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_inorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
  stack_bst_t *stack = malloc(sizeof(stack_bst_t));
  stack_bst_init(stack);
  bst_node_t *node = tree;
  bool flag = true;
  while(flag) {
    while (node != NULL) {
      bst_leftmost_inorder(node, stack); // viz funkce bst_leftmost_inorder
      node = stack_bst_pop(stack); // nacteni ze stacku
      while (node != NULL) { // dokud node je odlisny od NULL
      bst_add_node_to_items(node, items); // pridame ho na vystup
        if (node->right != NULL) { // ma praveho syna -> prejdeme do nej
          node = node->right;
          break;
        }
        else {  // je to list -> nacteme dalsi prvek ze stacku, neni-li stack prazdny
          if(stack->top == -1) {
            flag = false;
            node = NULL;
          }
          do {
            if(stack->top == -1) {
            flag = false;
            node = NULL;
            break;
            }
            node = stack_bst_pop(stack);
            bst_add_node_to_items(node, items);
            node = node->right;
          }while (node == NULL); // nema-li popnuty prvek praveho syna (levy je uz vyresen) -> popneme dalsi prvek ze stacku
          break;
        }
      }
    }
  }
  free(stack); // uvolnime stack
}

/*
 * Pomocná funkce pro iterativní postorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů. Do zásobníku bool hodnot ukládá informaci, že uzel
 * byl navštíven poprvé.
 *
 * Funkci implementujte iterativně pomocí zásobníku uzlů a bool hodnot a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
  bst_node_t *node = tree;
  while(node != NULL) { // projedeme vsechny leve syny od korene po list a pushneme je na stack a hodnoty true (navstiveno poprve) na bool stack
    stack_bst_push(to_visit, node);
    stack_bool_push(first_visit, true);
    node = node->left;
  }

}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_postorder a
 * zásobníku uzlů a bool hodnot a bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
  stack_bool_t *stack_bool = malloc(sizeof(stack_bool_t));
  stack_bool_init(stack_bool);
  stack_bst_t *stack = malloc(sizeof(stack_bst_t));
  stack_bst_init(stack);
  bst_node_t *node = tree;
  bool flag = true;
  while(flag) {
    while (node != NULL) {
      if (stack_bool->top == -1) { // jeli stack prazdny, viz funkce bst_leftmost_postorder
        bst_leftmost_postorder(node, stack, stack_bool);
      }
      if (stack_bool->items[stack_bool->top] == true) { // jeli prvek na vrcholu stacku nenavstiven, nacteme si ho a zadame false (znaci jiz navstiven)
        node = stack_bst_top(stack);
        stack_bool->items[stack_bool->top] = false;
      }
      while (1) { // nebite me za while true
        if (node->right != NULL) { //ma praveho syna
          node = node->right; // nacteme si praveho syna a pushneme ho na stacky
          stack_bst_push(stack, node);
          stack_bool_push(stack_bool, true);
          break;
        }
        else {  //nema praveho syna -> je to list -> vypisu na vystup
          bst_add_node_to_items(node, items);
          node = stack_bst_pop(stack);
          stack_bool->top--;
          node = stack_bst_top(stack);
          if (node->right != NULL) { // popnuty prvek ma praveho syna (levy uz je projity)
            if (stack_bool->items[stack_bool->top] == false) { // neni prvni navsteva -> vracime se -> zapisu na vystup
              bst_add_node_to_items(node, items);
              node = stack_bst_pop(stack);              
              stack_bool->top--;
            }
            else { // prvni navsteva -> nastavime false a prejdeme do praveho syna a pushneme ho na stacky
              stack_bool->items[stack_bool->top] = false;
              node = node->right;
              stack_bst_push(stack, node);
              stack_bool_push(stack_bool, true);
            }
          }
          if(stack->top == -1) { // jeli stack prazndy -> konec pruchodu
            flag = false;
            node = NULL;
          }
          break;
        }
      }
    }
  }
  free(stack); // uvolnime stacky
  free(stack_bool);
}
