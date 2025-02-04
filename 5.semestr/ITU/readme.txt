Jak zprovoznit projekt:

- Počítá se že jsou dostupní interpreti python3 a node.js

Backend:
Ve složce backend vytvořte nové virutální prostředí: "python -m venv venv"
Poté ho aktivujte: "source venv\Scripts\activate"
Pak nainstalujte knihovny: "pip install -r requirements.txt"
Nakonec ho můžete spustit: "python backend.py"

Frontend:
Frontend očekává že backend je spuštěn na stejném počítači
Uvnitř složky frontend spusťte "npm install" a poté "npm run dev"

Mobilní aplikace:
Ná své telefonní zařízení si stáhněte apliaci "Expo Go"
Uvnitř složky "mobileapp" nainstalujte knihovny "npm install"
Do souboru "backend_address.ts" zadejte IP adresu zařízení kde je spuštěn backend
Poté můžete pustit aplikaci pomocí: "npx expo start"
Objeví se Vám QR kód který můžete načíst pomocí aplikace Expo Go, tím spustíte aplikaci

Struktura projektu:

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