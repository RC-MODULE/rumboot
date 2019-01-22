#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/boot.h>
#include <stdbool.h>
#include <rumboot/timer.h>

int main()
{
	rumboot_printf("Hello, I'll check if timers are properly configured\n");

	uint32_t sim_start, sim_end, diff, t_diff, t_start, t_end;
	uint32_t jitter, delta;

	rumboot_sim_get_realtime(&sim_start);
	rumboot_sim_get_realtime(&sim_end);
	diff = sim_end - sim_start;
	delta = diff;
	rumboot_printf("Event delivery took %d ns\n", diff);

	rumboot_sim_get_realtime(&sim_start);
	t_start = rumboot_platform_get_uptime();
	t_end = rumboot_platform_get_uptime();
	rumboot_sim_get_realtime(&sim_end);
	diff = sim_end - sim_start;
	t_diff = t_end - t_start;
	delta = diff - delta;
	rumboot_printf("2 timer reads take %d ns, approx %d ns per read\n", delta, delta/2);

	uint32_t delay;
	for (delay = 100; delay < 300; delay += 100) {
		t_start = rumboot_platform_get_uptime();
		rumboot_sim_get_realtime(&sim_start);
		udelay(delay);
		rumboot_sim_get_realtime(&sim_end);
		t_end = rumboot_platform_get_uptime();
		diff = sim_end - sim_start;
		t_diff = t_end - t_start;
		rumboot_printf("%d us delay: Took %d ns modelling, %d ns timer\n", delay, diff - delta, t_diff * 1000 - delta);
		/* TODO: Error checking */
	}

	return 0;
}
