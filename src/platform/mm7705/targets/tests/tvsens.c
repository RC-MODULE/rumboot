#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/io.h>

#include <platform/regs_tvsens.h>
#include <platform/devices.h>

static const int temp_table[] = {
	650	,	-40	,
	642	,	-35	,
	635	,	-30	,
	627	,	-25	,
	619	,	-20	,
	611	,	-15	,
	603	,	-10	,
	595	,	-5	,
	588	,	0	,
	580	,	5	,
	572	,	10	,
	564	,	15	,
	556	,	20	,
	548	,	25	,
	541	,	30	,
	533	,	35	,
	525	,	40	,
	517	,	45	,
	509	,	50	,
	501	,	55	,
	493	,	60	,
	485	,	65	,
	477	,	70	,
	469	,	75	,
	462	,	80	,
	454	,	85	,
	446	,	90	,
	438	,	95	,
	430	,	100	,
	422	,	105	,
	414	,	110	,
	406	,	115	,
	398	,	120	,
	390	,	125	,
};

int main() {
    volatile uint32_t t0, t1, t2, i;
    double tC;

    rumboot_printf("tvsens: start\n");

    while (1) {
        for (i = 0; i<800000;) { i++; }

        t0 = ioread32 (TVSENS_BASE + TAVG);

        t2 = (t0 >> 20) & 0x3FF;
        t1 = (t0 >> 10) & 0x3FF;
        t0 = t0 & 0x3FF;

        //rumboot_printf("%d %d %d; ", t0, t1, t2);

        // t0 - min value, max temperature
        t0 = t0 < t1 ? t0 : t1;
        t0 = t0 < t2 ? t0 : t2;

        // t1 - max value, min temperature
        t1 = t1 > t2 ? t1 : t2;

        if ((t0 < 390) || (t1 > 650)) {
            rumboot_printf("ERROR: out of range min %d max %d\n", t0, t1);
            break;
        }

        for (i=0; i<68; i+=2) { if (t0 >= temp_table[i]) break; }

        // 5 = temp_table[i+1] - temp_table[i-1]
        if (t0 != temp_table[i]) {
            tC = temp_table[i-1] + (temp_table[i-2] - t0)*5.0/(temp_table[i-2]-temp_table[i]);

            t0 = (int)tC;   // int
            //tC = (tC - (int)tC)*100;
            //t1 = (int)tC;   // decimal
        }
        else {
            t0 = temp_table[i+1];
            //t1 = 0;
        }

        // t real <= tC
        //rumboot_printf ("tC = %d.%d\n", t0, t1);
        rumboot_printf ("tC = %d\n", t0);
    }

    return 0;
}