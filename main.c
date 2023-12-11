#include <stdio.h>
#include "memory.h"
#include "disk.h"
#include "cpu.h"
#include "scheduler.h"
#include "smm.h"

int main()
{
    initialize_head_smm();
    char fname[] = {"program_list_invalid_access.txt"};
    load_programs(fname);
    // printTable();
    int run = 1;
    int count = 1;
    int num = 0;
    while (run == 1 || run == 0)
    {
        // for (int i = 0; i < 200; i++){
        // printf("count: %i\n", count);
        num = clock_cycle();
        // printf("num: %i\n", num);
        run = schedule(count, num);
        // printf("run: %i\n", run);
        if (run == 0)
        {
            count = 1;
        }
        else
        {
            count++;
        }
    }

    /*
    int run = 1;
    while (run == 1)
    {
        run = clock_cycle();
    }
    // Print out first 20 locations in memory
    */
    // for (int i = 0; i < 300; i++)
    //{
    printf("30: %i %i\n", memory[30][0], memory[30][1]);
    printf("150: %i %i\n", memory[150][0], memory[150][1]);
    printf("230: %i %i\n", memory[230][0], memory[230][1]);

    //}
}
