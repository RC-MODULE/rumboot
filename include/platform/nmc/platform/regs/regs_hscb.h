/*
 * hscb.h
 *
 *  Created on: Jul 19, 2018
 *      Author: r.galiulin
 */

#ifndef REGS_HSCB_H
#define REGS_HSCB_H


#include <rumboot/macros.h>

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


/**
 * \defgroup regs_fields_hscb HSCB controller registers' fields
 * \ingroup regs_hscb
 *
 * @{
 */
DECLARE_CONST(HSCB_DESCR_SIZE, 0x8)

BEGIN_ENUM( HSCB_SW_RESET_FIELD )
    DECLARE_ENUM_VAL(   HSCB_SW_RESET_RST_i,        0 )         /**< Programmnyi sbros Space Wire */
    DECLARE_ENUM_VAL(   HSCB_SW_RESET_RST_n,        1 )
    DECLARE_ENUM_VAL(   HSCB_SW_RESET_RST_mask,     FIELD_MASK32( HSCB_SW_RESET_RST_i, HSCB_SW_RESET_RST_n ) )
END_ENUM( HSCB_SW_RESET_FIELD )

BEGIN_ENUM( HSCB_SETTINGS_FIELD )
    DECLARE_ENUM_VAL(   HSCB_SETTINGS_EN_HSCB_i,        0 )         /**< Vklyucheniye Space Wire interfeysa */
    DECLARE_ENUM_VAL(   HSCB_SETTINGS_EN_HSCB_n,        1 )
    DECLARE_ENUM_VAL(   HSCB_SETTINGS_EN_HSCB_mask,     FIELD_MASK32( HSCB_SETTINGS_EN_HSCB_i, HSCB_SETTINGS_EN_HSCB_n ) )
//    DECLARE_ENUM_VAL(   HSCB_SETTINGS_TX_ENDIAN_i,      1 )         /**< Vybor poryadka baitov pri peredache dannyh*/
//    DECLARE_ENUM_VAL(   HSCB_SETTINGS_TX_ENDIAN_n,      1 )
//    DECLARE_ENUM_VAL(   HSCB_SETTINGS_TX_ENDIAN_mask,   FIELD_MASK32( HSCB_SETTINGS_TX_ENDIAN_i, HSCB_SETTINGS_TX_ENDIAN_n ) )
//    DECLARE_ENUM_VAL(   HSCB_SETTINGS_RX_ENDIAN_i,      2 )         /**< Vybor poryadka baitov pri poluchenii dannyh */
//    DECLARE_ENUM_VAL(   HSCB_SETTINGS_RX_ENDIAN_n,      1 )
//    DECLARE_ENUM_VAL(   HSCB_SETTINGS_RX_ENDIAN_mask,   FIELD_MASK32( HSCB_SETTINGS_RX_ENDIAN_i, HSCB_SETTINGS_RX_ENDIAN_n ) )
    DECLARE_ENUM_VAL(   HSCB_SETTINGS_TIME_MODE_i,      3 )         /**< Rezhim peredachi Time-code */
    DECLARE_ENUM_VAL(   HSCB_SETTINGS_TIME_MODE_n,      1 )
    DECLARE_ENUM_VAL(   HSCB_SETTINGS_TIME_MODE_mask,   FIELD_MASK32( HSCB_SETTINGS_TIME_MODE_i, HSCB_SETTINGS_TIME_MODE_n ) )
    DECLARE_ENUM_VAL(   HSCB_SETTINGS_RESERVE_PATH_i,      4 )         /**< Rezhim peredachi Time-code */
    DECLARE_ENUM_VAL(   HSCB_SETTINGS_RESERVE_PATH_n,      1 )
    DECLARE_ENUM_VAL(   HSCB_SETTINGS_RESERVE_PATH_mask,   FIELD_MASK32( HSCB_SETTINGS_RESERVE_PATH_i, HSCB_SETTINGS_RESERVE_PATH_n ) )
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
    DECLARE_ENUM_VAL(   HSCB_SETTINGS_DISCARD_PERMIT_i,      9 )         /**< Vosstanovleniye taktovogo signala priyomnika iz prinimayemykh dannykh. Ustoychivyy priyom vozmozhen tol'ko yesli chastote priyomnika v 4 ili boleye raz prevyshayet chastotu peredatchika */
    DECLARE_ENUM_VAL(   HSCB_SETTINGS_DISCARD_PERMIT_n,      1 )
    DECLARE_ENUM_VAL(   HSCB_SETTINGS_DISCARD_PERMIT_mask,   FIELD_MASK32( HSCB_SETTINGS_DISCARD_PERMIT_i, HSCB_SETTINGS_DISCARD_PERMIT_n ) )
END_ENUM( HSCB_SETTINGS_FIELD )
/** Poryadok bayt dannykh priyomnogo/peredayushchego kanala*/
BEGIN_ENUM( hscb_rtx_endian_t )
    DECLARE_ENUM_VAL(   hscb_rtx_endian_big,    1 )                 /**< Big Endian */
    DECLARE_ENUM_VAL(   hscb_rtx_endian_little, 0 )                 /**< Little Endian */
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
    DECLARE_ENUM_VAL(   HSCB_STATUS_ACTIVE_LINK_mask,       FIELD_MASK32( HSCB_STATUS_ACTIVE_LINK_i, HSCB_STATUS_ACTIVE_LINK_n ) )
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

BEGIN_ENUM( HSCB_TIMINGS_FIELD )
    DECLARE_ENUM_VAL(   HSCB_TIMINGS_TIME_64_i,         0 )     /**< Schotchik 6,4 mks (mezhdu ErrRST i ErrWAIT) */
    DECLARE_ENUM_VAL(   HSCB_TIMINGS_TIME_64_n,         12 )
    DECLARE_ENUM_VAL(   HSCB_TIMINGS_TIME_64_mask,      FIELD_MASK32( HSCB_TIMINGS_TIME_64_i, HSCB_TIMINGS_TIME_64_n ) )
    DECLARE_ENUM_VAL(   HSCB_TIMINGS_TIME_128_i,        12 )    /**< Schotchik 12,8 mks (mezhdu ErrWAIT i CONNECT) */
    DECLARE_ENUM_VAL(   HSCB_TIMINGS_TIME_128_n,        12 )
    DECLARE_ENUM_VAL(   HSCB_TIMINGS_TIME_128_mask,     FIELD_MASK32( HSCB_TIMINGS_TIME_128_i, HSCB_TIMINGS_TIME_128_n ) )
    DECLARE_ENUM_VAL(   HSCB_TIMINGS_SILENCE_TIME_i,    24 )    /**< Maksimal'noye vremya mezhdu simvolami, pri kotorom soyedineniya sokhranyayetsya */
    DECLARE_ENUM_VAL(   HSCB_TIMINGS_SILENCE_TIME_n,    8 )
    DECLARE_ENUM_VAL(   HSCB_TIMINGS_SILENCE_TIME_mask, FIELD_MASK32( HSCB_TIMINGS_SILENCE_TIME_i, HSCB_TIMINGS_SILENCE_TIME_n ) )
END_ENUM( HSCB_TIMINGS_FIELD )

BEGIN_ENUM( HSCB_TIME_CODE_INNER_FIELD )
    DECLARE_ENUM_VAL(   HSCB_TIME_CODE_INNER_INIT_CODE_i,     0 )
    DECLARE_ENUM_VAL(   HSCB_TIME_CODE_INNER_INIT_CODE_n,     6 )
    DECLARE_ENUM_VAL(   HSCB_TIME_CODE_INNER_INIT_CODE_mask, FIELD_MASK32( HSCB_TIME_CODE_INNER_INIT_CODE_i, HSCB_TIME_CODE_INNER_INIT_CODE_n ) )

    DECLARE_ENUM_VAL(   HSCB_TIME_CODE_INNER_INIT_CODE_CONTROL_FLAGS_i,     6 )
    DECLARE_ENUM_VAL(   HSCB_TIME_CODE_INNER_INIT_CODE_CONTROL_FLAGS_n,     2 )
    DECLARE_ENUM_VAL(   HSCB_TIME_CODE_INNER_INIT_CODE_CONTROL_FLAGS_mask, FIELD_MASK32( HSCB_TIME_CODE_INNER_INIT_CODE_CONTROL_FLAGS_i, HSCB_TIME_CODE_INNER_INIT_CODE_CONTROL_FLAGS_n ) )

    DECLARE_ENUM_VAL(   HSCB_TIME_CODE_INNER_CUR_CODE_i,     8 )
    DECLARE_ENUM_VAL(   HSCB_TIME_CODE_INNER_CUR_CODE_n,     6 )
    DECLARE_ENUM_VAL(   HSCB_TIME_CODE_INNER_CUR_CODE_mask, FIELD_MASK32( HSCB_TIME_CODE_INNER_CUR_CODE_i, HSCB_TIME_CODE_INNER_CUR_CODE_n ) )

    DECLARE_ENUM_VAL(   HSCB_TIME_CODE_INNER_CUR_CODE_CONTROL_FLAGS_i,    14 )
    DECLARE_ENUM_VAL(   HSCB_TIME_CODE_INNER_CUR_CODE_CONTROL_FLAGS_n,     2 )
    DECLARE_ENUM_VAL(   HSCB_TIME_CODE_INNER_CUR_CODE_CONTROL_FLAGS_mask, FIELD_MASK32( HSCB_TIME_CODE_INNER_CUR_CODE_CONTROL_FLAGS_i, HSCB_TIME_CODE_INNER_CUR_CODE_CONTROL_FLAGS_n ) )
END_ENUM( HSCB_TIME_CODE_INNER_FIELD )

BEGIN_ENUM( HSCB_TIME_CODE_OUTER_FIELD )
    DECLARE_ENUM_VAL(   HSCB_TIME_CODE_OUTER_NEW_CODE_i,        0 )     /**< poluchennoye znacheniye */
    DECLARE_ENUM_VAL(   HSCB_TIME_CODE_OUTER_NEW_CODE_n,        8 )
    DECLARE_ENUM_VAL(   HSCB_TIME_CODE_OUTER_NEW_CODE_mask,     FIELD_MASK32( HSCB_TIME_CODE_OUTER_NEW_CODE_i, HSCB_TIME_CODE_OUTER_NEW_CODE_n ) )
    DECLARE_ENUM_VAL(   HSCB_TIME_CODE_OUTER_PREV_CODE_i,       8 )     /**< predydushcheye poluchennoye znacheniye */
    DECLARE_ENUM_VAL(   HSCB_TIME_CODE_OUTER_PREV_CODE_n,       8 )
    DECLARE_ENUM_VAL(   HSCB_TIME_CODE_OUTER_PREV_CODE_mask,    FIELD_MASK32( HSCB_TIME_CODE_OUTER_PREV_CODE_i, HSCB_TIME_CODE_OUTER_PREV_CODE_n ) )
END_ENUM( HSCB_TIME_CODE_OUTER_FIELD )

BEGIN_ENUM( HSCB_RMAP_PARAMS_FIELD )
    DECLARE_ENUM_VAL(   HSCB_RMAP_PARAMS_TARGET_ADDR_i,     0 )     /**< Adres v RMAP paketakh */
    DECLARE_ENUM_VAL(   HSCB_RMAP_PARAMS_TARGET_ADDR_n,     8 )
    DECLARE_ENUM_VAL(   HSCB_RMAP_PARAMS_TARGET_ADDR_mask,  FIELD_MASK32( HSCB_RMAP_PARAMS_TARGET_ADDR_i, HSCB_RMAP_PARAMS_TARGET_ADDR_n ) )
    DECLARE_ENUM_VAL(   HSCB_RMAP_PARAMS_KEY_i,             8 )     /**< Klyuch dlya RMAP paketov */
    DECLARE_ENUM_VAL(   HSCB_RMAP_PARAMS_KEY_n,             8 )
    DECLARE_ENUM_VAL(   HSCB_RMAP_PARAMS_KEY_mask,          FIELD_MASK32( HSCB_RMAP_PARAMS_KEY_i, HSCB_RMAP_PARAMS_KEY_n ) )
    DECLARE_ENUM_VAL(   HSCB_RMAP_PARAMS_EN_KEY_i,          16 )    /**< Ispol'zovat' klyuch */
    DECLARE_ENUM_VAL(   HSCB_RMAP_PARAMS_EN_KEY_n,          1 )
    DECLARE_ENUM_VAL(   HSCB_RMAP_PARAMS_EN_KEY_mask,       FIELD_MASK32( HSCB_RMAP_PARAMS_EN_KEY_i, HSCB_RMAP_PARAMS_EN_KEY_n ) )
    DECLARE_ENUM_VAL(   HSCB_RMAP_PARAMS_READ_TYPE_i,       20 )    /**< Buferizatsiya pri chtenii iz pamyati */
    DECLARE_ENUM_VAL(   HSCB_RMAP_PARAMS_READ_TYPE_n,       1 )
    DECLARE_ENUM_VAL(   HSCB_RMAP_PARAMS_READ_TYPE_mask,    FIELD_MASK32( HSCB_RMAP_PARAMS_READ_TYPE_i, HSCB_RMAP_PARAMS_READ_TYPE_n ) )
END_ENUM( HSCB_RMAP_PARAMS_FIELD )
/** Buferizatsiya pri chtenii iz pamyati */
BEGIN_ENUM( hscb_read_type_t )
    DECLARE_ENUM_VAL(   hscb_read_type_buf_off, 0 )     /**< buferizatsiya vyklyuchena */
    DECLARE_ENUM_VAL(   hscb_read_type_buf_on,  1 )     /**< buferizatsiya vklyuchena */
END_ENUM( hscb_read_type_t )

BEGIN_ENUM( HSCB_RMAP_STATUS_FIELD )
    DECLARE_ENUM_VAL(   HSCB_RMAP_STATUS_RMAP_STATUS_i,         0 )     /**< Pole statusa v sootvetstvii so standartom */
    DECLARE_ENUM_VAL(   HSCB_RMAP_STATUS_RMAP_STATUS_n,         8 )
    DECLARE_ENUM_VAL(   HSCB_RMAP_STATUS_RMAP_STATUS_mask,      FIELD_MASK32( HSCB_RMAP_STATUS_RMAP_STATUS_i, HSCB_RMAP_STATUS_RMAP_STATUS_n ) )
    DECLARE_ENUM_VAL(   HSCB_RMAP_STATUS_CUSTOM_STATUS_i,       8 )     /**< Dopolnitel'naya informatsiya ob oshibkakh */
    DECLARE_ENUM_VAL(   HSCB_RMAP_STATUS_CUSTOM_STATUS_n,       8 )
    DECLARE_ENUM_VAL(   HSCB_RMAP_STATUS_CUSTOM_STATUS_mask,    FIELD_MASK32( HSCB_RMAP_STATUS_CUSTOM_STATUS_i, HSCB_RMAP_STATUS_CUSTOM_STATUS_n ) )
END_ENUM( HSCB_RMAP_STATUS_FIELD )
/** Statusa v sootvetstvii so standartom */
BEGIN_ENUM( hscb_rmap_status_t )
    DECLARE_ENUM_VAL(   hscb_rmap_status_success,           0 )     /**< Command executed successfully */
    DECLARE_ENUM_VAL(   hscb_rmap_status_general_err_code , 1 )     /**< The detected error does not fit into the other error cases or the node does not support further distinction between the errors */
    DECLARE_ENUM_VAL(   hscb_rmap_status_unused_pt_cc,      2 )     /**< The Header CRC was decoded correctly but the packet type is reserved or the command is not used by the RMAP protocol */
    DECLARE_ENUM_VAL(   hscb_rmap_status_inv_key,           3 )     /**< The Header CRC was decoded correctly but the device key did not match that expected  by the target user application */
    DECLARE_ENUM_VAL(   hscb_rmap_status_inv_data_crc,      4 )     /**< Error in the CRC of the data field */
    DECLARE_ENUM_VAL(   hscb_rmap_status_early_eop,         5 )     /**< EOP marker detected before the end of the data */
    DECLARE_ENUM_VAL(   hscb_rmap_status_too_much_data,     6 )     /**< More than the expected amount of data in a command has been received */
    DECLARE_ENUM_VAL(   hscb_rmap_status_eep,               7 )     /**< EEP marker detected immediately after the header CRC or during the transfer of data and Data CRC or immediately thereafter. Indicates that there was a communication failure of some sort on the network */
    DECLARE_ENUM_VAL(   hscb_rmap_status_vrf_buf_overrun,   9 )     /**< The verify before write bit of the command was set so that the data field was buffered in order to verify the Data CRC before transferring the data to target memory. The data field was longer than able to fit inside the verify buffer resulting in a buffer overrun. Note that the command is not executed in this case */
    DECLARE_ENUM_VAL(   hscb_rmap_status_command_err,       10 )    /**< The target user application did not authorise the requested operation. This may be because the command requested has not been implemented */
    DECLARE_ENUM_VAL(   hscb_rmap_status_rmw_data_len_err,  11 )    /**< The amount of data in a RMW command is invalid (0x01, 0x03, 0x05, 0x07 or  greater than 0x08) */
    DECLARE_ENUM_VAL(   hscb_rmap_status_inv_tla,           12 )    /**< The Header CRC was decoded correctly but the Target Logical Address was not the value expected by the target */
END_ENUM( hscb_rmap_status_t )
/** Dopolnitel'naya informatsiya ob oshibkakh RMAP */
BEGIN_ENUM( hscb_rmap_custom_status_t )
    DECLARE_ENUM_VAL(   hscb_rmap_custom_status_ok,                 0 )     /**< Tranzaktsiya proshla korrektno */
    DECLARE_ENUM_VAL(   hscb_rmap_custom_status_chan_err,           1 )     /**< Oshibka v kanale HSCB (rmap_status==1) */
    DECLARE_ENUM_VAL(   hscb_rmap_custom_status_rcv_replay_mess,    2 )     /**< Polucheno soobshcheniye RMAP, sootvetstvuyushcheye otvetu (rmap_status==1) */
    DECLARE_ENUM_VAL(   hscb_rmap_custom_status_axi_r_err,          3 )     /**< Oshibka pri vypolnenii AXI tranzaktsii chteniya iz pamyati (rmap_status==1) */
    DECLARE_ENUM_VAL(   hscb_rmap_custom_status_axi_w_err,          4 )     /**< Oshibka pri vypolnenii AXI tranzaktsii zapisi v pamyat' (rmap_status==1) */
    DECLARE_ENUM_VAL(   hscb_rmap_custom_status_too_big_addr,       5 )     /**< Ukazannyy v komande adres ne podderzhivayetsya (bol'she maksimal'no dopustimogo v sisteme) (rmap_status==10) */
    DECLARE_ENUM_VAL(   hscb_rmap_custom_status_rmw_len_err,        6 )     /**< Dlya RMW operatsii zaprashivayetsya ne vernaya dlina dlya ukazannogo adresa (rmap_status==10) */
    DECLARE_ENUM_VAL(   hscb_rmap_custom_status_chann_err_after_op, 7 )     /**< Byl razryv soyedineniya, no paket RMAP byl prinyat polnost'yu i korrektno, poetomu zaversheny operatsii zapisi po AXI (Write, Write-verify i Read-Modify-Write komandy) (rmap_status==1) */
END_ENUM( hscb_rmap_custom_status_t )

BEGIN_ENUM( HSCB_ADMA_SW_RESET_FIELD )
    DECLARE_ENUM_VAL(   HSCB_ADMA_SW_RESET_i,       0 ) /**< Programmnyi sbros AXI DMA */
    DECLARE_ENUM_VAL(   HSCB_ADMA_SW_RESET_n,       1 )
    DECLARE_ENUM_VAL(   HSCB_ADMA_SW_RESET_mask,    FIELD_MASK32( HSCB_ADMA_SW_RESET_i, HSCB_ADMA_SW_RESET_n ) )
END_ENUM( HSCB_ADMA_SW_RESET_FIELD )

BEGIN_ENUM( HSCB_ADMA_CH_STATUS_FIELD )
    DECLARE_ENUM_VAL(   HSCB_ADMA_CH_STATUS_RDMA_IRQ_i,     0 ) /**< Vozniklo preryvaniye kanala chteniya */
    DECLARE_ENUM_VAL(   HSCB_ADMA_CH_STATUS_RDMA_IRQ_n,     1 )
    DECLARE_ENUM_VAL(   HSCB_ADMA_CH_STATUS_RDMA_IRQ_mask,  FIELD_MASK32( HSCB_ADMA_CH_STATUS_RDMA_IRQ_i, HSCB_ADMA_CH_STATUS_RDMA_IRQ_n ) )
    DECLARE_ENUM_VAL(   HSCB_ADMA_CH_STATUS_WDMA_IRQ_i,     16 ) /**< Vozniklo preryvaniye kanala zapisi */
    DECLARE_ENUM_VAL(   HSCB_ADMA_CH_STATUS_WDMA_IRQ_n,     1 )
    DECLARE_ENUM_VAL(   HSCB_ADMA_CH_STATUS_WDMA_IRQ_mask,  FIELD_MASK32( HSCB_ADMA_CH_STATUS_WDMA_IRQ_i, HSCB_ADMA_CH_STATUS_WDMA_IRQ_n ) )
END_ENUM( HSCB_ADMA_CH_STATUS_FIELD )

BEGIN_ENUM( HSCB_XDMA_STATUS_FIELD )
    DECLARE_ENUM_VAL(   HSCB_XDMA_STATUS_DESC_INT_i,      0 )     /**< Zakoncheno vypolneniye deskriptora kanala chteniya s 1 v pole Int */
    DECLARE_ENUM_VAL(   HSCB_XDMA_STATUS_DESC_INT_n,      1 )
    DECLARE_ENUM_VAL(   HSCB_XDMA_STATUS_DESC_INT_mask,   FIELD_MASK32( HSCB_XDMA_STATUS_DESC_INT_i, HSCB_XDMA_STATUS_DESC_INT_n ) )
    DECLARE_ENUM_VAL(   HSCB_XDMA_STATUS_DESC_END_i,      1 )     /**< Zakoncheno vypolneniye ostanovki XDMA */
    DECLARE_ENUM_VAL(   HSCB_XDMA_STATUS_DESC_END_n,      1 )
    DECLARE_ENUM_VAL(   HSCB_XDMA_STATUS_DESC_END_mask,   FIELD_MASK32( HSCB_XDMA_STATUS_DESC_END_i, HSCB_XDMA_STATUS_DESC_END_n ) )
    DECLARE_ENUM_VAL(   HSCB_XDMA_STATUS_BAD_DESC_i,      2 )     /**< Deskriptor kanala chteniya pomechen kak vypolnennyy (compl) */
    DECLARE_ENUM_VAL(   HSCB_XDMA_STATUS_BAD_DESC_n,      1 )
    DECLARE_ENUM_VAL(   HSCB_XDMA_STATUS_BAD_DESC_mask,   FIELD_MASK32( HSCB_XDMA_STATUS_BAD_DESC_i, HSCB_XDMA_STATUS_BAD_DESC_n ) )
    DECLARE_ENUM_VAL(   HSCB_XDMA_STATUS_AXI_ERROR_i,     5 )     /**< Signal preryvaniya pri vozniknovenii oshibki na sistemnoy shine (RRESP = SLVERR) kanal chteniya */
    DECLARE_ENUM_VAL(   HSCB_XDMA_STATUS_AXI_ERROR_n,     1 )
    DECLARE_ENUM_VAL(   HSCB_XDMA_STATUS_AXI_ERROR_mask,  FIELD_MASK32( HSCB_XDMA_STATUS_AXI_ERROR_i, HSCB_XDMA_STATUS_AXI_ERROR_n ) )
    DECLARE_ENUM_VAL(   HSCB_XDMA_STATUS_BUF_FULL_i,      6 )     /**< Bufer kanala chteniya polon */
    DECLARE_ENUM_VAL(   HSCB_XDMA_STATUS_BUF_FULL_n,      1 )
    DECLARE_ENUM_VAL(   HSCB_XDMA_STATUS_BUF_FULL_mask,   FIELD_MASK32( HSCB_XDMA_STATUS_BUF_FULL_i, HSCB_XDMA_STATUS_BUF_FULL_n ) )
    DECLARE_ENUM_VAL(   HSCB_XDMA_STATUS_BUF_EMPTY_i,     7 )     /**< Bufer kanala chteniya pust */
    DECLARE_ENUM_VAL(   HSCB_XDMA_STATUS_BUF_EMPTY_n,     1 )
    DECLARE_ENUM_VAL(   HSCB_XDMA_STATUS_BUF_EMPTY_mask,  FIELD_MASK32( HSCB_XDMA_STATUS_BUF_EMPTY_i, HSCB_XDMA_STATUS_BUF_EMPTY_n ) )
    DECLARE_ENUM_VAL(   HSCB_XDMA_STATUS_ACTIVE_TRANS_i,    8 )     /**< Priznak aktivnoy AXI-tranzaktsii: yesli v tekushchiy momeont vypolnyayetsya khotya by odna AXI-tranzaktsiya, to dannyy bit nakhoditsya v sostoyanii "1", v protivnom sluchaye ? v sostoyanii "0". */
    DECLARE_ENUM_VAL(   HSCB_XDMA_STATUS_ACTIVE_TRANS_n,    1 )
    DECLARE_ENUM_VAL(   HSCB_XDMA_STATUS_ACTIVE_TRANS_mask, FIELD_MASK32( HSCB_XDMA_STATUS_ACTIVE_TRANS_i, HSCB_XDMA_STATUS_ACTIVE_TRANS_n ) )
    DECLARE_ENUM_VAL(   HSCB_XDMA_STATUS_ACT_REQ_i,         12 )     /**< Nalichiye aktivnykh (ne zavershennykh) zaprosov na chteniye dannykh */
    DECLARE_ENUM_VAL(   HSCB_XDMA_STATUS_ACT_REQ_n,         1 )
    DECLARE_ENUM_VAL(   HSCB_XDMA_STATUS_ACT_REQ_mask,      FIELD_MASK32( HSCB_XDMA_STATUS_ACT_REQ_i, HSCB_XDMA_STATUS_ACT_REQ_n ) )
    DECLARE_ENUM_VAL(   HSCB_XDMA_STATUS_CNT_WORD_i,        16 )     /**< Kolichestvo slov v bufere dannykh kanala chteniya */
    DECLARE_ENUM_VAL(   HSCB_XDMA_STATUS_CNT_WORD_n,        7 )
    DECLARE_ENUM_VAL(   HSCB_XDMA_STATUS_CNT_WORD_mask,     FIELD_MASK32( HSCB_XDMA_STATUS_CNT_WORD_i, HSCB_XDMA_STATUS_CNT_WORD_n ) )
END_ENUM( HSCB_XDMA_STATUS_FIELD )

BEGIN_ENUM( HSCB_XDMA_SETTINGS_FIELD )
    DECLARE_ENUM_VAL(   HSCB_XDMA_SETTINGS_X_DESC_INT_i,              0 )     /**< Maska preryvaniya "Zakoncheno vypolneniye deskriptora kanala chteniya (zapisi) s 1 v pole Int" */
    DECLARE_ENUM_VAL(   HSCB_XDMA_SETTINGS_X_DESC_INT_n,              1 )
    DECLARE_ENUM_VAL(   HSCB_XDMA_SETTINGS_X_DESC_INT_mask,           FIELD_MASK32( HSCB_XDMA_SETTINGS_X_DESC_INT_i, HSCB_XDMA_SETTINGS_X_DESC_INT_n ) )
    DECLARE_ENUM_VAL(   HSCB_XDMA_SETTINGS_X_DESC_END_i,              1 )     /**< Maska preryvaniya "Zakoncheno vypolneniye ostanovki R(W)DMA" */
    DECLARE_ENUM_VAL(   HSCB_XDMA_SETTINGS_X_DESC_END_n,              1 )
    DECLARE_ENUM_VAL(   HSCB_XDMA_SETTINGS_X_DESC_END_mask,           FIELD_MASK32( HSCB_XDMA_SETTINGS_X_DESC_END_i, HSCB_XDMA_SETTINGS_X_DESC_END_n ) )
    DECLARE_ENUM_VAL(   HSCB_XDMA_SETTINGS_X_BAD_DESC_i,              2 )     /**< Maska preryvaniya "Deskriptor kanala chteniya pomechen kak vypolnennyy (compl)" */
    DECLARE_ENUM_VAL(   HSCB_XDMA_SETTINGS_X_BAD_DESC_n,              1 )
    DECLARE_ENUM_VAL(   HSCB_XDMA_SETTINGS_X_BAD_DESC_mask,           FIELD_MASK32( HSCB_XDMA_SETTINGS_X_BAD_DESC_i, HSCB_XDMA_SETTINGS_X_BAD_DESC_n ) )
    DECLARE_ENUM_VAL(   HSCB_XDMA_SETTINGS_X_AXI_ERROR_i,             5 )     /**< Maska preryvaniya "Signala preryvaniya pri vozniknovenii oshibki na sistemnoy shine (RRESP = SLVERR dlya kanala chteniya ili BRESP = SLVERR dlya kanala zapisi)" */
    DECLARE_ENUM_VAL(   HSCB_XDMA_SETTINGS_X_AXI_ERROR_n,             1 )
    DECLARE_ENUM_VAL(   HSCB_XDMA_SETTINGS_X_AXI_ERROR_mask,          FIELD_MASK32( HSCB_XDMA_SETTINGS_X_AXI_ERROR_i, HSCB_XDMA_SETTINGS_X_AXI_ERROR_n ) )
    DECLARE_ENUM_VAL(   HSCB_XDMA_SETTINGS_X_BUF_FULL_i,              6 )     /**< Maska preryvaniya "Bufer kanala chteniya (zapisi) polon" */
    DECLARE_ENUM_VAL(   HSCB_XDMA_SETTINGS_X_BUF_FULL_n,              1 )
    DECLARE_ENUM_VAL(   HSCB_XDMA_SETTINGS_X_BUF_FULL_mask,           FIELD_MASK32( HSCB_XDMA_SETTINGS_X_BUF_FULL_i, HSCB_XDMA_SETTINGS_X_BUF_FULL_n ) )
    DECLARE_ENUM_VAL(   HSCB_XDMA_SETTINGS_X_BUF_EMPTY_i,             7 )     /**< Maska preryvaniya "Bufer kanala chteniya (zapisi) pust" */
    DECLARE_ENUM_VAL(   HSCB_XDMA_SETTINGS_X_BUF_EMPTY_n,             1 )
    DECLARE_ENUM_VAL(   HSCB_XDMA_SETTINGS_X_BUF_EMPTY_mask,          FIELD_MASK32( HSCB_XDMA_SETTINGS_X_BUF_EMPTY_i, HSCB_XDMA_SETTINGS_X_BUF_EMPTY_n ) )
    DECLARE_ENUM_VAL(   HSCB_XDMA_SETTINGS_EN_XDMA_i,               28 )    /**< Vklyuchen kanal chteniya (zapisi) (1 ? aktivno) */
    DECLARE_ENUM_VAL(   HSCB_XDMA_SETTINGS_EN_XDMA_n,               1 )
    DECLARE_ENUM_VAL(   HSCB_XDMA_SETTINGS_EN_XDMA_mask,            FIELD_MASK32( HSCB_XDMA_SETTINGS_EN_XDMA_i, HSCB_XDMA_SETTINGS_EN_XDMA_n ) )
    DECLARE_ENUM_VAL(   HSCB_XDMA_SETTINGS_EN_XDMA_DESC_TBL_i,      29 )    /**< Podderzhka vneshney tablitsy deskriptorov (dolzhno byt' ustanovleno znacheniye 1) */
    DECLARE_ENUM_VAL(   HSCB_XDMA_SETTINGS_EN_XDMA_DESC_TBL_n,      1 )
    DECLARE_ENUM_VAL(   HSCB_XDMA_SETTINGS_EN_XDMA_DESC_TBL_mask,   FIELD_MASK32( HSCB_XDMA_SETTINGS_EN_XDMA_DESC_TBL_i, HSCB_XDMA_SETTINGS_EN_XDMA_DESC_TBL_n ) )
    DECLARE_ENUM_VAL(   HSCB_XDMA_SETTINGS_XDMA_LONG_LEN_i,         30 )    /**< Rasshireniye pole Length za schot polya Custom (dolzhno byt' ustanovleno znacheniye 1) */
    DECLARE_ENUM_VAL(   HSCB_XDMA_SETTINGS_XDMA_LONG_LEN_n,         1 )
    DECLARE_ENUM_VAL(   HSCB_XDMA_SETTINGS_XDMA_LONG_LEN_mask,      FIELD_MASK32( HSCB_XDMA_SETTINGS_XDMA_LONG_LEN_i, HSCB_XDMA_SETTINGS_XDMA_LONG_LEN_n ) )
    DECLARE_ENUM_VAL(   HSCB_XDMA_SETTINGS_CANCEL_XDMA_i,           31 )    /**< Ostanovka raboty R(W)DMA (1 ? aktivno) */
    DECLARE_ENUM_VAL(   HSCB_XDMA_SETTINGS_CANCEL_XDMA_n,           1 )
    DECLARE_ENUM_VAL(   HSCB_XDMA_SETTINGS_CANCEL_XDMA_mask,        FIELD_MASK32( HSCB_XDMA_SETTINGS_CANCEL_XDMA_i, HSCB_XDMA_SETTINGS_CANCEL_XDMA_n ) )
END_ENUM( HSCB_XDMA_SETTINGS_FIELD )
/**
 * @}
 */


/** HSCB registers' masks */
BEGIN_ENUM( HSCB_REG_MASK )
    /* HSCB System */
    DECLARE_ENUM_VAL(   HSCB_ID_MASK,               0xFFFFFFFF )
    DECLARE_ENUM_VAL(   HSCB_VERSION_MASK,          0xFFFFFFFF )
    DECLARE_ENUM_VAL(   HSCB_SW_RESET_MASK,         HSCB_SW_RESET_RST_mask )
    DECLARE_ENUM_VAL(   HSCB_SETTINGS_MASK,         HSCB_SETTINGS_EN_HSCB_mask
//                                                  | HSCB_SETTINGS_TX_ENDIAN_mask
//                                                  | HSCB_SETTINGS_RX_ENDIAN_mask
                                                  | HSCB_SETTINGS_TIME_MODE_mask
                                                  | HSCB_SETTINGS_RESERVE_PATH_mask
                                                  | HSCB_SETTINGS_LOOP_BACK_mask
                                                  | HSCB_SETTINGS_EN_RMAP_mask
                                                  | HSCB_SETTINGS_EN_TRIM_CLK_mask
                                                  | HSCB_SETTINGS_RX_FIX_EN_mask
                                                  | HSCB_SETTINGS_DISCARD_PERMIT_mask)
    DECLARE_ENUM_VAL(   HSCB_STATUS_MASK,           HSCB_STATUS_DISCONNECT_ERR_mask
                                                  | HSCB_STATUS_PARITY_ERR_mask
                                                  | HSCB_STATUS_ESC_ERR_mask
                                                  | HSCB_STATUS_TIME_CODE_mask
                                                  | HSCB_STATUS_TIME_CODE_ERR_mask
                                                  | HSCB_STATUS_TX_CREDIT_ERR_mask
                                                  | HSCB_STATUS_RX_CREDIT_ERR_mask
                                                  | HSCB_STATUS_ACTIVE_LINK_mask
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
    DECLARE_ENUM_VAL(   HSCB_TIMINGS_MASK,          HSCB_TIMINGS_TIME_64_mask
                                                  | HSCB_TIMINGS_TIME_128_mask
                                                  | HSCB_TIMINGS_SILENCE_TIME_mask )
    /* Time-code */
    DECLARE_ENUM_VAL(   HSCB_INTERVAL_MASK,         0xFFFFFFFF )
    DECLARE_ENUM_VAL(   HSCB_TIME_CODE_INNER_MASK,  HSCB_TIME_CODE_INNER_INIT_CODE_mask
                                                  | HSCB_TIME_CODE_INNER_INIT_CODE_CONTROL_FLAGS_mask
                                                  | HSCB_TIME_CODE_INNER_CUR_CODE_mask
                                                  | HSCB_TIME_CODE_INNER_CUR_CODE_CONTROL_FLAGS_mask)
    DECLARE_ENUM_VAL(   HSCB_TIME_CODE_OUTER_MASK,  HSCB_TIME_CODE_OUTER_NEW_CODE_mask
                                                  | HSCB_TIME_CODE_OUTER_PREV_CODE_mask )
    /* RMAP protocol */
    DECLARE_ENUM_VAL(   HSCB_RMAP_PARAMS_MASK,      HSCB_RMAP_PARAMS_TARGET_ADDR_mask
                                                  | HSCB_RMAP_PARAMS_KEY_mask
                                                  | HSCB_RMAP_PARAMS_EN_KEY_mask
                                                  | HSCB_RMAP_PARAMS_READ_TYPE_mask )
    DECLARE_ENUM_VAL(   HSCB_RMAP_STATUS_MASK,      HSCB_RMAP_STATUS_RMAP_STATUS_mask
                                                  | HSCB_RMAP_STATUS_CUSTOM_STATUS_mask )
    DECLARE_ENUM_VAL(   HSCB_RMAP_C_STATUS_MASK,    HSCB_RMAP_STATUS_RMAP_STATUS_mask
                                                  | HSCB_RMAP_STATUS_CUSTOM_STATUS_mask )
    /* AXI-DMA System */
    DECLARE_ENUM_VAL(   HSCB_ADMA_ID_MASK,          0xFFFFFFFF )
    DECLARE_ENUM_VAL(   HSCB_ADMA_REV_MASK,         0xFFFFFFFF )
    DECLARE_ENUM_VAL(   HSCB_ADMA_SW_RESET_MASK,    HSCB_ADMA_SW_RESET_mask )
    DECLARE_ENUM_VAL(   HSCB_ADMA_CH_STATUS_MASK,   HSCB_ADMA_CH_STATUS_RDMA_IRQ_mask
                                                  | HSCB_ADMA_CH_STATUS_WDMA_IRQ_mask )
    /* AXI */
    DECLARE_ENUM_VAL(   HSCB_ARLEN_MASK,            0x0000000F )
    DECLARE_ENUM_VAL(   HSCB_AWLEN_MASK,            0x0000000F )
    DECLARE_ENUM_VAL(   HSCB_BRESP_MASK,            0x00000003 )
    DECLARE_ENUM_VAL(   HSCB_RAXI_ERR_ADDR_MASK,    0x0000000F )
    DECLARE_ENUM_VAL(   HSCB_ARCACHE_MASK,          0x0000000F )
    DECLARE_ENUM_VAL(   HSCB_AWCACHE_MASK,          0x0000000F )
    DECLARE_ENUM_VAL(   HSCB_ARPROT_MASK,           0x00000007 )
    DECLARE_ENUM_VAL(   HSCB_AWPROT_MASK,           0x00000007 )
    DECLARE_ENUM_VAL(   HSCB_ARLOCK_MASK,           0x00000003 )
    DECLARE_ENUM_VAL(   HSCB_AWLOCK_MASK,           0x00000003 )
    DECLARE_ENUM_VAL(   HSCB_ARBURST_MASK,          0x00000003 )
    DECLARE_ENUM_VAL(   HSCB_AWBURST_MASK,          0x00000003 )
    /* Read DMA */
    DECLARE_ENUM_VAL(   HSCB_RDMA_STATUS_MASK,      HSCB_XDMA_STATUS_DESC_INT_mask
                                                  | HSCB_XDMA_STATUS_DESC_END_mask
                                                  | HSCB_XDMA_STATUS_BAD_DESC_mask
                                                  | HSCB_XDMA_STATUS_AXI_ERROR_mask
                                                  | HSCB_XDMA_STATUS_BUF_FULL_mask
                                                  | HSCB_XDMA_STATUS_BUF_EMPTY_mask
                                                  | HSCB_XDMA_STATUS_ACTIVE_TRANS_mask
                                                  | HSCB_XDMA_STATUS_ACT_REQ_mask
                                                  | HSCB_XDMA_STATUS_CNT_WORD_mask )
    /* Write DMA */
    DECLARE_ENUM_VAL(   HSCB_WDMA_STATUS_MASK,      HSCB_XDMA_STATUS_DESC_INT_mask
                                                  | HSCB_XDMA_STATUS_DESC_END_mask
                                                  | HSCB_XDMA_STATUS_BAD_DESC_mask
                                                  | HSCB_XDMA_STATUS_AXI_ERROR_mask
                                                  | HSCB_XDMA_STATUS_BUF_FULL_mask
                                                  | HSCB_XDMA_STATUS_BUF_EMPTY_mask )
    /* Read/Write DMA */
    DECLARE_ENUM_VAL(   HSCB_XDMA_SETTINGS_MASK,    HSCB_XDMA_SETTINGS_X_DESC_INT_mask
                                                  | HSCB_XDMA_SETTINGS_X_DESC_END_mask
                                                  | HSCB_XDMA_SETTINGS_X_BAD_DESC_mask
                                                  | HSCB_XDMA_SETTINGS_X_AXI_ERROR_mask
                                                  | HSCB_XDMA_SETTINGS_X_BUF_FULL_mask
                                                  | HSCB_XDMA_SETTINGS_X_BUF_EMPTY_mask
                                                  | HSCB_XDMA_SETTINGS_EN_XDMA_mask
                                                  | HSCB_XDMA_SETTINGS_EN_XDMA_DESC_TBL_mask
                                                  | HSCB_XDMA_SETTINGS_XDMA_LONG_LEN_mask
                                                  | HSCB_XDMA_SETTINGS_CANCEL_XDMA_mask )
    DECLARE_ENUM_VAL(   HSCB_XDMA_SYS_ADDR_MASK,    0xFFFFFFFF )
    DECLARE_ENUM_VAL(   HSCB_XDMA_TBL_SIZE_MASK,    0xFFFFFFFF )
    DECLARE_ENUM_VAL(   HSCB_XDMA_ADDR_MASK,        0xFFFFFFFF )
    DECLARE_ENUM_VAL(   HSCB_XDMA_LEN_MASK,         0xFFFFFFFF )
    DECLARE_ENUM_VAL(   HSCB_XDMA_DESC_ADDR_MASK,   0xFFFFFFFF )
END_ENUM( HSCB_REG_MASK )


/** HSCB register reset values */
BEGIN_ENUM( HSCB_REG_DFLT )
    /* HSCB System */
    DECLARE_ENUM_VAL(   HSCB_ID_DFLT,               0x42435348 )
    DECLARE_ENUM_VAL(   HSCB_VERSION_DFLT,          0x00000102 )
    DECLARE_ENUM_VAL(   HSCB_SW_RESET_DFLT,         0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_SETTINGS_DFLT,         0x00000351 )
    DECLARE_ENUM_VAL(   HSCB_STATUS_DFLT,           0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_IRQ_MASK_DFLT,         0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_TRANS_CLK_DFLT,        0x00C81414 )
    DECLARE_ENUM_VAL(   HSCB_TIMINGS_DFLT,          0xC8A00500 )
    /* Time-code */
    DECLARE_ENUM_VAL(   HSCB_INTERVAL_DFLT,         0x000007D0 )
    DECLARE_ENUM_VAL(   HSCB_TIME_CODE_INNER_DFLT,  0x00000101 )
    DECLARE_ENUM_VAL(   HSCB_TIME_CODE_OUTER_DFLT,  0x00000000 )
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
    DECLARE_ENUM_VAL(   HSCB_RDMA_STATUS_DFLT,      0x00000080 )
    DECLARE_ENUM_VAL(   HSCB_RDMA_SYS_ADDR_DFLT,    0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_RDMA_TBL_SIZE_DFLT,    0x00000800 )
    DECLARE_ENUM_VAL(   HSCB_RDMA_ADDR_DFLT,        0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_RDMA_LEN_DFLT,         0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_RDMA_DESC_ADDR_DFLT,   0x00000000 )
    /* Write DMA */
    DECLARE_ENUM_VAL(   HSCB_WDMA_SETTINGS_DFLT,    0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_WDMA_STATUS_DFLT,      0x00000080 )
    DECLARE_ENUM_VAL(   HSCB_WDMA_SYS_ADDR_DFLT,    0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_WDMA_TBL_SIZE_DFLT,    0x00000800 )
    DECLARE_ENUM_VAL(   HSCB_WDMA_ADDR_DFLT,        0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_WDMA_LEN_DFLT,         0x00000000 )
    DECLARE_ENUM_VAL(   HSCB_WDMA_DESC_ADDR_DFLT,   0x00000000 )
END_ENUM( HSCB_REG_DFLT )

BEGIN_ENUM( HSCB_RD_DESCR_FIELD )
    DECLARE_ENUM_VAL(   HSCB_RD_DESCR_START_ADDRESS_i,       0 )
    DECLARE_ENUM_VAL(   HSCB_RD_DESCR_START_ADDRESS_n,      31 )
    DECLARE_ENUM_VAL(   HSCB_RD_DESCR_START_ADDRESS_mask, FIELD_MASK32( HSCB_RD_DESCR_START_ADDRESS_i, HSCB_RD_DESCR_START_ADDRESS_n ) )
    DECLARE_ENUM_VAL(   HSCB_RD_DESCR_LENGTH_i,              6 )
    DECLARE_ENUM_VAL(   HSCB_RD_DESCR_LENGTH_n,             26 )
    DECLARE_ENUM_VAL(   HSCB_RD_DESCR_LENGTH_mask, FIELD_MASK32( HSCB_RD_DESCR_LENGTH_i, HSCB_RD_DESCR_LENGTH_n ) )
    DECLARE_ENUM_VAL(   HSCB_RD_DESCR_ACT_i,                 4 )
    DECLARE_ENUM_VAL(   HSCB_RD_DESCR_ACT_n,                 2 )
    DECLARE_ENUM_VAL(   HSCB_RD_DESCR_ACT_mask, FIELD_MASK32( HSCB_RD_DESCR_ACT_i, HSCB_RD_DESCR_ACT_n ) )
    DECLARE_ENUM_VAL(   HSCB_RD_DESCR_ACT0_i,                3 )
    DECLARE_ENUM_VAL(   HSCB_RD_DESCR_ACT0_n,                1 )
    DECLARE_ENUM_VAL(   HSCB_RD_DESCR_ACT0_mask, FIELD_MASK32( HSCB_RD_DESCR_ACT0_i, HSCB_RD_DESCR_ACT0_n ) )
    DECLARE_ENUM_VAL(   HSCB_RD_DESCR_IE_i,                  2 )
    DECLARE_ENUM_VAL(   HSCB_RD_DESCR_IE_n,                  1 )
    DECLARE_ENUM_VAL(   HSCB_RD_DESCR_IE_mask, FIELD_MASK32( HSCB_RD_DESCR_IE_i, HSCB_RD_DESCR_IE_n ) )
    DECLARE_ENUM_VAL(   HSCB_RD_DESCR_ERR_i,                 1 )
    DECLARE_ENUM_VAL(   HSCB_RD_DESCR_ERR_n,                 1 )
    DECLARE_ENUM_VAL(   HSCB_RD_DESCR_ERR_mask, FIELD_MASK32( HSCB_RD_DESCR_ERR_i, HSCB_RD_DESCR_ERR_n ) )
    DECLARE_ENUM_VAL(   HSCB_RD_DESCR_VALID_i,               0 )
    DECLARE_ENUM_VAL(   HSCB_RD_DESCR_VALID_n,               1 )
    DECLARE_ENUM_VAL(   HSCB_RD_DESCR_VALID_mask, FIELD_MASK32( HSCB_RD_DESCR_VALID_i, HSCB_RD_DESCR_VALID_n ) )
END_ENUM( HSCB_RD_DESCR_FIELD )

BEGIN_ENUM( hscb_axi_arwlen_t )
    DECLARE_ENUM_VAL( HSCB_ARWLEN_1,  0b0000 )
    DECLARE_ENUM_VAL( HSCB_ARWLEN_2,  0b0001 )
    DECLARE_ENUM_VAL( HSCB_ARWLEN_3,  0b0010 )
    DECLARE_ENUM_VAL( HSCB_ARWLEN_4,  0b0011 )
    DECLARE_ENUM_VAL( HSCB_ARWLEN_5,  0b0100 )
    DECLARE_ENUM_VAL( HSCB_ARWLEN_6,  0b0101 )
    DECLARE_ENUM_VAL( HSCB_ARWLEN_7,  0b0110 )
    DECLARE_ENUM_VAL( HSCB_ARWLEN_8,  0b0111 )
    DECLARE_ENUM_VAL( HSCB_ARWLEN_9,  0b1000 )
    DECLARE_ENUM_VAL( HSCB_ARWLEN_10, 0b1001 )
    DECLARE_ENUM_VAL( HSCB_ARWLEN_11, 0b1010 )
    DECLARE_ENUM_VAL( HSCB_ARWLEN_12, 0b1011 )
    DECLARE_ENUM_VAL( HSCB_ARWLEN_13, 0b1100 )
    DECLARE_ENUM_VAL( HSCB_ARWLEN_14, 0b1101 )
    DECLARE_ENUM_VAL( HSCB_ARWLEN_15, 0b1110 )
    DECLARE_ENUM_VAL( HSCB_ARWLEN_16, 0b1111 )
END_ENUM( hscb_axi_arwlen_t )

BEGIN_ENUM( hscb_axi_bresp_t )
    DECLARE_ENUM_VAL( HSCB_BRESP_OKAY,   0b00 )
    DECLARE_ENUM_VAL( HSCB_BRESP_EXOKAY, 0b01 )
    DECLARE_ENUM_VAL( HSCB_BRESP_SLVERR, 0b10 )
    DECLARE_ENUM_VAL( HSCB_BRESP_DECERR, 0b11 )
END_ENUM( hscb_axi_bresp_t )

BEGIN_ENUM( hscb_axi_arwcache_t )
    DECLARE_ENUM_VAL( HSCB_ARWCACHE_NONCACHE_NONBUF,       0b0000 )
    DECLARE_ENUM_VAL( HSCB_ARWCACHE_BUF_ONLY,              0b0001 )
    DECLARE_ENUM_VAL( HSCB_ARWCACHE_CACHE_NONALLOC,        0b0010 )
    DECLARE_ENUM_VAL( HSCB_ARWCACHE_CACHE_BUF_NONALLOC,    0b0011 )
    DECLARE_ENUM_VAL( HSCB_ARWCACHE_CACHE_WT_ALLOC_ON_RO,  0b0110 )
    DECLARE_ENUM_VAL( HSCB_ARWCACHE_CACHE_WB_ALLOC_ON_RO,  0b0111 )
    DECLARE_ENUM_VAL( HSCB_ARWCACHE_CACHE_WT_ALLOC_ON_WO,  0b1010 )
    DECLARE_ENUM_VAL( HSCB_ARWCACHE_CACHE_WB_ALLOC_ON_WO,  0b1011 )
    DECLARE_ENUM_VAL( HSCB_ARWCACHE_CACHE_WT_ALLOC_ON_RW,  0b1110 )
    DECLARE_ENUM_VAL( HSCB_ARWCACHE_CACHE_WB_ALLOC_ON_RW,  0b1111 )
END_ENUM( hscb_axi_arwcache_t )

BEGIN_ENUM( hscb_axi_arwprot_t )
    DECLARE_ENUM_VAL( HSCB_ARWPROT_D_SEC_NORM,       0b000 )
    DECLARE_ENUM_VAL( HSCB_ARWPROT_D_SEC_PRIV,       0b001 )
    DECLARE_ENUM_VAL( HSCB_ARWPROT_D_NONSEC_NORM,    0b010 )
    DECLARE_ENUM_VAL( HSCB_ARWPROT_D_NONSEC_PRIV,    0b011 )
    DECLARE_ENUM_VAL( HSCB_ARWPROT_I_SEC_NORM,       0b100 )
    DECLARE_ENUM_VAL( HSCB_ARWPROT_I_SEC_PRIV,       0b101 )
    DECLARE_ENUM_VAL( HSCB_ARWPROT_I_NONSEC_NORM,    0b110 )
    DECLARE_ENUM_VAL( HSCB_ARWPROT_I_NONSEC_PRIV,    0b111 )
END_ENUM( hscb_axi_arwprot_t )

BEGIN_ENUM( hscb_axi_arwlock_t )
    DECLARE_ENUM_VAL( HSCB_ARWLOCK_NORM,   0b00 )
    DECLARE_ENUM_VAL( HSCB_ARWLOCK_EX,     0b01 )
    DECLARE_ENUM_VAL( HSCB_ARWLOCK_LOCKED, 0b10 )
END_ENUM( hscb_axi_arwlock_t )

BEGIN_ENUM( hscb_axi_arwburst_t )
    DECLARE_ENUM_VAL( HSCB_ARWBURST_FIXED,   0b00 )
    DECLARE_ENUM_VAL( HSCB_ARWBURST_INCR,    0b01 )
    DECLARE_ENUM_VAL( HSCB_ARWBURST_WRAP,    0b10 )
END_ENUM( hscb_axi_arwburst_t )

BEGIN_ENUM( hscb_act_t )
    DECLARE_ENUM_VAL( HSCB_ACT_COMPL0,   0b00 )
    DECLARE_ENUM_VAL( HSCB_ACT_COMPL1,   0b01 )
    DECLARE_ENUM_VAL( HSCB_ACT_TRAN,     0b10 )
    DECLARE_ENUM_VAL( HSCB_ACT_COMPL3,   0b11 )
END_ENUM( hscb_act_t )

BEGIN_ENUM( hscb_act0_t )
    DECLARE_ENUM_VAL( HSCB_ACT0_NOLAST,   0b0 )
    DECLARE_ENUM_VAL( HSCB_ACT0_LAST,     0b01 )
END_ENUM( hscb_act0_t )

BEGIN_ENUM( hscb_descr_itrpt_t )
    DECLARE_ENUM_VAL( HSCB_DESCR_ITRPT_OFF,   0b0 )
    DECLARE_ENUM_VAL( HSCB_DESCR_ITRPT_ON,   0b01 )
END_ENUM( hscb_descr_itrpt_t )

BEGIN_ENUM( hscb_descr_validity_t )
    DECLARE_ENUM_VAL( HSCB_DESCR_INVALID,       0b0 )
    DECLARE_ENUM_VAL( HSCB_DESCR_VALID,         0b1 )
END_ENUM( hscb_descr_validity_t )

BEGIN_ENUM( hscb_state_machine_t )
    DECLARE_ENUM_VAL( HSCB_STATE_ErrRST,    0b00)
    DECLARE_ENUM_VAL( HSCB_STATE_ErrWAIT,   0b01)
    DECLARE_ENUM_VAL( HSCB_STATE_CONNECT,   0b10)
    DECLARE_ENUM_VAL( HSCB_STATE_RUN,       0b11)
END_ENUM( hscb_state_machine_t )


BEGIN_ENUM( hscb_rotate_bytes_t )
    DECLARE_ENUM_VAL( HSCB_ROTATE_BYTES_DISABLE,   0b0 )
    DECLARE_ENUM_VAL( HSCB_ROTATE_BYTES_ENABLE,    0b1 )
END_ENUM( hscb_rotate_bytes_t )

BEGIN_ENUM( hscb_status_for_preparing_rmap_packet_t )
    DECLARE_ENUM_NEXT(PREPARE_RMAP_PACKET_OK)
    DECLARE_ENUM_NEXT(PREPARE_RMAP_PACKET_ADDR_CHAIN_IS_NULL)
    DECLARE_ENUM_NEXT(PREPARE_RMAP_PACKET_REPLY_ADDR_CHAIN_IS_NULL)
    DECLARE_ENUM_NEXT(PREPARE_RMAP_PACKET_REPLY_ADDR_CHAIN_TOO_LONG)
    DECLARE_ENUM_NEXT(PREPARE_RMAP_PACKET_REPLY_ADDR_CHAIN_WRONG_SIZE)
    DECLARE_ENUM_NEXT(PREPARE_RMAP_PACKET_DATA_LEN_OVERFLOW)
    DECLARE_ENUM_NEXT(PREPARE_RMAP_PACKET_DATA_CHAIN_IS_NULL)
END_ENUM( hscb_status_for_preparing_rmap_packet_t )

DECLARE_CONST(HSCB_RMAP_PROTOCOL_ID, 0x1)

BEGIN_ENUM( hscb_rmap_packet_second_section_t)
    DECLARE_ENUM_VAL(HSCB_RMAP_TARGET_LOGICAL_ADDR_i, 0)
    DECLARE_ENUM_VAL(HSCB_RMAP_PROTOCOL_ID_i, 1)
    DECLARE_ENUM_VAL(HSCB_RMAP_INSTRUCTION_i, 2)
    DECLARE_ENUM_VAL(HSCB_RMAP_KEY_i, 3)
END_ENUM( hscb_rmap_packet_second_section_t)

DECLARE_CONST(HSCB_RMAP_DEFAULT_KEY, 0x77)
DECLARE_CONST(HSCB_RMAP_DEFAULT_TARGET_LOGICAL_ADDRESS, 0x99)
DECLARE_CONST(HSCB_RMAP_DEFAULT_LOGICAL_ADDRESS, 0xFE)

BEGIN_ENUM( hscb_rmap_packet_fourth_section_t)
    DECLARE_ENUM_VAL(HSCB_RMAP_INITIATOR_LOGICAL_ADDR_i, 0)
    DECLARE_ENUM_VAL(HSCB_RMAP_TRANSACTION_ID_MS_i, 1)
    DECLARE_ENUM_VAL(HSCB_RMAP_TRANSACTION_ID_LS_i, 2)
    DECLARE_ENUM_VAL(HSCB_RMAP_EXT_ADDR_i, 3)
    DECLARE_ENUM_VAL(HSCB_RMAP_ADDR_3_i, 4)
    DECLARE_ENUM_VAL(HSCB_RMAP_ADDR_2_i, 5)
    DECLARE_ENUM_VAL(HSCB_RMAP_ADDR_1_i, 6)
    DECLARE_ENUM_VAL(HSCB_RMAP_ADDR_0_i, 7)
    DECLARE_ENUM_VAL(HSCB_RMAP_DATA_LEN_2_i, 8)
    DECLARE_ENUM_VAL(HSCB_RMAP_DATA_LEN_1_i, 9)
    DECLARE_ENUM_VAL(HSCB_RMAP_DATA_LEN_0_i, 10)
    DECLARE_ENUM_VAL(HSCB_RMAP_HEADER_CRC8_i,11)
END_ENUM( hscb_rmap_packet_fourth_section_t)

BEGIN_ENUM( HSCB_RMAP_PACKET_INSTRUCTION_FIELD )
    DECLARE_ENUM_VAL(   HSCB_RMAP_PACKET_INSTRUCTION_FIELD_REPLY_ADDR_LEN_i,       0 )
    DECLARE_ENUM_VAL(   HSCB_RMAP_PACKET_INSTRUCTION_FIELD_REPLY_ADDR_LEN_n,       2 )
    DECLARE_ENUM_VAL(   HSCB_RMAP_PACKET_INSTRUCTION_FIELD_REPLY_ADDR_LEN_mask,
            FIELD_MASK32( HSCB_RMAP_PACKET_INSTRUCTION_FIELD_REPLY_ADDR_LEN_i,
                    HSCB_RMAP_PACKET_INSTRUCTION_FIELD_REPLY_ADDR_LEN_n ) )
    DECLARE_ENUM_VAL(   HSCB_RMAP_PACKET_INSTRUCTION_FIELD_INCREMENT_i,       2 )
    DECLARE_ENUM_VAL(   HSCB_RMAP_PACKET_INSTRUCTION_FIELD_INCREMENT_n,       1 )
    DECLARE_ENUM_VAL(   HSCB_RMAP_PACKET_INSTRUCTION_FIELD_INCREMENT_mask,
            FIELD_MASK32( HSCB_RMAP_PACKET_INSTRUCTION_FIELD_INCREMENT_i,
                    HSCB_RMAP_PACKET_INSTRUCTION_FIELD_INCREMENT_n ) )
    DECLARE_ENUM_VAL(   HSCB_RMAP_PACKET_INSTRUCTION_FIELD_W_REPLY_i,       3 )
    DECLARE_ENUM_VAL(   HSCB_RMAP_PACKET_INSTRUCTION_FIELD_W_REPLY_n,       1 )
    DECLARE_ENUM_VAL(   HSCB_RMAP_PACKET_INSTRUCTION_FIELD_W_REPLY_mask,
            FIELD_MASK32( HSCB_RMAP_PACKET_INSTRUCTION_FIELD_W_REPLY_i,
                    HSCB_RMAP_PACKET_INSTRUCTION_FIELD_W_REPLY_n ) )
    DECLARE_ENUM_VAL(   HSCB_RMAP_PACKET_INSTRUCTION_FIELD_VERIFY_DATA_i,       4 )
    DECLARE_ENUM_VAL(   HSCB_RMAP_PACKET_INSTRUCTION_FIELD_VERIFY_DATA_n,       1 )
    DECLARE_ENUM_VAL(   HSCB_RMAP_PACKET_INSTRUCTION_FIELD_VERIFY_DATA_mask,
            FIELD_MASK32( HSCB_RMAP_PACKET_INSTRUCTION_FIELD_VERIFY_DATA_i,
                    HSCB_RMAP_PACKET_INSTRUCTION_FIELD_VERIFY_DATA_n ) )
    DECLARE_ENUM_VAL(   HSCB_RMAP_PACKET_INSTRUCTION_FIELD_WRITE_i,       5 )
    DECLARE_ENUM_VAL(   HSCB_RMAP_PACKET_INSTRUCTION_FIELD_WRITE_n,       1 )
    DECLARE_ENUM_VAL(   HSCB_RMAP_PACKET_INSTRUCTION_FIELD_WRITE_mask,
            FIELD_MASK32( HSCB_RMAP_PACKET_INSTRUCTION_FIELD_WRITE_i,
                    HSCB_RMAP_PACKET_INSTRUCTION_FIELD_WRITE_n ) )
    DECLARE_ENUM_VAL(   HSCB_RMAP_PACKET_INSTRUCTION_FIELD_PACKET_TYPE_i,       6 )
    DECLARE_ENUM_VAL(   HSCB_RMAP_PACKET_INSTRUCTION_FIELD_PACKET_TYPE_n,       2 )
    DECLARE_ENUM_VAL(   HSCB_RMAP_PACKET_INSTRUCTION_FIELD_PACKET_TYPE_mask,
            FIELD_MASK32( HSCB_RMAP_PACKET_INSTRUCTION_FIELD_PACKET_TYPE_i,
                    HSCB_RMAP_PACKET_INSTRUCTION_FIELD_PACKET_TYPE_n ) )
    DECLARE_ENUM_VAL(   HSCB_RMAP_PACKET_INSTRUCTION_RMAP_COMMAND_i, HSCB_RMAP_PACKET_INSTRUCTION_FIELD_INCREMENT_i)
    DECLARE_ENUM_VAL(   HSCB_RMAP_PACKET_INSTRUCTION_RMAP_COMMAND_n, 4)
    DECLARE_ENUM_VAL(   HSCB_RMAP_PACKET_INSTRUCTION_RMAP_COMMAND_mask,
            FIELD_MASK32( HSCB_RMAP_PACKET_INSTRUCTION_RMAP_COMMAND_i,
                    HSCB_RMAP_PACKET_INSTRUCTION_RMAP_COMMAND_n ))
END_ENUM( HSCB_RMAP_PACKET_INSTRUCTION_FIELD )

BEGIN_ENUM(HSCB_RMAP_COMAND_CODES)
    DECLARE_ENUM_VAL(   HSCB_RMAP_COMMAND_READ_SINGLE_ADDRESS,                          0b0010)
    DECLARE_ENUM_VAL(   HSCB_RMAP_COMMAND_READ_INCREMENTING_ADDRESS,                    0b0011)
    DECLARE_ENUM_VAL(   HSCB_RMAP_COMMAND_RMW_INCREMENTING_ADDRESS,                     0b0111)
    DECLARE_ENUM_VAL(   HSCB_RMAP_COMMAND_WRITE_SINGLE_ADDR_DONT_VERIFY_NO_REPLY,       0b1000)
    DECLARE_ENUM_VAL(   HSCB_RMAP_COMMAND_WRITE_INCR_ADDR_DONT_VERIFY_NO_REPLY,         0b1001)
    DECLARE_ENUM_VAL(   HSCB_RMAP_COMMAND_WRITE_SINGLE_ADDR_DONT_VERIFY_REPLY,          0b1010)
    DECLARE_ENUM_VAL(   HSCB_RMAP_COMMAND_WRITE_INCR_ADDR_DONT_VERIFY_REPLY,            0b1011)
    DECLARE_ENUM_VAL(   HSCB_RMAP_COMMAND_WRITE_SINGLE_ADDR_VERIFY_NO_REPLY,            0b1100)
    DECLARE_ENUM_VAL(   HSCB_RMAP_COMMAND_WRITE_INCR_ADDR_VERIFY_NO_REPLY,              0b1101)
    DECLARE_ENUM_VAL(   HSCB_RMAP_COMMAND_WRITE_SINGLE_ADDR_VERIFY_REPLY,               0b1110)
    DECLARE_ENUM_VAL(   HSCB_RMAP_COMMAND_WRITE_INCR_ADDR_VERIFY_REPLY,                 0b1111)
END_ENUM(HSCB_RMAP_COMAND_CODES)

BEGIN_ENUM( hscb_rmap_packet_instruction_reply_addr_len_t)
    DECLARE_ENUM_VAL(   HSCB_RMAP_PACKET_REPLY_ADDR_0B,       0b00 )
    DECLARE_ENUM_VAL(   HSCB_RMAP_PACKET_REPLY_ADDR_4B,       0b01 )
    DECLARE_ENUM_VAL(   HSCB_RMAP_PACKET_REPLY_ADDR_8B,       0b10 )
    DECLARE_ENUM_VAL(   HSCB_RMAP_PACKET_REPLY_ADDR_12B,      0b11 )
END_ENUM( hscb_rmap_packet_instruction_reply_addr_len_t )

BEGIN_ENUM( hscb_rmap_packet_instruction_increment_t)
    DECLARE_ENUM_VAL(   HSCB_RMAP_PACKET_NO_INCREMENT,       0b0 )
    DECLARE_ENUM_VAL(   HSCB_RMAP_PACKET_INCREMENT,          0b1 )
END_ENUM( hscb_rmap_packet_instruction_increment_t )

BEGIN_ENUM( hscb_rmap_packet_instruction_reply_t)
    DECLARE_ENUM_VAL(   HSCB_RMAP_PACKET_NO_REPLY,       0b0 )
    DECLARE_ENUM_VAL(   HSCB_RMAP_PACKET_REPLY,          0b1 )
END_ENUM( hscb_rmap_packet_instruction_reply_t )

BEGIN_ENUM( hscb_rmap_packet_instruction_verify_t)
    DECLARE_ENUM_VAL(   HSCB_RMAP_PACKET_DONT_VERIFY,       0b0 )
    DECLARE_ENUM_VAL(   HSCB_RMAP_PACKET_VERIFY,            0b1 )
END_ENUM( hscb_rmap_packet_instruction_verify_t )

BEGIN_ENUM( hscb_rmap_packet_instruction_command_t)
    DECLARE_ENUM_VAL(   HSCB_RMAP_PACKET_READ,              0b0 )
    DECLARE_ENUM_VAL(   HSCB_RMAP_PACKET_WRITE,             0b1 )
END_ENUM( hscb_rmap_packet_instruction_command_t )

BEGIN_ENUM( hscb_rmap_packet_instruction_pack_type_t)
    DECLARE_ENUM_VAL(   HSCB_RMAP_PACKET_TYPE_REPLY,                0b00 )
    DECLARE_ENUM_VAL(   HSCB_RMAP_PACKET_TYPE_COMMAND,              0b01 )
    DECLARE_ENUM_VAL(   HSCB_RMAP_PACKET_TYPE_RESERVED_10,          0b10 )
    DECLARE_ENUM_VAL(   HSCB_RMAP_PACKET_TYPE_RESERVED_11,          0b11 )
END_ENUM( hscb_rmap_packet_instruction_pack_type_t )

BEGIN_ENUM( hscb_rmap_reply_packet_fields_t)
    DECLARE_ENUM_VAL(HSCB_RMAP_REPLY_INITIATOR_LOGICAL_ADDR_i, 0)
    DECLARE_ENUM_VAL(HSCB_RMAP_REPLY_PROTOCOL_ID_i, 1)
    DECLARE_ENUM_VAL(HSCB_RMAP_REPLY_INSTRUCTION_i, 2)
    DECLARE_ENUM_VAL(HSCB_RMAP_REPLY_STATUS_i, 3)
    DECLARE_ENUM_VAL(HSCB_RMAP_REPLY_TARGET_LOGICAL_ADDR_i, 4)
    DECLARE_ENUM_VAL(HSCB_RMAP_REPLY_TRANSACTION_ID_MS_i, 5)
    DECLARE_ENUM_VAL(HSCB_RMAP_REPLY_TRANSACTION_ID_LS_i, 6)
    DECLARE_ENUM_VAL(HSCB_RMAP_REPLY_RESERVED_R_HEADER_CRC8_W_i, 7)
    DECLARE_ENUM_VAL(HSCB_RMAP_REPLY_DATA_LEN_2_i, 8)
    DECLARE_ENUM_VAL(HSCB_RMAP_REPLY_DATA_LEN_1_i, 9)
    DECLARE_ENUM_VAL(HSCB_RMAP_REPLY_DATA_LEN_0_i, 10)
    DECLARE_ENUM_VAL(HSCB_RMAP_REPLY_HEADER_CRC8_R_i,11)
    DECLARE_ENUM_VAL(HSCB_RMAP_REPLY_DATA_START_i,12)
END_ENUM( hscb_rmap_reply_packet_fields_t)

BEGIN_ENUM( hscb_rmap_reply_packet_status_codes_t)
    DECLARE_ENUM_VAL(HSCB_RMAP_REPLY_STATUS_OK,                             0)
    DECLARE_ENUM_VAL(HSCB_RMAP_REPLY_STATUS_GENERAL_ERROR,                  1)
    DECLARE_ENUM_VAL(HSCB_RMAP_REPLY_STATUS_INVALID_INSTRUCTION,            2)
    DECLARE_ENUM_VAL(HSCB_RMAP_REPLY_STATUS_INVALID_KEY,                    3)
    DECLARE_ENUM_VAL(HSCB_RMAP_REPLY_STATUS_INVALID_DATA_CRC,               4)
    DECLARE_ENUM_VAL(HSCB_RMAP_REPLY_STATUS_EARLY_EOP,                      5)
    DECLARE_ENUM_VAL(HSCB_RMAP_REPLY_STATUS_TOO_MUCH_DATA,                  6)
    DECLARE_ENUM_VAL(HSCB_RMAP_REPLY_STATUS_EEP,                            7)
    DECLARE_ENUM_VAL(HSCB_RMAP_REPLY_STATUS_RESERVED_8,                     8)
    DECLARE_ENUM_VAL(HSCB_RMAP_REPLY_STATUS_VERIFY_BUFFER_OVERRUN,          9)
    DECLARE_ENUM_VAL(HSCB_RMAP_REPLY_STATUS_NOT_AUTORIZED_CMD,              10)
    DECLARE_ENUM_VAL(HSCB_RMAP_REPLY_STATUS_RMW_DATA_LENGTH_ERROR,          11)
    DECLARE_ENUM_VAL(HSCB_RMAP_REPLY_STATUS_INVALID_TARGET_LOGICAL_ADDR,    12)
END_ENUM( hscb_rmap_reply_packet_status_codes_t)


#define HSCB_CREATE_DESCRIPTOR_LEN_ATTR(length,act,act0,ie,valid)  (\
    (((length) << HSCB_RD_DESCR_LENGTH_i) & HSCB_RD_DESCR_LENGTH_mask)   | \
    (((act)    << HSCB_RD_DESCR_ACT_i)    & HSCB_RD_DESCR_ACT_mask)      | \
    (((act0)   << HSCB_RD_DESCR_ACT0_i)   & HSCB_RD_DESCR_ACT0_mask)     | \
    (((ie)     << HSCB_RD_DESCR_IE_i)     & HSCB_RD_DESCR_IE_mask)       | \
    (((valid)  << HSCB_RD_DESCR_VALID_i)  & HSCB_RD_DESCR_VALID_mask)       )

#define HSCB_CHANGE_ENDIAN_WORD(word)           \
    (((word) << 24) & 0xff000000) | \
    (((word) <<  8) & 0x00ff0000) | \
    (((word) >>  8) & 0x0000ff00) | \
    (((word) >> 24) & 0x000000ff)

#define HSCB_CREATE_DESCRIPTOR_LEN_ATTR_ENDIAN_EXE(length,act,act0,ie,valid,change_endian) \
    (change_endian) ?   \
        HSCB_CHANGE_ENDIAN_WORD( \
             HSCB_CREATE_DESCRIPTOR_LEN_ATTR( \
                     length, \
                     act, \
                     act0, \
                     ie, \
                     valid) ) \
        :   HSCB_CREATE_DESCRIPTOR_LEN_ATTR( \
                     length, \
                     act, \
                     act0, \
                     ie, \
                     valid)

/**
 * @}
 */


#endif /* REGS_HSCB_H */
