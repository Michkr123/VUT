/*
 * Použití binárních vyhledávacích stromů.
 *
 * S využitím Vámi implementovaného binárního vyhledávacího stromu (soubory ../iter/btree.c a ../rec/btree.c)
 * implementujte funkci letter_count. Výstupní strom může být značně degradovaný (až na úroveň lineárního seznamu) 
 * a tedy implementujte i druhou funkci (bst_balance), která strom, na požadavek uživatele, vybalancuje.
 * Funkce jsou na sobě nezávislé a tedy automaticky NEVOLEJTE bst_balance v letter_count.
 * 
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>


/**
 * Vypočítání frekvence výskytů znaků ve vstupním řetězci.
 * 
 * Funkce inicilializuje strom a následně zjistí počet výskytů znaků a-z (case insensitive), znaku 
 * mezery ' ', a ostatních znaků (ve stromu reprezentováno znakem podtržítka '_'). Výstup je v 
 * uložen ve stromu.
 * 
 * Například pro vstupní řetězec: "abBccc_ 123 *" bude strom po běhu funkce obsahovat:
 * 
 * key | value
 * 'a'     1
 * 'b'     2
 * 'c'     3
 * ' '     2
 * '_'     5
 * 
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce.
*/
void Upper_to_lower(char *letter) { // prevod pismen na lowercase a roztrideni znaku ' ' a ostatnich
    if (*letter >= 'A' && *letter <= 'Z') {
        *letter = *letter + 32;
    }
    else if (*letter == ' ' || (*letter >= 'a' && *letter <= 'z')) {
    }
    else {
        *letter = '_';
    }
}

void letter_count(bst_node_t **tree, char *input) {
    bst_init(tree);
    int value;
    char letter;
    for (int i = 0; input[i] != '\0'; i++) { // projdeme vstupni retezec
        letter = input[i];
        Upper_to_lower(&letter);
        if (bst_search(*tree, letter, &value)) { // pokud jiz toto pismeno existuje -> zvys value o 1 
            bst_insert(tree, letter, ++value);
        }
        else { // jeste neexistuje -> pridej do stromu s value 1
            bst_insert(tree, letter, 1); 
        }
    }
}



/**
 * Vyvážení stromu.
 * 
 * Vyvážený binární vyhledávací strom je takový binární strom, kde hloubka podstromů libovolného uzlu se od sebe liší maximálně o jedna.
 * 
 * Předpokládejte, že strom je alespoň inicializován. K získání uzlů stromu využijte vhodnou verzi vámi naimplmentovaného průchodu stromem.
 * Následně můžete například vytvořit nový strom, kde pořadím vkládaných prvků zajistíte vyváženost.
 *  
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce. Není nutné, aby funkce fungovala *in situ* (in-place).
*/
void bst_balanced_func(bst_items_t *items, bst_node_t **tree_balanced, int lower, int upper) {
        int middle = (lower+upper)/2; // spocitame si stred serazeneho pole
        if (!bst_search(*tree_balanced, items->nodes[middle]->key, &items->nodes[middle]->value)) { // prvek jeste neexistuje -> pridame
            bst_insert(tree_balanced, items->nodes[middle]->key, items->nodes[middle]->value);
            bst_balanced_func(items, tree_balanced, lower, middle); // volani rekurzivne nad spodni polovinou
            bst_balanced_func(items, tree_balanced, middle, upper); // volani rekurzivne nad vrchni polovinou (bere jiz jednou projite intervaly, ale podminka nahore to fixne)
        } 
}

void bst_balance(bst_node_t **tree) {
    bst_items_t *items = malloc(sizeof(bst_items_t)); // items init
    items->capacity = 0;
    items->nodes = NULL;
    items->size = 0;
    bst_inorder(*tree, items); // serazeni prvku abecedne a vlozeni do init
    bst_node_t *tree_balanced; // novy strom a init
    bst_init(&tree_balanced);
    bst_balanced_func(items, &tree_balanced, 0, items->size); // volani pomocne funkce viz bst_balance_func
    bst_dispose(tree); // smazani puvodniho stromu
    free(items->nodes); // uvolneni vsech zdroju
    free(items);
    *tree = tree_balanced;
}