#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/io.h>

#include <platform/regs_tvsens.h>
#include <platform/devices.h>

#define OUT_OF_RANGE    0xE7707

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

uint32_t get_tC(volatile uint32_t t0) {
    uint32_t t1, t2, i;
    double tC;

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

        t0 = OUT_OF_RANGE;
    }
    else {
        for (i=0; i<68; i+=2) { if (t0 >= temp_table[i]) break; }

        // 5 = temp_table[i+1] - temp_table[i-1]
        if (t0 != temp_table[i]) {
            tC = temp_table[i-1] + (temp_table[i-2] - t0)*5.0/(temp_table[i-2]-temp_table[i]);

            t0 = (int)tC;   // int
        }
        else { t0 = temp_table[i+1]; }
    }

    return t0;
}

void tvsens_tavg() {
    volatile uint32_t t0, i;

    rumboot_printf("tvsens_tavg: start\n");

    while (1) {
        for (i = 0; i<800000;) { i++; }

        t0 = ioread32 (TVSENS_BASE + TAVG);

        t0 = get_tC(t0);
        if (t0 == OUT_OF_RANGE) continue;

        rumboot_printf ("tC = %d\n", t0);
    }
}

int main() {
    volatile uint32_t tmp;

    rumboot_printf("tvsens: start\n");

    tmp = (0x10<<5) ^ 0x10;
    iowrite32 (tmp, TVSENS_BASE+TCTRL_AVG);
    //tmp = ioread32 (TVSENS_BASE+TCTRL_AVG);
    //rumboot_printf ("TCTRL_AVG: 0x%x\n", tmp);

    tvsens_tavg();

    return 0;
}