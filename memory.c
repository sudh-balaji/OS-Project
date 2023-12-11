#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"
#include "scheduler.h"
#include "smm.h"

int memory[1024][2];
int *mem_read(int addr, int pid)
{
    bool is_allowed;
    is_allowed = is_allowed_address(pid, addr);
    if (!is_allowed)
    {
        deallocate(pid);
        removeProcess(pid);
        return NULL;
    }
    if (addr < 1024 && addr > -1)
    {
        int *arr = (int *)malloc(2 * sizeof(int));
        arr[0] = memory[addr][0];
        arr[1] = memory[addr][1];
        return arr;
    }
    else
    {
        return NULL;
    }
}
void mem_write(int addr, int data[2], int pid)
{
    bool is_allowed;
    is_allowed = is_allowed_address(pid, addr);
    if (is_allowed)
    {
        memory[addr][0] = data[0];
        memory[addr][1] = data[1];
    }
    else
    {
        deallocate(pid);
        removeProcess(pid);
    }
}
void mem_write2(int addr, int data, int pid)
{
    bool is_allowed;
    is_allowed = is_allowed_address(pid, addr);
    if (is_allowed)
    {
        memory[addr][0] = data;
    }
    else
    {
        printf("invalid memory acces: pid: %d \n", pid);
        // deallocate(pid);
        // removeProcess(pid);
    }
}
