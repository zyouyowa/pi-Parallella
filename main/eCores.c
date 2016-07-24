#include "e_lib.h"
#include <math.h>

volatile unsigned *n = (unsigned*)0x7000;
volatile unsigned *cores = (unsigned*)0x7008;
volatile unsigned *row = (unsigned*)0x7010;
volatile unsigned *calc_end_flag = (unsigned*)0x7018;
volatile double *sum = (double*)0x7020;

double f(double x){
    return 4.0 / (1.0 + x * x);
}

int main(){
    (*calc_end_flag) = 0;
    e_coreid_t id = e_get_coreid();
    unsigned tmp_row, col;
    unsigned number;

    e_coords_from_coreid(id, &tmp_row, &col);

    number = (2 * (*row) * 4) + (2 * col + 1);
    double h = 1.0 / (double)(*n);
    for(unsigned i = number; i < (*n); i += (*cores)){
        double x = h * ((double)i + 0.5);
        (*sum) += f(x);
    }
    (*sum) *= h; //h == 1/n
    (*calc_end_flag) = 1;
    return 0;
}
