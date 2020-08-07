#include <rumboot/printf.h>
#include <rumboot/timer.h>
#include <math.h>
#include <rumboot/io.h>
char tmp[32];
void do_save(float a, float b, float c)
{
    iowrite32(a, tmp);
    iowrite32(b, tmp);
    iowrite32(c, tmp);
}

int main()
{
    register double a=0,b=0,c=0;
    int i = 10;
    while (i--) {
        a+=0.1;
        b+=0.2;
        c=a+b;
        do_save(a,b,c);   
    }
    return 0;
}