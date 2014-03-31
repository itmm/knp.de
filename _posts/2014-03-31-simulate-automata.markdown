---
layout: post
title:  "Archiv: Zelluläre Automaten simulieren"
date:   2014-03-31 21:45:00
Categories: archive, cellular
---

Archiv-Einträge haben sich aus älteren Posts ergeben. Sie erscheinen hier in einem neuen Layout.

Das folgende Programm in C++ simuliert einen endlichen Automaten.

{% inlineh /assets/cellular.c++ cpp %}

### Konfiguration

Über Kommandozeilen Optionen kann das Verhalten und die Ausgabe des Programms verändert werden.

#### Verhalten

Der Parameter `-n` oder `--nr` gibt an, welcher Automat simuliert werden soll. Die Ausgaben aller Automaten sind in alle einfachen Automaten abgebildet.

Die Nummer ergibt sich aus den Ausgaben des Automaten für alle möglichen Eingaben, etwa:

```
000 → 0 ⇒ 0 * 1
001 → 1 ⇒ 1 * 2
010 → 0 ⇒ 0 * 4
011 → 0 ⇒ 0 * 8
100 → 1 ⇒ 1 * 16
101 → 0 ⇒ 0 * 32
110 → 0 ⇒ 0 * 64
111 → 0 ⇒ 0 * 128
```

Die Summe der rechten Spalte ergibt `2 + 16 == 18` als Nummer für den Automaten.

Das Eingabemuster der ersten Zeile ist ein einzelnes gesetztes Pixel in der Spalte `0`.

#### Ausgabe

Die Anzahl der ausgegebenen Spalten wird über den Parameter `-r` oder `--rows` angegeben. Wenn dieser Parameter nicht angegeben wird, dann werden `30` Zeilen ausgegeben.

Die erste auszugebende Spalte kann mit `-b` oder `--begin` angegeben werden. Die erste Spalte, die nicht mehr ausgegeben wird, kann mit `-e` oder `--end` angegeben werden. Die Semantik ist analog zu Containern in der C++ STL.

Insgesamt werden

``` cpp
--end - --begin
```

viele Spalten ausgegeben. Wenn die Parameter nicht angegeben werden, dann wird `-39` für `--begin` und `40` für `--end` verwendet.

Für jedes gesetzte Bit wird eine `1` und für jedes gelöschte Bit eine `0` ausgegeben. Jede Zeile wird mit einem Zeilenvorschub terminiert.

Folgender Aufruf gibt zum Beispiel die mittlere Spalte als Binärzahl zurück (der `tr`-Aufruf entfernt die Zeilenumbrüche):

``` bash
cellular --nr=30 --rows=400 --begin=0 --end=1 | tr -d "\\n"
```

Liefert das Ergebnis

```
110111001100010110010011101011100111010101100001100101011010
101111110000111100010101110000010010110001110001101101101000
000010001111101110100111000111010111000001100100011001111001
111110000001111111011001011011100000101100011011000110001110
110110010101111111011010110110111101110010111011000100000000
001101110010110010111100100110000111110000001011011001111001
00001001111100000110100101100100101110101
```
