---
layout: post
title:  "Logos mit Mathematica erstellen"
date:   2015-03-08 11:00
Categories: mathematica
---

Ich habe schon seit längerem vor, meine Webseite zu überarbeiten. Ein Teil davon soll ein neues Logo sein, dass die Schwerpunkte Informatik, Mathematik und Musik miteinander vereint. Ich habe mir überlegt, dass ich dazu die Schnecke eines Kontrabasses stilisieren möchte. Ich probiere seit ein paar Jahren, Walking Bass zu erlernen. Aber wie so oft: Es gibt so vieles, dass wichtiger ist und meine Zeit in Beschlag nimmt.

Um die Mathematik ins Spiel zu bringen, soll die Schnecke eine logarithmische Spirale sein. Hier kommt Mathematica ins Spiel.

## Die Schnecke zeichnen

Die logarithmische Spirale hat im Allgemeinen folgende Formel:

![Formel der logarithmischen Spirale](/images/mth_curl_formular.png)

Die Parameter `a` und `b` müssen sinnvoll gewählt werden. Am leichtesten fällt `b`: damit das erhöhen von `t` um 1 jeweils einer vollen Drehung entspricht, sollte für `b` 2 π gewählt werden. Für `a` hat sich experimentell ein Wert von 1/2 als sinnvoll erwiesen:

````mathematica
a = 1/2; b = 2 Pi;
curl[t_] := {Exp[a t] Cos[b t], Exp[a t] Sin[b t]}
````

Damit lässt sich die Spirale schon einmal zeichnen. Zunächst müssen wir noch Start- und Endwerte von `t` festlegen:

````mathematica
minCurl = -5; maxCurl = 5/16;
ParametricPlot[curl[t], {t, minCurl, maxCurl}]
````

![Graph der Standard-Spirale](/images/mth_curl_norm.png)

Da ich für das Logo die Spirale gespiegelt benötige, habe ich den y-Wert negiert:

````mathematica
curl[t_] := {Exp[a t] Cos[b t], -Exp[a t] Sin[b t]}
````

![Graph der invertierten Standard-Spirale](/images/mth_curl_inv.png)

### Mehrere Teile zusammensetzen

Zunächst gibt es für meinen Bass-Kopf drei Teile: die Spirale und zwei kurze gerade Stücke. Die Verbindnug zwischen den Stücken, werde ich danach über BSplines ziehen.

Um in einem einheitlichen Wertebereich zu arbeiten, gibt es erstmal eine Funktion, welche die Werte 0 und 1 auf `minCurl` und `maxCurl` abbildet:

````mathematica
curlRange[x_] := (maxCurl - minCurl) * x + minCurl
````

Damit ändert sich der Plot-Befehl zu

````mathematica
ParametricPlot[curl[curlRange[x]], {x, 0, 1}]
````

Aber an der resultierenden Grafik ändert sich nichts.

Die beiden Balken haben eine Start-Koordinate und eine Breite. Die X-Koordinaten sind jeweils die selben. Nur in der Y-Koordinate unterscheiden sie sich natürlich.

````mathematica
startX = -3; width = 1;
upperY = 1/3; lowerY = -1/2;
````

Damit können alle drei Linien in einem Diagramm ausgegeben werden:

````mathematica
ParametricPlot[{
  curl[curlRange[x]],
  {x * width + startX, upperY},
  {x * width + startX, lowerY}
}, {x, 0, 1}]
````

![Einzelne Segmente](/images/mth_curl_parts.png)

### Tangenten an der Spirale

Um die BSplines schön anzusetzen, müssen wir zu einem Punkt der Spirale seine Tangente berechnen. In erster Näherung ist das einfach ein Vektor, der senkrecht auf dem Verbindungsvektor vom Ursprung steht. Den kann man durch Vertauschen den Komponenten leicht berechnen:

````mathematica
tangent[t_, o_] :=  Module[
  {p = curl[t], q}, 
  q = {p[[2]], -p[[1]]}; p + o q
]
````

Nennen wir den Parameter für den Punkt, an dem die obere Linie auf die Schnecke triffet `cutCurl`, dann ergibt sich:

````mathematica
cutCurl = -5/8;
ParametricPlot[{curl[curlRange[x]], tangent[cutCurl, x]}, {x, 0, 1}]
````

![Tangente an Spirale](/images/mth_curl_tangent.png)

Das sieht doch gut aus.

### Finalisieren

Damit lassen sich jetzt einfach die Splines zwischen den Teilstücken angeben:

````mathematica
ParametricPlot[{
    curl[curlRange[x]],
    {x * width + startX, upperY},
    {x * width + startX, lowerY}
  }, {x, 0, 1},
  Epilog -> {
    BSplineCurve[{
      {startX + width, upperY},
      {startX + width + 1/2, upperY},
      tangent[cutCurl, 3/2],
      curl[cutCurl]
    }],
    BSplineCurve[{
      {startX + width, lowerY},
      {startX + width + 1, lowerY},
      tangent[maxCurl, 1/2],
      curl[maxCurl]
    }]
  }
]
````

![Splines hinzugefügt](/images/mth_curl_combined.png)

Nun müssen nur noch alle Linien in Schwarz und mit der gleichen Strichdicke gezeichnet werden und das Koordinatensystem ausgeblendet werden:

````mathematica
ParametricPlot[{
    curl[curlRange[x]],
    {x * width + startX, upperY},
    {x * width + startX, lowerY}
  }, {x, 0, 1},
  Axes -> False,
  PlotStyle -> {AbsoluteThickness[1]},
  ColorFunction -> (Black &),
  Epilog -> {
    BSplineCurve[{
      {startX + width, upperY},
      {startX + width + 1/2, upperY},
      tangent[cutCurl, 3/2],
      curl[cutCurl]
    }],
    BSplineCurve[{
      {startX + width, lowerY},
      {startX + width + 1, lowerY},
      tangent[maxCurl, 1/2],
      curl[maxCurl]
    }]
  }
]
````

![endgültige Spirale](/images/mth_curl_final.png)

Mit Sketch habe ich dann noch Text hinzugefügt, um bei folgendem Stand zu landen:

![endgültige Spirale](/images/mth_curl_icon.png)

Das Freistellen des Textes war auch ein ziemlicher Aufwand. Aber das werde ich in einem eigenen Block-Eintrag berichten.