/*
 * test_oi10_cpu_power_helper.c
 *
 *  Created on: 
 *   Original Author: 
 *   Rebuild by     : 
 */
// 
#include <stdint.h>
#include <rumboot/io.h>
#include <rumboot/printf.h>
#include <platform/devices.h>
#include <devices/gpio_pl061.h>
#include <regs/regs_gpio_pl061.h>

#ifndef MATRIX_DIMENTION
#define MATRIX_DIMENTION 4
#endif

#define CACHEABLE_IM0            0x80090000
#define CACHEABLE_IM0_CODE       (CACHEABLE_IM0)
#define CACHEABLE_IM0_CODE_SIZE  (16*1024)
#define CACHEABLE_IM0_DATA       (CACHEABLE_IM0_CODE + CACHEABLE_IM0_CODE_SIZE)
#define CACHEABLE_IM0_DATA_SIZE  (32*1024)

uint32_t __attribute__((section(".text.test"))) check_cpu_power(void)
{
    uint32_t *int_A, *int_B, *total_res;
    float *flt_A, *flt_B;
    uint32_t *i, *j, *k; 
    uint32_t* int_res;
    float*    flt_res;
    
    rumboot_printf("Start of simult_matrix_multiply_prepare\n");
    int_A   = CACHEABLE_IM0_DATA;
    int_B   = int_A + MATRIX_DIMENTION*MATRIX_DIMENTION;
    flt_A   = int_B + MATRIX_DIMENTION*MATRIX_DIMENTION;
    flt_B   = flt_A + MATRIX_DIMENTION*MATRIX_DIMENTION;
    int_res = flt_B + MATRIX_DIMENTION*MATRIX_DIMENTION;
    flt_res = int_res + MATRIX_DIMENTION*MATRIX_DIMENTION;
    total_res = flt_res + 1;
    i         = total_res + 1;
    j         = i + 1;
    k         = j + 1;
    rumboot_printf("int_A = %x, int_B = %x, flt_A = %x, flt_B = %x, int_res =%x,  flt_res =%x, total_res =%x, i =%x, j=%x, k=%x, MATRIX_DIMENTION = %x\n", int_A, int_B, flt_A, flt_B, int_res, flt_res, total_res, i, j, k, MATRIX_DIMENTION);
    rumboot_printf("int_A[0][0] = %x, int_B = %x, flt_A = %x, flt_B = %x, int_res =%x,  flt_res =%x, total_res =%x, i =%x, j=%x, k=%x, MATRIX_DIMENTION = %x\n", int_A, int_B, flt_A, flt_B, int_res, flt_res, total_res, i, j, k, MATRIX_DIMENTION);
    gpio_set_value_by_mask( GPIO_1_BASE, GPIO_REG_MASK, 0x80);
    //rumboot_printf("Start of simult_matrix_multiply_start\n");
    for(*i = 0; *i < MATRIX_DIMENTION; *i = *i+1){
        rumboot_printf("Outer loop, itreration %x\n", *i);
        for(*j = 0; *j < MATRIX_DIMENTION; *j = *j+1){
            rumboot_printf("Inner loop, itreration %x\n", *j);
            for(*k = 0; *k < MATRIX_DIMENTION; *k = *k+1) {
                //int_res[i][j] += int_A[i][k] * int_B[k][j];
                //flt_res[i][j] += flt_A[i][k] * flt_B[k][j];
                rumboot_printf("Inner k-loop, itreration %x\n", *k);
                *(int_res+(*i)*MATRIX_DIMENTION+(*j)) += *(int_A+(*i)*MATRIX_DIMENTION+(*k)) * (*(int_B+(*k)*MATRIX_DIMENTION+(*j)));
                rumboot_printf("--- Curr int_res[%d][%d] = %x\n", *i, *j, *(int_res+(*i)*MATRIX_DIMENTION+(*j)));
                *(flt_res+(*i)*MATRIX_DIMENTION+(*j)) += *(flt_A+(*i)*MATRIX_DIMENTION+(*k)) * (*(flt_B+(*k)*MATRIX_DIMENTION+(*j)));
                rumboot_printf("--- Curr flt_res[%d][%d] = %f\n", *i, *j, *(flt_res+(*i)*MATRIX_DIMENTION+(*j)));
                *total_res += *(int_res+(*i)*MATRIX_DIMENTION+(*j)) + *(flt_res+(*i)*MATRIX_DIMENTION+(*j)) ;
            }
            rumboot_printf("Final int_res[%d][%d] = %x\n", *i, *j, *(int_res+(*i)*MATRIX_DIMENTION+(*j)));
            rumboot_printf("Final flt_res[%d][%d] = %f\n", *i, *j, *(flt_res+(*i)*MATRIX_DIMENTION+(*j)));
        }
    }
    rumboot_printf("End of simult_matrix_multiply_start\n");
    gpio_set_value_by_mask( GPIO_1_BASE, GPIO_REG_MASK, 0x00);
    if(*total_res != 1742944) {
        rumboot_printf("ERROR: *int_A = %d,  *int_res = %d, *flt_A = %d, *flt_res = %x, *total_res = %d\n", *int_A, *int_res, *flt_A, *flt_res, *total_res);
        return 1;
    }
    return 0;
}