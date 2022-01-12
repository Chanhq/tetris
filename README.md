# simple-tetris-game-in-c
Die Beschreibung sagt's schon, aber nochmal im Kontext: Das ist ein Uni-Projekt, in dem wir im 3er-Team unsere Skills zeigen sollen.


# Important links

- [GitHub link to our repository](https://github.com/NEOZEO64/simple-tetris-game-in-c)
- [SDL-Alternative](https://www.sfml-dev.org/tutorials/2.5/compile-with-cmake.php) (SDL is the graphics library)
- [Old Tetris C Code with too old libraries](http://csourcecodes.blogspot.com/2016/06/tetris-game-c-program-block-game.html)
- [Tetris C Code from the internet](https://github.com/dashed/tetris-sdl-c)


# Planung / Zielsetzung: Tetris!

## Notwendige Funktionen, damit das Programm funktioniert
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

## Programmstruktur / Aufbau des Programms

1. Bibliotheken einbinden
2. Variablen, Strukturen, Funktionen deklarieren
3. Main-Funktion beginnt
    1. Musik-Datei laden und im Loop starten
    2. Zeit-Stempel erzeugen
    3. Game-Loop
        - Fenster-Befehle abfragen
            - Wenn der Schließen-Knopf gedrückt wird: Schließen
        - Keyboard-Befehle abfragen
            - Wenn W gedrückt: Drehen im Uhrzeigersinn
            - Wenn A gedrückt: Wenn Platz da: Nach links bewegen
            - Wenn D gedrückt: Wenn Platz da: Nach rechts bewegen
            - Wenn S gedrückt: Schneller nach unten bewegen
        - Anderen Zeitstempel erzeugen
        - Wenn die Differenz des ersten Zeitstempels vom zweiten Zeitstempel eine bestimmte Größe übersteigen
            - Setze den ersten Zeitstempel auf den zweiten Zeitstempel
            - Wenn Teil nach unten bewegen möglich:
                - Blöcke des Teils nach unten bewegen
            - Ansonsten
                - Teil "festmachen"
                - Wenn Platz für neuen Block
                    - Neuen Block generieren
                - Ansonsten
                    - (vorerst) beenden
        - Fenster graphisch updaten
4. Funktionen definieren

## Wichtigste Funktionen und ihre Signatur (Parameter und Rückgabewert)

### Ingame-Management
- Rotation der Blöcke
- Randomisierung / Erstellen neuer Blöcke inkl. Farbe
- Checken der Blockkollision
- Bewegen der Blöcke
- Checken, ob Zeilen vollständig sind & Score vergeben
- Zeile löschen
- Verwalten der Tastatureingaben

### Fenster- & Grafik-Management
- Anzeigen des Hintergrunds
- Anzeigen der Statistiken
- Anzeigen des nächsten Blocks an der richtigen Position
- Anzeigen des Spielfelds (Schwarzes Rechteck)
- Anzeigen aller anderen, schon heruntergefallenen Blöcke
- Anzeigen des aktuellen Blocks
- Anzeigen eines Rechtecks
- Umwandeln von In-Game-Koordinaten in Fensterkoordinaten


# Zentrale (strukturierte) Datentypen

## Strukturen
- Block (Farbe, Position, etc.)
- evtl. Fenster

## Normale Variablen
### Ingame-Management
- Variationen jedes Blocks
- Festen Blöcke im Spielfeld
- Nächsten Blocks
- Aktuellen Blocks
- Position des aktuellen Blocks
- Scores
- Schwierigkeit
- Zeitabstände
- Aktuellen Zeit, seit dem das Spiel gestartet wurde
- Farben

### Fenster-Management
- Fensterbreite
- Fensterhöhe
- Blockbreite (hoch = breit, da quadratisch)
- Position des Spielfelds im Fenster
- Textart
