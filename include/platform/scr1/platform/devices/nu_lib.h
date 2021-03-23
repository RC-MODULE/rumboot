

#ifndef NU_LIB_H
#define NU_LIB_H

#include <stdint.h>

#define NU_VPE_CFG_PARAMS_NUM 54
#define NU_MPE_CFG_PARAMS_NUM 21
#define NU_PPE_CFG_PARAMS_NUM 23

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
    uint32_t LINES    ;
    uint32_t COLUMNS  ;
    uint32_t BUF_NMB  ;
    uint32_t RND_MODE ;
    uint32_t SAT      ;
    uint32_t RND_SIZE ;
  }ConfigMPE;

  

  typedef struct ConfigOp01 {
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
    DataType data_type;
    Enable flying;
    Enable op0_en;
    Enable op1_en;
    Enable op2_en;
    int32_t c3_offset;
    int16_t c3_scale;
    uint8_t c3_trunc; // !!!! вообще он uint6
    ConfigOp01 op0_config;
    ConfigOp01 op1_config;
    ConfigOp2 op2_config;
  }ConfigVPE;
  
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
  
  
void nu_vpe_load_config(ConfigVPE* cfg, void* cfg_bin);
void nu_vpe_print_config(ConfigVPE* cfg);

void nu_vpe_setup(uintptr_t base, ConfigVPE* cfg);

void nu_mpe_load_config(ConfigMPE* cfg, void* cfg_bin);
void nu_mpe_print_config(ConfigMPE* cfg);

void nu_mpe_setup(uintptr_t base, ConfigMPE* cfg);

void nu_mpe_load_buf(uintptr_t base, void* data, int size);

void nu_ppe_load_config(ConfigPPE* cfg, void* cfg_bin);
void nu_ppe_print_config(ConfigPPE* cfg);

void nu_ppe_setup(uintptr_t base, ConfigPPE* cfg);

void nu_vpe_config_rd_main_channel(uintptr_t dma_base, void *addr, int size);
void nu_vpe_run_rd_main_channel(uintptr_t dma_base);
void nu_vpe_wait_rd_main_channel_complete(uintptr_t dma_base);

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

