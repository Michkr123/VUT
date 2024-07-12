/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického výrazu
** do postfixového tvaru. Pro převod využijte zásobník (Stack), který byl
** implementován v rámci příkladu c202. Bez správného vyřešení příkladu c202
** se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix ... konverzní funkce pro převod infixového výrazu
**                      na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar ... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"

bool solved;

/**
 * Pomocná funkce untilLeftPar.
 * Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka bude
 * také odstraněna.
 * Pokud je zásobník prázdný, provádění funkce se ukončí.
 *
 * Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
 * znaků postfixExpression.
 * Délka převedeného výrazu a též ukazatel na první volné místo, na které se má
 * zapisovat, představuje parametr postfixExpressionLength.
 *
 * Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
 * nadeklarovat a používat pomocnou proměnnou typu char.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void untilLeftPar( Stack *stack, char *postfixExpression, unsigned *postfixExpressionLength ) {
	while (!Stack_IsEmpty(stack) && stack->array[stack->topIndex] != '(') { // vyprazdni zasobnik na vystupni retezec az do leve zavorky, ktera je take nasledovne odstranena ze zasobniku
		Stack_Top(stack, &postfixExpression[(*postfixExpressionLength)++]);
		Stack_Pop(stack);
	}
	if (!Stack_IsEmpty(stack)) Stack_Pop(stack);
}

/**
 * Pomocná funkce doOperation.
 * Zpracuje operátor, který je předán parametrem c po načtení znaku ze
 * vstupního pole znaků.
 *
 * Dle priority předaného operátoru a případně priority operátoru na vrcholu
 * zásobníku rozhodneme o dalším postupu.
 * Délka převedeného výrazu a taktéž ukazatel na první volné místo, do kterého
 * se má zapisovat, představuje parametr postfixExpressionLength, výstupním
 * polem znaků je opět postfixExpression.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param c Znak operátoru ve výrazu
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void doOperation( Stack *stack, char c, char *postfixExpression, unsigned *postfixExpressionLength ) {
	char StackTop;
	while (1) { //osklivy loop :(
		if (Stack_IsEmpty(stack)) { // pushni operator na zasobnik, jeli prazdny
			Stack_Push(stack, c);
			break;
		}
		else {
			Stack_Top(stack, &StackTop); // neni-li prazdny, prectu si, co je na vrcholu zasobniku
		}
		if (StackTop == '(' || ((StackTop == '+' || StackTop == '-') && (c == '/' || c == '*'))) { // jeli tam leva zavorka nebo operator s nizsi prioritou, pushni operator na zasobnik
			Stack_Push(stack, c);
			break;
		}
		else { // pokud je tam neco jineho (operator s vyssi prioritou), vypis ho ze zasobniku na vystupni retezec + opakujeme cely proces
			postfixExpression[(*postfixExpressionLength)++] = StackTop;
			Stack_Pop(stack);
		}
	}
}

/**
 * Konverzní funkce infix2postfix.
 * Čte infixový výraz ze vstupního řetězce infixExpression a generuje
 * odpovídající postfixový výraz do výstupního řetězce (postup převodu hledejte
 * v přednáškách nebo ve studijní opoře).
 * Paměť pro výstupní řetězec (o velikosti MAX_LEN) je třeba alokovat. Volající
 * funkce, tedy příjemce konvertovaného řetězce, zajistí korektní uvolnění zde
 * alokované paměti.
 *
 * Tvar výrazu:
 * 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
 *    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
 *    násobení má stejnou prioritu jako dělení. Priorita násobení je
 *    větší než priorita sčítání. Všechny operátory jsou binární
 *    (neuvažujte unární mínus).
 *
 * 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
 *    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
 *
 * 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
 *    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
 *    chybné zadání výrazu).
 *
 * 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen
 *    ukončovacím znakem '='.
 *
 * 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
 *
 * Poznámky k implementaci
 * -----------------------
 * Jako zásobník použijte zásobník znaků Stack implementovaný v příkladu c202.
 * Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
 *
 * Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
 *
 * Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
 * char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
 *
 * Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
 * nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
 * by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
 * ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
 * řetězce konstantu NULL.
 *
 * @param infixExpression vstupní znakový řetězec obsahující infixový výraz k převedení
 *
 * @returns znakový řetězec obsahující výsledný postfixový výraz
 */
char *infix2postfix( const char *infixExpression ) {
	char *postfixExpression = (char *)malloc(sizeof(char) * MAX_LEN); // alokace pameti pro vystupni retezec, selhani -> return NULL
	if (postfixExpression == NULL) {
		return NULL;
	}
	else {
		for (int i = 0; i < MAX_LEN; i++) { // naplnime vystupni retezec nulami, predejdeme nahodnym hodnotam
			postfixExpression[i] = '\0';
		}
	}
	Stack stack;
	Stack_Init(&stack);
	if (stack.array == NULL) { // pokud selze alokovani pameti pro stack, uvolnime pamet alokovanou pro postfixExpression a vratime NULL
		free(postfixExpression);
		return NULL;
	}

	unsigned postfixExpressionLength = 0;

	for (int i = 0; infixExpression[i] != '\0'; i++) { // prochazime vstupni retezec znak po znaku (viz prezentace)
		char currentLetter = infixExpression[i]; // pro snazsi manipulaci si vytvorime promennou
		if ((currentLetter >= 'a' && currentLetter <= 'z') || (currentLetter >= 'A' && currentLetter <= 'Z') || (currentLetter >= '0' && currentLetter <= '9')) { // je to operand -> zapiseme na konec vystupniho retezce
			postfixExpression[postfixExpressionLength++] = currentLetter;
		}
		else if (currentLetter == '=') { // rovna se -> vyprazdime zasobnik na vystupni retezec (pro zjednoduseni pouzijeme jiz existujici funkci untilLeftPar)
			untilLeftPar(&stack, postfixExpression, &postfixExpressionLength);
			postfixExpression[postfixExpressionLength++] = currentLetter;
		}
		else if (currentLetter == '(') { // levou zavorku dame na zasobnik
			 Stack_Push(&stack, currentLetter);
		}
		else if (currentLetter == ')') { // prava zavorka -> vyprazdneni zasobniku na vystupni retezec az po levou zavorku
			untilLeftPar(&stack, postfixExpression, &postfixExpressionLength);
		}
		else {	// operator -> doOperation
			doOperation(&stack, currentLetter, postfixExpression, &postfixExpressionLength);
		}
	}
	Stack_Dispose(&stack); //zrusime stack
	return postfixExpression;
}


/**
 * Pomocná metoda pro vložení celočíselné hodnoty na zásobník.
 *
 * Použitá implementace zásobníku aktuálně umožňuje vkládání pouze
 * hodnot o velikosti jednoho byte (char). Využijte této metody
 * k rozdělení a postupné vložení celočíselné (čtyřbytové) hodnoty
 * na vrchol poskytnutého zásobníku.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value hodnota k vložení na zásobník
 */
void expr_value_push( Stack *stack, int value ) {
	for (int i = 0; i < 4; i++) { // provedeme 4x (int 4 byte vs char 1 byte)
		Stack_Push(stack, ((value >> i*8) & 0xFF)); // provedeme bitovy posun o 8 (1 byte) a vynasobime s 255, jelikoz chceme jen 8 nejmene vyznamnych bitu a tuto hodnotu pushneme na zasobnik
	}
}

/**
 * Pomocná metoda pro extrakci celočíselné hodnoty ze zásobníku.
 *
 * Využijte této metody k opětovnému načtení a složení celočíselné
 * hodnoty z aktuálního vrcholu poskytnutého zásobníku. Implementujte
 * tedy algoritmus opačný k algoritmu použitému v metodě
 * `expr_value_push`.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné celočíselné hodnoty z vrcholu zásobníku
 */
void expr_value_pop( Stack *stack, int *value ) {
	char x;
	for (int i = 3; i >= 0; i--) { // to same akorat obracene
		Stack_Top(stack, &x);
		Stack_Pop(stack);
		*value |= (x << i*8); // nyni uz jen cteme ze zasobniku 1 bytove hodnoty a slozime z nich 4 bytovy int
	}
}


/**
 * Tato metoda provede vyhodnocení výrazu zadaném v `infixExpression`,
 * kde hodnoty proměnných použitých v daném výrazu jsou definovány
 * v poli `variableValues`.
 *
 * K vyhodnocení vstupního výrazu využijte implementaci zásobníku
 * ze cvičení c202. Dále také využijte pomocných funkcí `expr_value_push`,
 * respektive `expr_value_pop`. Při řešení si můžete definovat libovolné
 * množství vlastních pomocných funkcí.
 *
 * Předpokládejte, že hodnoty budou vždy definovány
 * pro všechy proměnné použité ve vstupním výrazu.
 *
 * @param infixExpression vstpní infixový výraz s proměnnými
 * @param variableValues hodnoty proměnných ze vstupního výrazu
 * @param variableValueCount počet hodnot (unikátních proměnných
 *   ve vstupním výrazu)
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné hodnoty vyhodnocení vstupního výrazu
 *
 * @return výsledek vyhodnocení daného výrazu na základě poskytnutých hodnot proměnných
 */
bool eval( const char *infixExpression, VariableValue variableValues[], int variableValueCount, int *value ) {
	char *postfixExpression = infix2postfix(infixExpression); // prevedem infix to postfix
	const int variableCount = variableValueCount; // pro snazsi implemetaci v kodu
	Stack stack;
	Stack_Init(&stack); 
	for (int i = 0; postfixExpression[i] != '\0'; i++) { // prochazime retezec znak po znaku, operandy pushneme na zasobnik a podle precteneho operatoru s nimi pocitame
		char currentLetter = postfixExpression[i];
		if ((currentLetter >= 'a' && currentLetter <= 'z') || (currentLetter >= 'A' && currentLetter <= 'Z') || (currentLetter >= '0' && currentLetter <= '9')) {
			expr_value_push(&stack, variableValues[variableCount - variableValueCount--].value);
		}
		else if (currentLetter == '=') { // = -> vysledek je na vrcholu zasobniku
			*value = 0;
			expr_value_pop(&stack, value);
		}
		else { // operator
			int first_operand = 0,second_operand = 0;
			expr_value_pop(&stack, &second_operand); // nacteme si operandy se stacku do promennych
			expr_value_pop(&stack, &first_operand); // provedeme potrebnou operaci a vysledek pushneme zpet na zasobnik
			if (currentLetter == '+') {
				expr_value_push(&stack, (first_operand+second_operand));
			}
			else if (currentLetter == '*') {
				expr_value_push(&stack, first_operand*second_operand);
			}
			else if (currentLetter == '-') {
				expr_value_push(&stack, first_operand-second_operand);
			}
			else {
				expr_value_push(&stack, first_operand/second_operand);
			}
		}
	}
	Stack_Dispose(&stack);
	free(postfixExpression);
	return true;
}

/* Konec c204.c */
