---
layout: post
title:  "Archiv: Modellierung von Tonhöhen"
date:   2014-04-06 18:47
amazon: o=3&p=8&l=as1&m=amazon&f=ifr&asins=0132350882&ref=tf_til
Categories: robust
---
<blockquote>
	What are you armed with?<br />
	<i>(Douglas&nbsp;N.&nbsp;Adams)</i>
</blockquote>

Archiv-Einträge haben sich aus älteren Posts ergeben. Sie erscheinen hier in einem neuen Layout.

### Physikalische Töne

Aus physikalischer Sicht, kann ein Ton als eine Welle mit einer Wellenlänge beschrieben werden. Unterschiedliche Wellenlängen entsprechen unterschiedliche Töne.

Als abstrakte Approximation verwenden wir statt dessen eine Sinus-Funktion. Für die Wellenlänge `l` ergibt sich

``` java
Math.sin(x * l / (2.0 * Math.PI))
```

Die folgende Abbildung zeigt eine Tonschwingung.

![Tonschwingung](/images/welle.png)

Anstatt der Wellenlänge `l`kann auch die Frequenz `f` verwendet werden, denn es gilt:

``` java
l == 1.0 / f
```

Damit ergibt sich als Sinus-Funktion

``` java
Math.sin(x / (f * 2.0 * Math.PI))
```

Damit ergibt sich eine erste Klasse für die Modellierung von physikalischen Tönen:

``` java
public final class PhysikalischerTon {
	private final double _frequenz;

	public PhysikalischerTon(double f) {
		_frequenz = f;
	}
	public double frequenz() {
		return _frequenz;
	}
	public double wellenLaenge() {
		return 1.0 / _frequenz;
	}
}
```

### Logische Töne

Für die Harmonielehre beschränken wir uns auf abzählbar viele Töne. Damit ergibt sich die folgende Klasse für einen Ton:

``` java
public final class Ton {
	private final int _position;

	public Ton(int p) {
		position = p;
	}
	public int position() {
		return _position;
	}
}
```

Zu jedem Ton kann ein physikalischer Ton gefunden werden. Die ersten dreizehn Töne sind:

``` java
frequenz[0] = 440.00;
frequenz[1] = 466.16;
frequenz[2] = 493.88;
frequenz[3] = 523.25;
frequenz[4] = 554.37;
frequenz[5] = 587.33;
frequenz[6] = 622.25;
frequenz[7] = 659.26;
frequenz[8] = 698.46;
frequenz[9] = 739.99;
frequenz[10] = 783.99;
frequenz[11] = 830.61;
frequenz[12] = 880.00;
```

<blockquote>
	Quelle: <a href="http://de.wikipedia.org/wiki/Bild:Glstufstimmung.png">Wikipedia</a>
</blockquote>

Der dreizehnte Ton hat genau die doppelte Frequenz von den ersten Ton. Analog sind alle weiteren Töne Vielfache von den hier abgebildeten Tönen. Jeweils zwölf aufeinander folgende Töne bilden eine *Oktave*. Die Töne der nächsten Oktave haben genau die doppelte Frequenz.

Dies ergibt eine genauere Verteilung der Frequenzen. Sie müssen gleichmäßig so angeordnet werden, dass mit zwölf Tönen eine Verdopplung stattfindet.

Es ergibt sich die folgende Klasse:

``` java
public final class TonUmwandler {
	public static PhysikalischerTon umwandeln(Ton t) {
		return new PhysikalischerTon (
			Math.pow(2, t.position() / 12.0) * 440.00
		);
	}
}
```

Die folgende Abbildung zeigt den Zusammenhang zwischen den Tönen und ihrer Frequenz:

![Tonhöhen](/images/tonhoehe.png)
