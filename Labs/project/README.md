# R-2R digtálno-analógový prevodník


### Zadanie

Popis funkcie R-2R digitálno-analógového prevodníku (DAC). Aplikácia generátoru analógového signálu využívajúci 8bitový DAC. 6 prednastavených typov signálu. Ovládanie pomocou klávesnice 4x3. Zobrazovanie informácií na displeji. Odosielanie informácií o stave aplikácie na UART. 


## Popis hardwaru

Použitý hardware:
Mikrokontrolér ATmega328P
Displej HD44780 2x16 znakov
Klávesnica 4x3
8-bitový R-2R D/A prevodník 
Mikrokontrolér obsahuje program, ktorý bude generovať signál na výstupe externého D/A prevodníka takého tvaru, aký si používateľ zvolí podľa čísla na klávesnici. 
Displej zobrazuje informáciu o tom, aký signál je generovaný mikrokontrolérom na výstupe externého D/A prevodníka.
Klávesnica funguje ako maticová, na výstupe mikrokontroléra sa generuje testovací signál privádzaný na klávesnicu a na vstupe do mikrokontroléra z klávesnice sa skenuje prichádzajúca postupnosť bitov. Vysoká úroveň indikuje stlačené tlačidlo. Toto sa opakuje v nekonečnej sľučke.
R-2R D/A prevodník prevádza 8-bitovú postupnosť 2 hodnôt napätia 0V a 5V na 256 hodnôt napätia na jeho výstupe v rozmedzí 0-5V pomocou odporovej siete R-2R.


## Popis zdrojového kódu a simulácie

Link na adresár [projektu](https://github.com/NechTaSilaSprevadza/Digital-electronics-2/tree/master/Labs/project) na GitHube: https://github.com/NechTaSilaSprevadza/Digital-electronics-2/tree/master/Labs/project

### Schéma zapojenia
![Schéma zapojenia](Images/SchemaZapojenia.png)

### Vývojový diagram
![Vývojový diagram](Images/VyvojovyDiagram.png)

### Popis zdrojového kódu

Na začiatku [programu](GccApplication1/GccApplication1/main.c) sú definície pinov pre obsluhu klávesnice a obsluhu výstupu na D/A prevodník. 
Ďalej je definovaná pracovná frekvencia mikrokontroléra pre správnu funkčnosť UART-u. 
Ďalej nasleduje import [knižníc](Libraries) využitých v programe. Sú to knižnice na definíciu vstupných a výstupných portov, knižnica časovača pre obsluhu prerušení, knižnice na obsluhu LCD displeja a knižnice na obsluhu UART-u. 
Ďalej sú definované globálne premenné použité v programe pri prerušeniach. 
Nasleduje look-up tabuľka, kde sú definované hodnoty na generovanie sínusového signálu. 

Nasleduje hlavný program. 
V ňom sú nakonfigurované piny na obsluhu klávesnice. 
Piny C0, C1 a C2 sú vstupné piny a reagujú na vysokú úroveň vstupného signálu. Piny C3, C4, C5 a C6 sú výstupné piny generujúce signál, ktorý je snímaný pinmi C0:C2 a z toho sa vypočítava pozícia zatlačeného tlačidla na klávesnici. 
Ďalej sú nakonfigurované výstupné porty B0:B7, na ktoré sa privádza generovaný signál. 
V nasledujúcom kroku sa inicializuje displej a v prvom riadku sa vypíše “Output Signal“. 
Čas pretečenia 8-bitového časovača 0 je nastavený na 128us a čas pretečenia 16-bitového časovača 1 je nastavený na 262ms. 
Ďalej je inicializovaný na UART na 8 bitové slovo, bez paritného bitu, symbolová rýchlosť je 9600 symbolov za sekundu. 
Nasleduje globálne povolenie prerušení. 
V nekonečnej sľučke sa nevykonáva žiaden program, všetky ostatné operácie sa vykonávajú počas obsluhy prerušení. 

V časovači 0 je definovaných 6 typov generovaných signálov. 
To, ktorý sa bude generovať, záleží na stlačenom tlačidle na klávesnici. 
Po stlačení 1 sa generuje sínusovka, po stlačení 2 sa generuje obdĺžnik, po stlačení 3 sa generuje rastúca píla, po stlačení 4 sa generuje klesajúca píla, po stlačení 5 sa generuje trojuholník, po stlačení 6 sa generujú Diracove impulzy. 
Po stlačení 7,8,9,*,0,# sa negeneruje nič. 
Program vie tieto signály generovať pri frekvencii 30 Hz. 

V časovači 1 program na 4 výstupných pinoch klávesnice generuje vysokú úroveň postupne na každom z týchto pinov a pri zatlačení tlačidla sa tento signál objaví na jednom zo vstupných pinov. 
Podľa toho aký signál sa zosníma pri meniacom sa nastavení výstupných pinov, nastaví sa hodnota v pamäti indikujúca stlačené tlačidlo. 
Podľa zatlačeného tlačidla sa na displej vypíše zvolený generovaný signál a cez UART sa odošle informácia o generovanom signále. 
Hodnota klávesnice sa sníma každých 262ms, rovnako sa každých 262ms odošle informácia o generovanom signále na UART a LCD displej.


## Video

Video - [stiahnuť](Video/Video.mp4)

Video - [pozrieť](https://drive.google.com/file/d/1nHFWSgAs_humQwwQS_2yR3hbuxNF-2Kv/view?usp=sharing)


## Referencie

1. [Príncíp fungovania R-2R D/A prevodníka](https://www.electronics-tutorials.ws/combination/r-2r-dac.html)
2. [Look-up tabuľka funkcie sínus](https://gist.github.com/funkfinger/965900)
3. [Aplikácia na tvorbu vývojového diagramu](https://app.diagrams.net/)
