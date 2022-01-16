/*
Aufgabe:
* Erweitern um Sortier-Funktion, alphabetisch nach Nachname
    * Verändern ausschließlich Zeiger-Variablen in den Struktur-Instanzen.
* Im Hauptprogramm die sortierte und unsortierte Liste präsentieren

Hinweis: Verwenden Sie die strcmp-Funktion aus der <string.h>-Bibliothek.
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
void printStudents(student *anyStudent);
void delStudent(student *student);
void reverseStudents(student **head);
void sortStudents(student *head);
student* getFirstStudent(student *anyStudent);

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
    strcpy(students[i].vorname, "Susi");
    strcpy(students[i].nachname, "Schmusi");
    students[i].matrikelnummer = 44444;
    strcpy(students[i].adresse, "Alter Postweg 93");
    students[i].anzahlPflichtkurse = 8;
    students[i].prev = &students[i-1];
    students[i].next = &students[i+1];

    i = 4;
    strcpy(students[i].vorname, "Frau");
    strcpy(students[i].nachname, "Herrmann");
    students[i].matrikelnummer = 55555;
    strcpy(students[i].adresse, "Niemannstraße 56");
    students[i].anzahlPflichtkurse = 8;
    students[i].prev = &students[i-1];
    students[i].next = NULL;

    printStudents(head);

    sortStudents(head);
    printf("##########################################\nListe der Studenten sortiert\n\n");
    printStudents(head);


    return 0;
}

student* getFirstStudent(student *anyStudent) {
    while (anyStudent->prev!=NULL) {anyStudent = anyStudent->prev;}
    return anyStudent;
}

void printStudent(student *student) {
    /* debug
    if (student->prev != NULL && student->next != NULL) {
        printf("%s-%s-%s\n",student->prev->nachname,student->nachname,student->next->nachname);
    } else if (student->prev == NULL) {
        printf("NULL-%s-%s\n",student->nachname,student->next->nachname);
    } else if (student->next == NULL) {
        printf("%s-%s-NULL\n",student->prev->nachname,student->nachname);
    }
    */

    printf("%s, %s\n",student->nachname,student->vorname);
    printf("Matrikelnummer: %d\n", student->matrikelnummer);
    printf("Adresse: %s\n", student->adresse);
    printf("Anzahl der Pflichtfächer: %d\n", student->anzahlPflichtkurse);
    printf("\n");
}

void printStudents(student* anyStudent) {
    anyStudent = getFirstStudent(anyStudent);
    while (anyStudent != NULL) {
        printStudent(anyStudent);
        anyStudent = anyStudent->next;
    }
    printf("\n");
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

void sortStudents(student *anyStudent) {
    int sorted = 0;
    while (!sorted) {
        anyStudent = getFirstStudent(anyStudent);
        sorted = 1;
        while (anyStudent->next!=NULL) {
            anyStudent = anyStudent->next;
            if (strcmp(anyStudent->nachname,anyStudent->prev->nachname)<0) {
                sorted = 0;

                student* prevStudent = anyStudent->prev->prev;
                student* secondStudent = anyStudent->prev;
                student* firstStudent = anyStudent;
                student* nextStudent = anyStudent->next;

                if (prevStudent != NULL) {prevStudent->next = firstStudent;}
                firstStudent->next = secondStudent;
                firstStudent->prev = prevStudent;
                secondStudent->next = nextStudent;
                secondStudent->prev = firstStudent;
                if (nextStudent!= NULL) {nextStudent->prev = secondStudent;}
            } 
        }
    }
}



/*
void sortStudents(student **anyStudent) {
    int sorted = 0; //false
    while (!sorted) {
        //get first student
        while ((*anyStudent)->prev!=NULL) {(*anyStudent) = (*anyStudent)->prev;}
        sorted = 1;

        while ((*anyStudent)->next!=NULL) { //go 1x through every student
            (*anyStudent) = (*anyStudent)->next;
            if (strcmp((*anyStudent)->nachname,(*anyStudent)->prev->nachname)<0) {
                sorted = 0;

                student* prevStudent = (*anyStudent)->prev->prev;
                student* secondStudent = (*anyStudent)->prev;
                student* firstStudent = (*anyStudent);
                student* nextStudent = (*anyStudent)->next;

                if (prevStudent != NULL) {prevStudent->next = firstStudent;}

                firstStudent->next = secondStudent;
                firstStudent->prev = prevStudent;

                secondStudent->next = nextStudent;
                secondStudent->prev = firstStudent;

                if (nextStudent!= NULL) {nextStudent->prev = secondStudent;}

            } 
        }
    }
}
*/