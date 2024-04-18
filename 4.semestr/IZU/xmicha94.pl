% Zadani c. 14:
% Napiste program resici ukol dany predikatem u14(LIN,VIN,VOUT), kde LIN je 
% vstupni ciselny seznam s nejmene jednim prvkem, VIN je vstupni promenna
% obsahujici cislo/prah a VOUT je vystupni promenna vracejici prumernou 
% hodnotu cisel seznamu LIN, jejichz absolutni hodnota je vetsi nez prah.
% Muzete predpokladat, ze absolutni hodnota alespon jednoho cisla seznamu
% je vetsi nez prah.

% Testovaci predikaty:                                  % VOUT
u14_1:- u14([5,27,-1,28,19,-40],10,VOUT),write(VOUT).    % 8.5
u14_2:- u14([2.5,3.6,4.7],3,VOUT),write(VOUT).             % 4.15
u14_3:- u14([8,6,-6,-8],2,VOUT),write(VOUT).        % 0
u14_r:- write('Zadej LIN: '),read(LIN),
        write('Zadej VIN: '),read(VIN),
        u14(LIN,VIN,VOUT),write(VOUT).

% Reseni:
u14(LIN, VIN, VOUT) :-
    u14(LIN, VIN, 0, 0, VOUT).  % 0,0 jsou pocatecni hodnoty pro akumulator

u14([], _, Sum, Count, VOUT) :- % vypocet prumeru, provede se az kdyz je LIN prazdny
    Count > 0,
    VOUT is Sum / Count.

u14([H|Z], VIN, Sum, Count, VOUT) :-    % H - prvni prvek seznamu a T - zbytek
    abs(H) > VIN,                       % prvni prvek je vyssi nez prah -> pridame ho do souctu, ze ktereho budeme pocitat prumer a zvysime pocet techto prvku
    NewSum is Sum + H,
    NewCount is Count + 1,
    u14(Z, VIN, NewSum, NewCount, VOUT).% Rekurzivne vola predikat u14/5 nad zbytkem seznamu

u14([_|Z], VIN, Sum, Count, VOUT) :-    % Toto se provede, kdyz prvni prvek nebude vetsi nez prah, tzn. predchozi pravidlo nebylo splneno -> prvni prvek H se odignoruje
    u14(Z, VIN, Sum, Count, VOUT).      % rekurzi se vola predikat u14/5 nad zbytkem seznamu