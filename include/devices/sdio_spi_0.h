
#ifndef SPI_H
#define SPI_H

/**
 * \defgroup devices_spi
 * \ingroup devices
 * \code{.c}
 * #include <devices/sdi_spi.h>
 * \endcode
 *
 * TODO: Insert usage example here 
 *
 * \addtogroup devices_spi
 *
 * @{
 */


/**
 * [Init ssp]
 */
  ssp_init(); 
/**
 * [go_dmac WREN]
 * @param writeenablecmd 
 * @param dummyarray     
 * @param [amount bytes]         
 */
  go_dmac(&writeenablecmd, dummyarray, 1);
  
/**
 * [go_dmac sectorerase]
 * @param writeenablecmd 
 * @param dummyarray     
 * @param [amount bytes]         
 */
  go_dmac(&sectorerase, dummyarray, 4);
/**  
 * [go_dmac ]
 * @param readstatusregistercmd [description]
 * @param dummyarray            [description]
 * @param [amount bytes]                [description]
 */
    go_dmac(&readstatusregistercmd , dummyarray , 3);
     
/**
 * [go_dmac Write Flash]
 * @param writeenablecmd 
 * @param dummyarray    
 * @param [amount bytes]         [description]
 */
    go_dmac(&writeenablecmd, dummyarray, 1);
   
/**
 * [go_dmac Run KDMAC Both Channels For Simultaneous SPI Transfer]
 * @param srcarray   
 * @param dummyarray 
 * @param [amount bytes]     
 */
    go_dmac(srcarray , dummyarray , 260);
   
/**
 * [go_dmac Wait Until Page Program Instruction Complete ]
 * @param readstatusregistercmd 
 * @param dummyarray            
 * @param [amount bytes]                
 */
      go_dmac(&readstatusregistercmd , dummyarray , 3);
/**
 * [go_dmac dWrite Disable Instruction]
 * @param writedisablecmd 
 * @param dummyarray      
 * @param [amount bytes]          
 */
    go_dmac(&writedisablecmd , dummyarray , 1);
  /**
   * [go_dmac Read Flash]
   * @param readcmd 
   * @param dstarray 
   * @param [amount bytes]  
   */

    go_dmac(readcmd , dstarray , 260);
    

/**
 * @}
 */


#endif




