
#include <rumboot/printf.h>

int __attribute__ ((section(".text.test"))) main ()
{
    static __attribute__((aligned(128))) int data[256];
    int *p = data, tmp;
    asm volatile(
        "subi %1,%0,4\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "stwu %1,4(%1)\n"
        "msync\n"
        "dcbf 0, %0\n"
        "sync\n"
        "addi %1,%1,4\n"
        "lwzu %0,-4(%1)\n"
        "lwzu %0,-4(%1)\n"
        "lwzu %0,-4(%1)\n"
        "msync\n"        
        "lwzu %0,-4(%1)\n"
        "lwzu %0,-4(%1)\n"
        "lwzu %0,-4(%1)\n"
        "msync\n"
        "lwzu %0,-4(%1)\n"
        "lwzu %0,-4(%1)\n"
        "lwzu %0,-4(%1)\n"
        "msync\n"
        "lwzu %0,-4(%1)\n"
        "lwzu %0,-4(%1)\n"
        "lwzu %0,-4(%1)\n"
        "msync\n"
        "lwzu %0,-4(%1)\n"
        "lwzu %0,-4(%1)\n"
        "lwzu %0,-4(%1)\n"
        "msync\n"
        "lwzu %0,-4(%1)\n"
        "lwzu %0,-4(%1)\n"
        "lwzu %0,-4(%1)\n"
        "msync\n"
        "lwzu %0,-4(%1)\n"
        "lwzu %0,-4(%1)\n"
        "lwzu %0,-4(%1)\n"
        "msync\n"
        "lwzu %0,-4(%1)\n"
        "lwzu %0,-4(%1)\n"
        "lwzu %0,-4(%1)\n"
        "msync\n"
        "lwzu %0,-4(%1)\n"
        "lwzu %0,-4(%1)\n"
        "lwzu %0,-4(%1)\n"
        "msync\n"
        "lwzu %0,-4(%1)\n"
        "lwzu %0,-4(%1)\n"
        "lwzu %0,-4(%1)\n"
        "msync\n"
        "lwzu %0,-4(%1)\n"
        "lwzu %0,-4(%1)\n"
        "lwzu %0,-4(%1)\n"
        "msync\n"
        "lwzu %0,-4(%1)\n"
        "lwzu %0,-4(%1)\n"
        "lwzu %0,-4(%1)\n"
        "msync\n"
        "lwzu %0,-4(%1)\n"
        "lwzu %0,-4(%1)\n"
        "lwzu %0,-4(%1)\n"
        "msync\n"
        "lwzu %0,-4(%1)\n"
        "lwzu %0,-4(%1)\n"
        "lwzu %0,-4(%1)\n"
        "msync\n"
        "lwzu %0,-4(%1)\n"
        "lwzu %0,-4(%1)\n"
        "lwzu %0,-4(%1)\n"
        "msync\n"
        "lwzu %0,-4(%1)\n"
        "lwzu %0,-4(%1)\n"
        "lwzu %0,-4(%1)\n"
        "msync\n"
        "lwzu %0,-4(%1)\n"
        "lwzu %0,-4(%1)\n"
        "lwzu %0,-4(%1)\n"
        "msync\n"
        "lwzu %0,-4(%1)\n"
        "lwzu %0,-4(%1)\n"
        "lwzu %0,-4(%1)\n"
        "msync\n"
        "lwzu %0,-4(%1)\n"
        "lwzu %0,-4(%1)\n"
        "lwzu %0,-4(%1)\n"
        "msync\n"
        "lwzu %0,-4(%1)\n"
        "lwzu %0,-4(%1)\n"
        "lwzu %0,-4(%1)\n"
        "msync\n"
        "lwzu %0,-4(%1)\n"
        "lwzu %0,-4(%1)\n"
        "lwzu %0,-4(%1)\n"
        "msync\n"
        "lwzu %0,-4(%1)\n"
        "subi %0,%0,4\n"
        : "+r"(p), "=r"(tmp)
        :
        : "memory"
    );

    if(p!=data)
    {
        //rumboot_printf("p = %x\n", p);
        return 1;
    }

    for(int i = 0; i < sizeof(data)/sizeof(data[0]); i++, p++)
    {
        //rumboot_printf("%x = %x\n", &data[i], data[i]);
    }
    return 0;
}
