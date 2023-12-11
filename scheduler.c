#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
int count = 0;
int TIME_QUANTUM = 10;
// PCB(Process Control Block) Data Structure
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
struct node {
    struct PCB *data;
    struct node *next;
};
// PCT(Process Control Table) Data Structure
struct PCB process_table[1024];
// Ready Queue Data Structure
struct node *head = NULL;
struct node *current = NULL;

void printList() {
    struct node *p = head;
    printf("\n[");

    while(p != NULL) {
        printf(" %d ",p->data->PID);
        p = p->next;
    }
    printf("]");
}

void insertAtEnd(struct node* lk) {
    struct node* linkedlist = head;
    while (linkedlist->next != NULL) {
        linkedlist = linkedlist->next;
    }
    linkedlist->next = lk;
}

void next_process() {
    if (head == NULL || head->next == NULL) {
        // Handle an empty list or only one more node in the list
        return;
    }

    struct node *temp = head;
    head = head->next;
    temp->next = NULL; 

    if (head == NULL) {
        // may not be necessary?
    } else {
        insertAtEnd(temp); // Insert the removed node at the end
    }
}

void new_process(int base, int size)
{
    struct PCB *pcb = (struct PCB *)malloc(sizeof(struct PCB));
    pcb->PID = count;
    count++;
    pcb->size = size;
    pcb->base = base;
    pcb->PC = 0;
    pcb->IR0 = 0;
    pcb->IR1 = 0;
    pcb->AC = 0;
    pcb->MAR = 0;
    pcb->MBR = 0;
    struct node *link = (struct node *)malloc(sizeof(struct node));
    link->data = pcb;
    link->next = NULL;
    if (head == NULL)
    {
        head = link;
        process_table[pcb->PID] = *pcb;
        // add these values to cpu.c registers
        context_switch(*pcb);
        return;
    }
    insertAtEnd(link);
    process_table[pcb->PID] = *pcb;
}
int currentProcess(){
    return head->data->PID;
}
void printTable(){
    printf("-------\n");
    for (int i = 0; i < 3; i++){
        printf("PID: %d size: %d base: %d PC: %d IR0: %d IR1: %d AC: %d\n", process_table[i].PID, process_table[i].size, process_table[i].base, process_table[i].PC, process_table[i].IR0, process_table[i].IR1, process_table[i].AC);
    }
    printf("-------\n");
}
int schedule(int cycle_num, int process_status){
    int PID_var;
    // check if process is done running
    if (process_status == 0){
        head = head->next;
        if (head == NULL){
            return 2; // process is done running and no more in ready queue
            
        }
        else {
            // printf("CONTEXT SWITCH\n");
            // already at next process, just context switch
            PID_var = head->data->PID;
            struct PCB *pcb = &process_table[PID_var];
            context_switch(*pcb);
            return 0; // process is done running but more in ready queue
            // reset count in main
        }
    }
    if (cycle_num >= TIME_QUANTUM && head->next != NULL) {
        // printf("CONTEXT SWITCH\n READY QUEUE: ");
        // printList();
        // printf("\n");
        // next process and context switch
        PID_var = head->data->PID; // old process PID
        next_process(); // new process in ready queue front
        struct PCB *pcb = &process_table[head->data->PID]; // pcb of new process from process table
        process_table[PID_var] = context_switch(*pcb); // context switch old process into process table
        context_switch(*pcb); // context switch new process into cpu.c registers
        // printTable();
        return 0; // need to reset count
    }
        // conditons not met, continue running process
        return 1; // continute until finished with last process
}

void removeProcess(int pid){
    struct node *temp = head;
    if (temp->data->PID == pid){
        head = head->next;
        return;
    }
    while (temp->next != NULL){
        if (temp->next->data->PID == pid){
            temp->next = temp->next->next;
            break;
        }
        temp = temp->next;
    }
}
