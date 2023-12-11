#ifndef SCHEDULER_H
#define SCHEDULER_H

int schedule(int, int);
void next_process();
void new_process(int, int);
void printList();
void printTable();
int currentProcess();
void removeProcess(int);

struct PCB
{
    int PID;
    int size;
    int base;
    int PC;
    int IR0;
    int IR1;
    int AC;
    int MAR;
    int MBR;
};

extern struct node *head;
extern struct node *current;
extern int count;

#endif // SCHEDULER_H
