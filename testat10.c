/*
Aufgabe:
1)  Realisieren Sie die Übungsaufgabe zur Vorlesung vom Übungsblatt 09
    in Form einer doppelt verketteten linearen Liste anstelle eines Feldes.
2)  Der Speicher für die Listenelemente soll dabei dynamisch alloziert werden.
3)  Fügen Sie zwei weitere Strukturinstanzen hinzu und
4)  löschen Sie das dritte Listenelement.
5)  Implementieren Sie zusätzlich eine Funktion, die dazu dient, die Reihenfolge der Elemente umzudrehen.
6)  Testen Sie deren Funktionalität im Hauptprogramm.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Student {
    char vorname[20];
    char nachname[20];
    unsigned int matrikelnummer;
    char adresse[100];
    unsigned short anzahlPflichtkurse;
    struct Student* next;
    struct Student* prev;
};

typedef struct Student student;

void printStudent(student *student);
void printStudents(student *head);
void delStudent(student *student);
void reverseStudents(student **head);

student* students;
unsigned int listLen = 10;

int main() {
    //Feld festlegen
    students = (student*) malloc(listLen*sizeof(student));
    student *head = &students[0];
    int i;

    
    i = 0;
    strcpy(students[i].vorname, "Anna");
    strcpy(students[i].nachname, "Musterfrau");
    students[i].matrikelnummer = 11111;
    strcpy(students[i].adresse, "Am Schwarzenberg-Campus 3");
    students[i].anzahlPflichtkurse = 4;
    students[i].prev = NULL;
    students[i].next = &students[i+1];    

    i = 1;
    strcpy(students[i].vorname, "Hans");
    strcpy(students[i].nachname, "Peter");
    students[i].matrikelnummer = 22222;
    strcpy(students[i].adresse, "Kasernenstrasse 12");
    students[i].anzahlPflichtkurse = 2;
    students[i].prev = &students[i-1];
    students[i].next = &students[i+1];

    i = 2;
    strcpy(students[i].vorname, "Lisa");
    strcpy(students[i].nachname, "Lustig");
    students[i].matrikelnummer = 33333;
    strcpy(students[i].adresse, "Denickestrasse 15");
    students[i].anzahlPflichtkurse = 8;
    students[i].prev = &students[i-1];
    students[i].next = &students[i+1];

    i = 3;
    strcpy(students[i].vorname, "Frau");
    strcpy(students[i].nachname, "Herrmann");
    students[i].matrikelnummer = 44444;
    strcpy(students[i].adresse, "Alter Postweg 93");
    students[i].anzahlPflichtkurse = 8;
    students[i].prev = &students[i-1];
    students[i].next = &students[i+1];

    i = 4;
    strcpy(students[i].vorname, "Susi");
    strcpy(students[i].nachname, "Schmusi");
    students[i].matrikelnummer = 55555;
    strcpy(students[i].adresse, "Niemannstraße 56");
    students[i].anzahlPflichtkurse = 8;
    students[i].prev = &students[i-1];
    students[i].next = NULL;

    printStudents(head);

    delStudent(&students[2]);
    printf("##########################################\nDritter Student gelöscht:\n\n");
    printStudents(head);

    reverseStudents(&head);

    printf("##########################################\nListe der Studenten vertauscht\n\n");
    printStudents(head);

    return 0;
}

void printStudent(student *student) {
    printf("Name: %s, %s\n", student->nachname, student->vorname);
    printf("Matrikelnummer: %d\n", student->matrikelnummer);
    printf("Adresse: %s\n", student->adresse);
    printf("Anzahl der Pflichtfächer: %d\n", student->anzahlPflichtkurse);
    printf("\n");
}

void printStudents(student* head) {
    student *tempStudent = head; 
    while (tempStudent != NULL) {
        printStudent(tempStudent);
        tempStudent = tempStudent->next;
    }
}

void delStudent(student* delStudent) {
    student *tempStudent = delStudent;
    student *prevStudent = tempStudent->prev;
    student *nextStudent = tempStudent->next;

    prevStudent->next = nextStudent;
    nextStudent->prev = prevStudent;
}

void reverseStudents(student **head) {
    while (1) {
        student *prevPointer = (*head)->prev;
        (*head)->prev = (*head)->next;
        (*head)->next = prevPointer;
        if ((*head)->prev==NULL) {return;}
        (*head) = (*head)->prev;
    }
}