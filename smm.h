#ifndef FILENAME_H
#define FILENAME_H

int allocate(int, int);
void deallocate(int);
void add_hole(int, int);
void remove_hole(int);
void merge_holes();
int find_hole(int);
int get_base_address(int);
int find_empty_row();
int is_allowed_address(int,int);
void print_holes();
void initialize_head_smm();
#endif // FILENAME_H
