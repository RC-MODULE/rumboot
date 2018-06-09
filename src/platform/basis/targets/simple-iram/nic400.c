#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#include <rumboot/printf.h>
#include <rumboot/io.h>


#include <platform/devices.h>
#include <devices/nic400.h>
#include <rumboot/platform.h>
#include <regs/regs_nic400.h>



int nic400_check_periph_regs(int ib)
{
    int ok = 0;
    int a = 3;
    nic400_set_wqos(a, ib );
    if (nic400_check_wqos(ib)==a){
        ok = 1;
    }

    nic400_set_rqos(a, ib );
    if (nic400_check_rqos(ib)==a){
        ok = 1;
    }

    nic400_set_fn_mod(a, ib );
    if (nic400_check_fn_mod(ib)==a){
        ok = 1;
    }
    if(ok){
        return 1;
    }
    else{
        return 0;
    }
}



int main ()
{
// Set up interrupt handlers


    int id_error_count = 0;
    int periph_error_count = 0;

    int pidr4 = 0;
    int pidr5 = 0;
    int pidr6 = 0;
    int pidr7 = 0;
    int pidr0 = 0;
    int pidr1 = 0;
    int pidr2 = 0;
    int pidr3 = 0;
    int cidr0 = 0;
    int cidr1 = 0;
    int cidr2 = 0;
    int cidr3 = 0;

    pidr4 = nic400_get_pidr4();
    pidr5 = nic400_get_pidr5();
    pidr6 = nic400_get_pidr6();
    pidr7 = nic400_get_pidr7();
    pidr0 = nic400_get_pidr0();
    pidr1 = nic400_get_pidr1();
    pidr2 = nic400_get_pidr2();
    pidr3 = nic400_get_pidr3();
    cidr0 = nic400_get_cidr0();
    cidr1 = nic400_get_cidr1();
    cidr2 = nic400_get_cidr2();
    cidr3 = nic400_get_cidr3();



    rumboot_printf("NIC400 test START\n");
    rumboot_printf("Checking ID regs \n" );


    rumboot_printf("    PIDR4   - ");
    if (pidr4 == 0x04 ){
        rumboot_printf("OK \n");
    }
    else {
        id_error_count ++;
        rumboot_printf("    BAD \n");
    }

    rumboot_printf("    PIDR5   - ");
    if (pidr5 == 0x00 ){
        rumboot_printf("OK \n");
    }
    else {
        rumboot_printf("    BAD \n");
        id_error_count ++;
    }

    rumboot_printf("    PIDR6   - ");
    if (pidr6 == 0x00 ){
        rumboot_printf("OK \n");
    }
    else {
        rumboot_printf("    BAD \n");
        id_error_count ++;
    }

    rumboot_printf("    PIDR7   - ");
    if (pidr7 == 0x00 ){
        rumboot_printf("OK \n");
    }
    else {
        rumboot_printf("    BAD \n");
        id_error_count ++;
    }

    rumboot_printf("    PIDR0   - ");
    if (pidr0 == 0x00 ){
        rumboot_printf("OK \n");
    }
    else {
        rumboot_printf("    BAD \n");
        id_error_count ++;
    }

    rumboot_printf("    PIDR1   - ");
    if (pidr1 == 0xB4 ){
        rumboot_printf("OK \n");
    }
    else {
        rumboot_printf("    BAD \n");
        id_error_count ++;
    }

    rumboot_printf("    PIDR2   - ");
    if (pidr2 == 0x5B ){
        rumboot_printf("OK \n");
    }
    else {
        rumboot_printf("    BAD \n");
        id_error_count ++;
    }

    rumboot_printf("    PIDR3   - ");
    if (pidr3 == 0x00 ){
        rumboot_printf("OK \n");
    }
    else {
        rumboot_printf("    BAD \n");
        id_error_count ++;
    }

    rumboot_printf("    CIDR0   - ");
    if (cidr0 == 0x0D ){
        rumboot_printf("OK \n");
    }
    else {
        rumboot_printf("    BAD \n");
        id_error_count ++;
    }

    rumboot_printf("    CIDR1   - ");
    if (cidr1 == 0xF0 ){
        rumboot_printf("OK \n");
    }
    else {
        rumboot_printf("    BAD \n");
        id_error_count ++;
    }

    rumboot_printf("    CIDR2   - ");
    if (cidr2 == 0x05 ){
        rumboot_printf("OK \n");
    }
    else {
        rumboot_printf("    BAD \n");
        id_error_count ++;
    }

    rumboot_printf("    CIDR3   - ");
    if (cidr3 == 0xB1 ){
        rumboot_printf("OK \n");
    }
    else {
        rumboot_printf("    BAD \n");
        id_error_count ++;
    }

    rumboot_printf("Errors in ID registers check - %d\n",id_error_count);
    if (id_error_count==0){
        rumboot_printf("ID Test PASSED \n");
    }
    else{
        rumboot_printf("ID Test FAILED \n");
    }

    rumboot_printf("Checking PERIPH REGS \n" );


    rumboot_printf("    ARM_S regs  - ");
    if (nic400_check_periph_regs(NIC400_ARM_S)){
        rumboot_printf("OK \n");
    }
    else {
        periph_error_count ++;
        rumboot_printf("    BAD \n");
    }


    rumboot_printf("    DMA0_S regs  - ");
    if (nic400_check_periph_regs(NIC400_DMA0_S)){
        rumboot_printf("OK \n");
    }
    else {
        periph_error_count ++;
        rumboot_printf("    BAD \n");
    }


    rumboot_printf("    DMA1_S regs  - ");
    if (nic400_check_periph_regs(NIC400_DMA1_S)){
        rumboot_printf("OK \n");
    }
    else {
        periph_error_count ++;
        rumboot_printf("    BAD \n");
    }


    rumboot_printf("    DMA2_S regs  - ");
    if (nic400_check_periph_regs(NIC400_DMA2_S)){
        rumboot_printf("OK \n");
    }
    else {
        periph_error_count ++;
        rumboot_printf("    BAD \n");
    }


    rumboot_printf("    DMA3_S regs  - ");
    if (nic400_check_periph_regs(NIC400_DMA3_S)){
        rumboot_printf("OK \n");
    }
    else {
        periph_error_count ++;
        rumboot_printf("    BAD \n");
    }


    rumboot_printf("    ETH0_S regs  - ");
    if (nic400_check_periph_regs(NIC400_ETH0_S)){
        rumboot_printf("OK \n");
    }
    else {
        periph_error_count ++;
        rumboot_printf("    BAD \n");
    }


    rumboot_printf("    ETH1_S regs  - ");
    if (nic400_check_periph_regs(NIC400_ETH1_S)){
        rumboot_printf("OK \n");
    }
    else {
        periph_error_count ++;
        rumboot_printf("    BAD \n");
    }


    rumboot_printf("    ETH2_S regs  - ");
    if (nic400_check_periph_regs(NIC400_ETH2_S)){
        rumboot_printf("OK \n");
    }
    else {
        periph_error_count ++;
        rumboot_printf("    BAD \n");
    }


    rumboot_printf("    ETH3_S regs  - ");
    if (nic400_check_periph_regs(NIC400_ETH3_S)){
        rumboot_printf("OK \n");
    }
    else {
        periph_error_count ++;
        rumboot_printf("    BAD \n");
    }


    rumboot_printf("    ETR_S regs  - ");
    if (nic400_check_periph_regs(NIC400_ETR_S)){
        rumboot_printf("OK \n");
    }
    else {
        periph_error_count ++;
        rumboot_printf("    BAD \n");
    }


    rumboot_printf("Errors in periph registers check - %d\n",periph_error_count);
    if (periph_error_count==0){
        rumboot_printf("PERIPH Test PASSED \n");
    }
    else{
        rumboot_printf("PERIPH Test FAILED \n");
    }

    if ((periph_error_count==0)|(id_error_count==0)){
        rumboot_printf("All Tests PASSED \n");
    }
    else{
        rumboot_printf("Errors in tests \n");
    }
    rumboot_printf("NIC400 test END\n");
    return 0;
}






