# ITU projekt

# Obsah
- [Role](#role)
  - [Administrátoři](#administrátoři)
  - [Uživatelé](#uživatelé)
  - [Pracovníci](#pracovníci)
- [Průzkum existujících řešení](#průzkum-existujících-řešení)
- [Autoři](#autoři)

## Role

### Administrátoři
- vytváření a mazání nových událostí
- správa událostí (nastavuje počet vstupenek, cenu, typ, lokaci)
- spravování recenzí
- navíc může dělat vše co uživatel

### Uživatelé
- vyhledávání událostí
- psaní recenzí
- přihlašování na akce
- přihlašování jako dobrovolník

## Průzkum existujících řešení
| VUT login  | Název aplikace      |                    |
|------------|---------------------|--------------------|
| xvrskaa00  | www.ticketportal.cz | www.fiverr.com     |
| xkonec85   | www.goout.net       | www.gotobrno.cz    |
| xanton07   | www.kdykde.cz	   | www.freelancer.com |
| xmicha94   | www.kudyznudy.cz    | www.prague.eu      |

## Autoři
| Jméno            | Jméno Git účtu  | VUT login  |
|------------------|-----------------|------------|
| Marek Antoňů     | antonu07        | xanton07   |
| Marek Konečný    | MarekKonec      | xkonec85   |
| Kryštof Michálek | Michkr123       | xmicha94   |
| Petr Hron        | catohas         | xhronpe00  |

## Struktura projektu

* backend - zde je uložen backend projektu
* frontend - zde je uložena webová aplikace
    * src - zdrojové soubory webové aplikace
        * components - zde jsou uloženy komponenty
            * CategoryDropZone.tsx - zóna pro přetažení kategorií - Marek Konečný (xkonec85)
            * DraggableButton.tsx - tlačítko, které lze přetahovat - Kryštof Michálek (xmicha94)
            * DraggableCategory.tsx - kategorie, která lze přetáhnout - Marek Konečný (xkonec85)
            * DropZone.tsx - zóna pro přetažení tlačítek - Kryštof Michálek (xmicha94)
            * EditReviewModal.tsx - úprava recenze -  Kryštof Michálek (xmicha94)
            * EventDetails.tsx - detail akce, a přidávání recenzí - Marek Konečný (xkonec85), Kryštof Michálek (xmicha94)
            * EventImage.tsx - zobrazení obrázku akce - Marek Konečný (xkonec85)
            * Header.tsx - hlavička stránky - Marek Konečný (xkonec85)
            * LikeDislikeButtons.tsx - tlačítka pro like a dislike - Kryštof Michálek (xmicha94)
            * NewEventForm.tsx - formulář pro vytvoření nové akce - Marek Konečný (xkonec85)
            * UpdateEventForm.tsx - formulář pro úpravu akce - Marek Konečný (xkonec85)
        * EventsPage.tsx - stránka pro zobrazení akcí - Marek Konečný (xkonec85)
        * main.tsx - soubor pro vstup do aplikace - Marek Konečný (xkonec85), Kryštof Michálek (xmicha94)
        * ProfileChangePage.tsx - profilová stránka s možností změny uživatele - Kryštof Michálek (xmicha94)
        * ProfileEditPage.tsx - profilová stránka s možností úpravy profilu - Kryštof Michálek (xmicha94)
        * ProfilePage.tsx - profilová stránka s kalendářem událostí - Kryštof Michálek (xmicha94)
        * UserContext.tsx - určuje druh uživatele

* mobileapp - zde je uložena mobilní aplikace
    * app - zde jsou uloženy zdrojové soubory mobilní aplikace
        * (events) - obrazovky pracující s akcemi
            * edit - obrazovka pro upravu akce
                * [id].tsx - obrazovka pro upravu akce - Marek Antoňů (xanton07)
            * [id].tsx - obrazovka detailu akce - Marek Antoňů (xanton07)
            * create.tsx - obrazovka pro vytvoření akce - Marek Antoňů (xanton07)
            * review.tsx - obrazovka pro upravu a vytvoření recenze - Petr Hron (xhronpe00)
        * (profile) - obrazovky pracující s profily
            * edit.tsx - obrazovka pro úpravu profilu - Petr Hron (xhronpe00), Marek Antoňů (xanton07)
        * (tabs) - obrazovky přístupné z lišty
            * _layout.tsx - lišta pro výběr obrazovky - Petr Hron (xhronpe00), Marek Antoňů (xanton07)
            * calendar.tsx - obrazovka s kalendářem akcí, na které je uživatel přihlášen - Petr Hron (xhronpe00)
            * index.tsx - obrazovka s přehledem všech akcí - Marek Antoňů (xanton07)
            * profile.tsx - obrazovka s profilem uživatele a přepnutím mezi uživateli - Petr Hron (xhronpe00), Marek Antoňů (xanton07)
        * _layout.tsx - zde je uložen expo router - Petr Hron (xhronpe00), Marek Antoňů (xanton07)
    * context - zde je ulozen uživatelský kontext
        * UserContext.tsx - určuje druh uživatele
    * types -zde jsou uloženy datové typy využívané v aplikaci
        * Event.tsx - datové typy představující akci
        * Profiles.tsx - datový typ představující přihlášeného uživatele
    * backend_address.tsx - zde je možné nastavit kde běží backend