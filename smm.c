#include <stdio.h>
#include <stdlib.h>
#include "smm.h"

// linked list of holes
struct node
{
    int addr;
    int size;
    struct node *next;
};
// struct node *head_smm = malloc(sizeof(struct node));
//  struct node *head_smm = {0, 1023, NULL};
struct node *head_smm = NULL; // Initialize to NULL

void initialize_head_smm()
{
    head_smm = malloc(sizeof(struct node)); // Allocate memory for head_smm
    if (head_smm != NULL)
    {
        head_smm->addr = 0;
        head_smm->size = 1023;
        head_smm->next = NULL;
    }
    else
    {
        // Handle memory allocation failure
    }
}

// allocation table
int allocation_table[256][3]; // PID, base, size

int allocate(int pid, int size)
{
    int base = find_hole(size);
    if (base == -1)
    {
        return 0;
    }
    int index = find_empty_row();
    allocation_table[index][0] = pid;
    allocation_table[index][1] = base;
    allocation_table[index][2] = size;
    add_hole(base + size, 1023 - base - size);
    // printf("pid: %d base: %d size: %d\n", pid, base, size);
    remove_hole(base);
    return 1;
}
void deallocate(int pid)
{
    int base, size;
    for (int i = 0; i < 256; i++)
    {
        if (allocation_table[i][0] == pid)
        {
            base = allocation_table[i][1];
            size = allocation_table[i][2];
            allocation_table[i][2] = 0;
            break;
        }
    }
    add_hole(base, size);
    merge_holes();
}
void add_hole(int base, int size)
{
    struct node *new_node = (struct node *)malloc(sizeof(struct node));

    new_node->addr = base;
    new_node->size = size;
    new_node->next = NULL;

    if (head_smm == NULL || head_smm->addr > base)
    {
        new_node->next = head_smm;
        head_smm = new_node;
        return;
    }

    struct node *temp = head_smm;
    while (temp->next != NULL && temp->next->addr <= base)
    {
        temp = temp->next;
    }
    new_node->next = temp->next;
    temp->next = new_node;
}

void remove_hole(int base)
{
    struct node *temp = head_smm;
    struct node *to_remove = NULL;

    // Check if head is to be removed
    if (temp != NULL && temp->addr == base)
    {
        head_smm = temp->next;
        free(temp); // Free memory of the removed node
        return;
    }

    // Find the node to remove and keep track of the previous node
    while (temp != NULL && temp->next != NULL)
    {
        if (temp->next->addr == base)
        {
            to_remove = temp->next;
            temp->next = temp->next->next;
            break;
        }
        temp = temp->next;
    }

    if (to_remove != NULL)
    {
        free(to_remove); // Free memory of the removed node
    }
}

void merge_holes()
{
    struct node *temp = head_smm;
    while (temp->next != NULL)
    {
        if (temp->addr + temp->size == temp->next->addr)
        {
            temp->size += temp->next->size;
            temp->next = temp->next->next;
        }
        temp = temp->next;
    }
}
int find_hole(int size)
{
    struct node *p = head_smm;
    while (p != NULL)
    {
        if (p->size >= size)
        {
            return p->addr;
        }
        p = p->next;
    }
    return -1;
}

int get_base_address(int pid)
{
    int addr = -1;
    for (int i = 0; i < 256; i++)
    {
        if (allocation_table[i][0] == pid)
        {
            addr = allocation_table[i][1];
            break;
        }
    }
    return addr;
}
int get_size(int pid)
{
    int size = -1;
    for (int i = 0; i < 256; i++)
    {
        if (allocation_table[i][0] == pid)
        {
            size = allocation_table[i][2];
            break;
        }
    }
    return size;
}
int find_empty_row()
{
    int index = -1;
    for (int i = 0; i < 256; i++)
    {
        if (allocation_table[i][2] == 0)
        {
            index = i;
            break;
        }
    }
    return index;
}
int is_allowed_address(int pid, int addr)
{
    int base = get_base_address(pid);
    int size = get_size(pid);
    if (addr >= base && addr < base + size)
    {
        return 1;
    }
    return 0;
}
void print_holes()
{
    struct node *p = head_smm;
    while (p != NULL)
    {
        printf("addr: %d, size: %d\n", p->addr, p->size);
        p = p->next;
    }
}