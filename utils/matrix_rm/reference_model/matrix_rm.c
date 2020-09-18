#include "matrix_rm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int is_nullptr (const char* fn) {
    printf ("ERROR: %s NULL pointer.\n", fn);
    return 1;
}

int prnt_strng(FILE* fp, const char* f) {
    int res = 0;

    if (fp == NULL) printf (f);
    //else res = fprintf (fp, f);
    else fprintf (fp, f);

    //if (res) printf ("%s.", __func__);
    return res;
}

int mtrx_prnt (mtrx_prnt_arg_t* mtrx_prnt_arg) {
    int res = 0;

    int i,j,k;
    char vt;

    const char* f;

    const char* name= mtrx_prnt_arg->name   ;
    size_t      size= mtrx_prnt_arg->size   ;
    const void* addr= mtrx_prnt_arg->addr   ;
    int         nr  = mtrx_prnt_arg->nr     ;
    int         nc  = mtrx_prnt_arg->nc     ;
    int         ne  = mtrx_prnt_arg->ne     ;
    char*       pf  = mtrx_prnt_arg->pf     ;

    FILE*       fp  = mtrx_prnt_arg->fp     ;

    if      (pf=="%016lx" || pf=="%lx" || pf=="%ld")    vt = 64;
    else if (pf=="%08x" || pf=="%x" || pf=="%d")        vt = 32;
    else {
        vt  = 32;
        pf  = "%x";
    }

    if (fp == NULL || fp && name == "dbg") {
        
        if(fp == NULL && strlen(name) > 0) printf ("\n%s\n", name);
    
        for (i=0; i<nr; i++) {
            for (j=0; j<nc; j++) {
                for (k=0; k<ne; k++) {
                    if (vt == 64) {
                        if (fp == NULL) printf(pf, *((long*)addr + i*nc*ne + j*ne + k));
                        else fprintf(fp, pf, *((long*)addr + i*nc*ne + j*ne + k));
                    }
                    else {
                        if (fp == NULL) printf(pf, *((int*)addr + i*nc*ne + j*ne + k));
                        else fprintf(fp, pf, *((int*)addr + i*nc*ne + j*ne + k));
                    }

                    if(k+1 == ne) {
                        f = ",";
                        res = prnt_strng (fp, f);
                    }
                    else {
                        f = " ";
                        res = prnt_strng (fp, f);
                    }
                }
                if (j+1 == nc) {
                    f = "\n";
                    res = prnt_strng(fp, f);
                }
                else {
                    f = " ";
                    res = prnt_strng (fp, f);
                }
            }
        }
    }
    else if (fp && name == "bin") res = !(nr*nc*ne == fwrite (addr, size, nr*nc*ne, fp));
    else res = 1;

    if (res) printf ("%s.", __func__);
    return res;
}

int check_matrices_addr (matrices_addr_t* ma) {
    int res = 0;

    if (ma==NULL || ma->HWC_addr==NULL || ma->HpWpC_addr==NULL || ma->HoWoK_addr==NULL || ma->fRSC_addr==NULL || ma->fRdSdC_addr==NULL || ma->HoWoRdSdC_addr==NULL) res = is_nullptr(__func__);

    return res;
}

int check_hwc_rsc_conf (hwc_rsc_conf_t* conf) {
    int res = 0;

    if (conf==NULL || conf->hwc_conf==NULL || conf->rsc_conf==NULL) res = is_nullptr(__func__);

    return res;
}

int check_mac_matrix_arg (mac_matrix_arg_t* mma) {
    int res = 0;

    if (mma==NULL || mma->d_pt==NULL || mma->w_pt==NULL || mma->p_pt==NULL) res = is_nullptr(__func__);

    return res;
}

int check_acc_arg (acc_arg_t* aa) {
    int res = 0;

    if (aa==NULL || aa->p_pt==NULL || aa->dst_addr==NULL || check_hwc_rsc_conf(aa->conf)) res = is_nullptr(__func__);

    return res;
}

int check_mpe_arg (mpe_arg_t* ma) {
    int res = 0;

    if (ma==NULL || ma->d_pt==NULL || ma->w_pt==NULL || ma->dst_addr==NULL || check_hwc_rsc_conf(ma->conf)) res = is_nullptr(__func__);

    return res;
}

int char2hex (char c, unsigned int* hex_s) {
    int res = 0;

    switch (c) {
        case '0':
            *hex_s = 0;
            break;
        
        case '1':
            *hex_s = 1;
            break;

        case '2':
            *hex_s = 2;
            break;

        case '3':
            *hex_s = 3;
            break;
        
        case '4':
            *hex_s = 4;
            break;
        
        case '5':
            *hex_s = 5;
            break;
        
        case '6':
            *hex_s = 6;
            break;
        
        case '7':
            *hex_s = 7;
            break;
        
        case '8':
            *hex_s = 8;
            break;
        
        case '9':
            *hex_s = 9;
            break;
        
        case 'a':
        case 'A':
            *hex_s = 10;
            break;
        
        case 'b':
        case 'B':
            *hex_s = 11;
            break;
        
        case 'c':
        case 'C':
            *hex_s = 12;
            break;
        
        case 'd':
        case 'D':
            *hex_s = 13;
            break;
        
        case 'e':
        case 'E':
            *hex_s = 14;
            break;
        
        case 'f':
        case 'F':
            *hex_s = 15;
            break;

        default:
            printf ("ERROR:char2hex Invalid character!\n");
            res = 1;
    }

    if (res) printf ("%s.", __func__);
    return res;
}

int rd_hex_v (FILE* fp, unsigned int* hex_v) {
    int res = 0;

    char c;
    char buf[8];

    int i, cn, is_hex_s, get_nxt_s;

    unsigned int hex_s;

    cn = 0;
    get_nxt_s = 1;
    do {
        c = fgetc (fp);

        //printf ("c: %d\n", c);

        is_hex_s =  (c >= 'a') && (c <= 'f') ||
                    (c >= 'A') && (c <= 'F') ||
                    (c >= '0') && (c <= '9');

        if (is_hex_s) {
            buf[cn] = c;
            cn++;
        }
        else if ((c == 'x') || (c == 'X')) {        // continue
            cn = 0;
            get_nxt_s = 1;
        }
        else if (!is_hex_s && !cn) get_nxt_s = 1;   // continue
        else if (!is_hex_s && cn) get_nxt_s = 0;    // break

    } while ((cn < 9) && (is_hex_s || get_nxt_s));

    if ((cn == 0) || (cn > 8)) {
        printf ("ERROR:rd_hex_v Data value isn't hex or it is too big.\n");
        res = 1;
    }
    else {
        for (*hex_v = 0x0, i = 0; i < cn; i++) {
            res = char2hex(buf[cn - 1 - i], &hex_s);

            if (!res) *hex_v += (hex_s << (i << 2));    // += (hex_s * 16 * i);
            else i = cn;                                // break with ERROR
        }
    }

    if (res) printf ("%s.", __func__);
    return res;
}

int make_signed_int (unsigned int* hex_v, unsigned int sn) {
    int res;

    res = (sn != 7) && (sn != 15);

    if(!res) *hex_v = (*hex_v >> sn) ? ((unsigned int)(-1) << sn+1) | *hex_v : *hex_v;
    else printf ("ERROR:make_signed_int Unsupported data size type.\n");

    if (res) printf ("%s.", __func__);
    return res;
}

int HWC_init (char* file_name, int* dst_addr, hwc_conf_t* conf) {
    int res = 0;

    int H, W, C, sn;
    int i, j, k;

    unsigned int hex_v;

    FILE* fp = fopen(file_name, "r");;

    if (!fp) {
        printf ("ERROR:HWC_init File %s opening failed.\n", file_name);
        res = 1;
    }

    if (dst_addr==NULL || conf==NULL) res = is_nullptr(__func__);

    if (!res) {
        H   = conf->H   ;
        W   = conf->W   ;
        C   = conf->C   ;
        sn  = conf->sn  ;

        for (i=0; i<H; i++) {
            for (j=0; j<W; j++) {
                for (k=0; k<C; k++) {

                    res = rd_hex_v(fp, &hex_v);

                    res = make_signed_int(&hex_v, sn);

                    *(dst_addr + i*W*C + j*C + k) = (int)hex_v;
                }
            }
        }

        fclose(fp);
    }

    if (res) printf ("%s.", __func__);
    return res;
}

int RSC_init (char* fnb, int* dst_addr, rsc_conf_t* conf) {
    int res = 0;

    hwc_conf_t hwc4rsc;
    char str[6];    // serves for K < 100,000
    char fn[128];   // up2 122 chars for fnb + up2 5 chars for K + \0

    int R, S, C, K, RSC, sn, i;

    if (dst_addr==NULL || conf==NULL || fnb==NULL) res = is_nullptr(__func__);

    if (strlen(fnb) > 122) {
        printf ("ERROR:RSC_init fnb should be less than 122 chars.\n");
        res = 1;
    }

    if (!res) {
        R   = conf->R   ;
        S   = conf->S   ;
        C   = conf->C   ;
        K   = conf->K   ;

        RSC = conf->RSC ;

        sn  = conf->sn  ;

        hwc4rsc.H   = R ;
        hwc4rsc.W   = S ;
        hwc4rsc.C   = C ;
        hwc4rsc.sn  = sn;
    }

    for (i=0; i<K && !res; i++) {
        fn[0] = '\0';

        sprintf (str, "%d", i);
        strcat(&fn[0], fnb);
        strcat(&fn[0], &str[0]);

        //printf ("%d %s\n", i, fn);

        res = HWC_init(&fn[0], dst_addr+i*RSC, &hwc4rsc);
    }

    if (res) printf ("%s.", __func__);
    return res;
}

int HWC_padding (int* src_addr, int* dst_addr, hwc_conf_t* conf) {
    int res = 0;

    int H, W, C, Hp, Wp;
    int Tp, Bp, Lp, Rp;

    int i, j, k;

    int np_v;   // null padding or value from src_addr

    if (src_addr==NULL || dst_addr==NULL || conf==NULL) res = is_nullptr(__func__);

    if (!res) {
        H   = conf->H   ;
        W   = conf->W   ;
        C   = conf->C   ;

        Tp  = conf->Tp  ;
        Bp  = conf->Bp  ;
        Lp  = conf->Lp  ;
        Rp  = conf->Rp  ;

        Hp  = conf->Hp  ;
        Wp  = conf->Wp  ;

        for (i=0; i<Hp; i++) {
            for (j=0; j<Wp; j++) {
                for (k=0; k<C; k++) {
                    np_v =  (i < Tp) || (i > H+Tp-1) ||
                            (j < Lp) || (j > W+Lp-1) ? 0x0 :

                            *(src_addr + (i-Tp)*W*C + (j-Lp)*C + k);

                    *(dst_addr + i*Wp*C + j*C + k) = np_v;
                }
            }
        }
    }

    if (res) printf ("%s.", __func__);
    return res;
}

int RSC_dilat (int* src_addr, int* dst_addr, rsc_conf_t* conf) {
    int res = 0;

    int Rd, Sd, C, K, Ds, Dr;
    int id, jd, k, f, dx, dy;
    int S, RSC, RdSdC;
    int i, j;

    int nd_v;   // null dilation or value from src_addr

    if (src_addr==NULL || dst_addr==NULL || conf==NULL) res = is_nullptr(__func__);

    if (!res) {
        S       = conf->S   ;
        C       = conf->C   ;
        K       = conf->K   ;
        Ds      = conf->Ds  ;
        Dr      = conf->Dr  ;

        Rd      = conf->Rd   ;   // kernel sizes after dilation
        Sd      = conf->Sd   ;

        RSC     = conf->RSC     ;
        RdSdC   = conf->RdSdC   ;

        for (f=0; f<K; f++) {
            for (id=0, i=0, dy=Dr-1; id<Rd; id++) {
                dy++;

                for (jd=0, j=0, dx=Ds-1; jd<Sd; jd++) {
                    if (dy == Dr) dx++;

                    for (k=0; k<C; k++) {

                        nd_v =  (dx == Ds) && (dy == Dr) ?
                                *(src_addr + f*RSC + i*S*C + j*C + k) : 0x0;

                        *(dst_addr + f*RdSdC + id*Sd*C + jd*C + k) = nd_v;
                    }

                    if (dx == Ds) {
                        j++;
                        if (jd+1 < Sd) dx = 0x0;
                    }
                }

                if (dy == Dr) {
                    i++;
                    if (id+1 < Rd) dy = 0x0;
                }
            }
        }
    }

    if (res) printf ("%s.", __func__);
    return res;
}

int HoWoRdSdC_init (int* src_addr, int* dst_addr, hwc_rsc_conf_t* conf) {
    int res = 0;

    int i , j , k;
    int ir, js;
    int ih, jw;

    int C, Hp, Wp, Rd, Sd, RdSdC, Sw, Sh;

    int dst_offset, src_offset;

    if (src_addr==NULL || dst_addr==NULL || check_hwc_rsc_conf(conf)) res = is_nullptr(__func__);

    if (!res) {
        C       = conf->hwc_conf->C ;
        Hp      = conf->hwc_conf->Hp;   // H, W after null padding
        Wp      = conf->hwc_conf->Wp;

        Sw      = conf->rsc_conf->Sw;
        Sh      = conf->rsc_conf->Sh;

        Rd      = conf->rsc_conf->Rd ;  // kernel sizes after dilation
        Sd      = conf->rsc_conf->Sd ;

        RdSdC   = conf->rsc_conf->RdSdC;

        for (i=0, j=0, ih=0; ih<Hp-Rd+1;) {
            for (jw=0; jw<Wp-Sd+1;) {

                for (ir=0; ir<Rd; ir++) {
                    for (js=0; js<Sd; js++) {

                        for (k=0; k<C; k++, j++) {

                            dst_offset = i*RdSdC + j;
                            src_offset = (ih + ir)*Wp*C + (jw + js)*C + k;

                            *(dst_addr + dst_offset) = *(src_addr + src_offset);
                        }
                    }
                }

                jw = jw + Sw;

                j = 0;
                i++;
            }

            ih = ih + Sh;
        }
    }

    if (res) printf ("%s.", __func__);
    return res;
}

int mac_matrix (mac_matrix_arg_t* mac_matrix_arg) {
    int res = 0;
    unsigned int dbg = 0;

    int     nr  ;
    int     nc  ;
    int     nw  ;
    int*    d_pt;
    int*    w_pt;
    long*   p_pt;

    int     LINES   ;   //<! number of lines in mac buffer
    int     COLUMNS ;   //<! number of columns in mac buffer
    int     BUF_NMB ;   //<! nubmer of mac buffers

    int i , j;  // to count rows/columns in data matrix
    int iw;     // to count rows(which really are columns) in weight matrix

    long dmw;   // to compute partial sum value: multiply data row to weight column(row)

    if (check_mac_matrix_arg(mac_matrix_arg)) res = is_nullptr(__func__);

    if (!res) {
        nr  = mac_matrix_arg->nr    ;
        nc  = mac_matrix_arg->nc    ;
        nw  = mac_matrix_arg->nw    ;
        d_pt= mac_matrix_arg->d_pt  ;
        w_pt= mac_matrix_arg->w_pt  ;
        p_pt= mac_matrix_arg->p_pt  ;

        LINES   = mac_matrix_arg->LINES     ;
        COLUMNS = mac_matrix_arg->COLUMNS   ;
        BUF_NMB = mac_matrix_arg->BUF_NMB   ;

        if (nr > LINES*BUF_NMB || nc > COLUMNS) {
            printf ("ERROR:mac_matrix nr or nc is incorrect.\n");
            res = 1;
        }
    }

    if (!res) {
        //printf ("\nnr %d, nw %d, nc %d\n", nr, nw, nc);

        for (i=0; i<nr; i++) {
            for (iw=0; iw<nw; iw++) {
                for (dmw=0, j=0; j<nc; j++) {

                    dmw += (long)*(d_pt + i*COLUMNS + j) * (long)*(w_pt + iw*COLUMNS + j);
                }

                *(p_pt + i*LINES*BUF_NMB + iw) = dmw;
            }
        }
    }

    if (res) printf ("%s.", __func__);
    return res;
}

int acc (acc_arg_t* acc_arg) {
    int res = 0;
    unsigned int dbg = 0;

    int             ip      ;
    int             fp      ;
    int             nr      ;
    int             nc      ;
    long*           p_pt    ;
    long*           dst_addr;
    hwc_rsc_conf_t* conf    ;

    int K, HoWo;

    int LINES   ;   //<! number of lines in mac buffer
    int BUF_NMB ;   //<! nubmer of mac buffers

    int i , j ; // to count rows/columns in partial sum matrix
    int id, jd; // to count rows/columns in destination matrix (HoWoK)

    if (check_acc_arg(acc_arg)) res = is_nullptr(__func__);

    if (!res) {
        ip      =acc_arg->ip        ;
        fp      =acc_arg->fp        ;
        nr      =acc_arg->nr        ;
        nc      =acc_arg->nc        ;
        p_pt    =acc_arg->p_pt      ;
        dst_addr=acc_arg->dst_addr  ;
        conf    =acc_arg->conf      ;

        K       = conf->rsc_conf->K ;
        HoWo    = conf->HoWo        ;

        LINES   = conf->LINES       ;
        BUF_NMB = conf->BUF_NMB     ;

        if (nr+ip > HoWo || nc+fp > K) {
            printf ("ERROR:acc nr or nc or or ip or fp is incorrect.\n");
            res = 1;
        }
    }

    if (!res) {
        //printf ("\nnr %d, nc %d\n", nr, nc);

        for (i=0; i<nr; i++) {
            for (j=0; j<nc; j++) {

                id = ip + i;
                jd = fp + j;

                *(dst_addr + id*K + jd) += *(p_pt + i*LINES*BUF_NMB + j);
            }
        }
    }

    if (res) printf ("%s.", __func__);
    return res;
}

int mpe (mpe_arg_t* mpe_arg) {
    int res = 0;
    unsigned int dbg = 0;

    mtrx_prnt_arg_t mtrx_prnt_arg;

    int*            d_pt    ;
    int*            w_pt    ;
    long*           dst_addr;
    hwc_rsc_conf_t* conf    ;

    int K, RdSdC, HoWo;

    int LINES   ;   //<! number of lines in mac buffer
    int COLUMNS ;   //<! number of columns in mac buffer
    int BUF_NMB ;   //<! nubmer of mac buffers

    int i , j ; // to count rows/columns of data matrix HoWoRdSdC
    int ib, jb; // to count rows/columns of matrix in data buffer
    int fb, f ; // to count columns(rows) of matrix in weight buffer / of weight matrix f*RdSdC

    mac_matrix_arg_t mac_matrix_arg  ;
    acc_arg_t        acc_arg         ;

    // the nmb of row, column in matrix HoWoK to determine where should be put patial sums
    int ip, fp;

    int*    d_buf = NULL;   // for data buffer matrix
    int*    w_buf = NULL;   // for weight buffer matrix
    long*   p_buf = NULL;   // for partial sums

    if (check_mpe_arg(mpe_arg)) res = is_nullptr(__func__);

    if (!res) {
        d_pt    = mpe_arg->d_pt         ;
        w_pt    = mpe_arg->w_pt         ;
        dst_addr= mpe_arg->dst_addr     ;
        conf    = mpe_arg->conf         ;

        K       = conf->rsc_conf->K     ;
        RdSdC   = conf->rsc_conf->RdSdC ;
        HoWo    = conf->HoWo            ;

        LINES   = conf->LINES           ;
        COLUMNS = conf->COLUMNS         ;
        BUF_NMB = conf->BUF_NMB         ;

        d_buf = calloc(LINES*COLUMNS*BUF_NMB, sizeof(int));
        w_buf = calloc(LINES*COLUMNS*BUF_NMB, sizeof(int));
        p_buf = calloc((LINES*BUF_NMB)*(LINES*BUF_NMB), sizeof(long));

        if (d_buf==NULL || w_buf==NULL || p_buf==NULL) res = is_nullptr(__func__);
    }

    for (f=0, j=0; f<K && !res; ) {
        for (i=0; i<HoWo && !res; ) {

            ip = i;
            fp = f;

            for (ib=0, fb=0; i<HoWo && ib<LINES*BUF_NMB || f<K && fb<LINES*BUF_NMB;) {
                for (jb=0; jb<COLUMNS && j<RdSdC; jb++, j++) {

                    if (i<HoWo && ib<LINES*BUF_NMB) {
                        *(d_buf + ib*COLUMNS + jb) = *(d_pt + i*RdSdC + j);
                    }

                    if (f<K && fb<LINES*BUF_NMB) {
                        *(w_buf + fb*COLUMNS + jb) = *(w_pt + f*RdSdC + j);
                    }
                }

                if (i<HoWo && ib<LINES*BUF_NMB) {
                    i++;
                    ib++;
                }
                if (f<K && fb<LINES*BUF_NMB) {
                    f++;
                    fb++;
                }

                if (i<HoWo && ib<LINES*BUF_NMB || f<K && fb<LINES*BUF_NMB) j = j - jb;
            }

            mac_matrix_arg.nr       = ib        ;
            mac_matrix_arg.nc       = jb        ;
            mac_matrix_arg.nw       = fb        ;
            mac_matrix_arg.d_pt     = d_buf     ;
            mac_matrix_arg.w_pt     = w_buf     ;
            mac_matrix_arg.p_pt     = p_buf     ;
            mac_matrix_arg.LINES    = LINES     ;
            mac_matrix_arg.COLUMNS  = COLUMNS   ;
            mac_matrix_arg.BUF_NMB  = BUF_NMB   ;

            res = mac_matrix (&mac_matrix_arg);

            acc_arg.ip          = ip        ;
            acc_arg.fp          = fp        ;
            acc_arg.nr          = ib        ;
            acc_arg.nc          = fb        ;
            acc_arg.p_pt        = p_buf     ;
            acc_arg.dst_addr    = dst_addr  ;
            acc_arg.conf        = conf      ;

            res = acc (&acc_arg);

            // if the last processed data wasn't the last data in HoWoRdSdC
            // else f has already pointed to the next unprocessed filter
            if (i<HoWo || j<RdSdC) f = f - fb;

            // if the end of the row move j to the beginnig of the next row
            // else move i to the row of the next buffer interation
            if (j == RdSdC) j = 0;      // i has already pointed to the next row
            else            i = i - ib; // j has already pointed to the next column
        }
    }

    if (d_buf) free(d_buf);
    if (w_buf) free(w_buf);
    if (p_buf) free(p_buf);

    if (res) printf ("%s.", __func__);
    return res;
}

int hwc_rsc_conf_init (matrix_config_t* mc, hwc_rsc_conf_t* conf) {
    int res = 0;

    if (mc==NULL || check_hwc_rsc_conf(conf)) res = is_nullptr(__func__);

    if (!res) {
        conf->hwc_conf->H   = mc->H ;
        conf->hwc_conf->W   = mc->W ;
        conf->hwc_conf->C   = mc->C ;
        conf->hwc_conf->Tp  = mc->Tp;
        conf->hwc_conf->Bp  = mc->Bp;
        conf->hwc_conf->Lp  = mc->Lp;
        conf->hwc_conf->Rp  = mc->Rp;

        conf->hwc_conf->sn  = mc->sn;

        conf->rsc_conf->R   = mc->R ;
        conf->rsc_conf->S   = mc->S ;
        conf->rsc_conf->C   = mc->C ;
        conf->rsc_conf->Ds  = mc->Ds;
        conf->rsc_conf->Dr  = mc->Dr;
        conf->rsc_conf->Sw  = mc->Sw;
        conf->rsc_conf->Sh  = mc->Sh;
        conf->rsc_conf->K   = mc->K ;
        conf->rsc_conf->sn  = mc->sn;

        conf->LINES         = mc->LINES     ;
        conf->COLUMNS       = mc->COLUMNS   ;
        conf->BUF_NMB       = mc->BUF_NMB   ;
    }

    if (res) printf ("%s.", __func__);
    return res;
}

int config_complete (hwc_rsc_conf_t* conf) {
    int res = 0;

    int H, W, C, Tp, Bp, Lp, Rp, Hp, Wp, sn;
    int R, S, K, Ds, Dr, Rd, Sd;

    int Sw, Sh, Ho, Wo;

    int HWC, HpWpC, RSC, RdSdC, HoWo, HoWoK, HoWoRdSdC, KRdSdC; 

    double Ho_db, Wo_db;

    if (check_hwc_rsc_conf(conf)) res = is_nullptr(__func__);

    if (!res) {
        H   = conf->hwc_conf->H ;
        W   = conf->hwc_conf->W ;
        C   = conf->hwc_conf->C ;
        Tp  = conf->hwc_conf->Tp;
        Bp  = conf->hwc_conf->Bp;
        Lp  = conf->hwc_conf->Lp;
        Rp  = conf->hwc_conf->Rp;

        sn  = conf->hwc_conf->sn;

        R   = conf->rsc_conf->R ;
        S   = conf->rsc_conf->S ;
        K   = conf->rsc_conf->K ;
        Ds  = conf->rsc_conf->Ds;
        Dr  = conf->rsc_conf->Dr;
        Sw  = conf->rsc_conf->Sw;
        Sh  = conf->rsc_conf->Sh;

        if (sizeof(long) < 2*(int)sizeof(int)) {
            printf ("ERROR:sizeof(long) is too small.\n");
            res = 1;
        }

        if (conf->hwc_conf->C != conf->rsc_conf->C ||
            conf->hwc_conf->sn != conf->rsc_conf->sn) {
            printf ("ERROR:Dimensions or sizes of HWC and RSC are not equal.\n");
            res = 1;
        }

        if ((H<1) || (W<1) || (C<1) || (R<1) || (S<1) || (K<1) || (Ds<1) || (Dr<1) || (Sw<1) || (Sh<1) || sn<1) {
            printf ("ERROR:H, W, C, R, S, K, Ds, Dr, Sw, Sh, sn should be > 1.\n");
            res = 1;
        }

        if ((unsigned int)(-1) & 0xFFFFFFFF != 0xFFFFFFFF) {
            printf("ERROR: -1 is not b1...1.\n");
            res = 1;
        }
    }

    if (!res) {
        Hp  = H+Tp+Bp;          // H, W after null padding
        Wp  = W+Lp+Rp;

        Rd  = (R-1)*Dr + 1;     // kernel sizes after dilation
        Sd  = (S-1)*Ds + 1;

        Ho  = (Hp-Rd)/Sh + 1;   // Hout, Wout with respect to striding, dilation, null padding
        Wo  = (Wp-Sd)/Sw + 1;

        Ho_db = ((double)Hp - (double)Rd) / (double)Sh + 1;
        Wo_db = ((double)Wp - (double)Sd) / (double)Sw + 1;

        if (((double)Ho != Ho_db) || ((double)Wo != Wo_db)) {
            printf ("ERROR:Hout or Wout is not integer.\n");
            res = 1;
        }

        if ((Ho < 1) || (Wo < 1)) {
            printf ("ERROR:Hout or Wout are < 1 (Rd or Sd are too big).\n");
            res = 1;
        }

        if ((Tp > Rd) || (Bp > Rd) || (Lp > Sd) || (Rp > Sd)) {
            printf ("ERROR:Rd or Sd are too small.\n");
            res = 1;
        }
    }

    if (!res) {
        HWC         = H  * W  * C           ;
        HpWpC       = Hp * Wp * C           ;

        RSC         = R  * S  * C           ;
        RdSdC       = Rd * Sd * C           ;
        RdSdC       = Rd * Sd * C           ;
        KRdSdC      = Rd * Sd * C * K       ;
        KRdSdC      = Rd * Sd * C * K       ;

        HoWo        = Ho * Wo               ;
        HoWoK       = Ho * Wo * K           ;
        HoWoRdSdC   = Ho * Wo * Rd * Sd * C ;

        conf->hwc_conf->Hp      = Hp        ;
        conf->hwc_conf->Wp      = Wp        ;
        conf->hwc_conf->HWC     = HWC       ;
        conf->hwc_conf->HpWpC   = HpWpC     ;

        conf->rsc_conf->Rd      = Rd        ;
        conf->rsc_conf->Sd      = Sd        ;
        conf->rsc_conf->RSC     = RSC       ;
        conf->rsc_conf->RdSdC   = RdSdC     ;
        conf->rsc_conf->KRdSdC  = KRdSdC    ;

        conf->Ho                = Ho        ;
        conf->Wo                = Wo        ;
        conf->HoWo              = HoWo      ;
        conf->HoWoK             = HoWoK     ;
        conf->HoWoRdSdC         = HoWoRdSdC ;

        //printf("K %d, HoWo %d, RdSdC %d\n", K, HoWo, RdSdC);
    }

    if (res) printf ("%s.", __func__);
    return res;
}

int matrices_addr_calloc (hwc_rsc_conf_t* conf, matrices_addr_t* ma) {
    int res = 0;

    int HWC         ;
    int K           ;
    int RSC         ;
    int HpWpC       ;
    int RdSdC       ;
    int HoWoRdSdC   ;
    int HoWoK       ;

    if (check_hwc_rsc_conf(conf) || ma==NULL) res = is_nullptr(__func__);

    if (!res) {
        HWC         = conf->hwc_conf->HWC   ;      
        HpWpC       = conf->hwc_conf->HpWpC ;
        K           = conf->rsc_conf->K     ;
        RSC         = conf->rsc_conf->RSC   ;
        RdSdC       = conf->rsc_conf->RdSdC ;
        HoWoRdSdC   = conf->HoWoRdSdC       ;
        HoWoK       = conf->HoWoK           ;

        ma->HWC_addr        = calloc(HWC        ,   sizeof(int) );
        ma->fRSC_addr       = calloc(K*RSC      ,   sizeof(int) );
        ma->HpWpC_addr      = calloc(HpWpC      ,   sizeof(int) );
        ma->fRdSdC_addr     = calloc(K*RdSdC    ,   sizeof(int) );
        ma->HoWoRdSdC_addr  = calloc(HoWoRdSdC  ,   sizeof(int) );
        ma->HoWoK_addr      = calloc(HoWoK      ,   sizeof(long));

        res = check_matrices_addr(ma);
    }

    if (res) printf ("%s.", __func__);
    return res;
}

int hwc_rsc_values_init (matrix_config_t* mc, hwc_rsc_conf_t* conf, matrices_addr_t* ma) {
    int res = 0;

    int*    HWC_addr    ;
    int*    fRSC_addr   ;

    char* hwc   = "hwc" ;
    char* rsc_  = "rsc_";

    if (mc==NULL || check_hwc_rsc_conf(conf) || check_matrices_addr(ma)) res = is_nullptr(__func__);

    if (!res) {
        HWC_addr    = ma->HWC_addr  ;
        fRSC_addr   = ma->fRSC_addr ;

        hwc = mc->user_X_data_file_name     ;
        rsc_= mc->user_W_data_base_file_name;

        res = HWC_init (hwc, HWC_addr, conf->hwc_conf);

        if (!res) res = RSC_init (rsc_, fRSC_addr, conf->rsc_conf);
    }

    if (res) printf ("%s.", __func__);
    return res;
}

int make_convolution (hwc_rsc_conf_t* conf, matrices_addr_t* ma) {
    int res = 0;

    mpe_arg_t   mpe_arg         ;

    int*        HWC_addr        ;
    int*        fRSC_addr       ;
    int*        HpWpC_addr      ;
    int*        fRdSdC_addr     ;
    int*        HoWoRdSdC_addr  ;
    long*       HoWoK_addr      ;

    if (check_hwc_rsc_conf(conf) || check_matrices_addr(ma)) res = is_nullptr(__func__);

    if (!res) {
        HWC_addr        = ma->HWC_addr      ;
        fRSC_addr       = ma->fRSC_addr     ;
        HpWpC_addr      = ma->HpWpC_addr    ;
        fRdSdC_addr     = ma->fRdSdC_addr   ;
        HoWoRdSdC_addr  = ma->HoWoRdSdC_addr;
        HoWoK_addr      = ma->HoWoK_addr    ;
    }

    if (!res) res = HWC_padding (HWC_addr, HpWpC_addr, conf->hwc_conf);

    if (!res) res = RSC_dilat (fRSC_addr, fRdSdC_addr, conf->rsc_conf);

    if (!res) res = HoWoRdSdC_init (HpWpC_addr, HoWoRdSdC_addr, conf);

    if (!res) {
        mpe_arg.d_pt    = HoWoRdSdC_addr;
        mpe_arg.w_pt    = fRdSdC_addr   ;
        mpe_arg.dst_addr= HoWoK_addr    ;
        mpe_arg.conf    = conf          ;

        res = mpe (&mpe_arg);
    }

    if (res) printf ("%s.", __func__);
    return res;
}

int xwy_prnt (matrix_config_t* mc, hwc_rsc_conf_t* conf, matrices_addr_t* ma) {
    int res = 0;

    int*    HoWoRdSdC_addr  ;
    int*    fRdSdC_addr     ;
    long*   HoWoK_addr      ;

    int K, Rd, Sd, C, Ho, Wo;

    FILE* fp;

    mtrx_prnt_arg_t mtrx_prnt_arg;

    if (check_hwc_rsc_conf(conf) || check_matrices_addr(ma)) res = is_nullptr(__func__);

    if (!res) {
        HoWoRdSdC_addr  = ma->HoWoRdSdC_addr;
        fRdSdC_addr     = ma->fRdSdC_addr   ;
        HoWoK_addr      = ma->HoWoK_addr    ;

        K               = conf->rsc_conf->K ;
        Rd              = conf->rsc_conf->Rd;
        Sd              = conf->rsc_conf->Sd;
        C               = conf->rsc_conf->C ;
        Ho              = conf->Ho          ;
        Wo              = conf->Wo          ;

        mtrx_prnt_arg.name  = "KRdSdC";
        mtrx_prnt_arg.pf    = "%d";
        mtrx_prnt_arg.addr  = (void*)fRdSdC_addr;
        mtrx_prnt_arg.nr    = K;
        mtrx_prnt_arg.nc    = Rd*Sd*C;
        mtrx_prnt_arg.ne    = 1;
        mtrx_prnt_arg.fp    = NULL;

        //mtrx_prnt (&mtrx_prnt_arg);

        mtrx_prnt_arg.name  = "HoWoRdSdC";
        mtrx_prnt_arg.addr  = (void*)HoWoRdSdC_addr;
        mtrx_prnt_arg.nr    = Ho*Wo;
        mtrx_prnt_arg.nc    = Rd*Sd*C;

        //mtrx_prnt (&mtrx_prnt_arg);

        mtrx_prnt_arg.name  = "HoWoK";
        mtrx_prnt_arg.pf    = "%ld";
        mtrx_prnt_arg.addr  = (void*)HoWoK_addr;
        mtrx_prnt_arg.nc    = K;

        //mtrx_prnt (&mtrx_prnt_arg);

        fp = fopen(mc->user_Y_data_file_name, "w");

        mtrx_prnt_arg.name  = "dbg";
        mtrx_prnt_arg.pf    = "%ld";
        mtrx_prnt_arg.addr  = (const void*)HoWoK_addr;
        mtrx_prnt_arg.size  = sizeof(long);
        mtrx_prnt_arg.fp    = fp;

        mtrx_prnt (&mtrx_prnt_arg);

        fclose(fp);
    }

    if (res) printf ("%s.", __func__);
    return res;
}

int xwy_2binfile (matrix_config_t* mc, hwc_rsc_conf_t* conf, matrices_addr_t* ma) {
    int res = 0;

    char*   HoWoRdSdC_fn    ;
    char*   KRdSdC_fn       ;
    char*   HoWoK_fn        ;

    int*    HoWoRdSdC_addr  ;
    int*    fRdSdC_addr     ;
    long*   HoWoK_addr      ;

    int KRdSdC, HoWoRdSdC, HoWoK;

    FILE* fpx;
    FILE* fpw;
    FILE* fpy;

    mtrx_prnt_arg_t mtrx_prnt_arg;

    if (mc==NULL || check_hwc_rsc_conf(conf) || check_matrices_addr(ma)) res = is_nullptr(__func__);

    if (!res) {
        HoWoRdSdC_fn    = mc->X_data_file_name  ;
        KRdSdC_fn       = mc->W_data_file_name  ;
        HoWoK_fn        = mc->Y_data_file_name  ;

        HoWoRdSdC_addr  = ma->HoWoRdSdC_addr    ;
        fRdSdC_addr     = ma->fRdSdC_addr       ;
        HoWoK_addr      = ma->HoWoK_addr        ;

        KRdSdC          = conf->rsc_conf->KRdSdC;
        HoWoRdSdC       = conf->HoWoRdSdC       ;
        HoWoK           = conf->HoWoK           ;

        fpx = fopen(HoWoRdSdC_fn, "wb");
        fpw = fopen(KRdSdC_fn   , "wb");
        fpy = fopen(HoWoK_fn    , "wb");

        mtrx_prnt_arg.name  = "bin";
        mtrx_prnt_arg.addr  = (const void*)HoWoRdSdC_addr;
        mtrx_prnt_arg.size  = sizeof(int);
        mtrx_prnt_arg.nr    = 1;
        mtrx_prnt_arg.nc    = HoWoRdSdC;
        mtrx_prnt_arg.ne    = 1;
        mtrx_prnt_arg.fp    = fpx;

        mtrx_prnt (&mtrx_prnt_arg);

        // print W matrix (KRdSdC)
        mtrx_prnt_arg.addr  = (const void*)fRdSdC_addr;
        mtrx_prnt_arg.nc    = KRdSdC;
        mtrx_prnt_arg.fp    = fpw;

        mtrx_prnt (&mtrx_prnt_arg);

        mtrx_prnt_arg.addr  = (const void*)HoWoK_addr;
        mtrx_prnt_arg.size  = sizeof(long);
        mtrx_prnt_arg.nc    = HoWoK;
        mtrx_prnt_arg.fp    = fpy;

        mtrx_prnt (&mtrx_prnt_arg);

        fclose(fpx);
        fclose(fpw);
        fclose(fpy);
    }

    if (res) printf ("%s.", __func__);
    return res;
}

void matrices_addr_free (matrices_addr_t* ma) {
    if (ma->HWC_addr      ) free (ma->HWC_addr      );
    if (ma->fRSC_addr     ) free (ma->fRSC_addr     );
    if (ma->HpWpC_addr    ) free (ma->HpWpC_addr    );
    if (ma->fRdSdC_addr   ) free (ma->fRdSdC_addr   );
    if (ma->HoWoRdSdC_addr) free (ma->HoWoRdSdC_addr);
    if (ma->HoWoK_addr    ) free (ma->HoWoK_addr    );

}

int make_all_dbg (matrix_config_t* mc, matrices_addr_t* ma_dbg, hwc_rsc_conf_t* conf_dbg) {
    int res = 0;

    hwc_conf_t          hwc_conf_l  ;
    rsc_conf_t          rsc_conf_l  ;
    hwc_rsc_conf_t      conf_l      ;
    matrices_addr_t     ma_l        ;

    hwc_rsc_conf_t*     conf;
    matrices_addr_t*    ma  ;

    if (mc==NULL) res = is_nullptr(__func__);

    if (!res) {
        if (ma_dbg && conf_dbg) {   // dbg mode
            conf= conf_dbg  ;
            ma  = ma_dbg    ;
        }
        else {
            conf_l.hwc_conf = &hwc_conf_l;
            conf_l.rsc_conf = &rsc_conf_l;

            conf= &conf_l;
            ma  = &ma_l;
        }

                    res = hwc_rsc_conf_init (mc, conf);
        if (!res)   res = config_complete (conf);
        if (!res)   res = matrices_addr_calloc (conf, ma);
        if (!res)   res = hwc_rsc_values_init (mc, conf, ma);
        if (!res)   res = make_convolution (conf, ma);
        
        if (!res)   res = xwy_prnt(mc, conf, ma);
        if (!res)   res = xwy_2binfile(mc, conf, ma);
        
        if (!ma_dbg) matrices_addr_free (ma);
    }

    if (res) printf ("%s.", __func__);
    return res;
}

int make_all (matrix_config_t* mc) {
    int res = 0;

    hwc_conf_t          hwc_conf_l  ;
    rsc_conf_t          rsc_conf_l  ;
    hwc_rsc_conf_t      conf_l      ;
    matrices_addr_t     ma_l        ;

    hwc_rsc_conf_t*     conf;
    matrices_addr_t*    ma  ;

    if (mc==NULL) res = is_nullptr(__func__);

    if (!res) {
        conf_l.hwc_conf = &hwc_conf_l;
        conf_l.rsc_conf = &rsc_conf_l;

        conf= &conf_l;
        ma  = &ma_l;

        ma->HWC_addr        = NULL;
        ma->fRSC_addr       = NULL;
        ma->HpWpC_addr      = NULL;
        ma->fRdSdC_addr     = NULL;
        ma->HoWoRdSdC_addr  = NULL;
        ma->HoWoK_addr      = NULL;

                    res = hwc_rsc_conf_init (mc, conf);
        if (!res)   res = config_complete (conf);
        if (!res)   res = matrices_addr_calloc (conf, ma);
        if (!res)   res = hwc_rsc_values_init (mc, conf, ma);
        if (!res)   res = make_convolution (conf, ma);
        
        if (!res)   res = xwy_prnt(mc, conf, ma);
        if (!res)   res = xwy_2binfile(mc, conf, ma);
        
        matrices_addr_free (ma);
    }

    if (res) printf ("%s.", __func__);
    return res;
}

//end