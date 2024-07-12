# Implementační dokumentace k 1. úloze do IPP 2023/2024

**Jméno a příjmení:** Kryštof Michálek  
**Login:** xmicha94

První část projektu jsem rozdělil do 3 hlavních funkcí: **scanner**, **parser** a **generátor kódu**.

## Scanner

Hledání hlavičky mi přijde standardním a nijak zvláštním způsobem. Ostatní řádky se vždy zbaví komentářů a prázdných znaků na začátku a konci řádků. Poté se kontrolují jednotlivá slova se seznamem instrukcí a určuje se, zdali je to instrukce/var/symb/label/type. Zde mám zvláštně vyřešené vytváření tokenů, kde použití dictionary neumožňuje duplicitní záznamy s jinými hodnotami a dvě pole (jedno s `token_name` a druhé s `token_type`) mi přišla zbytečně složitá. Spojil jsem tedy vždy název tokenu a jeho typ znakem `'` a poté ho opět pomocí něj rozdělil.

## Parser

Zde probíhá syntaktická analýza. Již je v kódu stručně popsáno. Když je očekávaný argument prázdný, čeká se, že přijde instrukce, jinak nastává chyba. Tato instrukce se vyhledá v dictionary a potřebné argumenty k této instrukci se ukládají do proměnné `expected_argument`. Přijde-li jiný token neodpovídající očekávanému argumentu, dochází k chybě. Součástí parseru je i typová kontrola implementovaná jako funkce níže.

## Generování kódu

Zde jsem si pomocí dostupných knihoven pro generování XML kódu značně usnadnil práci. Zde asi není co neobvyklého k popsání.
