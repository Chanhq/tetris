# simple-tetris-game-in-c
This is a education project to learn how to code in C.


# Important links

- [SDL-Alternative](https://www.sfml-dev.org/tutorials/2.5/compile-with-cmake.php) (SDL is the graphics library)
- [Old Tetris C Code with too old libraries](http://csourcecodes.blogspot.com/2016/06/tetris-game-c-program-block-game.html)


# Planung / Zielsetzung: Vollständiges Tetris-Spiel

## Notwendige Funktionen, damit das Program funktioniert
- Speicherung der Blockmöglichkeiten
- Speicherung der temporen Blockverteilung im Spielfeld durch Listen
 Tastaturbefehlmanagement
- Checken der Kollision und Überführung des bewegenden Blocks in einen festen
- Randomisierte Blöcke
- Bewegung der Blöcke herunter
- Vollständige Zeilen verschwinden automatisch
- Drehen der Blöcke

## Wichtige Extra-Funktionen
- Erhöhung der Schwierigkeit, desto höher der Score
	- Spiel wird schneller
- Scoremanagement
	- Für jede vollständige Zeile wird der Score größer
- GUI im Spiel
	- Label
		- Aktueller Score (und Highscore)
		- Anzeige des nächsten Blocks
		- Eventuell Anzeige von temporären Spiel-Statistiken
- Eingangsmenü
	- Titellogo
	- Knöpfe
		- Spiel starten
		- Statistiken sehen
		- Beenden
- Pausenmenü
	- Pausenlogo
	- Knöpfe
		- Fortsetzen
		- Neustarten
		- Hauptmenü


## Optionale Dekorationen
- Highscoremanagement (Speichern des Highscore, auch wenn das Programm beendet ist)
- Auf ATtiny portieren
- Bessere Texturen / Farben
- Coole Animationen
- Hintergrundmusik



# Zusammengefasst: 8 Leistungsmerkmale
- Blockmanagement
	- Randomisierung der Blöcke
	- Herabfallen
	- Drehung
	- Blockkollisionen
- Zeitmanagement
	- Sofortige Effekte der Tastatureingaben
	- Blockbewegung in Zeitabständen
- Spielfeldmanagement
	- Heruntergefallene Blöcke
	- Vollständige Zeilen verschwinden
- Tastatureingaben
	- Richtigen Events werden aufgerufen (Drehen, schneller herabbewegen)
- Score- & Schwierigkeitmanagement
	- Schnelligkeit des Spiels ~ Score
	- Score wird höher, je mehr Zeilen verschwinden
	- Highscorespeicherung
- GUI
	- Im Spiel (Statistiken, nächster Block)
	- Außerhalb des Spiels (Spiel starten, beenden etc., optional)

# Notwendiges Design / Funktionen

## Programmstruktur/Aufbau des Programms

>
>

## Wichtigste Funktionen und ihre Signatur (Parameter und Rückgabewert)

Ingame-Management
- Rotation der Blöcke
- Randomisierung / Erstellen neuer Blöcke inkl. Farbe
- Checken der Blockkollision
- Bewegen der Blöcke
- Checken, ob Zeilen vollständig sind & Score vergeben
- Zeile löschen
- Verwalten der Tastatureingaben

Fenster- & Grafik-Management
- Anzeigen des Hintergrunds
- Anzeigen der Statistiken
- Anzeigen des nächsten Blocks an der richtigen Position

* Anzeigen des Spielfelds (Schwarzes Rechteck)
* Anzeigen aller anderen, schon heruntergefallenen Blöcke
* Anzeigen des aktuellen Blocks
* Anzeigen eines Rechtecks

* Umwandeln von In-Game-Koordinaten in Fensterkoordinaten


# Zentrale (strukturierte) Datentypen

Ingame-Management
- Speicherung der Variationen jedes Blocks
- Speicherung der festen Blöcke im Spielfeld
- Speicherung des nächsten Blocks
- Speicherung des aktuellen Blocks
- Speicherung der Position des aktuellen Blocks
- Speicherung des Scores
- Speicherung der Schwierigkeit
- Speicherung der Zeitabstände
- Speicherung der aktuellen Zeit, seit dem das Spiel gestartet wurde
- Speicherung der Farben

Fenster-Management
- Speicherung der Fensterbreite
- Speicherung der Fensterhöhe
- Speicherung der Blockbreite (hoch = breit, da quadratisch)
- Speicherung der Position des Spielfelds im Fenster
- Speicherung der Textart