---
layout: post
title:  "Zufällige Farben mit Mathematica"
date:   2015-03-01 18:00
Categories: mathematica
---

Dieses Jahr habe ich mir vorgenommen, Mathematica zu erlernen. In diesem Blog gibt es kleine Erkenntnisse.

In diesem ersten Versuch habe ich probiert eine Stelle aus dem Einführungsvideo nachzubauen: ein Foto von der Kamera wird aufgenommen und mit kleinen Kästchen in zufälligen Farben hinterlegt.

### Das Bild aufnehmen

Das Bild aufzunehmen ist einfach:

````mathematica
CurrentImage[]
````

Das Ergebnis ist ein farbiges Foto:

![ursprüngliches Foto](/images/mth_shot.png)

Damit die zufälligen Farben besser zur Geltung kommen, soll das Bild zunächst in Graustufen umgerechnet werden:

````mathematica
ColorConvert[%, "Grayscale"]
````

Das Ergebnis ist entsprechend:

![Foto in Graustufen](/images/mth_shot_grey.png)

Um später das Bild mit zufälligen Farben anzureichen, muss das Ergebnis wieder in den RGB-Farbraum umgewandelt werden:

````mathematica
ColorConvert[%, "RGB"];
````

Durch das Semikolon am Ende wird verhindert, dass das Foto erneut ausgegeben wird (es hat sich ja sichtbar nichts geändert).

### Das Bild zerlegen und einfärben

Mit dem folgenden Befehl kann ein Bild in Teilbilder unterteilt werden. Jedes Bild ist 20 Pixel breit und hoch:

````mathematica
ImagePartition[%, 20]
````

![zerlegtes Bild](/images/mth_shot_split.png)

Um die Teilbilder zufällig einzufärben, werden zufällig drei Farbanteile ermittelt (rot, grün und blau) und die Pixel der Bilder mit den entsprechenden Werten multipliziert:

````mathematica
Map[ImageMultiply[#, RandomReal[{0, 1}, 3]] &, %, {2}]
````

![eingefärbte Teilbilder](/images/mth_shot_color.png)

### Bild zusammensetzen

Das Zusammensetzen kann ebenfalls mit einem einzigen Befehl erreicht werden:

````mathematica
ImageAssemble[%]
````

![zusammengesetztes Bild](/images/mth_shot_combine.png)

Das zusammengesetzte Bild kann mit `Export` gespeichert werden.