#include <stdio.h>
#include "memory.h"
#include "cpu.h"
#include "scheduler.h"

// Base Register: Stores the memory address of the currently executing program.
int base;
// Stores the memory address (relative to Base) of the currently executing instruction.
int PC;
// Instruction Register 0: Stores the OP Code of the currently executing instruction.
int IR0;
// Instruction Register 1: Stores the argument of the currently executing instruction, if applicable. Set to 0 if there is no argument.
int IR1;
// Accumulator Register: Stores the results of instructions
int AC;
// Memory Address Register: Stores memory address that will be accessed during a read/write command to memory
int MAR;
// Memory Buffer Register: Stores the data that will be read/written to memory.
int MBR;
int PID;
int size;
int buf;
// Read the instruction in memory at location addr, and place the OP Code in IR0 and the argument (if applicable) in IR1.
struct PCB context_switch(struct PCB new_vals)
{
    struct PCB old_vals;
    old_vals.PID = PID;
    old_vals.size = size;
    old_vals.base = base;
    old_vals.PC = PC;
    old_vals.IR0 = IR0;
    old_vals.IR1 = IR1;
    old_vals.AC = AC;
    old_vals.MAR = MAR;
    old_vals.MBR = MBR;
    base = new_vals.base;
    PC = new_vals.PC;
    IR0 = new_vals.IR0;
    IR1 = new_vals.IR1;
    AC = new_vals.AC;
    MAR = new_vals.MAR;
    MBR = new_vals.MBR;
    PID = new_vals.PID;
    size = new_vals.size;

    // printf("base: %i\n", base);
    return old_vals;
}
void fetch_instruction(int addr)
{
    int pid = currentProcess();
    int *arr = mem_read(addr + base, pid);
    IR0 = arr[0];
    if (IR0 == 1 || IR0 == 12)
    {
        IR1 = arr[1];
    }
    else
    {
        IR1 = 0;
    }
}
void execute_instruction(int pid)
{
    int temp;
    if (IR0 == 0) // exit
    {
    }
    else if (IR0 == 1) // load_const
    {
        AC = IR1;
    }
    else if (IR0 == 2) // move_from_mar
    {
        AC = MBR;
    }
    else if (IR0 == 3) // move_from_mar
    {                  // MAR to AC
        AC = MAR;
    }
    else if (IR0 == 4) // move_to_mbr
    {                  // AC to MBR
        MBR = AC;
    }
    else if (IR0 == 5) // move_to_mar
    {                  // AC to MAR
        MAR = AC;
    }
    else if (IR0 == 6) // load_at_addr
    {
        // MAR -> addr
        MBR = mem_read(MAR, pid); // TODO was *mem_raed(MAR)
    }
    else if (IR0 == 7) // write_at_addr
    {
        mem_write2(MAR, MBR, pid);
        
    }
    else if (IR0 == 8) // add -> MBR into AC
    {
        AC = MBR + AC;
    }
    else if (IR0 == 9) // multiply
    {
        AC = AC * MBR;
    }
    else if (IR0 == 10) // and
    {
        // store result of AC && MBR in AC
        if (AC != 0 && MBR != 0)
        {
            AC = 1;
        }
        else
        {
            AC = 0;
        }
    }
    else if (IR0 == 11)
    {
        // store result of AC || MBR in AC
        if (AC != 0 || MBR != 0)
        {
            AC = 1;
        }
        else
        {
            AC = 0;
        }
    }
    else if (IR0 == 12)
    {
        if (AC != 0)
        {
            // PC = addr - 1
            PC = IR1 - 1;
        }
    }
    else if (IR0 == 13)
    {
    }
}
int mem_address(int l_addr)
{
    return base + l_addr;
}
int clock_cycle()
{
    int temp;
    int pid = currentProcess();
    fetch_instruction(PC);

    // printf("IR0: %i\n", IR0);
    // printf("%s", "before execute reached");
    execute_instruction(pid);
    // printf("%s", "after execute reached");
    // printf("PID: %i PC: %i IR0: %i AC: %i\n", PID, PC, IR0, AC);
    PC++;
    if (IR0 == 0)
    {
        return 0;
    }
    return 1;
}
