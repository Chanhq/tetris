#include <stdio.h>
#include <string.h>

struct Student {
    char vorname[20];
    char nachname[20];
    unsigned int matrikelnummer;
    char adresse[100];
    unsigned short anzahlPflichtkurse;
} student;


void printStudent(struct Student *student);
void printStudents();

#define NUMSTUDENTS 3
struct Student students[NUMSTUDENTS];


int main() {
    //Feld festlegen
    int i;
    i = 0; //Erster Student
    strcpy(students[i].vorname, "Anna");
    strcpy(students[i].nachname, "Musterfrau");
    students[i].matrikelnummer = 22222;
    strcpy(students[i].adresse, "Am Schwarzenberg-Campus 3");
    students[i].anzahlPflichtkurse = 4;

    i = 1; //Zweiter Student
    strcpy(students[i].vorname, "Hans");
    strcpy(students[i].nachname, "Peter");
    students[i].matrikelnummer = 44444;
    strcpy(students[i].adresse, "Kasernenstrasse 12");
    students[i].anzahlPflichtkurse = 2;

    i = 2;//Dritter Student
    strcpy(students[i].vorname, "Lisa");
    strcpy(students[i].nachname, "Lustig");
    students[i].matrikelnummer = 66666;
    strcpy(students[i].adresse, "Denickestrasse 15");
    students[i].anzahlPflichtkurse = 8;

    printStudents();

    //Erstes und letztes Element tauschen
    struct Student tempStudent = students[0];
    students[0] = students[NUMSTUDENTS-1];
    students[NUMSTUDENTS-1] = tempStudent;
    printf("##########################################\nErster und letzter Student vertauscht.\n\n");

    printStudents();

    return 0;
}


void printStudent(struct Student *student) {
    printf("Name: %s, %s\n", student->nachname, student->vorname);
    printf("Matrikelnummer: %d\n", student->matrikelnummer);
    printf("Adresse: %s\n", student->adresse);
    printf("Anzahl der Pflichtfächer: %d\n", student->anzahlPflichtkurse);
}

void printStudents() {
    for (int i = 0; i<NUMSTUDENTS; i++) {
        printf("##########################################\nStudent %d:\n",i+1);
        printStudent(&students[i]);
        printf("\n");
    }
}