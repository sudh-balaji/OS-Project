#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"
#include "scheduler.h"
#include "smm.h"

int *translate(char *instruction, int translation[])
{
    instruction = strtok(instruction, "\n");
    char *command = strtok(instruction, " ");
    char *argument = strtok(NULL, " ");

    if (strcmp(command, "exit") == 0)
    {
        translation[0] = 0;
        translation[1] = 0;
    }
    else if (strcmp(command, "load_const") == 0)
    {
        translation[0] = 1;
        translation[1] = atoi(argument);
    }
    else if (strcmp(command, "move_from_mbr") == 0)
    {
        translation[0] = 2;
        translation[1] = 0;
    }
    else if (strcmp(command, "move_from_mar") == 0)
    {
        translation[0] = 3;
        translation[1] = 0;
    }
    else if (strcmp(command, "move_to_mbr") == 0)
    {
        translation[0] = 4;
        translation[1] = 0;
    }
    else if (strcmp(command, "move_to_mar") == 0)
    {
        translation[0] = 5;
        translation[1] = 0;
    }
    else if (strcmp(command, "load_at_addr") == 0)
    {
        translation[0] = 6;
        translation[1] = 0;
    }
    else if (strcmp(command, "write_at_addr") == 0)
    {
        translation[0] = 7;
        translation[1] = 0;
    }
    else if (strcmp(command, "add") == 0)
    {
        translation[0] = 8;
        translation[1] = 0;
    }
    else if (strcmp(command, "multiply") == 0)
    {
        translation[0] = 9;
        translation[1] = 0;
    }
    else if (strcmp(command, "and") == 0)
    {
        translation[0] = 10;
        translation[1] = 0;
    }
    else if (strcmp(command, "or") == 0)
    {
        translation[0] = 11;
        translation[1] = 0;
    }
    else if (strcmp(command, "ifgo") == 0)
    {
        translation[0] = 12;
        translation[1] = atoi(argument);
    }
    else if (strcmp(command, "sleep") == 0)
    {
        translation[0] = 13;
        translation[1] = 0;
    }
    else
    {
        printf("%s", "invalid instruction");
        exit(1);
    }

    return translation;
}

void load_prog(char fname[], int addr, int pid, int *temp_size)
{
    int count = 0;
    FILE *file_pointer = fopen(fname, "r");

    char *line = NULL;
    size_t len;
    ssize_t read;

    while ((read = getline(&line, &len, file_pointer)) != -1)
    {
        if (line[0] == '/' && line[1] == '/') //
        {
        }
        else
        {
            int translation[2];
            translate(line, translation);
            mem_write(addr, translation, pid);
            // printf("%d %d\n", translation[0], translation[1]);
            addr++;
            count++;
        }
    }
    *temp_size = count;
}

void load_programs(char fname[])
{
    // open multiple files
    // read each line

    FILE *file_pointer = fopen(fname, "r");

    char *line = NULL;
    size_t len;
    ssize_t read;
    int allocated, pid, base;
    while ((read = getline(&line, &len, file_pointer)) != -1)
    {
        // print list of holes
        // print_holes();
        //
        int zero = 0;
        int *temp_size = &zero; // how many lines of code
        char *size;
        char *name;
        line = strtok(line, "\n");
        size = strtok(line, " "); // size, not changing variable name because lazy
        name = strtok(NULL, " ");

        // check with smm
        pid = count;
        allocated = allocate(pid, atoi(size));
        // if allocated = 0, print error
        if (allocated == 0)
        {
            printf("ERROR: not enough memory for process %d\n", pid);
            count++;
        }
        else
        {
            base = get_base_address(pid);
            load_prog(name, base, pid, temp_size);
            new_process(base, *temp_size);
        }
        // printf("%s base: %d ", size, base);
        // printf("%s\n", name);
    }
}
