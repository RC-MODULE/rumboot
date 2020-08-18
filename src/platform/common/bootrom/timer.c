#include <stdio.h>
#include <rumboot/printf.h>
#include <rumboot/irq.h>
#include <rumboot/platform.h>
#include <rumboot/boot.h>
#include <stdbool.h>
#include <rumboot/timer.h>

#define THRESHOLD 10
int main()
{
    struct rumboot_config conf;
    rumboot_platform_read_config(&conf);
    rumboot_platform_init_loader(&conf);

	rumboot_printf("Hello, I'll check if timers are properly configured\n");

	uint32_t sim_start, sim_end, t_start, t_end;
	int32_t diff, t_diff;
	int32_t delta;

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

	int32_t delay;
	for (delay = 300; delay < 2100; delay += 300) {
		t_start = rumboot_platform_get_uptime();
		rumboot_sim_get_realtime(&sim_start);
		udelay(delay);
		rumboot_sim_get_realtime(&sim_end);
		t_end = rumboot_platform_get_uptime();
		diff = sim_end - sim_start;
		t_diff = t_end - t_start;
		int32_t error = 100 - (((diff-delta) * 100) / (t_diff * 1000 - delta));
		rumboot_printf("%d us delay: Took %d ns modelling, %d ns timer, %d%% error\n",
			delay,
			diff - delta,
			t_diff * 1000 - delta,
			error);
		if (error > 10) {
			rumboot_printf("Error exceeds a threshold of 10%%. Your timer setup is likely fucked up\n");
			return 1;
		}
	}

	return 0;
}
