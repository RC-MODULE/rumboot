#ifndef RUMBOOT_PLATFORM_H
#define RUMBOOT_PLATFORM_H

#include <stdint.h>
#include <stddef.h>
#include <setjmp.h>
#include <unistd.h>

/**
 *
 * \defgroup platform_glue Platform bindings
 *
 * These functions should be implemented in the platform support code.
 * Rumboot core modules depend on these for proper operation
 * \code{.c}
 * #include <rumboot/platform.h>
 * \endcode
 *
 * \defgroup platform_glue_base Basics
 * \ingroup platform_glue
 *
 * \defgroup platform_glue_irq IRQ subsystem glue functions
 * \ingroup platform_glue
 *
 * \defgroup platform_glue_lds Global variables defined by linker
 * \ingroup platform_glue
 *
 * \addtogroup platform_glue_base
 *  @{
 */


/**
 * This enumeration represents common events from rumboot to
 * simulation environment. Platfoms should implement their
 * specific ways of delivering these events.
 */
enum rumboot_simulation_event {
    EVENT_NOP, /** No-op event */
    EVENT_TERM, /** Termination request */
    EVENT_STDOUT, /** A character is ready for output */
    EVENT_STDIN, /** Request for next character input */
    EVENT_TRACE, /** A function call trace */
    EVENT_PRINTF, /** Simulator printf */
    EVENT_UPLOAD, /** Request file upload to memory */
    EVENT_PERF, /** Performance metric checkpoint */
    EVENT_DOWNLOAD, /** Request file download from memory */
    EVENT_LPROBE, /** Lprobe data IO event */
    EVENT_MEMSET, /** memset substitution */
    EVENT_STACKTRACE, /** Tells environment to trace the stack */
    EVENT_GCDA, /** Request to store coverage information */
    EVENT_MEMCPY, /** memcpy acceleration */
    EVENT_PERF_FUNC, /** Perf checkpoint via function addr */
    EVENT_SIM_SAVE, /** Request to save simulation state to file */
    EVENT_SIM_RESTORE, /** Request to load simulation state from a file */
    EVENT_TESTEVENT, /** Special event to be sent to SystemVerilog part of the test */
    EVENT_RELOCATE_RUNTIME, /** Request to poll a different cell for runtime structure */
    EVENT_MEMFILLSEQ, /** Fill the memory with 32-bit incrementing values */
    EVENT_MEMCHECKSEQ, /** Check that memory filled with 32-bit incrementing values */
    EVENT_GENERIC, /** Generic SystemVerilog event trigger */
    EVENT_MEMCMP, /** memcmp acceleration */
    EVENT_MEMFILL8, /** memfill8 acceleration */
    EVENT_REALTIME, /** Get $realtime. Timer tests only! */
    EVENT_CRC32, /** CRC32 calculation stuff */
    EVENT_CRC8, /** CRC8 calculation */
};


/**
 * Emergency exit, optionally printing out a reason why
 * This will terminate the simulation or cause an infinite loop
 * in production builds.
 *
 * WARNING! Use with extreme care for production code!
 *
 * @param fmt format for description message. Can be NULL
 */
#ifndef RUMBOOT_SILENT_PANICS
#define rumboot_platform_panic(fmt, ...) \
    { \
        if (fmt) { \
            rumboot_printf("PANIC: %s", fmt, ##__VA_ARGS__); \
        } \
        exit(13); /* Trigger an exit */ \
        while(1);; /* And loop if the above didn't work */ \
    }
#else
#define rumboot_platform_panic(fmt, ...) \
    { \
        while(1);; /* And loop if the above didn't work */ \
    }
#endif
/**
 * This function will be called before main(). This function should contains
 * platform-specific initialization. This should initilize at lest:
 * - A timer for rumboot_platform_get_uptime()
 * - The event system for rumboot_platform_event_raise()
 *
 */
void rumboot_platform_setup();


/**
  * This is common initialization code needed to bring up proper C environment.
  * It does the following:
  * 1. Initializes the global runtime table with sane initial values
  * 2. Initializes bss section with zeroes (if we're using the BSS)
  * 3. Calls main() function
  * 4. Collects return code and passes it to exit() if we had a return from main
  * 5. Handles longjump from exit() function
  * 6. Returns to the caller with the exit code
  *
  * This function should be called from platform-specific assembly startup file
  * or serve as an entry point for SPL images
  *
  * For SPL images it returns to ROM loader so that it can handle the exit code
  * and load the next image
  *
  * For ROM images it returns to assembly startup code that should do an
  * infinite loop/processor halt/something else
  *
  *
  * \callgraph
  */
 int rumboot_main();

/**
 * Returns the current system uptime (useconds)
 * @return useconds
 */
uint32_t rumboot_platform_get_uptime();

/**
 *  Get the $realtime from simulation environment.
 *  _WARNING_: DO NOT USE THIS IN YOUR CODE, UNLESS YOU ARE VERIFYING TIMERS
 *  AGAINST MODELLING TIME!!! Use rumboot_platform_get_uptime();
 *
 * @param rt Where to place the result
 */
void rumboot_sim_get_realtime(uint32_t *rt);

/**
 * This function is called to trace a function call with a pc value
 * @param pc register value
 */
void rumboot_platform_trace(void *pc);

/**
 * Raise an event to notify the environment. The EVENT_FINISH and EVENT_CRASH
 * can be used to terminate the simulation
 *
 *
 * @param event Event code to send to modelling environment
 * @param data  Data to send to modelling environment (up to 8 words)
 * @param len   Length of data (in 32-bit words)
 */
void rumboot_platform_event_raise(enum rumboot_simulation_event event,
     uint32_t const *data, uint32_t len);

/**
 * Get the next event from the simulation environment.
 * This function will BLOCK until the next event arrives.
 * USE WITH EXTREME CAUTION
 * The pointer to the data associated with event will be placed
 * in data. When your are done working with the event data call
 * rumboot_platform_event_clear() to clear pending event status.
 *
 * @param  data This variable will be assigned a pointer to the
 *              event data
 * @return      The next event code.
 */
enum rumboot_simulation_event rumboot_platform_event_get(
     				  volatile uint32_t **data);


/**
 * Clear inbound event status. Call this function prior to calling next
 * rumboot_platform_event_get()
 */
void rumboot_platform_event_clear();

/**
 * Send a character to stdout.
 * @param c [description]
 */
void rumboot_platform_putchar(uint8_t c);

/**
 * Return the character from stdint casted to int or
 * -1 if reception timed out. 
 *
 * This function should be implemented by platform glue
 * and should not be used directly. 
 *
 * Use rumboot_getchar() instead
 *
 * Timeout may be 0 to return immediately
 *
 * @param  timeout_us [description]
 * @return            [description]
 */
int rumboot_platform_getchar(uint32_t timeout_us);

/**
 * Request simulation environment to upload a file from plusarg to address addr
 *
 * @param plusarg the name of plusarg that contains the filename or the filename
 * @param addr   address to load the file to
 */
void rumboot_platform_request_file(const char *plusarg, uint32_t addr);


/**
 * Request simulation environment to save contents of specified physical memory region
 * to a file
 * @param filename filaneme to create
 * @param addr     physical address of buffer to dump
 * @param len      length of the buffer to dump
 */
void rumboot_platform_dump_region(const char* filename, uint32_t addr, uint32_t len);

/**
 * Request simulation environment to save contents gcda data to a file to a file
 * The simulation environment should take care of handling paths and copying gcno
 * file to a relevant location
 *
 * @param filename filaneme to create
 * @param addr     physical address of buffer to dump
 * @param len      length of the buffer to dump
 */

void rumboot_platform_store_gcda(const char *filename, uint32_t addr, uint32_t len);

/**
 * Record a performance metric checkpoint with optional name.
 * @param tag string tag. NULL finishes previous performance metric region.
 */
void rumboot_platform_perf(const char *tag);

/**
 * Record a performance metric checkpoint when entering a function by function adress.
 * Used internally by auto-profiling code
 * @param addr Address of a function starting execution.
 *  NULL tells environment we're exiting the function.
 */

void rumboot_platform_perf_func(void *addr);

/**
 * Save simulation state to a file
 * @param filename [description]
 */
void rumboot_platform_sim_save(const char *filename);

/**
 * Restore simulation state from a file.
 * This function should never return, unless simulator is broken
 * @param filename [description]
 */
void rumboot_platform_sim_restore(const char *filename);

/**
 * Request to relocate rumboot_runtime_info structure to a different
 * location in memory. Once this request is sent all events should use the
 * new location. Usually it's needed to chainload to a new image with it's own
 * that will runtime location that be further initialized by image's rumboot_main()
 * @param addr [description]
 */
void rumboot_platform_relocate_runtime(uint32_t addr);

/**
 * Trigger a generic SV event
 * @param name event name
 */
void rumboot_platform_sv_event(const char *name);

/**
 * The function fills memory sequentially with val that is incremented each time by incr s
 * tarting from address pointed by addr until all sz bytes are filled
 *
 * @param  addr start address
 * @param  sz   count of bytes
 * @param  val  starting value
 * @param  incr increment
 * @return      pointer to the start of the filled area
 */
void* rumboot_memfill8(void *addr, size_t sz, uint8_t val, int8_t incr);

/**
 * The function fills memory sequentially with val that is incremented each time by incr s
 * tarting from address pointed by addr until all sz bytes are filled.
 * 
 * WARNING: This function accepts words as sz arguments 
 * 
 * @param addr start address
 * @param sz size in 32-bit words
 * @param val initial value to start with
 * @param incr increments
 * @return int 
 */
int rumboot_memfill32(void *addr, size_t sz, int val, int incr);

/**
 * Checks two buffers. 
 * 
 * @warning Perhaps we should ditch this one
 * 
 * @param src 
 * @param dst 
 * @param sz 
 * @return int 
 */
int rumboot_memcheck32(void *src, void *dst, size_t sz);

/**
 * Returns 64-bit physical address from a virtual 32-bit
 * address on ppc platforms. On ARM just platforms returns
 * the addr cast to 64-bit.
 *
 * This function may return -1 on error.
 *
 * @param  addr [description]
 * @return      [description]
 */
int64_t rumboot_virt_to_phys(volatile void *addr);

/**
 * Returns 32-bit address for dma operations
 *
 * @param  addr [description]
 * @return      [description]
 */
uint32_t rumboot_virt_to_dma(volatile const void *addr);


/**
 * Architecture-specific code that prints out
 * a stack-trace.
 */
void rumboot_arch_stacktrace(void);

/**
 * Architecture-specific code that prints out useful info
 * when an exception happens (like registers, stacktrace).
 * For ROM code this is set as default exception handler.
 *
 * @param id   exception id
 * @param name exception description
 */
void rumboot_arch_exception(int id, const char *name);

/**
 * Declares a function to be a constructor (will be called before main())
 * @param  name - function name
 */
#define RUMBOOT_CONSTRUCTOR(name) \
    void __attribute__((constructor)) name()


/**
 * @brief Platform-specific exit hook.
 * 
 * Called from _exit() upon each test termination.
 *
 * Production testing code may override this one
 * Default implementation does nothing
 * 
 * @param status exit code
 */
void rumboot_platform_exit(int status);

/**
 * Declares a function to be a constructor (will be called upon returning from
 * main or when user calls exit()
 *
 * @param  name function name
 * @return      [description]
 */

#define RUMBOOT_DESTRUCTOR(name) \
    void __attribute__((constructor)) name()


/**
*
*  @}
*/

/**
*
* \addtogroup platform_glue_lds
*  @{
*/

#define RUMBOOT_SYNC_MAGIC_IN  0x0001c0de
#define RUMBOOT_SYNC_MAGIC_OUT 0x0002c0de

/**
 * This represents a syncbuffer for ram-based operations
 * The syncbuffer's directtion is coded with 'magic value'
 * if opcode field is EVENT_NOP there's nothing there.
 *
 */
struct rumboot_syncbuffer {
    volatile uint32_t magic;
    volatile uint32_t opcode;
    volatile uint32_t data[8];
} __attribute__((packed));

#ifndef RUMBOOT_PLATFORM_NUM_HEAPS
#define RUMBOOT_PLATFORM_NUM_HEAPS 4
#endif

#ifndef RUMBOOT_PLATFORM_NUM_IRQ_CONTROLLERS
#define RUMBOOT_PLATFORM_NUM_IRQ_CONTROLLERS 4
#endif

#ifndef RUMBOOT_PLATFORM_NUM_GPIO_CONTROLLERS
#define RUMBOOT_PLATFORM_NUM_GPIO_CONTROLLERS 4
#endif

/**
 * This represents a memory heap that can be used to
 * allocate memory using rumboot_malloc_from_heap
 * A set of these are stored in rumboot_runtime_info
 * RUMBOOT_PLATFORM_NUM_HEAPS is used specify maximum heap
 * count
 */
struct rumboot_heap {
    void *start;
    void *end;
    void *pos;
    void *save;
    const char *name;
};


struct rumboot_irq_controller;

struct rumboot_gpio_controller;

/**
 * This global structure stores internal romboot state and some useful variables
 * It stores selftest results for further inspection by secondary
 * loader. The instance is always available via rumboot_platform_runtime_info
 */
struct rumboot_runtime_info {
    /** Magic value. Indicates that this structure contains valid data */
    uint32_t magic;
    /** Modelling memory IO buffer. Do not move it inside struct! */
    struct rumboot_syncbuffer out;
    /** Modelling memory IO buffer. Do not move it inside struct! */
    struct rumboot_syncbuffer in;
    /** This array will be preserved during soft-resets. Needed by watchdog tests */
    uint32_t persistent[8];
    /** Dummy. Everything from this point to the very end will be cleaned */
    uint32_t clean_me_marker;
    /** How many heaps are registered ? */
    int num_heaps;
    /** An array of available memory heaps */
    struct rumboot_heap heaps[RUMBOOT_PLATFORM_NUM_HEAPS];
    /** How many irq controllers are registered */
    int num_irq_controllers;
    /** An array of available memory controllers */
    const struct rumboot_irq_controller * irq_ctl_dev[RUMBOOT_PLATFORM_NUM_IRQ_CONTROLLERS];
    /** Pointer to current active irq table. Do not use directly, use rumboot_irq_table_get() */
    void *irq_handler_table;
    /** Pointer to irq default handler */
    void (*irq_def_hndlr)(int irq, void *arg);
    /** User-specified argument for irq_def_arg */
    void *irq_def_arg;
    /** Pointer to exception handler */
    void (*irq_exception_hndlr)(int id, const char *name);
    /** IRQ Context counter. Increments every time we start servicing interrupt, decrements when we exit from isr */
    int irq_context_counter;
    /** Type of current IRQ */
    int irq_type;
    /** An array of available GPIO controllers */
    struct rumboot_gpio_controller *gpio_ctrl_dev[RUMBOOT_PLATFORM_NUM_GPIO_CONTROLLERS];
    /** How many GPIO controllers are registered */
    int num_gpio_controllers;
    /** Level of function nesting (used by function tracing code) */
    int nestlevel;
    /** If non-zero, all serial debug output will be suspended */
    int silent;
    /** jmp_buf to jump from _exit() to rumboot_main(). Required for chaining tests for production */
    jmp_buf exit_trampoline;
} __attribute__((packed));

/**
 * Global instance of struct rumboot_runtime_info.
 */
#ifdef RUMBOOT_NATIVE
extern struct rumboot_runtime_info *rumboot_platform_runtime_info;
#else
extern struct rumboot_runtime_info rumboot_platform_runtime;
#define rumboot_platform_runtime_info (&rumboot_platform_runtime)
#endif

/**
 * This global variable defined by the linker points to the start of SPL
 * area. Use &reference to this variable to get the desired address.
 */
extern char rumboot_platform_spl_start;

/**
 * This global variable defined by the linker points to the end of SPL
 * area. Use &reference to this variable to get the desired address.
 */
extern char rumboot_platform_spl_end;

/**
* This global variable defined by the linker points to the top of the stack.
* Use &reference to this variable to get the desired address.
*/
extern void *rumboot_platform_stack_top;

/**
 * This global variable defined by the linker points to the lowest address where
 * stack(s) are stored. Use &reference to this variable to get the desired address.
 */
extern char rumboot_platform_stack_area_start;

/**
 * This global variable defined by the linker points to the highest address where
 * stack(s) are stored. Use &reference to this variable to get the desired address.
 */
extern char rumboot_platform_stack_area_end;

/**
 * Start of the BSS section
 */
extern char rumboot_platform_bss_start;

/**
 * End of bss section
 */
extern char rumboot_platform_bss_end;

extern char rumboot_data_start;
extern char rumboot_data_end;
extern char rumboot_rom_data;

/**
 * Rumboot's entry point. Usually provided by startup assembly code.
 * This will be placed as the entry point of the secondary image
 */
extern void rumboot_entry_point();
/** @}*/

#endif /* end of include guard: RUMBOOT_PLATFORM_H */
