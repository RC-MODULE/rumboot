

#ifndef NU_LIB_H
#define NU_LIB_H

#include <stdint.h>

#define NU_VPE_CFG_PARAMS_NUM 59
#define NU_MPE_CFG_PARAMS_NUM 18
#define NU_PPE_CFG_PARAMS_NUM 23
#define NU_PPE_REG_CFG_PARAMS_NUM 48

  /**
  Режим работы блока
  */
  typedef enum Mode {
    Mode_Unitary,
    Mode_Channel,
    Mode_Element
  } Mode;

  /**
  Режим округления нормализатора
  */
  typedef enum RoundMode {
    RoundMode_Nearest,
    RoundMode_Down,
    RoundMode_Up
  }RoundMode;


  /**
  Активация элемента
  */
  typedef enum Enable {
    Enable_NotEn,
    Enable_En
  }Enable;


  /**
  Операции alu
  */
  typedef enum AluOperationSwitch {
    AluOperationSwitch_Max,
    AluOperationSwitch_Min,
    AluOperationSwitch_Sum
  }AluOperationSwitch;

  /**
  Расширенны набор операци alu
  */
  typedef enum AluOperationExtSwitch {
    AluOperationExtSwitch_Max,
    AluOperationExtSwitch_Min,
    AluOperationExtSwitch_Sum,
    AluOperationExtSwitch_Eql
  }AluOperationExtSwitch;

  /**
  Операции pooling
  */
  typedef enum PoolingOperationSwitch {
    PoolingOperationSwitch_Avg,
    PoolingOperationSwitch_Max,
    PoolingOperationSwitch_Min
  }PoolingOperationSwitch;


  /**
  Тип обрабатываемых данных
  */
  typedef enum DataType {
    DataType_Int8,
    DataType_Int16,
    DataType_Fp16
  }DataType;


  /**
  Тип вычислений
  */
  typedef enum DataTypeExt {
    DataTypeExt_Int8,
    DataTypeExt_Int16,
    DataTypeExt_Fp16,
    DataTypeExt_Int32,
    DataTypeExt_Fp32
  }DataTypeExt;

  typedef struct ConfigMPE {
    uint32_t H        ;
    uint32_t W        ;
    uint32_t C        ;
    uint32_t Tp       ;
    uint32_t Bp       ;
    uint32_t Lp       ;
    uint32_t Rp       ;
    uint32_t R        ;
    uint32_t S        ;
    uint32_t Ds       ;
    uint32_t Dr       ;
    uint32_t Sw       ;
    uint32_t Sh       ;
    uint32_t K        ;
    DataType dt       ;
    uint32_t RND_MODE ;
    uint32_t SAT      ;
    uint32_t RND_SIZE ;
  }ConfigMPE;



  typedef struct ConfigOp01 {
    DataType coef_type;
    Enable alu_en;
    Enable mux_en;
    Enable relu_en;
    Enable prelu_en;
    AluOperationSwitch alu_operation;
    uint8_t lshift_value; // !!!! вообще он uint6
    Mode alu_mode;
    uint32_t alu_value; //!!!!
    Mode mux_mode;
    uint32_t mux_value; //!!!! Тут может бытb другои тип
    // T prelu_value;
    RoundMode norm_round_mode;
    Enable norm_saturation_en;
    uint8_t norm_round_size;
  }ConfigOp01;

  typedef struct ConfigOp2 {
    DataType coef_type;
    Enable alu_en;
    Enable mux_en;
    Enable lut_en;
    Enable prelu_en;
    AluOperationExtSwitch alu_operation;
    Enable c1_en;
    Enable c2_en;
    Mode alu_mode;
    uint32_t alu_value; //!!!! Тут может бытb другои тип
    Mode mux_mode;
    uint32_t mux_value; //!!!! Тут может бытb другои тип
    // T prelu_value;
    RoundMode norm_round_mode;
    Enable norm_saturation_en;
    uint8_t norm_round_size;
    int16_t c1_offset;
    int16_t c1_scale;
    uint8_t c1_trunc; // !!!! вообще он uint6
    int16_t c2_offset;
    int16_t c2_scale;
    uint8_t c2_trunc; // !!!! вообще он uint6
  }ConfigOp2;

  typedef struct ConfigVPE {
    DataTypeExt in_data_type;
    DataType out_data_type;
    Enable flying;
    Enable op0_en;
    Enable op1_en;
    Enable op2_en;
    int32_t c3_offset;
    int16_t c3_scale;
    uint8_t c3_trunc; // !!!! вообще он uint6
    Enable nan_to_zero;
    ConfigOp01 op0_config;
    ConfigOp01 op1_config;
    ConfigOp2 op2_config;
  }ConfigVPE;

  typedef struct ConfigDMAVPE {
    Enable dma_src_en;
    Enable dma_op0_en;
    Enable dma_op1_en;
    Enable dma_op2_en;
    Enable dma_dst_en;
    int32_t H;
    int32_t W;
    int32_t C;
    // Add Here DMA Parameter Fields
  }ConfigDMAVPE;

  typedef struct ConfigDMAMPE {
    int32_t H;
    int32_t W;
    int32_t C;
    int32_t R;
    int32_t S;
    int in_data_partition;
    int warr_partition;
    // Add Here DMA Parameter Fields
  }ConfigDMAMPE;

  /**
  Настройки блока субдескритизации
  */
  typedef struct ConfigPPE{
    uint32_t H      ;
    uint32_t W      ;
    uint32_t C      ;
    uint32_t Kh     ;
    uint32_t Kw     ;
    uint32_t Sh     ;
    uint32_t Sw     ;
    uint32_t Kh_r   ;
    uint32_t Kw_r   ;
    uint32_t Tp     ;
    uint32_t Bp     ;
    uint32_t Lp     ;
    uint32_t Rp     ;
    uint32_t pv[7]  ;
    PoolingOperationSwitch  meth;
    DataType                dt  ;
    uint32_t MC     ;
  }ConfigPPE;

  typedef struct ConfigREGPPE{
    // rdma
    // uint32_t rSt      ; // STATUS
    uint32_t rOpEn    ; // OP_ENABLE          / Start
    uint32_t rWi      ; // DATA_W_IN          / Input Width size
    uint32_t rHi      ; // DATA_H_IN          / Input Height size
    uint32_t rCi      ; // DATA_C_IN          / Input Channel size
    uint32_t rBALs    ; // SRC_BASE_ADDR_L    / Start address of Cube
    // uint32_t rBAHs    ; // SRC_BASE_ADDR_H    / Start address of Cube
    uint32_t rVSs     ; // SRC_VECTOR_STRIDE  / DMA Stride
    uint32_t rLSs     ; // SRC_LINE_STRIDE    / DMA Stride
    uint32_t rESs     ; // SRC_ELEMENT_STRIDE / DMA Stride
    uint32_t rOpM     ; // OP_MODE            / Operation mode params
    uint32_t rBSWi    ; // BOX_SIZE_W_IN      / DMA Working Box Size w/o Offset
    uint32_t rBSHi    ; // BOX_SIZE_H_IN      / DMA Working Box Size
    uint32_t rBSCi    ; // BOX_SIZE_C_IN      / DMA Working Box Size
    uint32_t rStWi    ; // BOX_START_W_IN     / DMA First part Working Box Size
    uint32_t rOfWi    ; // BOX_OFFSET_W_IN    / DMA Box Offset
    uint32_t rK       ; // KERNEL             / Pooling kernel params
    // ppe + wdma
    // uint32_t wSt      ; // STATUS
    uint32_t wOpEn    ; // OP_ENABLE          / Start
    uint32_t wWi      ; // DATA_W_IN          / Input Width size
    uint32_t wHi      ; // DATA_H_IN          / Input Height size
    uint32_t wCi      ; // DATA_C_IN          / Input Channel size
    uint32_t wWo      ; // DATA_W_OUT         / Output Width size
    uint32_t wHo      ; // DATA_H_OUT         / Output Height size
    uint32_t wCo      ; // DATA_C_OUT         / Output Channel size
    uint32_t wBALd    ; // DST_BASE_ADDR_L    / Start address of Cube
    // uint32_t wBAHd    ; // DST_BASE_ADDR_H    / Start address of Cube
    uint32_t wVSd     ; // DST_VECTOR_STRIDE  / DMA Stride
    uint32_t wLSd     ; // DST_LINE_STRIDE    / DMA Stride
    uint32_t wESd     ; // DST_ELEMENT_STRIDE / DMA Stride
    uint32_t wOpM     ; // OP_MODE            / Operation mode params
    uint32_t wBSWi    ; // BOX_SIZE_W_IN      / DMA Input Working Box Size w/o Offset
    uint32_t wBSHi    ; // BOX_SIZE_H_IN      / DMA Input Working Box Size
    uint32_t wBSCi    ; // BOX_SIZE_C_IN      / DMA Input Working Box Size
    uint32_t wStWi    ; // BOX_START_W_IN     / DMA Input First part Working Box Size
    uint32_t wOfWi    ; // BOX_OFFSET_W_IN    / DMA Input Box Offset
    uint32_t wBSWo    ; // BOX_SIZE_W_OUT     / DMA Output Working Box Size w/o Offset
    uint32_t wBSHo    ; // BOX_SIZE_H_OUT     / DMA Output Working Box Size
    uint32_t wBSCo    ; // BOX_SIZE_C_OUT     / DMA Output Working Box Size
    uint32_t wStWo    ; // BOX_START_W_OUT    / DMA Output First part Working Box Size
    uint32_t wOfWo    ; // BOX_OFFSET_W_OUT   / DMA Output Box Offset
    uint32_t wK       ; // KERNEL             / Pooling kernel params
    uint32_t wKWr     ; // RECIP_KERNEL_W     /
    uint32_t wKHr     ; // RECIP_KERNEL_H     /
    uint32_t wP       ; // PADDING            /
    uint32_t wPV1     ; // PADDING_VALUE_1    /
    uint32_t wPV2     ; // PADDING_VALUE_2    /
    uint32_t wPV3     ; // PADDING_VALUE_3    /
    uint32_t wPV4     ; // PADDING_VALUE_4    /
    uint32_t wPV5     ; // PADDING_VALUE_5    /
    uint32_t wPV6     ; // PADDING_VALUE_6    /
    uint32_t wPV7     ; // PADDING_VALUE_7    /
    // uint32_t wINi     ; // INF_NUM_IN
    // uint32_t wNNi     ; // NAN_NUM_IN
    // uint32_t wNNo     ; // NAN_NUM_OUT
  }ConfigREGPPE;

  typedef struct CubeMetrics{
    int32_t s;   // Size That Cube Occupies (in bytes)
    int32_t H;
    int32_t W;
    int32_t C;
  }CubeMetrics;

  typedef struct WarrMetrics{
    int32_t s;   // Size That Warr Occupies (in bytes)
    int32_t H;
    int32_t W;
    int32_t C;
    int32_t K;
  }WarrMetrics;

  typedef struct VectorMetrics{
    int32_t s;   // Size That Vector Occupies (in bytes)
    int32_t vec_size; // Number Of Elements
  }VectorMetrics;


void nu_vpe_load_config(ConfigVPE* cfg, void* cfg_bin);
void nu_vpe_print_config(ConfigVPE* cfg);
void nu_vpe_print_config_dma(ConfigDMAVPE* cfg);

void nu_vpe_setup(uintptr_t base, ConfigVPE* cfg, ConfigDMAVPE* cfg_dma);
void nu_vpe_decide_dma_config_trivial(ConfigVPE* cfg, CubeMetrics* metrics, ConfigDMAVPE* cfg_dma);

void nu_mpe_load_config(ConfigMPE* cfg, void* cfg_bin);
void nu_mpe_print_config(ConfigMPE* cfg);
void nu_mpe_print_config_dma(ConfigDMAMPE* cfg);

int  nu_mpe_get_size_in_partitions(int size_in_bytes);
void nu_mpe_setup(uintptr_t base, ConfigMPE* cfg, ConfigDMAMPE* cfg_dma);
int  nu_mpe_decide_dma_config_trivial(ConfigMPE* cfg, CubeMetrics* cube_metrics, WarrMetrics* warr_metrics, ConfigDMAMPE* cfg_dma);

void nu_mpe_load_buf(uintptr_t base, void* data, int size);

void nu_ppe_load_config(ConfigPPE* cfg, void* cfg_bin);
void nu_ppe_print_config(ConfigPPE* cfg);

void nu_ppe_setup(uintptr_t base, ConfigPPE* cfg);
void nu_ppe_setup_reg(uintptr_t rbase, uintptr_t wbase, ConfigREGPPE* cfg);

void nu_ppe_rdma_run(uintptr_t rbase, ConfigREGPPE* cfg);
void nu_ppe_run(uintptr_t wbase, ConfigREGPPE* cfg);

void nu_ppe_rdma_wait_complete(uintptr_t rbase);
void nu_ppe_wait_complete(uintptr_t wbase);

void nu_vpe_config_rd_main_channel(uintptr_t dma_base, void *addr, int size);
void nu_vpe_run_rd_main_channel(uintptr_t dma_base);
void nu_vpe_wait_rd_main_channel_complete(uintptr_t dma_base);

void nu_vpe_run(uintptr_t vpe_base, ConfigDMAVPE* cfg);
void nu_vpe_wait(uintptr_t vpe_base, ConfigDMAVPE* cfg);

void nu_mpe_run(uintptr_t mpe_base, ConfigDMAMPE* cfg_dma);
void nu_mpe_wait(uintptr_t mpe_base, ConfigDMAMPE* cfg_dma);

void nu_vpe_config_wr_main_channel(uintptr_t dma_base, void *addr, int size);
void nu_vpe_run_wr_main_channel(uintptr_t dma_base);
void nu_vpe_wait_wr_main_channel_complete(uintptr_t dma_base);

void nu_mpe_config_wr_main_channel(uintptr_t dma_base, void *addr, int size);
void nu_mpe_run_wr_main_channel(uintptr_t dma_base);
void nu_mpe_wait_wr_main_channel_complete(uintptr_t dma_base);

void nu_ppe_config_rd_main_channel(uintptr_t dma_base, void *addr, int size);
void nu_ppe_run_rd_main_channel(uintptr_t dma_base);
void nu_ppe_wait_rd_main_channel_complete(uintptr_t dma_base);


#endif
