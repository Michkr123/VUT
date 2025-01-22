# Aplikace ovládaná dvojicí rotačních enkodérů

Cílem tohoto projektu je za použití dvojice rotaˇcních enkodér  ̊u s tlaˇcítky (KX/Y-040) a mikroˇradiˇcem
ESP32 sestrojit jednoduchou aplikaci. Jako aplikaci jsem si zvolil hru Pong, která se spoleˇcnˇe napˇríklad s hrami
Space Invaders ˇci Pac-Man ˇradí mezi naprosté klasiky.
Enkodéry jsou využity k ovládání odrazových plošin jednotlivých hráˇc  ̊u. Pomocí tlaˇcítek lze hru znovu
spustit. V tomto projektu jsem využil pˇrevážnˇe technologie pˇrerušení, real-time operaˇcního systém FreeRTOS
a pˇrístup ke sdílenému zdroji pomocí semaforu.
