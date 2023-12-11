#ifndef DISK_H
#define DISK_H

//disk.c headers
int *translate(char *instruction, int translation[]);
void load_prog(char fname[], int addr, int);
void load_programs(char fname[]);

#endif // DISK_H
