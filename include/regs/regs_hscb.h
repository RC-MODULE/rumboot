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

BEGIN_ENUM( HSCB_SW_RESET_FIELD )
    DECLARE_ENUM_VAL(   HSCB_SW_RESET_RST_i,        0 )
    DECLARE_ENUM_VAL(   HSCB_SW_RESET_RST_n,        1 )
    DECLARE_ENUM_VAL(   HSCB_SW_RESET_RST_mask,     FIELD_MASK32( HSCB_SW_RESET_RST_i, HSCB_SW_RESET_RST_n ) )
END_ENUM( HSCB_SW_RESET_FIELD )

BEGIN_ENUM( HSCB_SETTINGS_FIELD )
    DECLARE_ENUM_VAL(   HSCB_SETTINGS_EN_HSCB_i,        0 )         /**< Vklyucheniye Space Wire interfeysa */
    DECLARE_ENUM_VAL(   HSCB_SETTINGS_EN_HSCB_n,        1 )
    DECLARE_ENUM_VAL(   HSCB_SETTINGS_EN_HSCB_mask,     FIELD_MASK32( HSCB_SETTINGS_EN_HSCB_i, HSCB_SETTINGS_EN_HSCB_n ) )
    DECLARE_ENUM_VAL(   HSCB_SETTINGS_TX_ENDIAN_i,      1 )
    DECLARE_ENUM_VAL(   HSCB_SETTINGS_TX_ENDIAN_n,      1 )
    DECLARE_ENUM_VAL(   HSCB_SETTINGS_TX_ENDIAN_mask,   FIELD_MASK32( HSCB_SETTINGS_TX_ENDIAN_i, HSCB_SETTINGS_TX_ENDIAN_n ) )
    DECLARE_ENUM_VAL(   HSCB_SETTINGS_RX_ENDIAN_i,      2 )
    DECLARE_ENUM_VAL(   HSCB_SETTINGS_RX_ENDIAN_n,      1 )
    DECLARE_ENUM_VAL(   HSCB_SETTINGS_RX_ENDIAN_mask,   FIELD_MASK32( HSCB_SETTINGS_RX_ENDIAN_i, HSCB_SETTINGS_RX_ENDIAN_n ) )
    DECLARE_ENUM_VAL(   HSCB_SETTINGS_TIME_MODE_i,      3 )
    DECLARE_ENUM_VAL(   HSCB_SETTINGS_TIME_MODE_n,      1 )
    DECLARE_ENUM_VAL(   HSCB_SETTINGS_TIME_MODE_mask,   FIELD_MASK32( HSCB_SETTINGS_TIME_MODE_i, HSCB_SETTINGS_TIME_MODE_n ) )
    DECLARE_ENUM_VAL(   HSCB_SETTINGS_LOOP_BACK_i,      5 )         /**< Vklyuchen rezhim petli (loopback) */
    DECLARE_ENUM_VAL(   HSCB_SETTINGS_LOOP_BACK_n,      1 )
    DECLARE_ENUM_VAL(   HSCB_SETTINGS_LOOP_BACK_mask,   FIELD_MASK32( HSCB_SETTINGS_LOOP_BACK_i, HSCB_SETTINGS_LOOP_BACK_n ) )
    DECLARE_ENUM_VAL(   HSCB_SETTINGS_EN_RMAP_i,        6 )         /**< Obrabotka RMAP */
    DECLARE_ENUM_VAL(   HSCB_SETTINGS_EN_RMAP_n,        1 )
    DECLARE_ENUM_VAL(   HSCB_SETTINGS_EN_RMAP_mask,     FIELD_MASK32( HSCB_SETTINGS_EN_RMAP_i, HSCB_SETTINGS_EN_RMAP_n ) )
    DECLARE_ENUM_VAL(   HSCB_SETTINGS_EN_TRIM_CLK_i,    7 )         /**< Avtopodstroyka chastoty */
    DECLARE_ENUM_VAL(   HSCB_SETTINGS_EN_TRIM_CLK_n,    1 )
    DECLARE_ENUM_VAL(   HSCB_SETTINGS_EN_TRIM_CLK_mask, FIELD_MASK32( HSCB_SETTINGS_EN_TRIM_CLK_i, HSCB_SETTINGS_EN_TRIM_CLK_n ) )
    DECLARE_ENUM_VAL(   HSCB_SETTINGS_RX_FIX_EN_i,      8 )         /**< Vosstanovleniye taktovogo signala priyomnika iz prinimayemykh dannykh. Ustoychivyy priyom vozmozhen tol'ko yesli chastote priyomnika v 4 ili boleye raz prevyshayet chastotu peredatchika */
    DECLARE_ENUM_VAL(   HSCB_SETTINGS_RX_FIX_EN_n,      1 )
    DECLARE_ENUM_VAL(   HSCB_SETTINGS_RX_FIX_EN_mask,   FIELD_MASK32( HSCB_SETTINGS_RX_FIX_EN_i, HSCB_SETTINGS_RX_FIX_EN_n ) )
END_ENUM( HSCB_SETTINGS_FIELD )
/** Poryadok bayt dannykh priyomnogo/peredayushchego kanala*/
BEGIN_ENUM( hscb_rtx_endian_t )
    DECLARE_ENUM_VAL(   hscb_rtx_endian_big,    1 )
    DECLARE_ENUM_VAL(   hscb_rtx_endian_little, 0 )
END_ENUM( hscb_rtx_endian_t )
/** Rezhim peredachi Time-code */
BEGIN_ENUM( hscb_time_mode_t )
    DECLARE_ENUM_VAL(   hscb_time_mode_slave,   0 )                 /**< priyomnik */
    DECLARE_ENUM_VAL(   hscb_time_mode_master,  1 )                 /**< peredatchik */
END_ENUM( hscb_time_mode_t )

BEGIN_ENUM( HSCB_STATUS_FIELD )
    DECLARE_ENUM_VAL(   HSCB_STATUS_DISCONNECT_ERR_i,       0 )     /**< Poterya svyazi po kanalu (libo ne udalos' podklyuchit'sya, libo prevyshen interval mezhdu simvolami) */
    DECLARE_ENUM_VAL(   HSCB_STATUS_DISCONNECT_ERR_n,       1 )
    DECLARE_ENUM_VAL(   HSCB_STATUS_DISCONNECT_ERR_mask,    FIELD_MASK32( HSCB_STATUS_DISCONNECT_ERR_i, HSCB_STATUS_DISCONNECT_ERR_n ) )
    DECLARE_ENUM_VAL(   HSCB_STATUS_PARITY_ERR_i,           1 )     /**< Oshibka bita chetnosti */
    DECLARE_ENUM_VAL(   HSCB_STATUS_PARITY_ERR_n,           1 )
    DECLARE_ENUM_VAL(   HSCB_STATUS_PARITY_ERR_mask,        FIELD_MASK32( HSCB_STATUS_PARITY_ERR_i, HSCB_STATUS_PARITY_ERR_n ) )
    DECLARE_ENUM_VAL(   HSCB_STATUS_ESC_ERR_i,              2 )     /**< Poluchena oshibochnaya posledovatel'nost' simvolov */
    DECLARE_ENUM_VAL(   HSCB_STATUS_ESC_ERR_n,              1 )
    DECLARE_ENUM_VAL(   HSCB_STATUS_ESC_ERR_mask,           FIELD_MASK32( HSCB_STATUS_ESC_ERR_i, HSCB_STATUS_ESC_ERR_n ) )
    DECLARE_ENUM_VAL(   HSCB_STATUS_TIME_CODE_i,            5 )     /**< Poluchen time-code */
    DECLARE_ENUM_VAL(   HSCB_STATUS_TIME_CODE_n,            1 )
    DECLARE_ENUM_VAL(   HSCB_STATUS_TIME_CODE_mask,         FIELD_MASK32( HSCB_STATUS_TIME_CODE_i, HSCB_STATUS_TIME_CODE_n ) )
    DECLARE_ENUM_VAL(   HSCB_STATUS_TIME_CODE_ERR_i,        6 )     /**< Poluchennyy time-code ne sootvetstvuyet ozhidayemomu */
    DECLARE_ENUM_VAL(   HSCB_STATUS_TIME_CODE_ERR_n,        1 )
    DECLARE_ENUM_VAL(   HSCB_STATUS_TIME_CODE_ERR_mask,     FIELD_MASK32( HSCB_STATUS_TIME_CODE_ERR_i, HSCB_STATUS_TIME_CODE_ERR_n ) )
    DECLARE_ENUM_VAL(   HSCB_STATUS_TX_CREDIT_ERR_i,        7 )     /**< Prevyshen dopustimyy razmer bufera priyomnika (chislo poluchennykh FCT prevyshayet dopustimoye po standartu) */
    DECLARE_ENUM_VAL(   HSCB_STATUS_TX_CREDIT_ERR_n,        1 )
    DECLARE_ENUM_VAL(   HSCB_STATUS_TX_CREDIT_ERR_mask,     FIELD_MASK32( HSCB_STATUS_TX_CREDIT_ERR_i, HSCB_STATUS_TX_CREDIT_ERR_n ) )
    DECLARE_ENUM_VAL(   HSCB_STATUS_RX_CREDIT_ERR_i,        8 )     /**< Perepolneniye priyomnogo bufera (kolichestvo prinyatykh simvolov prevyshayet znacheniye, sootvetstvuyushcheye chislu otpravlennykh FCT) */
    DECLARE_ENUM_VAL(   HSCB_STATUS_RX_CREDIT_ERR_n,        1 )
    DECLARE_ENUM_VAL(   HSCB_STATUS_RX_CREDIT_ERR_mask,     FIELD_MASK32( HSCB_STATUS_RX_CREDIT_ERR_i, HSCB_STATUS_RX_CREDIT_ERR_n ) )
    DECLARE_ENUM_VAL(   HSCB_STATUS_ACTIVE_LINK_i,          9 )     /**< Perepolneniye priyomnogo bufera (kolichestvo prinyatykh simvolov prevyshayet znacheniye, sootvetstvuyushcheye chislu otpravlennykh FCT) */
    DECLARE_ENUM_VAL(   HSCB_STATUS_ACTIVE_LINK_n,          1 )
    DECLARE_ENUM_VAL(   HSCB_STATUS_ACTIVE_LINK_nmask,      FIELD_MASK32( HSCB_STATUS_ACTIVE_LINK_i, HSCB_STATUS_ACTIVE_LINK_n ) )
    DECLARE_ENUM_VAL(   HSCB_STATUS_TX_FREE_BYTE_CNT_i,     12 )
    DECLARE_ENUM_VAL(   HSCB_STATUS_TX_FREE_BYTE_CNT_n,     6 )
    DECLARE_ENUM_VAL(   HSCB_STATUS_TX_FREE_BYTE_CNT_mask,  FIELD_MASK32( HSCB_STATUS_TX_FREE_BYTE_CNT_i, HSCB_STATUS_TX_FREE_BYTE_CNT_n ) )
    DECLARE_ENUM_VAL(   HSCB_STATUS_RX_BYTE_CNT_i,          20 )
    DECLARE_ENUM_VAL(   HSCB_STATUS_RX_BYTE_CNT_n,          6 )
    DECLARE_ENUM_VAL(   HSCB_STATUS_RX_BYTE_CNT_mask,       FIELD_MASK32( HSCB_STATUS_RX_BYTE_CNT_i, HSCB_STATUS_RX_BYTE_CNT_n ) )
    DECLARE_ENUM_VAL(   HSCB_STATUS_RX_FIFO_EMPTY_i,        26 )
    DECLARE_ENUM_VAL(   HSCB_STATUS_RX_FIFO_EMPTY_n,        1 )
    DECLARE_ENUM_VAL(   HSCB_STATUS_RX_FIFO_EMPTY_mask,     FIELD_MASK32( HSCB_STATUS_RX_FIFO_EMPTY_i, HSCB_STATUS_RX_FIFO_EMPTY_n ) )
    DECLARE_ENUM_VAL(   HSCB_STATUS_STATE_i,                28 )
    DECLARE_ENUM_VAL(   HSCB_STATUS_STATE_n,                2 )
    DECLARE_ENUM_VAL(   HSCB_STATUS_STATE_mask,             FIELD_MASK32( HSCB_STATUS_STATE_i, HSCB_STATUS_STATE_n ) )
END_ENUM( HSCB_STATUS_FIELD )

BEGIN_ENUM( HSCB_IRQ_MASK_FIELD )
    DECLARE_ENUM_VAL(   HSCB_IRQ_MASK_DISCONNECT_ERR_i,     0 )     /**< Maska preryvaniya poteri svyazi po kanalu */
    DECLARE_ENUM_VAL(   HSCB_IRQ_MASK_DISCONNECT_ERR_n,     1 )
    DECLARE_ENUM_VAL(   HSCB_IRQ_MASK_DISCONNECT_ERR_mask,  FIELD_MASK32( HSCB_IRQ_MASK_DISCONNECT_ERR_i, HSCB_IRQ_MASK_DISCONNECT_ERR_n ) )
    DECLARE_ENUM_VAL(   HSCB_IRQ_MASK_PARITY_ERR_i,         1 )     /**< Maska preryvaniya ob oshibke bita chetnosti */
    DECLARE_ENUM_VAL(   HSCB_IRQ_MASK_PARITY_ERR_n,         1 )
    DECLARE_ENUM_VAL(   HSCB_IRQ_MASK_PARITY_ERR_mask,      FIELD_MASK32( HSCB_IRQ_MASK_PARITY_ERR_i, HSCB_IRQ_MASK_PARITY_ERR_n ) )
    DECLARE_ENUM_VAL(   HSCB_IRQ_MASK_ESC_ERR_i,            2 )     /**< Maska preryvaniya oshibochnoy posledovatel'nost' simvolov */
    DECLARE_ENUM_VAL(   HSCB_IRQ_MASK_ESC_ERR_n,            1 )
    DECLARE_ENUM_VAL(   HSCB_IRQ_MASK_ESC_ERR_mask,         FIELD_MASK32( HSCB_IRQ_MASK_ESC_ERR_i, HSCB_IRQ_MASK_ESC_ERR_n ) )
    DECLARE_ENUM_VAL(   HSCB_IRQ_MASK_RX_WORD_i,            3 )     /**< Maska preryvaniya o priyome slova pri rabote bez DMA */
    DECLARE_ENUM_VAL(   HSCB_IRQ_MASK_RX_WORD_n,            1 )
    DECLARE_ENUM_VAL(   HSCB_IRQ_MASK_RX_WORD_mask,         FIELD_MASK32( HSCB_IRQ_MASK_RX_WORD_i, HSCB_IRQ_MASK_RX_WORD_n ) )
    DECLARE_ENUM_VAL(   HSCB_IRQ_MASK_TX_WORD_i,            4 )     /**< Maska preryvaniya o peredache slova pri rabote bez DMA */
    DECLARE_ENUM_VAL(   HSCB_IRQ_MASK_TX_WORD_n,            1 )
    DECLARE_ENUM_VAL(   HSCB_IRQ_MASK_TX_WORD_mask,         FIELD_MASK32( HSCB_IRQ_MASK_TX_WORD_i, HSCB_IRQ_MASK_TX_WORD_n ) )
    DECLARE_ENUM_VAL(   HSCB_IRQ_MASK_TIME_CODE_i,          5 )     /**< Maska preryvaniya o priyome time-code */
    DECLARE_ENUM_VAL(   HSCB_IRQ_MASK_TIME_CODE_n,          1 )
    DECLARE_ENUM_VAL(   HSCB_IRQ_MASK_TIME_CODE_mask,         FIELD_MASK32( HSCB_IRQ_MASK_TIME_CODE_i, HSCB_IRQ_MASK_TIME_CODE_n ) )
    DECLARE_ENUM_VAL(   HSCB_IRQ_MASK_TIME_CODE_ERR_i,      6 )     /**< Maska preryvaniya ob oshibke time-code */
    DECLARE_ENUM_VAL(   HSCB_IRQ_MASK_TIME_CODE_ERR_n,      1 )
    DECLARE_ENUM_VAL(   HSCB_IRQ_MASK_TIME_CODE_ERR_mask,   FIELD_MASK32( HSCB_IRQ_MASK_TIME_CODE_ERR_i, HSCB_IRQ_MASK_TIME_CODE_ERR_n ) )
    DECLARE_ENUM_VAL(   HSCB_IRQ_MASK_TX_CREDIT_ERR_i,      7 )     /**< Maska preryvaniya o prevyshenii dopustimyy razmer bufera priyomnika */
    DECLARE_ENUM_VAL(   HSCB_IRQ_MASK_TX_CREDIT_ERR_n,      1 )
    DECLARE_ENUM_VAL(   HSCB_IRQ_MASK_TX_CREDIT_ERR_mask,   FIELD_MASK32( HSCB_IRQ_MASK_TX_CREDIT_ERR_i, HSCB_IRQ_MASK_TX_CREDIT_ERR_n ) )
    DECLARE_ENUM_VAL(   HSCB_IRQ_MASK_RX_CREDIT_ERR_i,      8 )     /**< Maska preryvaniya o perepolnenii priyomnogo bufera */
    DECLARE_ENUM_VAL(   HSCB_IRQ_MASK_RX_CREDIT_ERR_n,      1 )
    DECLARE_ENUM_VAL(   HSCB_IRQ_MASK_RX_CREDIT_ERR_mask,   FIELD_MASK32( HSCB_IRQ_MASK_RX_CREDIT_ERR_i, HSCB_IRQ_MASK_RX_CREDIT_ERR_n ) )
    DECLARE_ENUM_VAL(   HSCB_IRQ_MASK_ACTIVE_LINK_i,        9 )     /**< Maska preryvaniya ob uspeshnom ustanovlenii soyedineniya */
    DECLARE_ENUM_VAL(   HSCB_IRQ_MASK_ACTIVE_LINK_n,        1 )
    DECLARE_ENUM_VAL(   HSCB_IRQ_MASK_ACTIVE_LINK_mask,     FIELD_MASK32( HSCB_IRQ_MASK_ACTIVE_LINK_i, HSCB_IRQ_MASK_ACTIVE_LINK_n ) )
END_ENUM( HSCB_IRQ_MASK_FIELD )

BEGIN_ENUM( HSCB_TRANS_CLK_FIELD )
    DECLARE_ENUM_VAL(   HSCB_TRANS_CLK_FREQ_i,          0 )     /**< Koeffitsiyent chastoty peredatchika (reguliruyet tekushchuyu skorost' peredachi) */
    DECLARE_ENUM_VAL(   HSCB_TRANS_CLK_FREQ_n,          8 )
    DECLARE_ENUM_VAL(   HSCB_TRANS_CLK_FREQ_mask,       FIELD_MASK32( HSCB_TRANS_CLK_FREQ_i, HSCB_TRANS_CLK_FREQ_n ) )
    DECLARE_ENUM_VAL(   HSCB_TRANS_CLK_INIT_FREQ_i,     8 )     /**< Koeffitsiyent chastoty peredatchika, sootvetstvuyushchiy skorosti ustanovleniya soyedineniya - 10 Mbit/s. (koeffitsiyent rasschityvayetsya tak zhe kak i freq) */
    DECLARE_ENUM_VAL(   HSCB_TRANS_CLK_INIT_FREQ_n,     8 )
    DECLARE_ENUM_VAL(   HSCB_TRANS_CLK_INIT_FREQ_mask,  FIELD_MASK32( HSCB_TRANS_CLK_INIT_FREQ_i, HSCB_TRANS_CLK_INIT_FREQ_n ) )
    DECLARE_ENUM_VAL(   HSCB_TRANS_CLK_LIMIT_i,         16 )    /**< Minimal'no dopustimaya skorost' peredachi. (koeffitsiyent rasschityvayetsya tak zhe kak i freq). Yesli dannyye prinimayutsya, no v techeniye limit taktov sistemnoy chastoty ne registriruyetsya ni odnogo izmeneniya vosstanovlennogo sinkhrosignala , to schitayetsya, chto priyom vedetsya na skorosti 400 Mbit/s */
    DECLARE_ENUM_VAL(   HSCB_TRANS_CLK_LIMIT_n,         8 )
    DECLARE_ENUM_VAL(   HSCB_TRANS_CLK_LIMIT_mask,      FIELD_MASK32( HSCB_TRANS_CLK_LIMIT_i, HSCB_TRANS_CLK_LIMIT_n ) )
END_ENUM( HSCB_TRANS_CLK_FIELD )

/** HSCB registers' masks */
BEGIN_ENUM( HSCB_REG_MASK )
    /* HSCB System */
    DECLARE_ENUM_VAL(   HSCB_ID_MASK,               0xFFFFFFFF )
    DECLARE_ENUM_VAL(   HSCB_VERSION_MASK,          0xFFFFFFFF )
    DECLARE_ENUM_VAL(   HSCB_SW_RESET_MASK,         HSCB_SW_RESET_RST_mask )
    DECLARE_ENUM_VAL(   HSCB_SETTINGS_MASK,         HSCB_SETTINGS_EN_HSCB_mask
                                                  | HSCB_SETTINGS_TX_ENDIAN_mask
                                                  | HSCB_SETTINGS_RX_ENDIAN_mask
                                                  | HSCB_SETTINGS_TIME_MODE_mask
                                                  | HSCB_SETTINGS_LOOP_BACK_mask
                                                  | HSCB_SETTINGS_EN_RMAP_mask
                                                  | HSCB_SETTINGS_EN_TRIM_CLK_mask
                                                  | HSCB_SETTINGS_RX_FIX_EN_mask )
    DECLARE_ENUM_VAL(   HSCB_STATUS_MASK,           HSCB_STATUS_DISCONNECT_ERR_mask
                                                  | HSCB_STATUS_PARITY_ERR_mask
                                                  | HSCB_STATUS_ESC_ERR_mask
                                                  | HSCB_STATUS_TIME_CODE_mask
                                                  | HSCB_STATUS_TIME_CODE_ERR_mask
                                                  | HSCB_STATUS_TX_CREDIT_ERR_mask
                                                  | HSCB_STATUS_RX_CREDIT_ERR_mask
                                                  | HSCB_STATUS_ACTIVE_LINK_nmask
                                                  | HSCB_STATUS_TX_FREE_BYTE_CNT_mask
                                                  | HSCB_STATUS_RX_BYTE_CNT_mask
                                                  | HSCB_STATUS_RX_FIFO_EMPTY_mask
                                                  | HSCB_STATUS_STATE_mask )
    DECLARE_ENUM_VAL(   HSCB_IRQ_MASK_MASK,         HSCB_IRQ_MASK_DISCONNECT_ERR_mask
                                                  | HSCB_IRQ_MASK_PARITY_ERR_mask
                                                  | HSCB_IRQ_MASK_ESC_ERR_mask
                                                  | HSCB_IRQ_MASK_RX_WORD_mask
                                                  | HSCB_IRQ_MASK_TX_WORD_mask
                                                  | HSCB_IRQ_MASK_TIME_CODE_mask
                                                  | HSCB_IRQ_MASK_TIME_CODE_ERR_mask
                                                  | HSCB_IRQ_MASK_TX_CREDIT_ERR_mask
                                                  | HSCB_IRQ_MASK_RX_CREDIT_ERR_mask
                                                  | HSCB_IRQ_MASK_ACTIVE_LINK_mask )
    DECLARE_ENUM_VAL(   HSCB_TRANS_CLK_MASK,        HSCB_TRANS_CLK_FREQ_mask
                                                  | HSCB_TRANS_CLK_INIT_FREQ_mask
                                                  | HSCB_TRANS_CLK_LIMIT_mask )
    DECLARE_ENUM_VAL(   HSCB_TIMINGS_MASK,          0xC8A00500 )
    /* Time-code */
    DECLARE_ENUM_VAL(   HSCB_INTERVAL_MASK,         0x000007D0 )
    DECLARE_ENUM_VAL(   HSCB_TIME_CODE_INNER_MASK,  0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_TIME_CODE_OUTER_MASK,  0x00000000 )
    /* Word-based data exchange */
//  DECLARE_ENUM_VAL(   HSCB_DATA_SEND_H_MASK,      0xXXXXXXXX )
//  DECLARE_ENUM_VAL(   HSCB_DATA_SEND_L_MASK,      0xXXXXXXXX )
//  DECLARE_ENUM_VAL(   HSCB_DSTRB_SEND_MASK,       0xXXXXXXXX )
//  DECLARE_ENUM_VAL(   HSCB_SEND_LEN_MASK,         0xXXXXXXXX )
//  DECLARE_ENUM_VAL(   HSCB_DATA_REC_H_MASK,       0xXXXXXXXX )
//  DECLARE_ENUM_VAL(   HSCB_DATA_REC_L_MASK,       0xXXXXXXXX )
//  DECLARE_ENUM_VAL(   HSCB_DSTRB_REC_MASK,        0xXXXXXXXX )
//  DECLARE_ENUM_VAL(   HSCB_REC_LEN_MASK,          0xXXXXXXXX )
    /* RMAP protocol */
    DECLARE_ENUM_VAL(   HSCB_RMAP_PARAMS_MASK,      0x00017799 )
    DECLARE_ENUM_VAL(   HSCB_RMAP_STATUS_MASK,      0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_RMAP_C_STATUS_MASK,    0x00000000 )
    /* AXI-DMA System */
    DECLARE_ENUM_VAL(   HSCB_ADMA_ID_MASK,          0x414d4441 )
    DECLARE_ENUM_VAL(   HSCB_ADMA_REV_MASK,         0x00000011 )
    DECLARE_ENUM_VAL(   HSCB_ADMA_SW_RESET_MASK,    0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_ADMA_CH_STATUS_MASK,   0x00000000 )
    /* AXI */
    DECLARE_ENUM_VAL(   HSCB_ARLEN_MASK,            0x0000000F )
    DECLARE_ENUM_VAL(   HSCB_AWLEN_MASK,            0x0000000F )
    DECLARE_ENUM_VAL(   HSCB_BRESP_MASK,            0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_RAXI_ERR_ADDR_MASK,    0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_ARCACHE_MASK,          0x00000003 )
    DECLARE_ENUM_VAL(   HSCB_AWCACHE_MASK,          0x00000003 )
    DECLARE_ENUM_VAL(   HSCB_ARPROT_MASK,           0x00000002 )
    DECLARE_ENUM_VAL(   HSCB_AWPROT_MASK,           0x00000002 )
    DECLARE_ENUM_VAL(   HSCB_ARLOCK_MASK,           0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_AWLOCK_MASK,           0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_ARBURST_MASK,          0x00000001 )
    DECLARE_ENUM_VAL(   HSCB_AWBURST_MASK,          0x00000001 )
    /* Read DMA */
    DECLARE_ENUM_VAL(   HSCB_RDMA_SETTINGS_MASK,    0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_RDMA_STATUS_MASK,      0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_RDMA_SYS_ADDR_MASK,    0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_RDMA_TBL_SIZE_MASK,    0x00000800 )
    DECLARE_ENUM_VAL(   HSCB_RDMA_ADDR_MASK,        0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_RDMA_LEN_MASK,         0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_RDMA_DESC_ADDR_MASK,   0x00000000 )
    /* Write DMA */
    DECLARE_ENUM_VAL(   HSCB_WDMA_SETTINGS_MASK,    0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_WDMA_STATUS_MASK,      0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_WDMA_SYS_ADDR_MASK,    0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_WDMA_TBL_SIZE_MASK,    0x00000800 )
    DECLARE_ENUM_VAL(   HSCB_WDMA_ADDR_MASK,        0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_WDMA_LEN_MASK,         0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_WDMA_DESC_ADDR_MASK,   0x00000000 )
END_ENUM( HSCB_REG_MASK )

/**
 * @}
 */


#endif /* REGS_HSCB_H */
