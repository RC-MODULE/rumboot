#ifndef MATRIX_RM_H
#define MATRIX_RM_H

typedef struct matrix_config {
  int H;   // data cube sizes
  int W;
  int C;
  int Tp;   // null padding
  int Bp;
  int Lp;
  int Rp;

  int R;   // kernel sizes
  int S;   
  // int C;   
  int Dx;   // dilation
  int Dy;   
  int Sx;   // striding
  int Sy;   
  int K;   // number of kernels

  int LINES;
  int COLUMNS;
  int BUF_NMB;

  char *user_X_data_file_name;
  char *user_W_data_base_file_name;
  char *user_Y_data_file_name;

  char *X_data_file_name;
  char *W_data_file_name;
  char *Y_data_file_name;
} matrix_config;


int ReLU(int x);

#endif // MATRIX_RM_H
