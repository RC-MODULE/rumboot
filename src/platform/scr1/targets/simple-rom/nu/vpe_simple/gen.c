
#include <stdio.h>
#include <stdint.h>

#define SIZE (64*128)

#define T int16_t

T data[SIZE];

int main(){
  FILE *fp;
  
  T i;
  
  for (i=0;i<SIZE;i++) {
    data[i] = i;
  }
  
  fp = fopen("etalon.bin","wb");
  fwrite(data,SIZE*sizeof(T),sizeof(data)/sizeof(T),fp);
  fclose(fp);
  return 0;
}
