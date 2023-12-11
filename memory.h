#ifndef MEMORY_H
#define MEMORY_H

int *mem_read(int, int);
void mem_write(int addr, int data[2], int pid);
void mem_write2(int addr, int data, int pid);

extern int memory[1024][2];

#endif // MEMORY_H
