#include <stdint.h>
#include <stdbool.h>

#include <devices/sctl.h>
#include <devices/bisr.h>

#include <rumboot/io.h>
#include <rumboot/timer.h>
#include <rumboot/printf.h>

#include <platform/devices.h>

#define BISR_TIMEOUT 500

void bisr_prog_start()
{
	uint32_t sctlreg = ioread32(SCTL_BASE + SCTL_BISR_L2C);

	iowrite32(sctlreg | (1 << SCTL_PROG_START_i), SCTL_BASE + SCTL_BISR_L2C);
}

void bisr_prog_stop()
{
	uint32_t sctlreg = ioread32(SCTL_BASE + SCTL_BISR_L2C);

	iowrite32(sctlreg & ~(1 << SCTL_PROG_START_i), SCTL_BASE + SCTL_BISR_L2C);
}

void bisr_start()
{
  uint32_t sctlreg = ioread32(SCTL_BASE + SCTL_BISR_L2C);

  iowrite32(sctlreg | (1 << SCTL_START_i), SCTL_BASE + SCTL_BISR_L2C);
}

void bisr_stop()
{
	uint32_t sctlreg = ioread32(SCTL_BASE + SCTL_BISR_L2C);

	iowrite32(sctlreg & ~(1 << SCTL_START_i), SCTL_BASE + SCTL_BISR_L2C);
}

bool bisr_wait()
{
	uint32_t count = 0;

	while (!(ioread32(SCTL_BASE + SCTL_BISR_L2C) & (1 << SCTL_FINISH_i)))
		if (count++ == BISR_TIMEOUT)
			return false;

	return true;
}

static bool bisr_dump_results() {

  uint32_t err_vector_low = ioread32(BISR_L2C + BISR_ERROR_VECTOR_HIGH_ARR0);
  uint32_t err_vector_high = ioread32(BISR_L2C + BISR_ERROR_VECTOR_HIGH_ARR0);

  rumboot_printf("Error vector Low ARRx: %x\n", err_vector_low);
  rumboot_printf("Error vector High ARRx: %x\n", err_vector_high);

  return (err_vector_low || err_vector_high) ? false : true;
}

static void bisr_analyze()
{
  uint32_t l2cbisr = ioread32(SCTL_BASE + SCTL_BISR_L2C);

  bool redok = l2cbisr & (1 << SCTL_RSLT_i);
  bool rslt = l2cbisr & (1 << SCTL_REDOK_i);

  if(!redok && !rslt) rumboot_printf("FAIL.\n");
  if(!redok && rslt) rumboot_printf("GOOD.\n");
  if(redok && rslt) rumboot_printf("PERFECT.\n");
}

bool bisr_program_test()
{
  rumboot_printf("Set test mode.\n");
	iowrite32(0, BISR_L2C + BISR_REDUNDANCY_BUS_ARR0);
  rumboot_printf("Program start L2C BISR.\n");
	bisr_prog_start();
	udelay(1);
  rumboot_printf("Program stop L2C BISR.\n");
	bisr_prog_stop();
  rumboot_printf("Wait L2C BISR.\n");
	if (!bisr_wait())
		return false;

  rumboot_printf("Dump results.\n");
  return bisr_dump_results();
}

bool bisr_hard_test()
{
  rumboot_printf("Start L2C BISR.\n");
  bisr_start();
  udelay(1);
  rumboot_printf("Stop L2C BISR.\n");
  bisr_stop();
  bisr_wait();
  bisr_analyze();

  return false;
}
