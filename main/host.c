#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <e-hal.h>
#include <e-loader.h>
//円周率を求める
int main(){
    unsigned cores, n;
    e_platform_t platform;
    e_epiphany_t dev[4];

    e_init(NULL);
    e_reset_system();
    e_get_platform_info(&platform);

    scanf("%u %u", &cores, &n);
    unsigned use_rows = 0;
    unsigned use_cores[4] = {0};

    unsigned core_counter = cores;
    for(int row = 0; row < 4; row++){
        use_cores[row] = (core_counter >= 4)? 4 : core_counter;
        e_open(&dev[row], row, 0, 1, use_cores[row]);
        e_reset_group(&dev[row]);
        e_load_group("eCores.elf", &dev[row], 0, 0, 1, use_cores[row], E_FALSE);
        if(core_counter <= 4){
            use_rows = row + 1;
            break;
        }
        core_counter -= 4;
    }
    unsigned row_number[4] = {0};
    for(unsigned row = 0; row < use_rows; row++){
        row_number[row] = row;
        for(unsigned col = 0; col < use_cores[row]; col++){
            if(e_write(&dev[row], 0, col, 0x7000, &n, sizeof(unsigned)) != sizeof(unsigned)){
                fprintf(stderr, "Failed to write to local memory\n");
                return -1;
            }
            if(e_write(&dev[row], 0, col, 0x7008, &cores, sizeof(unsigned)) != sizeof(unsigned)){
                fprintf(stderr, "Failed to write to local memory\n");
                return -1;
            }
            if(e_write(&dev[row], 0, col, 0x7010, &row_number[row], sizeof(unsigned)) != sizeof(unsigned)){
                fprintf(stderr, "Failed to write to local memory\n");
                return -1;
            }
        }
    }
    struct timeval start, end;
    gettimeofday(&start, NULL);

    unsigned end_flag_total = 0;
    unsigned end_flags[4][4] = {0};
    for(unsigned row = 0; row < use_rows; row++){
        e_start_group(&dev[row]);
    }
    while(!end_flag_total){
        //usleep(10);
        end_flag_total = 1;
        for(unsigned row = 0; row < use_rows; row++){
            for(unsigned col = 0; col < use_cores[row]; col++){
                if(e_read(&dev[row], 0, col, 0x7018, &end_flags[row][col], sizeof(unsigned)) != sizeof(unsigned)){
                    fprintf(stderr, "Failed to read\n");
                }
                end_flag_total = end_flag_total && end_flags[row][col];
            }
        }
    }
    double integral = 0.0;
    for(unsigned row = 0; row < use_rows; row++){
        for(unsigned col = 0; col < use_cores[row]; col++){
            double sum;
            if(e_read(&dev[row], 0, col, 0x7020, &sum, sizeof(double)) != sizeof(double)){
                fprintf(stderr, "Failed to read\n");
            }
            integral += sum;
        }
    }
    gettimeofday(&end, NULL);
    double calc_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) * 1.0e-6;
    printf("%u %lf %lf\n", cores, calc_time, integral);
    e_finalize();
}