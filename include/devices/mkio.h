#ifndef MKIO_H
#define MKIO_H

#include <stdint.h>

/**
 *
 * \defgroup mkio Gaisler MKIO functions
 * \ingroup devices
 *
 * \code{.c}
 * #include <devices/mkio.h>
 * \endcode
 *
 *  @{
 */
    
    /**
     * Standard 1553 mode codes for control transactions
     * Used in Gaisler MKIO controller
     */
    enum mkio_mode_code {
        DYNAMIC_BUS_CONTROL                          = 0b00000,
        SYNCHRONIZE                                  = 0b00001,
        TRANSMIT_STATUS_WORD                         = 0b00010,
        INITIATE_SELF_TEST                           = 0b00011,
        TRANSMITTER_SHUTDOWN                         = 0b00100,
        OVERRIDE_TRANSMITTER_SHUTDOWN                = 0b00101,
        INHIBIT_TERMINAL_FLAG_BIT                    = 0b00110,
        OVERRIDE_INHIBIT_TERMINAL_FLAG_BIT           = 0b00111,
        RESET_REMOTE_TERMINAL                        = 0b01000,
        TRANSMIT_VECTOR_WORD                         = 0b10000,
        SYNCHRONIZE_DATA                             = 0b10001,
        TRANSMIT_LAST_COMMAND                        = 0b10010,
        TRANSMIT_BIT_WORD                            = 0b10011,
        SELECTED_TRANSMITTER_SHUTDOWN                = 0b10100,
        OVERRIDE_SELECTED_TRANSMITTER_SHUTDOWN       = 0b10101,
    };
    
    /**
     * This function is for checking, that MKIO is present in current project in that base address.
     * It checks MKIO configuration, presence of BC, RT and BM blocks
     *
     * @param base_address             base address of MKIO controller
     *
     * @return pass
     */
    uint32_t mkio_present (uint32_t base_address);

    /**
     * This function display core BCSL and IRQ registers values with rumboot_printf function
     *
     * @param base_address             base address of MKIO controller
     *
     */
    void mkio_report_bc_status (uint32_t base_address);

    /**
     * This function display core RTS and IRQ registers values with rumboot_printf function
     *
     * @param base_address             base address of MKIO controller
     *
     */
    void mkio_report_rt_status (uint32_t base_address);

    /**
     * This function start simple MKIO work in BC mode
     *
     * @param base_address             base address of MKIO controller
     * @param descriptor_pointer       address of first descriptor in SoC memory (SRAM, DDR, PCIe ...)
     *
     */
    void mkio_bc_start_schedule (uint32_t base_address, uint32_t descriptor_pointer);

    /**
     * This function start simple MKIO work in RT mode
     *
     * @param base_address             base address of MKIO controller
     * @param subaddress_table_pointer address of subaddress table in SoC memory (SRAM, DDR, PCIe ...)
     *
     */
    void mkio_rt_start_schedule (uint32_t base_address, uint32_t subaddress_table_pointer);

    /**
     * This function start simple MKIO work in BM mode
     *
     * @param base_address             base address of MKIO controller
     * @param ring_buffer_pointer      address of ring buffer in SoC memory (SRAM, DDR, PCIe ...)
     * @param ring_buffer_size         size in bytes of ring buffer in SoC memory (SRAM, DDR, PCIe ...)
     *
     */
    void mkio_bm_start_logging (uint32_t base_address, uint32_t ring_buffer_pointer, uint32_t ring_buffer_size);

    /**
     * This function is for simple array transaction from BC to RT.
     *   It is specifical for Basis SoC, use one MKIO like BC and another like RT.
     *   Both MKIOs must be connected to one external 1553 bus.
     *
     * @param data_src             head address of data source array
     * @param data_dst             head address of data destination array
     * @param size                 data array size
     * @param bc_base_address      base address of MKIO BC controller
     * @param rt_base_address      base address of MKIO RT controller
     * @param bus_sel              select, what 1553 bus (A or B) to use (first)
     *
     */
    uint32_t mkio_write_to_rt (uint32_t data_src, uint32_t data_dst, uint32_t size, uint32_t bc_base_address, uint32_t rt_base_address, uint32_t bus_sel);

    /**
     * This function is for simple array transaction from RT to BC.
     *   It is specifical for Basis SoC, use one MKIO like BC and another like RT.
     *   Both MKIOs must be connected to one external 1553 bus.
     *
     * @param data_src             head address of data source array
     * @param data_dst             head address of data destination array
     * @param size                 data array size
     * @param bc_base_address      base address of MKIO BC controller
     * @param rt_base_address      base address of MKIO RT controller
     * @param bus_sel              select, what 1553 bus (A or B) to use (first)
     *
     */
    uint32_t mkio_read_from_rt (uint32_t data_src, uint32_t data_dst, uint32_t size, uint32_t bc_base_address, uint32_t rt_base_address, uint32_t bus_sel);

    /**
     * This function is for setting MKIO signals polarity.
     *   This function must be called one time before all other work with MKIO
     *   Watch polarity_reg format in the documentation.
     *
     * @param base_address             base address of MKIO controller
     * @param polarity_reg             value of polarity register
     *
     */
    void mkio_set_polarity (uint32_t base_address, uint32_t polarity_reg);

    /**
     * This function arms first timestamp trigger for selected MKIO
     *
     * @param base_address          base address of MKIO controller
     *
     */
    void mkio_trig_first_timestamp (uint32_t base_address);

    /**
     * This function is a command to fix current timestamp
     * It is needed, because timestamp has 64 bit width format and could not be readed in one moment
     *
     * @param base_address          base address of MKIO controller
     *
     */
    void mkio_fix_timestamp (uint32_t base_address);

    /**
     * This function reports current timestamp with rumboot_printf mechanism
     * It includes mkio_fix_timestamp function
     *
     * @param base_address          base address of MKIO controller
     *
     */
    void mkio_report_timestamp (uint32_t base_address);


/** @}*/

struct __attribute__ ((__packed__)) mkio_bc_descriptor
{
    //-----------------------------------------------------
    //      [31] Must be 0 to identify as descriptor
    //      [30] Wait for external trigger (WTRIG)
    //      [29] Exclusive time slot (EXCL) - Do not schedule asynchronous messages
    //      [28] IRQ after transfer on Error (IRQE)
    //      [27] IRQ normally (IRQN) - Always interrupts after transfer
    //      [26] Suspend on Error (SUSE) - Suspends the schedule (or stops the async transfer list) on error
    //      [25] Suspend normally (SUSN) - Always suspends after transfer
    //      [24 : 23] Retry mode (RETMD). 00 - Retry on same bus only. 01 - Retry alternating on both buses
    //          10 - Retry first on same bus, then on alternating bus. 11 - Reserved, do not use
    //      [22 : 20] Number of retries (NRET) - Number of automatic retries per bus
    //          The total number of tries (including the first attempt) is NRET+1 for RETMD=00, 2 x (NRET+1) for RETMD=01/
    //          10
    //      [19] Store bus (STBUS) - If the transfer succeeds and this bit is set, store the bus on which the transfer succeeded (0
    //          for bus A, 1 for bus B) into the per-RT bus swap register. If the transfer fails and this bit is set, store the opposite
    //          bus instead. (only if the per-RT bus mask is supported in the core)
    //          See section 32.4.3 for more information.
    //      [18] Extended intermessage gap (GAP) - If set, adds an additional amount of gap time, corresponding to the RTTO
    //          field, after the transfer
    //      [17 : 16] Reserved - Set to 0 for forward compatibility
    //      [15 : 0] Slot time (STIME) - Allocated time in 4 microsecond units, remaining time after transfer will insert delay
    //-----------------------------------------------------
    uint32_t ctrl_word_0         : 32;
    //-----------------------------------------------------
    //      [31] Dummy transfer (DUM) - If set to ‘1’ no bus traffic is generated and transfer “succeeds” immediately
    //          For dummy transfers, the EXCL,IRQN,SUSN,STBUS,GAP,STIME settings are still in effect, other bits and
    //          the data buffer pointer are ignored.
    //      [30] Bus selection (BUS) - Bus to use for transfer, 0 - Bus A, 1 - Bus B
    //      [29:26] RT Timeout (RTTO) - Extra RT status word timeout above nominal in units of 4 us (0000 -14 us, 1111 -74
    //          us). Note: This extra time is also used as extra intermessage gap time if the GAP bit is set.
    //      [25:21] Second RT Address for RT-to-RT transfer (RTAD2) See table 357 for details on how to setup
    //          RTAD1,RTSA1,RTAD2,RTSA2,WCMC,TR
    //          for different transfer types.
    //          Note that bits 15:0 correspond to the (first)
    //          command word on the 1553 bus
    //      [20:16] Second RT Subaddress for RT-to-RT transfer (RTSA2)
    //      [15:11] RT Address (RTAD1)
    //      [10] Transmit/receive (TR)
    //      [9:5] RT Subaddress (RTSA1)
    //      [4:0] Word count/Mode code (WCMC)
    //-----------------------------------------------------
    uint32_t ctrl_word_1         : 32;
    //-----------------------------------------------------
    //          Data buffer pointer, 16-bit aligned.
    //          For write buffers, if bit 0 is set the received
    //          data is discarded and the pointer is ignored.
    //          This can be used for RT-to-RT transfers where
    //          the BC is not interested in the data transferred.
    //-----------------------------------------------------
    uint32_t data_pointer        : 32;
    //-----------------------------------------------------
    //      [31] Always written as 0
    //      [30:24] Reserved - Mask away on read for forward compatibility
    //      [23:16] RT 2 Status Bits (RT2ST) - Status bits from receiving RT in RT-to-RT transfer, otherwise 0
    //          Same bit pattern as for RTST below
    //      [15:8] RT Status Bits (RTST) - Status bits from RT (transmitting RT in RT-to-RT transfer)
    //          15 - Message error, 14 - Instrumentation bit or reserved bit set, 13 - Service request,
    //          12 - Broadcast command received, 11 - Busy bit, 10 - Subsystem flag, 9 - Dynamic bus control acceptance, 8 - Terminal
    //          flag
    //      [7:4] Retry count (RETCNT) - Number of retries performed
    //      [3] Reserved - Mask away on read for forward compatibility
    //      [2:0] Transfer status (TFRST) - Outcome of last try
    //          000 - Success (or dummy bit was set)
    //          001 - RT did not respond (transmitting RT in RT-to-RT transfer)
    //          010 - Receiving RT of RT-to-RT transfer did not respond
    //          011 - A responding RT:s status word had message error, busy, instrumentation or reserved bit set (*)
    //          100 - Protocol error (improperly timed data words, decoder error, wrong number of data words)
    //          101 - The transfer descriptor was invalid
    //          110 - Data buffer DMA timeout or error response
    //          111 - Transfer aborted due to loop back check failure
    //-----------------------------------------------------
    uint32_t result_word         : 32;

    uint32_t condition_word      : 32;
    uint32_t branch_address      : 32;
    uint32_t reserved_0          : 32;
    uint32_t reserved_1          : 32;
};

struct __attribute__ ((__packed__)) mkio_rt_subaddress_table
{
    //-----------------------------------------------------
    //      [31 : 19] Reserved - set to 0 for forward compatibility
    //      [18] Auto-wraparound enable (WRAP) - Enables a test mode for this subaddress, where transmit transfers send back the
    //          last received data. This is done by copying the finished transfer’s descriptor pointer to the transmit descriptor pointer
    //          address after each successful transfer.
    //          Note: If WRAP=1, you should not set TXSZ > RXSZ as this might cause reading beyond buffer end
    //      [17] Ignore data valid bit (IGNDV) - If this is ‘1’ then receive transfers will proceed (and overwrite the buffer) if the receive
    //          descriptor has the data valid bit set, instead of not responding to the request.
    //          This can be used for descriptor rings where you don’t care if the oldest data is overwritten.
    //      [16] Broadcast receive enable (BCRXEN) - Allow broadcast receive transfers to this subaddress
    //      [15] Receive enable (RXEN) - Allow receive transfers to this subaddress
    //      [14] Log receive transfers (RXLOG) - Log all receive transfers in event log ring (only used if RXEN=1)
    //      [13] Interrupt on receive transfers (RXIRQ) - Each receive transfer will cause an interrupt (only if also RXEN,RXLOG=1)
    //      [12 : 8] Maximum legal receive size (RXSZ) to this subaddress - in16-bit words, 0 means 32
    //      [7] Transmit enable (TXEN) - Allow transmit transfers from this subaddress
    //      [6] Log transmit transfers (TXLOG) - Log all transmit transfers in event log ring (only if also TXEN=1)
    //      [5] Interrupt on transmit transfers (TXIRQ) - Each transmit transfer will cause an interrupt (only if TXEN,TXLOG=1)
    //      [4 : 0] Maximum legal transmit size (TXSZ) from this subaddress - in 16-bit words, 0 means 32
    //-----------------------------------------------------
    uint32_t sa0_ctrl_word                     : 32;
    //-----------------------------------------------------
    //      16-byte aligned (0x3 to indicate invalid pointer)
    //-----------------------------------------------------
    uint32_t sa0_tx_descriptor_pointer         : 32;
    //-----------------------------------------------------
    //      16-byte aligned (0x3 to indicate invalid pointer)
    //-----------------------------------------------------
    uint32_t sa0_rx_descriptor_pointer         : 32;
    //-----------------------------------------------------
    uint32_t sa0_reserved                      : 32;


    //-----------------------------------------------------
    //  Add more sabaddress table entries, if necessary
    //    But not more than 32
    //-----------------------------------------------------
    uint32_t sa1_ctrl_word                     : 32;
    uint32_t sa1_tx_descriptor_pointer         : 32;
    uint32_t sa1_rx_descriptor_pointer         : 32;
    uint32_t sa1_reserved                      : 32;

    uint32_t sa2_ctrl_word                     : 32;
    uint32_t sa2_tx_descriptor_pointer         : 32;
    uint32_t sa2_rx_descriptor_pointer         : 32;
    uint32_t sa2_reserved                      : 32;

    uint32_t sa3_ctrl_word                     : 32;
    uint32_t sa3_tx_descriptor_pointer         : 32;
    uint32_t sa3_rx_descriptor_pointer         : 32;
    uint32_t sa3_reserved                      : 32;

    uint32_t sa4_ctrl_word                     : 32;
    uint32_t sa4_tx_descriptor_pointer         : 32;
    uint32_t sa4_rx_descriptor_pointer         : 32;
    uint32_t sa4_reserved                      : 32;

    uint32_t sa5_ctrl_word                     : 32;
    uint32_t sa5_tx_descriptor_pointer         : 32;
    uint32_t sa5_rx_descriptor_pointer         : 32;
    uint32_t sa5_reserved                      : 32;

    uint32_t sa6_ctrl_word                     : 32;
    uint32_t sa6_tx_descriptor_pointer         : 32;
    uint32_t sa6_rx_descriptor_pointer         : 32;
    uint32_t sa6_reserved                      : 32;

    uint32_t sa7_ctrl_word                     : 32;
    uint32_t sa7_tx_descriptor_pointer         : 32;
    uint32_t sa7_rx_descriptor_pointer         : 32;
    uint32_t sa7_reserved                      : 32;

    uint32_t sa8_ctrl_word                     : 32;
    uint32_t sa8_tx_descriptor_pointer         : 32;
    uint32_t sa8_rx_descriptor_pointer         : 32;
    uint32_t sa8_reserved                      : 32;

    uint32_t sa9_ctrl_word                     : 32;
    uint32_t sa9_tx_descriptor_pointer         : 32;
    uint32_t sa9_rx_descriptor_pointer         : 32;
    uint32_t sa9_reserved                      : 32;

    uint32_t sa10_ctrl_word                     : 32;
    uint32_t sa10_tx_descriptor_pointer         : 32;
    uint32_t sa10_rx_descriptor_pointer         : 32;
    uint32_t sa10_reserved                      : 32;

    uint32_t sa11_ctrl_word                     : 32;
    uint32_t sa11_tx_descriptor_pointer         : 32;
    uint32_t sa11_rx_descriptor_pointer         : 32;
    uint32_t sa11_reserved                      : 32;

    uint32_t sa12_ctrl_word                     : 32;
    uint32_t sa12_tx_descriptor_pointer         : 32;
    uint32_t sa12_rx_descriptor_pointer         : 32;
    uint32_t sa12_reserved                      : 32;

    uint32_t sa13_ctrl_word                     : 32;
    uint32_t sa13_tx_descriptor_pointer         : 32;
    uint32_t sa13_rx_descriptor_pointer         : 32;
    uint32_t sa13_reserved                      : 32;

    uint32_t sa14_ctrl_word                     : 32;
    uint32_t sa14_tx_descriptor_pointer         : 32;
    uint32_t sa14_rx_descriptor_pointer         : 32;
    uint32_t sa14_reserved                      : 32;

    uint32_t sa15_ctrl_word                     : 32;
    uint32_t sa15_tx_descriptor_pointer         : 32;
    uint32_t sa15_rx_descriptor_pointer         : 32;
    uint32_t sa15_reserved                      : 32;

    uint32_t sa16_ctrl_word                     : 32;
    uint32_t sa16_tx_descriptor_pointer         : 32;
    uint32_t sa16_rx_descriptor_pointer         : 32;
    uint32_t sa16_reserved                      : 32;

    uint32_t sa17_ctrl_word                     : 32;
    uint32_t sa17_tx_descriptor_pointer         : 32;
    uint32_t sa17_rx_descriptor_pointer         : 32;
    uint32_t sa17_reserved                      : 32;

    uint32_t sa18_ctrl_word                     : 32;
    uint32_t sa18_tx_descriptor_pointer         : 32;
    uint32_t sa18_rx_descriptor_pointer         : 32;
    uint32_t sa18_reserved                      : 32;

    uint32_t sa19_ctrl_word                     : 32;
    uint32_t sa19_tx_descriptor_pointer         : 32;
    uint32_t sa19_rx_descriptor_pointer         : 32;
    uint32_t sa19_reserved                      : 32;

    uint32_t sa20_ctrl_word                     : 32;
    uint32_t sa20_tx_descriptor_pointer         : 32;
    uint32_t sa20_rx_descriptor_pointer         : 32;
    uint32_t sa20_reserved                      : 32;

};

struct __attribute__ ((__packed__)) mkio_rt_descriptor
{
    //-----------------------------------------------------
    //      [31] Data valid (DV) - Should be set to 0 by software before and set to 1 by hardware after transfer.
    //          If DV=1 in the current receive descriptor before the receive transfer begins then a descriptor table error will
    //          be triggered. You can override this by setting the IGNDV bit in the subaddress table.
    //      [30] IRQ Enable override (IRQEN) - Log and IRQ after transfer regardless of SA control word settings
    //          Can be used for getting an interrupt when nearing the end of a descriptor list.
    //      [29 : 26] Reserved - Write 0 and mask out on read for forward compatibility
    //      [25 : 10] Transmission time tag (TTIME) - Set by the core to the value of the RT timer when the transfer finished.
    //      [9] Broadcast (BC) - Set by the core if the transfer was a broadcast transfer
    //      [8 : 3] Transfer size (SZ) - Count in 16-bit words (0-32)
    //      [2 : 0] Transfer result (TRES)
    //          000 = Success
    //          001 = Superseded (canceled because a new command was given on the other bus)
    //          010 = DMA error or memory timeout occurred
    //          011 = Protocol error (improperly timed data words or decoder error)
    //          100 = The busy bit or message error bit was set in the transmitted status word and no data was sent
    //          101 = Transfer aborted due to loop back checker error
    //-----------------------------------------------------
    uint32_t ctrl_status_word            : 32;
    //-----------------------------------------------------
    //      Data buffer pointer, 16-bit aligned
    //-----------------------------------------------------
    uint32_t data_pointer                : 32;
    //-----------------------------------------------------
    //      Pointer to next descriptor, 16-byte aligned
    //      or 0x0000003 to indicate end of list
    //-----------------------------------------------------
    uint32_t next_descriptor_pointer     : 32;
};

typedef enum mkio_bus
{
    MKIO_BUS_A  = 0b0,
    MKIO_BUS_B  = 0b1
} mkio_bus_t;

struct __attribute__ ((__packed__)) mkio_rt_sa_table
{
    uint32_t sa_ctrl_word                     : 32;
    uint32_t sa_tx_descriptor_pointer         : 32;
    uint32_t sa_rx_descriptor_pointer         : 32;
    uint32_t sa_reserved                      : 32;
};

struct mkio_instance
{
    uint32_t                                src_mkio_base_addr;
    uint32_t                                dst_mkio_base_addr;
    uint32_t*                               src_addr;
    uint32_t*                               dst_addr;
    struct mkio_bc_descriptor volatile *    bc_desr;
    struct mkio_rt_descriptor volatile *    rt_descr;
    uint32_t            size;
    mkio_bus_t          bus;
    struct mkio_rt_sa_table volatile *      rt_sa_tbl;
};

struct mkio_irqe
{
    bool    bceve;
    bool    bcde;
    bool    bcwke;
    bool    rteve;
    bool    rtde;
    bool    rttee;
    bool    bmde;
    bool    bmtoe;
    bool    all;
};

typedef enum mkio_bc_schedule_state
{
    MKIO_BCSL_SCST_STOPPED        = 0b000,
    MKIO_BCSL_SCST_EXEC           = 0b001,
    MKIO_BCSL_SCST_WAIT_SLOT      = 0b010,
    MKIO_BCSL_SCST_SUSPEND        = 0b011,
    MKIO_BCSL_SCST_WAIT_EXT_TRIG  = 0b100
} mkio_bc_schedule_state_t;

#define MKIO_WC_MAX     32

#define MKIO_TIMEOUT    100

void mkio_prepare_rt_descr(uint32_t base_addr, uint32_t* data_ptr, uint32_t size, struct mkio_rt_descriptor volatile* mkio_rt_rx_descriptor, struct mkio_rt_sa_table volatile* rt_sa_tbl);
void mkio_prepare_bc_descr(uint32_t base_addr, uint32_t* data_ptr, uint32_t size, struct mkio_bc_descriptor volatile* descr_ptr, mkio_bus_t bus, uint32_t TR);
void mkio_bc_run_schedule (uint32_t base_address);
void mkio_rt_run_schedule (uint32_t base_address);
void mkio_set_bcrd(uint32_t base_address, uint32_t bc_irq_ring_addr);
void mkio_enable_all_irq(uint32_t base_address);
bool mkio_wait_bc_schedule_state(uint32_t base_address, mkio_bc_schedule_state_t sched_state);





#endif /* end of include guard: GP_TIMERS_H */
