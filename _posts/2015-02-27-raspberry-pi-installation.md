---
layout: post
title:  "Raspberry Pi Installation"
date:   2015-02-27 21:00
Categories: raspi
---

### Bootstrap

Am Freitag Abend hatte ich alles zusammen, um meinen Raspberry Pi in Betrieb zu nehmen:

* die Platine kam am Donnerstag,
* die microSD-Karte und das WLAN-Modul schon am Dienstag,
* ein iPhone-Ladegerät mit Adapterkabel hatte ich bereits,
* am Freitag holte ich auf dem Heimweg Tastatur, Maus, Kartenleser und HDMI-Kabel bei Computeruniverse in Friedrichsdorf ab.

Ich hatte bereits das NOOB-Paket auf die Karte kopiert und startete sie im Pi. Als Monitor verwendete ich meinen Beamer.

Anstatt des WLANs war der Pi zunächst direkt mit einem Kabel an die FRITZ!Box angeschlossen.

Soweit, so gut: das System startete, ich konnte Rasbian als Betriebssystem auswählen und aus dem Netz wurde alles auf die 32 GB Speicherkarte installiert.

Nach dem Reboot startete das Konfigurationsprogramm und kurze Zeit später hatte ich eine X11-Oberfläche laufen.

### WLAN einrichten

Unter X11 ging das Einrichten des WLANs problemlos. In den Einstellungen gab es dafür einen eigenen Menüpunkt. Sichtbare Netze wurden nicht angezeigt, aber ich konnte ein neues Netz anlegen und den WPA2 Schlüssel hinterlegen.

Nachdem ich die FRITZ!Box so umkonfiguriert hatte, dass sie neue Geräte akzeptierte, wurde die Verbindung aufgebaut und der Pi sprach über WLAN.

Auch nachdem ich das X beendet hatte und beim Neustart direkt in die Konsole bootete, blieb die WLAN Konfiguration erhalten.

### Unerreichbar

Also zog ich Tastatur, Maus und HMDI Kabel ab, und legte den Pi nur mit einem Stromanschluss in die Ecke.

Er bootete, ging ins Netz, ich kam vom Mac per SSH drauf, aber nicht lange. Nach ein paar Minuten wurde die Verbindung unterbrochen.

Zuerst dachte ich an ein zu schwaches Netzteil. Aber als ich dann doch das HDMI-Kabel und die Tastatur wieder anschloss, war der Rechner immer noch da. Ich konnte mich direkt anmelden, und auf der Konsole www.google.com anpingen. Alles ohne Probleme.

Aber von außen war der Rechner nicht sichtbar. Weder unter seinem Namen, noch unter der IP. Firewall war nicht aktiviert.

Wenn ich aber eine `ssh`-Verbindung vom Pi zum Mac aufmachte, konnte ich auch vom Mac zurück zum Pi eine Verbindung erstellen. Beendete ich die Verbindung vom Pi, brach kurze Zeit später auch wieder dei andere Verbindung zusammen.

### Powermanagement des WLAN-Treibers

Nach einigem Googeln kam ich dann zur Lösung: der WLAN-Treiber hat ein eigenes Powermanagement. Sobald er erkennt, dass das WLAN nicht gebraucht wird, fährt er es runter.

Prüfen kann man es mit dem folgenden Kommando:

````bash
cat /sys/module/8192cu/parameters/rtw_power_mgnt
````

Wenn als Ergebnis eine `1` oder `2` erscheint, dann ist das Powermanagement aktiv. Damit der Pi dauerhaft von außen über WLAN erreichbar ist, muss es deaktiviert werden.

Dafür muss in die Datei `/etc/modprobe.d/8192cu.conf` die folgende Zeile stehen:

````
options 8192cu rtw_power_mgnt=0 rtw_enusbss=0
````

Nach einem Neustart sollte dann der obige `cat`-Befehl eine `0` liefern.

Damit funktionierte der Pi bei mir wie erwartet.

### Mathematica

Wenn der `ssh`-Befehl mit `-X` gestartet wird und lokal auf dem Mac ein X11 Server läuft, können auch grafische Programme auf dem Pi gestartet werden. Die Ausgabe erfolgt dann auf dem Mac.

Die Eingabe von `mathematica` startet das gleichnamige Programm. Es läuft zwar nicht so schnell, wie auf dem Mac, aber ist durchaus verwendbar.

### Fazit

Der erste Abend mit dem Pi war ein voller Erfolg. Nur die Hakeleien mit dem WLAN-Modul haben mir etwas Kopfzerbrechen bereitet. Ansonsten erhält man einen wahnsinnig günstigen und kompakten Linux-Rechner, der Lust auf mehr macht.