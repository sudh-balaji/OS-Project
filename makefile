my_program: main.c cpu.c memory.c disk.c scheduler.c smm.c
	gcc -Wall -g -o main main.c cpu.c memory.c disk.c scheduler.c smm.c

clean:
	rm -f main
