# LP-Compiladors
Pràctica de Compiladors de l'assignatura Llenguatges de Programació (LP) de la Facultat d'Informàtica de Barcelona (FIB)

## Instal·lació prèvia

Necessitareu els compiladors `gcc` de C i `g++` de C++

## Compilació i execució

La carpeta _pccts_ inclou les llibreries de compilació i generació dels arxius necessaris per l'execució en _Linux_.

S'inclou també el script **`antlrc`** per compilar el fitxer `.g` si està escrit en C i el script **`antlrcpp`** per C++
Assegureu-vos de donar permissos d'execució en aquest fitxer: __`chmod +x antlrc`__

El fitxer principal de la construcció del llenguatge és **`plumber.g`**

```
./antlrcpp plumber
./plumber
```

Més informació sobre compilació, execució i funcionament al document **`intro-pccts.pdf`**

També es pot executar amb `Java` per qualsevol sistema operatiu com s'explica al document **`ANTLR3_with_Java.pdf`**

## Exemple

Pots trobar exemples del llenguatge creat en aquesta pràctica als fitxers **`test.txt`** i **`inp.txt`**
