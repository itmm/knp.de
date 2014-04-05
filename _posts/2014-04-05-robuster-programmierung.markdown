---
layout: post
title:  "Einstieg in die Robuste Programmierung"
date:   2014-04-05 18:27
amazon: o=3&p=8&l=as1&m=amazon&f=ifr&asins=0132350882&ref=tf_til
Categories: robust
---

Die Art, wie wir Software entwickeln, wirkt noch so unvollkommen. Trotz der ganzen Meetings, Prozesse und Weiterentwicklungen der letzten Jahre, werden unsere Programme nicht stabiler. Es wird nicht ein höherer Anteil der Projekt in time & budget fertig. Das Gegenteil scheint der Fall zu sein.

In einer Reihe von Blog-Einträgen zur Robusten Programmierung möchte ich Inspirationen geben, wie wir als Programmierer daran etwas ändern können. Ich habe bisher leider kein Werzeug gefunden, das uns diese Arbeit abnimmt. Aber ich beschreibe, wie ich für mich den Aufwand reduzieren konnte.

### Warum Objective-C?

Von außen vorgegeben ist die Programmiersprache. Da ich hauptsächlich Apps für iOS und Mac OS X entwickel, gibt es keine ernst zu nehmende Alternative. Und die Sprache hat viele Vorteile.

Zwei Merkmale möchte ich an dieser Stelle besonders hervorheben: die Sprache ist kompatibel mit C und hat eine interessante Interpretation von `nil`-Objekten.

#### Die Nähe zu C

Mit der Nähe zu C sind zwei Vorteile verbunden. Zum einen können Zeiger und Zahlen als boolesche Ausdrücke verwendet werden. Wenn ich in Java nur dann eine Funktion `f` auf einem Attribut `a` aufrufen will, wenn dieses nicht `null` ist, dann benötige ich folgenden Code in Java:

``` java
if (a != null) a.f();
```

In Objective-C kann `a` direkt als Wahrheitswert interpretiert werden: Wenn `a` `nil` ist, dann ist es falsch. Andernfalls ist es wahr. Der obige Code reduziert sich in Objective C zu

``` objc
if (a) [a f];
```

Dadurch wird der Code kompakter. Je weniger Zeichen getippt werden müssen, desto weniger Zeichen können Fehler enthalten.

Zudem ist die Sprache durch den Präprozessor leichter erweiterbar, als C. Dies werden wir brauchen, um uns die Sprache zurechtzubiegen, so dass robuster Code leichter zu schreiben ist. Es gibt Sprachen wie Lisp, die durch ihr Makro-Konzept deutlich leichter zu erweitern sind. Sprachen wie C# oder Java sind aber deutlich unflexibler.

#### `nil`-Objekte

Objective-C hat eine sehr robuste Interpretation des `nil`-Objektes. Auf ihm kann jede Methode aufgerufen werden. Es passiert nur nichts. Für die Robustheit ist das super.

Wir hatten oben den Test in Java, der notwendig ist, wenn ich nicht sicher sein kann, dass `a` initialisiert ist. In Objective-C ergibt sich als äquivalenter Code eigentlich

``` objc
[a f];
```

Dies entspricht vollständig dem Java-Code

``` java
if (a != null) a.f();
```

Leider ist Objective-C nicht an jeder Stelle so tolerant. Wenn auf ein Array-Element zugegriffen wird, das es gar nicht gibt, so wird eine Exception geworfen. Ebenso wenig kann mit der `for` Schleife über `nil` iteriert werden. Dies zwingt uns, sicherzustellen, dass diese Szenarien nicht auftreten können.

### Konsistenz

Die oberste Maxime von robuster Programmierung lautet meiner Meinung: verletze niemals die interne Konsistenz eines Objekts. Wenn das Objekt initialisiert wurde, dann ist es konsistent. Kein noch so unsinniger Aufruf von Methoden darf diese Konsistenz verletzen.

Nehmen wir ein einfaches Beispiel. Angenommen, ein Objekt hält zwei Zahlen: eine kleinere und eine größere:

``` objc
@interface Pair : NSObject

	@property (nonatomic) NSInteger smaller;
	@property (nonatomic) NSInteger bigger;

@end
```

Auch mit einer leeren Implementierung würde diese Klasse funktionieren. Sie ist aber nicht robust. Die Klass ist genau dann konsistent, wenn

``` objc
self.smaller < self.bigger
```

Und die Initialisierung muss einen solchen Zustand sicherstellen. Macht sie aber nicht. Der Standard-Konstruktor initialisiert beide Werte mit `0`.

Welche Werte machen denn Sinn? Mir fallen als erstes `0`, `1` ein. Aber vielleicht auch `-1`, `1` da es symmetrischer ist? Schieben wir die Frage nach hinten, bis die Setter geklärt sind.

Diese dürfen natürlich einen neuen Wert nur zulassen, wenn er die Konsistenz nicht verletzt:

``` objc
- (void) setSmaller: (NSInteger) value {
	if (value < _bigger) {
		_smaller = value;
	} else NSLog(@"%@ >= %@", value, _bigger);
}

- (void) setBigger: (NSInteger) value {
	if (value > _smaller) {
		_bigger = value;
	} else NSLog(@"%@ <= %@", value, _smaller);
}
```

Wenn die Setter mit einem falschen Wert aufgerufen werden, dann wird der Zustand nicht verändert und statt dessen eine Warnung ausgegeben. In späteren Blogs werde ich zeigen, wie ich durch Makros die Schreibweise reduzieren kann.

Damit zeigt sich aber, dass die Initialisierung mit kleinen Werten keinen Sinn macht. Dadurch werden die nachfolgenden Set-Operationen nur unnötig kompliziert. Die Initialisierung sollte so weit wie möglich gefasst werden:

``` objc
- (id) init {
	self = [super init];
	if (self) {
		_smaller = INT_MIN;
		_bigger = INT_MAX;
	}
	return self;
}
```

### Wann darf eine Methode eine Exception generieren?

In Objective-C ist die Frage leicht zu beantworten: Nie! Robuster Code sollte niemals eine Exception werfen. Wenn die Exception nicht abgefangen wird, dann wird das Programm abgebrochen. Das ist sicher nicht, was der Benutzer erwartet.

Andere Sprachen gehen mit Exceptions anders um. In eine an Java angelehnten Sprache, könnte eine Exception generiert werden, wann versucht wird, über das Ende einer Datei hinaus zu lesen:

``` java
int sum = 0;
try{
	for (;;) {
		sum = sum * 7 + in.read();
	}
}
catch (FileNotFoundException x) {}
finally { in.close(); }
```

Bei langen Dateien kann das von Vorteil sein. Es muss nicht für jedes Zeichen geprüft werden, ob das Ende erreicht ist. Das kann auch durch das Lesen von Blöcken gemildert werden, aber Exceptions finde ich hier einen gangbaren Weg.

Aber nicht bei uns. Der Aufruf eine Methode darf keine Exception generieren, wenn nicht sichergestellt werden kann, dass diese gefangen wird.

#### Exceptions von anderen Methoden

Wenn in der Methode eine andere Methode aufgerufen wird, die eine Exception generieren kann, dann muss diese gefangen werden. Wir können die Verantwortung nicht an die aufrufende Methode abgeben. Diese weiß gar nicht, dass eine Exception generiert werden kann.

#### Exceptions im Konstruktor

In C++ und Java werden oft Exceptions generiert, wenn der Konstruktor kein initialisiertes Objekt erzeugen kann. Etwa weil der Speicher fehlt, oder ein Argument keinen Sinn macht.

In Objective-C ist diese Ausrede nicht notwendig. Wenn das Objekt nicht initialisiert werden kann, dann kann `nil` anstatt des Objekts zurück gegeben werden:

``` objc
- (id) initWithPath: (NSString *) path {
	if (!path) { NSLog(@"path is nil"); return nil;}
	self = [super init];
	if (self) {
		_path = path;
	}
	return self;
}
```

Auch hier wird eine Meldung ausgegeben, wenn es nicht geklappt hat.

### Was ist mit `NSAssert`?

Ich verwende `NSAssert` nicht mehr. Wenn es aktiv ist, dann wirft es eine Exception. Das möchte ich ja gerade verhindern. Wenn es nicht aktiv ist, dann wird auch nichts geloggt. Das ist mir zu wenig.

Außerdem weiß der Code nicht, ob `NSAssert` gerade wegkonfiguriert wurde.

### Zusammenfassung

Folgende Punkte sind mir in diesem Artikel wichtig:

* Objekte müssen immer konsistent sein.
* Wenn eine Operation zum Brechen der Konsistenz führt, so wird sie nicht ausgeführt und eine Meldung ausgegeben.
* Eine Methode darf keine Exception nach außen geben, da dies zu einem Absturz führen kann.

Folgende Punkte werde ich in späteren Blogs behandeln:

* `NSLog` ist der Entwicklung eine zu schwache Ausgabe, da sie oft übersehen wird.
* Die drei Schritte prüfen, loggen und return sind noch recht aufwändig zu tippen. Mit Makros können wir versuchen, sie etwas kompakter zu formulieren.

Aber das muss alles bis zu einem anderen Artikel warten.
