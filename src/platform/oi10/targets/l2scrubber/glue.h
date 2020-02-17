#ifndef GLUE_H
# define GLUE_H

#define L2SCRUB_MODE_POLLING -2
#define L2SCRUB_MODE_MCK -1
#define L2SCRUB_MODE_IRQ 0

struct l2_scrubber *l2scrubber_tests_init(int mode);
void *l2scrubber_tests_get_cachable_area();

#endif
