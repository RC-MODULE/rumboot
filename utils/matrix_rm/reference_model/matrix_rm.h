#ifndef MATRIX_RM_H
#define MATRIX_RM_H

#include <stdio.h>
#include <stdint.h>

typedef struct matrix_config {
    int H   ;   // data cube sizes
    int W   ;
    int C   ;
    int Tp  ;   // null padding
    int Bp  ;
    int Lp  ;
    int Rp  ;

    int R   ;   // kernel sizes
    int S   ;
    int Ds  ;   // dilation
    int Dr  ;   
    int Sw  ;   // striding
    int Sh  ;   
    int K   ;   // number of kernels

    int sn  ;   // the number of a sign bit
    int LINES   ;   //<! number of lines in mac buffer
    int COLUMNS ;   //<! number of columns in mac buffer
    int BUF_NMB ;   //<! nubmer of mac buffers

    char *user_X_data_file_name     ;
    char *user_W_data_base_file_name;
    char *user_Y_data_file_name     ;

    char *X_data_file_name          ;
    char *W_data_file_name          ;
    char *Y_data_file_name          ;
} matrix_config_t;

typedef struct hwc_conf {
    int H   ;   // data cube sizes
    int W   ;
    int C   ;
    int Tp  ;   // null padding
    int Bp  ;
    int Lp  ;
    int Rp  ;

    int sn  ;   // 7/15 for 8b/16b data size
    char* dt;   // float/integer

    int Hp      ;
    int Wp      ;
    int HWC     ;
    int HpWpC   ;
} hwc_conf_t;

typedef struct rsc_conf {
    int R   ;   // kernel sizes
    int S   ;   
    int C   ;   
    int Ds  ;   // dilation
    int Dr  ;   
    int Sw  ;   // striding
    int Sh  ;   
    int K   ;   // number of kernels

    int sn  ;   // 7/15 for 8b/16b data size
    char* dt;   // float/integer

    int Rd      ;
    int Sd      ;
    int RSC     ;
    int RdSdC   ;
    int KRdSdC  ;
} rsc_conf_t;

typedef struct hwc_rsc_conf {
    hwc_conf_t* hwc_conf;
    rsc_conf_t* rsc_conf;

    int LINES       ;   //<! number of lines in mac buffer
    int COLUMNS     ;   //<! number of columns in mac buffer
    int BUF_NMB     ;   //<! nubmer of mac buffers

    int Ho          ;   //<! Hout
    int Wo          ;   //<! Wout
    int HoWo        ;   //<! Hout * Wout
    int HoWoK       ;   //<! Hout * Wout * K
    int HoWoRdSdC   ;
} hwc_rsc_conf_t;

/**
* A struct of parameters for mtrx_prnt
*/
typedef struct mtrx_prnt_arg {
    const char* name; //!< name of matrix used for debug print
    size_t      size; //!< sizeof(element of matrix) used for print to bin file
    char*       pf  ; //!< int 32 or long 64 used for debug pirnt
    const void* addr; //!< address where it's located
    int         nr  ; //!< number of rows in matrix
    int         nc  ; //!< number of columns in matrix
    int         ne  ; //!< number of elements in vector

    FILE*       fp  ; //!< bin file pointer where the matrix value should be written
} mtrx_prnt_arg_t;

/**
* A struct of parameters for rshp_values
*/
typedef struct rshp_values_arg {
    void*   src_addr; //!< where take values
    void*   dst_addr; //!< where put reshaped values
    int     nmb     ; //!< number of values to reshape
    char*   fsrc    ; //!< origin value format
    char*   fdst    ; //!< new value format
} rshp_values_arg_t;

/**
* A struct of addresses of matrices used in mac.c
*/
typedef struct matrices_addr {
    int*    HWC_addr        ;
    int*    fRSC_addr       ;
    int*    HpWpC_addr      ;
    int*    fRdSdC_addr     ;
    int*    HoWoRdSdC_addr  ;
    long*   HoWoK_addr      ;

    uint16_t*    X_addr      ;
    uint16_t*    W_addr      ;
    uint64_t*    Y_addr      ;

} matrices_addr_t;

/**
* A struct of parameters for mac_matrix.
*
* The nmb of the last column in data value matrix and in weight matrix is the same value.
* Columns/rows in weight matrix theoretically are rows/columns (in the context of matrix mult).
*/
typedef struct mac_matrix_arg {
    int     nr  ; //<! nmb of the rows in data value matrix
    int     nc  ; //<! nmb of the columns in data value matrix (the same for weight matrix)
    int     nw  ; //<! nmb of the rows in weight matrix
    int*    d_pt; //<! pointer to data buffer (there's part of data matrix HoWoRdSdC)
    int*    w_pt; //<! pointer to weight buffer (there's part of weight matrix f*RdSdC)
    long*   p_pt; //<! pointer to partial sum buffer (there will be partial sum matrix)

    int     LINES   ;   //<! number of lines in mac buffer
    int     COLUMNS ;   //<! number of columns in mac buffer
    int     BUF_NMB ;   //<! nubmer of mac buffers
} mac_matrix_arg_t;

/**
* A struct of parameters for acc
*
* ip, fp are used to determine where should be put new partial sum values.
* Put new part. sum values means to sum up them with the previous values from dst matrix HoWok.
*/
typedef struct acc_arg {
    int             ip      ; //<! ip is the nmb of a row in dst matrix HoWoK
    int             fp      ; //<! fp is the nmb of a column in dst matrix HoWoK
    int             nr      ; //<! nmb of valid rows in partial sum matrix
    int             nc      ; //<! nmb of valid columns in partial sum matrix
    long*           p_pt    ; //<! pointer to partial sum matrix
    long*           dst_addr; //<! address of dst matrix HoWoK
    hwc_rsc_conf_t* conf    ;
} acc_arg_t;

/**
* A struct of parameters for mac
*/
typedef struct mpe_arg {
    int*            d_pt    ; //<! address of matrix HoWoRdSdC
    int*            w_pt    ; //<! address of matrix f*RdSdC
    long*           dst_addr; //<! address of matrix HoWoK
    hwc_rsc_conf_t* conf    ;
} mpe_arg_t;

/**
* Print null pointer error message.
*/
int is_nullptr (const char* fn);

/**
* Print a string to file or to STDOUT
*/
int prnt_strng(FILE* fp, const char* f);

/**
* Matrix debug/bin file print.
*
* Is used to print values of a matrix.
*/
int mtrx_prnt (mtrx_prnt_arg_t* mtrx_prnt_arg);

int check_hwc_rsc_conf (hwc_rsc_conf_t* conf);

int check_matrices_addr (matrices_addr_t* ma);

int check_mac_matrix_arg (mac_matrix_arg_t* mma);

int check_acc_arg (acc_arg_t* aa);

int check_mpe_arg (mpe_arg_t* ma);

int char2hex (char c, unsigned int* hex_s);

int rd_hex_v (FILE* fp, unsigned int* hex_v);

int make_signed_int (unsigned int* hex_v, unsigned int sn);

int HWC_init (char* file_name, int* dst_addr, hwc_conf_t* conf);

int RSC_init (char* fnb, int* dst_addr, rsc_conf_t* conf);

int HWC_padding (int* src_addr, int* dst_addr, hwc_conf_t* conf);

int RSC_dilat (int* src_addr, int* dst_addr, rsc_conf_t* conf);

int HoWoRdSdC_init (int* src_addr, int* dst_addr, hwc_rsc_conf_t* conf);

/**
* Multiplier & ACcumlator Matrix.
*
* Size of multiplier matrix is LINES*BUF_NMB rows with COLUMNS columns.
*/
int mac_matrix (mac_matrix_arg_t* mac_matrix_arg);

/**
* ACCumulator.
*
* Accumulator accumulates new partial sum values with previous results from HoWoK matrix.
*/
int acc (acc_arg_t* acc_arg);

/**
* Matrix Processing Engine. Matrix multiplier & ACcumlator.
*
* Is used to fetch data values from data matrix HoWoRdSdC and to put them to data buffer
* Is used to fetch weights from weight matrix f*RdSdC and to put them to weight buffer
* Is used to call mac_matrix to multiply values from data buffer to values from weight buffer * Is used to call acc to perform partial summation with previous results
*/
int mpe (mpe_arg_t* mpe_arg);

/**
* Get configuration from matrix_config structure.
*/
int hwc_rsc_conf_init (matrix_config_t* mc, hwc_rsc_conf_t* conf);

/**
* Compute and check all necessary parameters for convolution functions.
*/
int config_complete (hwc_rsc_conf_t* conf);

int matrices_addr_calloc (hwc_rsc_conf_t* conf, matrices_addr_t* ma);

/**
* Init hwc data matrix and rsc filter matrices with data and weights values from proper files.
*/
int hwc_rsc_values_init (matrix_config_t* mc, hwc_rsc_conf_t* conf, matrices_addr_t* ma);

int make_convolution (hwc_rsc_conf_t* conf, matrices_addr_t* ma);

void matrices_addr_free (matrices_addr_t* ma);

int xwy_prnt (matrix_config_t* mc, hwc_rsc_conf_t* conf, matrices_addr_t* ma);

/**
* Is used to reshape values from one data type to another.
*/
int rshp_values (rshp_values_arg_t* rv);

int xwy_2binfile (matrix_config_t* mc, hwc_rsc_conf_t* conf, matrices_addr_t* ma);

int make_all_dbg (matrix_config_t* mc, matrices_addr_t* ma_dbg, hwc_rsc_conf_t* conf_dbg);

int make_all (matrix_config_t* mc);

int ReLU(int x);

#endif // MATRIX_RM_H
