#ifndef NIC400_LIB_H
#define NIC400_LIB_H

int nic400_get_pidr4() ;
int nic400_get_pidr5() ;
int nic400_get_pidr6() ;
int nic400_get_pidr7() ;
int nic400_get_pidr0() ;
int nic400_get_pidr1() ;
int nic400_get_pidr2() ;
int nic400_get_pidr3() ;
int nic400_get_cidr0() ;
int nic400_get_cidr1() ;
int nic400_get_cidr2() ;
int nic400_get_cidr3() ;


int nic400_read_reg(int offset);
void nic400_write_reg(int a, int offset);


int nic400_check_rqos  (int interface_block );
int nic400_check_wqos  (int interface_block );
int nic400_check_fn_mod(int interface_block );

void nic400_set_rqos(int a, int interface_block )  ;
void nic400_set_wqos(int a, int interface_block )  ;
void nic400_set_fn_mod(int a, int interface_block );



#endif
