/*
 * hscb.h
 *
 *  Created on: Jul 19, 2018
 *      Author: r.galiulin
 */

#ifndef REGS_HSCB_H
#define REGS_HSCB_H


#include <platform/common_macros/common_macros.h>

/**
 * \defgroup regs_hscb HSCB controller
 * \ingroup regs
 *
 * Register offsets and constants
 * \code{.c}
 * #include <regs/regs_hscb.h>
 * \endcode
 *
 * @{
 */

/** HSCB register offsets */
BEGIN_ENUM( HSCB_REG )
    /* HSCB System */
    DECLARE_ENUM_VAL(   HSCB_ID,                0x000 ) /**< Identifikator ustroystva. Read only */
    DECLARE_ENUM_VAL(   HSCB_VERSION,           0x004 ) /**< Nomer versii ustroystva. Read only */
    DECLARE_ENUM_VAL(   HSCB_SW_RESET,          0x008 ) /**< Programmnyy sbros (zapis' 1 sbrasyvayet ustroystvo, kogda snova ustanavlivayetsya 0 - sbros vypolnen) */
    DECLARE_ENUM_VAL(   HSCB_SETTINGS,          0x00C ) /**< Registr nastroyek */
    DECLARE_ENUM_VAL(   HSCB_STATUS,            0x010 ) /**< Registr sostoyaniya ustroystva. Read only */
    DECLARE_ENUM_VAL(   HSCB_IRQ_MASK,          0x014 ) /**< Maska preryvaniy */
    DECLARE_ENUM_VAL(   HSCB_TRANS_CLK,         0x018 ) /**< Koeffitsiyent chastoty peredatchika. Read only */
    DECLARE_ENUM_VAL(   HSCB_TIMINGS,           0x01C ) /**< Nastroyka vremennykh parametrov kontrollera */
    /* Time-code */
    DECLARE_ENUM_VAL(   HSCB_INTERVAL,          0x020 ) /**< Interval mezhdu vydachey Time-kodov (v taktakh sistemnoy chastoty) Po umolchaniyu ustanovlen interval 10 mks (dlya chastoty 200 MGts)c */
    DECLARE_ENUM_VAL(   HSCB_TIME_CODE_INNER,   0x024 ) /**< Vnutrenneye (sistemnoye) znacheniye time-code (znachimy tol'ko [7:0]) */
    DECLARE_ENUM_VAL(   HSCB_TIME_CODE_OUTER,   0x028 ) /**< Znacheniye time-code poluchennoye ot drugogo ustroystva: [7:0] - new_code - poluchennoye znacheniye, [15:8] - prev_code - predydushcheye poluchennoye znacheniye */
    /* Word-based data exchange */
//  DECLARE_ENUM_VAL(   HSCB_DATA_SEND_H,       0x02C ) /**< . Read only */
//  DECLARE_ENUM_VAL(   HSCB_DATA_SEND_L,       0x030 ) /**<  */
//  DECLARE_ENUM_VAL(   HSCB_DSTRB_SEND,        0x034 ) /**<  */
//  DECLARE_ENUM_VAL(   HSCB_SEND_LEN,          0x038 ) /**<  */
//  DECLARE_ENUM_VAL(   HSCB_DATA_REC_H,        0x03C ) /**< . Read only */
//  DECLARE_ENUM_VAL(   HSCB_DATA_REC_L,        0x040 ) /**< . Read only */
//  DECLARE_ENUM_VAL(   HSCB_DSTRB_REC,         0x044 ) /**< . Read only */
//  DECLARE_ENUM_VAL(   HSCB_REC_LEN,           0x048 ) /**< . Read only */
    /* RMAP protocol */
    DECLARE_ENUM_VAL(   HSCB_RMAP_PARAMS,       0x04C ) /**< Registr nastroyki parametrov RMAP */
    DECLARE_ENUM_VAL(   HSCB_RMAP_STATUS,       0x050 ) /**< Status soyedineniya RMAP. Read only */
    DECLARE_ENUM_VAL(   HSCB_RMAP_C_STATUS,     0x054 ) /**< Status predydushchego soyedineniya RMAP. Read only */
    /* AXI-DMA System */
    DECLARE_ENUM_VAL(   HSCB_ADMA_ID,           0x800 ) /**< Identifikator ustroystva */
    DECLARE_ENUM_VAL(   HSCB_ADMA_REV,          0x804 ) /**< Nomer revizii i kolichestvo kanalov chteniya i zapisi DMA */
    DECLARE_ENUM_VAL(   HSCB_ADMA_SW_RESET,     0x808 ) /**< Programmnyy sbros */
    DECLARE_ENUM_VAL(   HSCB_ADMA_CH_STATUS,    0x80C ) /**< Status kanalov DMA. */
    /* AXI */
    DECLARE_ENUM_VAL(   HSCB_ARLEN,             0x810 ) /**< Znacheniye ARLEN dlya tranzaktsiy po AXI (znachimy tol'ko mladshiye chetyre bita - [3:0]) */
    DECLARE_ENUM_VAL(   HSCB_AWLEN,             0x814 ) /**< Znacheniye AWLEN dlya tranzaktsiy po AXI (znachimy tol'ko mladshiye chetyre bita - [3:0]) */
    DECLARE_ENUM_VAL(   HSCB_BRESP,             0x818 ) /**< Znacheniye BRESP dlya tranzaktsiy po AXI (znachimy tol'ko mladshiye dva bita - [1:0]). Read only */
    DECLARE_ENUM_VAL(   HSCB_RAXI_ERR_ADDR,     0x81C ) /**< Adres AXI-tranzaktsii chteniya, vyzvavshey oshibku na shine AXI. Read only */
    DECLARE_ENUM_VAL(   HSCB_ARCACHE,           0x820 ) /**< Znacheniye ARCACHE dlya tranzaktsiy po AXI (znachimy tol'ko mladshiye chetyre bita - [3:0]) */
    DECLARE_ENUM_VAL(   HSCB_AWCACHE,           0x824 ) /**< Znacheniye AWCACHE dlya tranzaktsiy po AXI (znachimy tol'ko mladshiye chetyre bita - [3:0]) */
    DECLARE_ENUM_VAL(   HSCB_ARPROT,            0x828 ) /**< Znacheniye ARPROT dlya tranzaktsiy po AXI (znachimy tol'ko mladshiye tri bita - [2:0]) */
    DECLARE_ENUM_VAL(   HSCB_AWPROT,            0x82C ) /**< Znacheniye AWPROT dlya tranzaktsiy po AXI (znachimy tol'ko mladshiye tri bita - [2:0]) */
    DECLARE_ENUM_VAL(   HSCB_ARLOCK,            0x830 ) /**< Znacheniye ARLOCK dlya tranzaktsiy po AXI (znachimy tol'ko mladshiye dva bita - [1:0]) */
    DECLARE_ENUM_VAL(   HSCB_AWLOCK,            0x834 ) /**< Znacheniye AWLOCK dlya tranzaktsiy po AXI (znachimy tol'ko mladshiye dva bita - [1:0]) */
    DECLARE_ENUM_VAL(   HSCB_ARBURST,           0x838 ) /**< Znacheniye ARBURST dlya tranzaktsiy po AXI (znachimy tol'ko mladshiye dva bita - [3:0]) */
    DECLARE_ENUM_VAL(   HSCB_AWBURST,           0x83C ) /**< Znacheniye AWBURST dlya tranzaktsiy po AXI (znachimy tol'ko mladshiye dva bita - [1:0]) */
    /* Read DMA */
    DECLARE_ENUM_VAL(   HSCB_RDMA_SETTINGS,     0x900 ) /**< Nastroyki DMA kanala zapisi */
    DECLARE_ENUM_VAL(   HSCB_RDMA_STATUS,       0x904 ) /**< Status kanala zapisi. Read only */
    DECLARE_ENUM_VAL(   HSCB_RDMA_SYS_ADDR,     0x908 ) /**< Adres tablitsy deskriptorov dlya chteniya iz pamyati */
    DECLARE_ENUM_VAL(   HSCB_RDMA_TBL_SIZE,     0x90C ) /**< Razmer tablitsy deskriptorov (po umolchaniyu 2 kbayta) */
    DECLARE_ENUM_VAL(   HSCB_RDMA_ADDR,         0x910 ) /**< Tekushchiy adres */
    DECLARE_ENUM_VAL(   HSCB_RDMA_LEN,          0x914 ) /**< Dlina bloka dannykh */
    DECLARE_ENUM_VAL(   HSCB_RDMA_DESC_ADDR,    0x918 ) /**< Adres tekushchego deskriptora */
    /* Write DMA */
    DECLARE_ENUM_VAL(   HSCB_WDMA_SETTINGS,     0xA00 ) /**< Nastroyki DMA kanala chteniya */
    DECLARE_ENUM_VAL(   HSCB_WDMA_STATUS,       0xA04 ) /**< Status kanala chteniya. Read only */
    DECLARE_ENUM_VAL(   HSCB_WDMA_SYS_ADDR,     0xA08 ) /**< Adres tablitsy deskriptorov dlya chteniya iz pamyati */
    DECLARE_ENUM_VAL(   HSCB_WDMA_TBL_SIZE,     0xA0C ) /**< Razmer tablitsy deskriptorov (po umolchaniyu 2 kbayta) */
    DECLARE_ENUM_VAL(   HSCB_WDMA_ADDR,         0xA10 ) /**< Tekushchiy adres */
    DECLARE_ENUM_VAL(   HSCB_WDMA_LEN,          0xA14 ) /**< Dlina bloka dannykh */
    DECLARE_ENUM_VAL(   HSCB_WDMA_DESC_ADDR,    0xA18 ) /**< Adres tekushchego deskriptora */
END_ENUM( HSCB_REG )

/** HSCB register reset values */
BEGIN_ENUM( HSCB_REG_DFLT )
    /* HSCB System */
    DECLARE_ENUM_VAL(   HSCB_ID_DFLT,               0x43425348 )
    DECLARE_ENUM_VAL(   HSCB_VERSION_DFLT,          0x00000101 )
    DECLARE_ENUM_VAL(   HSCB_SW_RESET_DFLT,         0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_SETTINGS_DFLT,         0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_STATUS_DFLT,           0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_IRQ_MASK_DFLT,         0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_TRANS_CLK_DFLT,        0x00C81414 )
    DECLARE_ENUM_VAL(   HSCB_TIMINGS_DFLT,          0xC8A00500 )
    /* Time-code */
    DECLARE_ENUM_VAL(   HSCB_INTERVAL_DFLT,         0x000007D0 )
    DECLARE_ENUM_VAL(   HSCB_TIME_CODE_INNER_DFLT,  0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_TIME_CODE_OUTER_DFLT,  0x00000000 )
    /* Word-based data exchange */
//  DECLARE_ENUM_VAL(   HSCB_DATA_SEND_H_DFLT,      0xXXXXXXXX )
//  DECLARE_ENUM_VAL(   HSCB_DATA_SEND_L_DFLT,      0xXXXXXXXX )
//  DECLARE_ENUM_VAL(   HSCB_DSTRB_SEND_DFLT,       0xXXXXXXXX )
//  DECLARE_ENUM_VAL(   HSCB_SEND_LEN_DFLT,         0xXXXXXXXX )
//  DECLARE_ENUM_VAL(   HSCB_DATA_REC_H_DFLT,       0xXXXXXXXX )
//  DECLARE_ENUM_VAL(   HSCB_DATA_REC_L_DFLT,       0xXXXXXXXX )
//  DECLARE_ENUM_VAL(   HSCB_DSTRB_REC_DFLT,        0xXXXXXXXX )
//  DECLARE_ENUM_VAL(   HSCB_REC_LEN_DFLT,          0xXXXXXXXX )
    /* RMAP protocol */
    DECLARE_ENUM_VAL(   HSCB_RMAP_PARAMS_DFLT,      0x00017799 )
    DECLARE_ENUM_VAL(   HSCB_RMAP_STATUS_DFLT,      0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_RMAP_C_STATUS_DFLT,    0x00000000 )
    /* AXI-DMA System */
    DECLARE_ENUM_VAL(   HSCB_ADMA_ID_DFLT,          0x414d4441 )
    DECLARE_ENUM_VAL(   HSCB_ADMA_REV_DFLT,         0x00000011 )
    DECLARE_ENUM_VAL(   HSCB_ADMA_SW_RESET_DFLT,    0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_ADMA_CH_STATUS_DFLT,   0x00000000 )
    /* AXI */
    DECLARE_ENUM_VAL(   HSCB_ARLEN_DFLT,            0x0000000F )
    DECLARE_ENUM_VAL(   HSCB_AWLEN_DFLT,            0x0000000F )
    DECLARE_ENUM_VAL(   HSCB_BRESP_DFLT,            0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_RAXI_ERR_ADDR_DFLT,    0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_ARCACHE_DFLT,          0x00000003 )
    DECLARE_ENUM_VAL(   HSCB_AWCACHE_DFLT,          0x00000003 )
    DECLARE_ENUM_VAL(   HSCB_ARPROT_DFLT,           0x00000002 )
    DECLARE_ENUM_VAL(   HSCB_AWPROT_DFLT,           0x00000002 )
    DECLARE_ENUM_VAL(   HSCB_ARLOCK_DFLT,           0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_AWLOCK_DFLT,           0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_ARBURST_DFLT,          0x00000001 )
    DECLARE_ENUM_VAL(   HSCB_AWBURST_DFLT,          0x00000001 )
    /* Read DMA */
    DECLARE_ENUM_VAL(   HSCB_RDMA_SETTINGS_DFLT,    0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_RDMA_STATUS_DFLT,      0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_RDMA_SYS_ADDR_DFLT,    0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_RDMA_TBL_SIZE_DFLT,    0x00000800 )
    DECLARE_ENUM_VAL(   HSCB_RDMA_ADDR_DFLT,        0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_RDMA_LEN_DFLT,         0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_RDMA_DESC_ADDR_DFLT,   0x00000000 )
    /* Write DMA */
    DECLARE_ENUM_VAL(   HSCB_WDMA_SETTINGS_DFLT,    0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_WDMA_STATUS_DFLT,      0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_WDMA_SYS_ADDR_DFLT,    0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_WDMA_TBL_SIZE_DFLT,    0x00000800 )
    DECLARE_ENUM_VAL(   HSCB_WDMA_ADDR_DFLT,        0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_WDMA_LEN_DFLT,         0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_WDMA_DESC_ADDR_DFLT,   0x00000000 )
END_ENUM( HSCB_REG_DFLT )

/**
 * @}
 */

#endif /* REGS_HSCB_H */
