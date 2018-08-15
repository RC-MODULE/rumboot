
#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/memfillseq.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>

#include <rumboot/rumboot.h>
#include <stdlib.h>



uint32_t test_arr[32];

int main()
{
        rumboot_printf("Test memfill\n");
        rumboot_memfillseq(test_arr,0,32);
        for(int i=0;i<32;i++)
        {
          rumboot_printf("%x\n",test_arr[i]);
          if(test_arr[i] != i)
            return 1;
        }
        test_arr[29]=0; // Inject Error
        rumboot_memcheckseq(test_arr,0,32); // Should Return With ERROR
        return 0;
}
