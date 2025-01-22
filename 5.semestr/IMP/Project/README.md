# Aplikace ovládaná dvojicí rotačních enkodérů

Cílem tohoto projektu je za použití dvojice rotačních enkodérů s tlačítky (KX/Y-040) a mikrořadičem
ESP32 sestrojit jednoduchou aplikaci. Jako aplikaci jsem si zvolil hru Pong, která se společně například s hrami
Space Invaders či Pac-Man řadí mezi naprosté klasiky.
Enkodéry jsou využity k ovládání odrazových plošin jednotlivých hráčů. Pomocí tlačítek lze hru znovu
spustit. V tomto projektu jsem využil převážně technologie přerušení, real-time operačního systém FreeRTOS
a přístup ke sdílenému zdroji pomocí semaforu.\

Více v [dokumentaci projektu](doc/projekt.pdf)

Výsledné hodnocení: 14/14
