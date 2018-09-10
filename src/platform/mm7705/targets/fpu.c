#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>



#if 0
#define CRGCPU__ 0x38006000
int freq_apply_mpw(uint32_t fbdiv, uint32_t prediv, uint32_t postdiv)
{
	uint32_t ckdiv = 1;

	if ((fbdiv > 511) || (fbdiv < 8) ||
	    (prediv > 31) || (prediv < 1) ||
	    ((postdiv % 2 != 0) && (postdiv != 1))) {
		rumboot_printf("Wrong values for div.\n");
		rumboot_printf("fbdiv %d\n", fbdiv);
		rumboot_printf("prediv %d\n", prediv);
		rumboot_printf("postdiv %d\n", postdiv);
		return -1;
	}


	uint32_t freq = 25 * fbdiv / prediv / postdiv / (ckdiv + 1);
	rumboot_printf("Switching to %d Mhz ... ", freq);
	iowrite32(0x1ACCE551, CRGCPU__ + 0xc);
	iowrite32((fbdiv << 0) | (prediv << 16) | (postdiv << 24), CRGCPU__ + 0x8);
	iowrite32(0x1, CRGCPU__ + 0x4);
	rumboot_printf("OK");

	return 0;
}

void perform_mpw(/*struct freq_entry *frq*/)
{
	uint32_t prediv = 2;
	uint32_t postdiv = 1;
	uint32_t fbdiv;

	for (fbdiv = 8; fbdiv < 511; fbdiv++) {
		if (freq_apply_mpw(fbdiv, prediv, postdiv) < 0)
			break;

		test(5);
	}
}

void test(int n)
{
	while (n--) {
		//Before call this func ENABLE FPU!
		test_fpu();

		//test_cpu();
	}
}

#endif

int main()
{

	#if 0
		double a = 1.0;
		double b = 2.0;
		rumboot_printf("rumboot: %f\n", a + asin(b));

		test(5);
		perform_mpw();
	#endif

	rumboot_printf("Hello world: %d !\n", 1);

	return 0;
}
