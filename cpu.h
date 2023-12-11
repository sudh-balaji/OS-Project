#ifndef CPU_H
#define CPU_H

// cpu.c headers
void fetch_instruction(int addr);
void execute_instruction(int pid);
int mem_address(int l_addr);
int clock_cycle();
struct PCB context_switch(struct PCB regs);

#endif // CPU_H
