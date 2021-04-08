
#include <stdio.h>
#include <stdint.h>

#define SIZE (1024/8 * 48 / sizeof(T)) // сколbко 16-ра3рядныx слов в 48 1024-ра3рядныx словаx
#define SIZE_W (1024/8 * 32 / sizeof(Tw))
#define SIZE_CMD 21

#define Tw int16_t
#define T int16_t

T data[SIZE];
Tw warr[SIZE_W];
char zero;


uint64_t cmdarr[SIZE_CMD];

int main(){
  FILE *fp;
  
  T i;
  
  int c;
  int size;
  
  c=0;
  for (i=0;i<SIZE;i++) {
    if(i % (1024/8/2))
      data[i]=0;
    else {
      data[i] = c;
      c++;
    }
  }
  
  fp = fopen("in_data.bin","wb");
  
  fwrite(data,sizeof(T),SIZE,fp);
  fclose(fp);
  
  size = SIZE * sizeof(T);
  fp = fopen("in_data.bin.metrics","wb");
  fwrite (&size,sizeof(int),1,fp);
  fclose(fp);
  
  for(i=0;i<SIZE_W;i++) {
    warr[i]=1;
  }
  
  fp = fopen("warr.bin","wb");
  fwrite(warr,sizeof(Tw),SIZE_W,fp);
  fclose(fp);
  
  size = SIZE_W * sizeof(Tw);
  fp = fopen("warr.bin.metrics","wb");
  fwrite (&size,sizeof(int),1,fp);
  fclose(fp);
  
  fp = fopen("etalon.bin","r");
  fseek(fp, 0L, SEEK_END);
  size = ftell(fp);
  fclose(fp);
  
  fp = fopen("etalon.bin.metrics","wb");
  fwrite (&size,sizeof(int),1,fp);
  fclose(fp);
  
  //
       cmdarr[0]  = 0x0000000000000030ul;  //write REG WA =30h
       cmdarr[1]  = 0x0000000100000000ul;  //write REG DA =0
       cmdarr[2]  = 0x0000000600000030ul;  //write REG BUFBRDR =30h
       cmdarr[3]  = 0x0000000200000000ul;  //write REG VRA =0h
       cmdarr[4]  = 0x000000030000002ful;  //write REG N =2fh (48 repeating)
       cmdarr[5]  = 0x0000000400000002ul;  //write REG TYPEOP =2h (INT16)
       cmdarr[6]  = 0x000000050000001Eul;  //write REG NORM =30
        
        //            3322222222221111111111000000000033222222222211111111110000000000
        //            1098765432109876543210987654321010987654321098765432109876543210
       cmdarr[7]  = 0b0000000000000000000000010100000110010000000000000000000000000000ul;  //load Wsh
       cmdarr[8]  = 0b0000000000000000000000010000001000000001000000000000000000000000ul;  //Wsh -> W
       cmdarr[9]  = 0b0000000000000000000000010000001101100000100000000111111111000001ul;  //AU 0 + DW -> OFIFO - N from reg,     with 48 repe
       cmdarr[10] = 0b0000000000000000000000010000010000000000110010011111111110100001ul;  //AU 0 + DW -> OFIFO - N from command, with 20 repe
       cmdarr[11] = 0b0000000000000000000000010000010100000000110010000111111110000001ul;  //AU 0 + DW -> OFIFO - N from command, with 17 repe
       cmdarr[12] = 0b0000000000000000000000010000011000000000110001001111111110100001ul;  //AU 0 + DW -> OFIFO - N from command, with 10 repe
       cmdarr[13] = 0b0000000000000000000000010000011100000000110000110111111110100001ul;  //AU 0 + DW -> OFIFO - N from command, with 7 repea
       cmdarr[14]  = 0x0000000300000011ul;  //write REG N =17 (18 repeating)
       cmdarr[15] = 0b0000000000000000000000010100100000000000100010001111111110000001ul;  //AU 0 + DW -> OFIFO - N from reg,     with 18 repe
       cmdarr[16] = 0b0000000000000000000000010000100100000000110000111111111110100001ul;  //AU 0 + DW -> OFIFO - N from command, with 8 repea
       cmdarr[17] = 0b0000000000000000000000010000101000000000110001001111111111100001ul;  //AU 0 + DW -> OFIFO - N from command, with 10 repe
       cmdarr[18] = 0b0000000000000000000000010000101100000000110000100111111111100001ul;  //AU 0 + DW -> OFIFO - N from command, with 5 repea
       cmdarr[19] = 0b0000000000000000000000010000110000000000110000011111111111100001ul;  //AU 0 + DW -> OFIFO - N from command, with 4 repea
       cmdarr[20] = 0b0000000000000000000000010000110100000000110000010111111110000001ul;  //AU 0 + DW -> OFIFO - N from command, with 3 repea
  //
  size = SIZE_CMD*sizeof(uint64_t);
  
  fp = fopen("cmd.bin","wb");
  fwrite(cmdarr,sizeof(uint64_t),SIZE_CMD,fp);
  fclose(fp);
  
  fp = fopen("cmd.bin.metrics","wb");
  fwrite (&size,sizeof(int),1,fp);
  fclose(fp);
  

  return 0;
}
