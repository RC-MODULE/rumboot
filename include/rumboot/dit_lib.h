#ifndef DIT_LIB_H
#define DIT_LIB_H



void oneshot_n_cycles_0(int cycles, int count_to);
void oneshot_n_cycles_1(int cycles, int count_to);
void periodic_n_cycles_0(int cycles, int count_to);
void freerun_0(int optional_load);
void freerun_1(int optional_load);
void periodic_0(int load, int optional_bgload);
void periodic_1(int load, int optional_bgload);

#endif
