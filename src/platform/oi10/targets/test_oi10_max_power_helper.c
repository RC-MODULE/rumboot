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
    register uint32_t i, j, k; 
    uint32_t* int_res;
    float*    flt_res;
    
    //rumboot_printf("Start of simult_matrix_multiply_prepare\n");
    int_A   = (void*)(CACHEABLE_IM0_DATA);
    int_B   = int_A + MATRIX_DIMENTION*MATRIX_DIMENTION;
    flt_A   = (void*)(int_B + MATRIX_DIMENTION*MATRIX_DIMENTION);
    flt_B   = flt_A + MATRIX_DIMENTION*MATRIX_DIMENTION;
    int_res = (void*)(flt_B + MATRIX_DIMENTION*MATRIX_DIMENTION);
    flt_res = (void*)(int_res + MATRIX_DIMENTION*MATRIX_DIMENTION);
    total_res = (void*)(flt_res + MATRIX_DIMENTION*MATRIX_DIMENTION);
    //rumboot_printf("int_A = %x, int_B = %x, flt_A = %x, flt_B = %x, int_res =%x,  flt_res =%x, total_res =%x, MATRIX_DIMENTION = %x\n", int_A, int_B, flt_A, flt_B, int_res, flt_res, total_res, MATRIX_DIMENTION);
    //rumboot_printf("int_A[0][0] = %x, int_B[0][0] = %x, flt_A[0][0] = %x, flt_B[0][0] = %x\n", *int_A, *int_B, (uint32_t)(*flt_A), (uint32_t)(*flt_B));
    gpio_set_value_by_mask( GPIO_1_BASE, GPIO_REG_MASK, 0x80);
    //rumboot_printf("Start of simult_matrix_multiply_start\n");
    *total_res = 0;
    for(i = 0; i < MATRIX_DIMENTION; i++){
        for(j = 0; j < MATRIX_DIMENTION; j++){
            (*(int_res+i*MATRIX_DIMENTION+j)) = 0;
            (*(flt_res+i*MATRIX_DIMENTION+j)) = 0;
            for(k = 0; k < MATRIX_DIMENTION; k++) {
                //rumboot_printf("i=%d, j=%d, k=%d\n", i,j,k);
                //rumboot_printf("int_A[%d][%d] = %d\n", i, k, (*(int_A+i*MATRIX_DIMENTION+k)));
                //rumboot_printf("int_B[%d][%d] = %d\n", k, j, (*(int_B+k*MATRIX_DIMENTION+j)));
                (*(int_res+i*MATRIX_DIMENTION+j)) += (*(int_A+i*MATRIX_DIMENTION+k)) * (*(int_B+k*MATRIX_DIMENTION+j));
                //rumboot_printf("Curr int_res[%d][%d] = %d\n", i, j, (*(int_res+i*MATRIX_DIMENTION+j)));
                //-------------------------
                //rumboot_printf("flt_A[%d][%d] = %d\n", i, k, (uint32_t)(*(flt_A+i*MATRIX_DIMENTION+k)));
                //rumboot_printf("flt_B[%d][%d] = %d\n", k, j, (uint32_t)(*(flt_B+k*MATRIX_DIMENTION+j)));
                (*(flt_res+i*MATRIX_DIMENTION+j)) += (*(flt_A+i*MATRIX_DIMENTION+k)) * (*(flt_B+k*MATRIX_DIMENTION+j));
                //rumboot_printf("Curr flt_res[%d][%d] = %d\n", i, j, (uint32_t)(*(flt_res+i*MATRIX_DIMENTION+j)));
            }
            //rumboot_printf("--------Prev *total_res = %d, addition = %d \n", *total_res, (*(int_res+i*MATRIX_DIMENTION+j) + (uint32_t)(*(flt_res+i*MATRIX_DIMENTION+j))));
            (*total_res) += (*(int_res+i*MATRIX_DIMENTION+j) + (uint32_t)(*(flt_res+i*MATRIX_DIMENTION+j)));
            //rumboot_printf("--------Final int_res[%d][%d] = %d\n", i, j, *(int_res+i*MATRIX_DIMENTION+j));
            //rumboot_printf("--------Final flt_res[%d][%d] = %d\n", i, j, (uint32_t)(*(flt_res+i*MATRIX_DIMENTION+j)));
            //rumboot_printf("--------Final *total_res = %d\n", *total_res);
        }
    }
    //rumboot_printf("End of simult_matrix_multiply_start\n");
    gpio_set_value_by_mask( GPIO_1_BASE, GPIO_REG_MASK, 0x00);
    if(*total_res != 1753328) {
        rumboot_printf("ERROR: *int_A = %d,  *int_res = %d, *flt_A = %d, *flt_res = %d, *total_res = %d\n", *int_A, *int_res, (uint32_t)(*flt_A), (uint32_t)(*flt_res), *total_res);
        return 1;
    }
    return 0;
}