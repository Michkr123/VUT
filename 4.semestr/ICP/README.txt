2D simulace autonomních robotů (Roomba 1.0)

Authors:
    Blažek Michal <xblaze38>
    Michálek Kryštof <xmicha94>

1. Představení

    Tento projekt, jak již název napovídá, je simulací autonomních robotů.

    Skládá se ze scény, ve které se nachází všechny objekty (roboti, překážky a smetí).

    Má pevně danou velikost 990x690 pixelů, celé okno má velikost 1600x900.

    V okně jsou tlačítka na ovládání simulace, přidávání, nastavení a ovládání robotů, překážek a smetí. (prodrobnosti v sekci 2.)

    Roboti jezdí po dané ploše určitou rychlostí a detekují překážky před sebou, po detekci se otočí o nastavený úhel (jednou či vícekrát), dokud před sebou nemají volnou cestu, poté opět pokračují rovně.
    
    Překážky jsou pevné objekty, které se během simulace nepohybují.

    Roboty i překážky lze při nezapnuté simulaci přesouvat a transformovat (otáčet, překážky lze i zvětšovat či zmenšovat)

    Smetí jsou náhodně generované kusy odpadků v rámci scény, které roboti při kolizi "vysají".

    Generují se každou vteřinu ve scéně tak, aby nekolidovaly s překážkami.

2. Překlad, spuštění programu a generování dokumentace

    Po stažení a rozbalení tohoto projektu použijte Makefile, který má tyto funkce:

        make            - přeložení projektů
        make compile    - přeložení projektů
        make run        - spuštění projektu
        make all        - přeložení a spuštění projektu
        make clean      - smazání binárních souborů vytvořené příkazem make (make compile)
        make doxygen    - vytvoření doxygen dokumentace

3. Popis grafického uživatelského rozhraní

    3.1 Operace se scénou a simulace

        Scéna:
            
            Před spuštěním:
                
                Load        - načte uložené informace o objekech ze souborů do scény
                Save        - uloží informace o objektech ze scény do souboru
                Clear scéně - vyčistí scénu
                End         - ukončí program

            Po spuštění:

                Load    - načte uložené informace o objekech ze souboru do scény
                End     - ukončí program

        Simulace:

            Před spuštěním:

                Start   - spustí simulaci

            Po spuštění:

                Pause       - pozastaví běh simulace
                Continue    - odpozastaví běh simulace
                Reset       - načte scénu do stavu před spuštěním

    3.2 Operace s roboty

        Zde máme dva typy robotů:

            Hráčský robot
                
                Tento robot může být pouze jeden a je ovládán hráčem.

                Před spuštěním:

                    Spawn player robot  - přidá do scény hráčského robota.

                Po spuštění:

                    Forward - robot se rozjede vpřed
                    Left    - otočení doleva
                    Right   - otočení doprava
                    stop    - zastavení pohybu robota

                Zastaví se i automaticky při detekci překážky.

            Autonomní robot

                Těchto robotů může být (teoreticky) nekonečně mnoho a fungují autonomně.

                Před spuštěním:

                    Spawn robot         - přidá do scény robota
                    Detection distance  - změní vzdálenost detekce překážky daného robota
                    Rotation angle      - změní velikost úhlu otáčení daného robota
                    L                   - otáčení doleva
                    R                   - otáčení doprava
                    Savé settings       - změní nastavení jednotlivých robotů (provede se také automaticky při spuštění simulace)
                
                Po spuštění:

                    Detection distance  - změní vzdálenost detekce překážky daného robota
                    Rotation angle      - změní velikost úhlu otáčení daného robota
                    L                   - otáčení doleva
                    R                   - otáčení doprava
                    Save settings       - změní nastavení jednotlivých robotů (i za běhu programu)

        Oba dva typy robotů lze přesouvat ve scéně při neběžící simulaci pomocí podržení levého tlačítka myši.

        S oběma typy robotů lze provádět následující transforamce po kliknutí pravého tlačítka myši:

        Rotate left - rotace robota doleva o 45°
        Rotate righ - rotace robota doprava o 45°

                
    3.3 Operace s překážkami

        S překážkami jde manipulovat pouze před spuštěním simulace.

        Spawn obstacle  - přidá překážku do scény

        Jsou zde také dva slidery pro změnu velikosti překážky.

        Překážky lze přesouvat ve scéně při neběžící simulaci pomocí podržení levého tlačítka myši.

        S překážkami lze provádět následující transforamce po kliknutí pravého tlačítka myši:

        Rotate left - rotace robota doleva o 22,5°
        Rotate righ - rotace robota doprava o 22,5°
        Scale up    - zvětšení objektu na 110 % původní velikosti
        Scale down  - zmenšení objektu na 90 % původní velikosti


    3.4 Operace se smětím

        Generování smětí funguje při spuštěné simulaci stejně, jako bez spuštění simulace.

        Generate rubbish        - spustí generování smětí (generuje se každou vteřinu)
        Stop generating rubbish - zastaví generováni smětí

4. Chyby

    Zde jsou sepsány implementační chyby, které se v našem programu nachází, víme o nich a které jsme neopravili:

        špatná detekce selekce robota a překážek (dále "objekty")

            Tento problém nastane, když kliknete pravým tlačítkem na libovolný objekt a poté chcete provést interakci s jiným objektem.

            Při pokusu o přesunu pomocí levého tlačíka se první objekt teleportuje na zvolený druhý objekt a je přesouváno s prvním objektem.

            Při otevření kontextového menu pomocí pravého tlačítka myši na druhém objektu se otevře kontextové menu prvního objektu.

            Řešení:

                Před interakcí s objektem (jak posouvání nebo transforamce) je potřeba na něj kliknout levým tlačítkem pro jeho selekci.
                
                Při otevřeném kontextovém menu kliknutí levého tlačítka provede zavření menu bez selekce objektu, je tedy kliknout jednou pro zavření menu a podruhé pro selekci objektu.

        Další zavážné chyby nebyly objeveny.






