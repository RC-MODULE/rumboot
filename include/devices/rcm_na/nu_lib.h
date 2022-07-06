

#ifndef NU_LIB_H
#define NU_LIB_H

#include <stdint.h>
#include <rumboot/io.h>

#define NU_VPE_CFG_PARAMS_NUM 83
#define NU_MPE_CFG_PARAMS_NUM 23
#define NU_PPE_CFG_PARAMS_NUM 22
#define NU_PPE_REG_CFG_PARAMS_NUM 62

#define NU_MPE_DMA_PARAM_TABLE_ROW_SIZE 279

#define NU_VPE_LUT1_SIZE 257
#define NU_VPE_LUT2_SIZE 65

  /**
  Режим работы блока
  */
  typedef enum Mode {
    Mode_Unitary = 0,
    Mode_Channel = 2,
    Mode_Element = 3
  } Mode;

  /**
  Режим округления нормализатора
  */
  typedef enum RoundMode {
    RoundMode_Down,
    RoundMode_TowardsZero,
    RoundMode_Up,
    RoundMode_AwayFromZero,
    RoundMode_HalfDown,
    RoundMode_HalfTowardsZero,
    RoundMode_Nearest, // Half Up
    RoundMode_HalfAwayFromZero
  }RoundMode;

  /**
  Режим обхода при чтении данных из DMA
  */
  typedef enum TraceMode {
    TraceMode_PPE,
    TraceMode_MPE,
    TraceMode_Linear,
    TraceMode_MPE_DW
  }TraceMode;

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
    AluOperationSwitch_Sum,
    AluOperationSwitch_Sub
  }AluOperationSwitch;

  /**
  Расширенны набор операци alu
  */
  typedef enum AluOperationExtSwitch {
    AluOperationExtSwitch_Max,
    AluOperationExtSwitch_Min,
    AluOperationExtSwitch_Sum,
    AluOperationExtSwitch_Sub,
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


  /**
  Type DMA_RAM_TYPE
  */
  typedef enum DmaRamType {
    DmaRamType_CV,
    DmaRamType_MC
  }DmaRamType;


  /**
  Type DMA_DATA_SIZE
  */
  typedef enum DmaDSizeType {
    DmaDSize_One_Byte,
    DmaDSize_Two_Byte
  }DmaDSizeType;

  /**
  Type DMA_DATA_USE
  */
  typedef enum DmaDUseType {
    DmaDUse_Off,
    DmaDUse_Mux,
    DmaDUse_Alu,
    DmaDUse_Both
  }DmaDUseType;
  
  
  typedef enum DmaXYZDirectionType {
    DmaXYZDirection_X,
    DmaXYZDirection_Y,
    DmaXYZDirection_Z
  }DmaXYZDirectionType;
  
  typedef enum CubeRole {
    CubeRole_NotLastInBatch,
    CubeRole_LastInBatch
  }CubeRole;

  /**
  VPE modes for ResNet50
  */
  typedef enum VPE_Mode {
    VPE_MODE_BN,
    VPE_MODE_BN_RELU,
    VPE_MODE_BN_RELU_ADD
  }VPE_Mode;

  typedef struct ConfigMAMPE {
    uint16_t ADD_CountI0;
    uint16_t CMP_CountI0;
    uint16_t ADD_CountI1;
    uint16_t CMP_CountI1;
    uint16_t ADD_CountJ;
    uint16_t CMP_CountJ;
    uint16_t ADD_CountPPS;
    uint16_t CMP_CountPPS;
    uint8_t ADD_CountLCZ;
    uint8_t CMP_CountLCZ;
    uint16_t ADD_CountPLC;
    uint16_t CMP_CountPLC;
    uint16_t ADD_CountX;
    uint16_t CMP_CountX;
    uint16_t ADD_CountY;
    uint16_t CMP_CountY;
    uint16_t ADD_CountM;
    uint16_t CMP_CountM;
    
    uint16_t BRDR;  // IN_BUF_CFG
    uint16_t WA;
    uint16_t DA;
    uint16_t VRA  ;
    uint8_t NR   ;
    uint8_t D_BIAS;
    uint32_t D_NULL;
  }ConfigMAMPE;
  
  typedef struct ConfigRDDMAMPEBias {
    Enable   BiasEn;
    uint8_t  ThreCtrl;
    uint8_t  DecCtrl;
    Enable   PXBSEn;
    Enable   PYBSEn;
    uint32_t Bias ;
    uint32_t AOffset ;
    uint16_t CntSha ;
    Enable   CntOffsetEn ;
    uint8_t  CntOffset ;
    uint16_t CntThresholdSha ;
    uint16_t CntCmp ;
  }ConfigRDDMAMPEBias;
  
  typedef struct ConfigWRDMAMPEBias {
    uint16_t CntSha;
    uint16_t CntCmp;
  }ConfigWRDMAMPEBias;
  
  typedef struct ConfigRDDMAMPE {
    ConfigRDDMAMPEBias Bias[7];
    uint32_t BFCA;
    uint32_t AOffset;
    uint32_t LPXOffset;
    uint32_t RPXOffset;
    uint32_t TPYOffset;
    uint32_t BPYOffset;
    uint16_t CntSha;
    uint16_t CntThresholdSha;
    uint8_t  ThreCtrl;
    Enable LPXEn ;
    Enable RPXEn ;
    Enable TPYEn ;
    Enable BPYEn ;
    uint16_t LPXData;
    uint16_t RPXData;
    uint16_t TPYData;
    uint16_t BPYData;
  }ConfigRDDMAMPE;
  
  typedef struct ConfigWRDMAMPE {
    ConfigWRDMAMPEBias Bias[6];
    uint16_t BADR;
    uint16_t LADR;
    Enable  USED ;
    Enable  ADR_PROL ;
    Enable  C_BND ;  // C > 8bytes -> Enable_En, C <= 8bytes -> Enable_NotEn
    Enable  GLUE_EN ;
    uint8_t BStrideX ;
    uint8_t BConvX ;
    uint8_t Batch;
    uint8_t Thre_PLC ;
    uint8_t Thre_VLC ;
    uint8_t Thre_CXC ;
    uint8_t Dec_PLC;
    uint8_t PLC_CntSha;
    uint8_t VLC_CntSha;
    uint16_t PLC_ThreSha;
    uint16_t VLC_ThreSha;
  }ConfigWRDMAMPE;
  
  typedef struct ConfigDMAMPE {
    ConfigRDDMAMPE rdma;
    ConfigWRDMAMPE wdma;
    uint16_t MAINCNT;
    uint32_t depend_mask;
  }ConfigDMAMPE;

  typedef struct ConfigMPE {
    uint32_t  H         ;
    uint32_t  W         ;
    uint32_t  C         ;
    uint32_t  Tp        ;
    uint32_t  Bp        ;
    uint32_t  Lp        ;
    uint32_t  Rp        ;
    uint32_t  R         ;
    uint32_t  S         ;
    uint32_t  Dr        ;
    uint32_t  Ds        ;
    uint32_t  Sh        ;
    uint32_t  Sw        ;
    uint32_t  K         ;
    DataType  dt        ;
    uint32_t  batch_size;
    RoundMode rnd_mode  ;
    Enable    sat_en    ;
    uint32_t  rnd_size  ;
    uint32_t  TpData    ;
    uint32_t  BpData    ;
    uint32_t  LpData    ;
    uint32_t  RpData    ;

    ConfigDMAMPE  dma_d_config;
    ConfigDMAMPE  dma_w_config;
    ConfigMAMPE   ma_config   ;
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
    DataType  coef_type;
    Enable    alu_en;
    Enable    mux_en;
    Enable    lut_en;
    Enable    prelu_en;
    AluOperationExtSwitch alu_operation;
    Enable    c1_en;
    Enable    c2_en;
    Mode      alu_mode;
    uint32_t  alu_value; //!!!! Тут может бытb другои тип
    Mode      mux_mode;
    uint32_t  mux_value; //!!!! Тут может бытb другои тип
    // T prelu_value;
    RoundMode norm_round_mode;
    Enable    norm_saturation_en;
    uint8_t   norm_round_size;
    int16_t   c1_offset;
    int16_t   c1_scale;
    uint8_t   c1_trunc; // !!!! вообще он uint6
    int16_t   c2_offset;
    int16_t   c2_scale;
    uint8_t   c2_trunc; // !!!! вообще он uint6
    uint8_t   lut_sel;  // !!! bit
    uint8_t   lut_right_priority;
    uint8_t   lut_left_priority;
    uint32_t  lut_tab1_x_start; //!!!! Тут может бытb другои тип
    uint32_t  lut_tab1_x_end;
    uint32_t  lut_tab2_x_start;
    uint32_t  lut_tab2_x_end;
    uint32_t  lut_tab1_slope_ovrf_scale;//!!!! Тут может бытb другои тип
    uint32_t  lut_tab1_slope_undf_scale;
    uint32_t  lut_tab2_slope_ovrf_scale;
    uint32_t  lut_tab2_slope_undf_scale;
    int8_t    lut_tab1_slope_ovrf_shift;
    int8_t    lut_tab1_slope_undf_shift;
    int8_t    lut_tab2_slope_ovrf_shift;
    int8_t    lut_tab2_slope_undf_shift;
    Enable lut_mrr_en;
    int8_t lut_mrr_type;
    uint32_t lut_xoffset; //  T_OP2 
    uint16_t lut_yoffset; //  T_OP2 Encoded Into int16_t Or half
  }ConfigOp2;

  typedef struct ConfigDMA {
    Enable       dma_en              ;
    DmaRamType   dma_ram_type        ;
    Mode         dma_data_mode       ; // convert ????
    DmaDSizeType dma_data_size       ;
    DmaDUseType  dma_data_use        ;
    uint32_t     dma_baddr           ;
    uint16_t     dma_cube_size_h     ;
    uint16_t     dma_cube_size_w     ;
    uint16_t     dma_cube_size_c     ;
    uint32_t     dma_border_x        ;
    uint32_t     dma_border_y        ;
    uint32_t     dma_border_z        ;
    uint32_t     dma_stride_x        ;
    uint32_t     dma_stride_y        ;
    uint32_t     dma_stride_z        ;
    uint32_t     dma_frag_size       ;
    uint32_t     dma_frag_last_size  ;
    DmaXYZDirectionType  dma_xyz_drct;
    uint16_t     dma_box_st_size_x   ;
    uint16_t     dma_box_st_size_y   ;
    uint16_t     dma_box_st_size_z   ;
    uint16_t     dma_box_size_x      ;
    uint16_t     dma_box_size_y      ;
    uint16_t     dma_box_size_z      ;
    uint16_t     dma_box_offset_x    ;
    uint16_t     dma_box_offset_y    ;
    uint16_t     dma_box_offset_z    ;
    uint8_t      dma_axi_len         ;
    uint8_t      dma_bsize           ;
    uint32_t     dma_bstride         ;
  }ConfigDMA;

  typedef struct ConfigVPE {
    DataTypeExt in_data_type    ;
    DataType    out_data_type   ;
    Enable      src_flying      ;
    Enable      dst_flying      ;
    Enable      op0_en          ;
    Enable      op1_en          ;
    Enable      op2_en          ;
    int32_t     c3_offset       ;
    int16_t     c3_scale        ;
    uint8_t     c3_trunc        ; // !!!! вообще он uint6
    Enable      c3_satur_en     ;
    RoundMode   c3_round_mode   ;
    Enable      nan_to_zero_input;
    Enable      nan_to_zero_output;
    TraceMode   trace_mode      ;
    ConfigOp01  op0_config      ;
    ConfigOp01  op1_config      ;
    ConfigOp2   op2_config      ;
    Enable      ninf_cnt_en     ;
    Enable      ovrf_cnt_en     ;
    Enable      lut_cnt_en      ;
    ConfigDMA   src_rdma_config ;
    ConfigDMA   op0_rdma_config ;
    ConfigDMA   op1_rdma_config ;
    ConfigDMA   op2_rdma_config ;
    ConfigDMA   wdma_config     ;
    int32_t     cube_size       ;
    Enable      mark            ;
    uint32_t    depend_mask     ;
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
//    uint32_t Kh_r   ;
//    uint32_t Kw_r   ;

    uint32_t Kwh_r   ;

    uint32_t Tp     ;
    uint32_t Bp     ;
    uint32_t Lp     ;
    uint32_t Rp     ;
    uint32_t pv[7]  ;
    PoolingOperationSwitch  meth;
    DataType                dt  ;

    Enable nan_to_zero_input;
  }ConfigPPE;

  typedef struct ConfigREGPPE{
    // MY_PPE_RDMA_BASE
    uint32_t rOpEn;
    uint32_t rAXIp;
    uint32_t rBALi; // PPE_RDMA_BASE_ADDR
    uint32_t rBrdX; // PPE_RDMA_BORDER_X
    uint32_t rBrdY; // PPE_RDMA_BORDER_Y
    uint32_t rBrdZ; // PPE_RDMA_BORDER_Z
    uint32_t rStrX; // PPE_RDMA_STRIDE_X
    uint32_t rStrY; // PPE_RDMA_STRIDE_Y
    uint32_t rStrZ; // PPE_RDMA_STRIDE_Z
    uint32_t rFrgs; // PPE_RDMA_FRAG_SIZE
    uint32_t rFrgl; // PPE_RDMA_FRAG_LAST_SIZE
    uint32_t rXYZd; // PPE_RDMA_XYZ_DRCT
    uint32_t rBstX; // PPE_RDMA_BOX_ST_SIZE_X
    uint32_t rBstY; // PPE_RDMA_BOX_ST_SIZE_Y
    uint32_t rBstZ; // PPE_RDMA_BOX_ST_SIZE_Z
    uint32_t rBxtX; // PPE_RDMA_BOX_SIZE_X
    uint32_t rBxtY; // PPE_RDMA_BOX_SIZE_Y
    uint32_t rBxtZ; // PPE_RDMA_BOX_SIZE_Z
    uint32_t rBffX; // PPE_RDMA_BOX_OFFSET_X
    uint32_t rBffY; // PPE_RDMA_BOX_OFFSET_Y
    uint32_t rBffZ; // PPE_RDMA_BOX_OFFSET_Z

    // MY_PPE_REGS_BASE
    uint32_t wOpEn;
    uint32_t wAXIp;
    uint32_t wBALo; // PPE_WDMA_BASE_ADDR
    uint32_t wBrdX; // PPE_WDMA_BORDER_X
    uint32_t wBrdY; // PPE_WDMA_BORDER_Y
    uint32_t wBrdZ; // PPE_WDMA_BORDER_Z
    uint32_t wStrX; // PPE_WDMA_STRIDE_X
    uint32_t wStrY; // PPE_WDMA_STRIDE_Y
    uint32_t wStrZ; // PPE_WDMA_STRIDE_Z
    uint32_t wFrgs; // PPE_WDMA_FRAG_SIZE
    uint32_t wFrgl; // PPE_WDMA_FRAG_LAST_SIZE
    uint32_t wXYZd; // PPE_WDMA_XYZ_DRCT
    uint32_t wBstX; // PPE_WDMA_BOX_ST_SIZE_X
    uint32_t wBstY; // PPE_WDMA_BOX_ST_SIZE_Y
    uint32_t wBstZ; // PPE_WDMA_BOX_ST_SIZE_Z
    uint32_t wBxtX; // PPE_WDMA_BOX_SIZE_X
    uint32_t wBxtY; // PPE_WDMA_BOX_SIZE_Y
    uint32_t wBxtZ; // PPE_WDMA_BOX_SIZE_Z
    uint32_t wBffX; // PPE_WDMA_BOX_OFFSET_X
    uint32_t wBffY; // PPE_WDMA_BOX_OFFSET_Y
    uint32_t wBffZ; // PPE_WDMA_BOX_OFFSET_Z
    uint32_t wIstX; // PPE_INP_BOX_ST_SIZE_X
    uint32_t wIxtX; // PPE_INP_BOX_SIZE_X
    uint32_t wIffX; // PPE_INP_BOX_OFFSET_X
    uint32_t wWi  ; // PPE_DATA_W_IN
    uint32_t wHi  ; // PPE_DATA_H_IN
    uint32_t wCi  ; // PPE_DATA_C_IN
    uint32_t wWo  ; // PPE_DATA_W_OUT
    uint32_t wHo  ; // PPE_DATA_H_OUT
    uint32_t wCo  ; // PPE_DATA_C_OUT
    uint32_t wOpM ; // PPE_OP_MODE
    uint32_t wK   ; // PPE_KERNEL

    uint32_t wKWHr ; // PPE_RECIP_KERNEL_WH

    uint32_t wP   ; // PPE_PADDING
    uint32_t wPV1 ; // PPE_PADDING_VALUE_1
    uint32_t wPV2 ; // PPE_PADDING_VALUE_2
    uint32_t wPV3 ; // PPE_PADDING_VALUE_3
    uint32_t wPV4 ; // PPE_PADDING_VALUE_4
    uint32_t wPV5 ; // PPE_PADDING_VALUE_5
    uint32_t wPV6 ; // PPE_PADDING_VALUE_6
    uint32_t wPV7 ; // PPE_PADDING_VALUE_7
  } ConfigREGPPE;

  typedef struct CubeMetrics{
    int32_t s;   // Size That Cube Occupies (in bytes)
    int32_t H;
    int32_t W;
    int32_t C;
    CubeRole role;
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

  typedef struct MPECmdMetrics{
    int32_t s; // Size Of Command Blob In Bytes
  }MPECmdMetrics;

  typedef struct StatusRegs {
    /** Статус операции сравнения кубов данных. eq==0, neq==1 */
    int32_t cmp_result;
    /** Число входных элементов со значением бесконечность. */
    uint32_t inf_num_in;
    /** Число входных элементов nan. */
    uint32_t nan_num_in;
    /** Число выходных элементов nan. */
    uint32_t nan_num_out;
    /** Число элементов с возникшим переполнением при выходном преобразовании данных. */
    uint32_t satur_num_out;
    /** Кол-во совместных промахов отн-но верхних границ TAB0 и TAB1. */
    uint32_t lut_cnt_ovrf;
    /** Кол-во совместных промахов отн-но нижних границ TAB0 и TAB1. */
    uint32_t lut_cnt_undf;
    /** Кол-во исключительных попаданий в TAB0. */
    uint32_t lut_cnt_hit_tab0;
    /** Кол-во исключительных попаданий в TAB1. */
    uint32_t lut_cnt_hit_tab1;
    /** Кол-во совместных попаданий в TAB0 и TAB1. */
    uint32_t lut_cnt_hit_hybr;

    uint32_t ppe_inf_num_in ;
    uint32_t ppe_nan_num_in ;
    uint32_t ppe_nan_num_out;

    uint32_t mpe_inf_num_in ;
    uint32_t mpe_nan_num_in ;
    uint32_t mpe_nan_num_out;

  } StatusRegs;

  typedef struct NPEReg {
    uint32_t address;
    uint32_t value;
  } NPEReg;
  
#define SIZEOF_StatusRegs_BIN sizeof(StatusRegs)
  
static __attribute__((no_instrument_function)) inline void nu_write_reg_or_dump(int dump, uint32_t const value, uint32_t const base_addr)
{
  if(dump) {
    *((uint32_t*)(base_addr)) = value;
  }
  else
    iowrite32(value,base_addr);
}


void nu_vpe_load_config(ConfigVPE* cfg, void* cfg_bin);
void nu_vpe_print_Mode(Mode mode, char* name);
void nu_vpe_print_RoundMode(RoundMode mode, char* name);
void nu_vpe_print_TraceMode(TraceMode mode, char* name);
void nu_vpe_print_Enable(Enable enable, char* name);
void nu_vpe_print_AluOperationSwitch(AluOperationSwitch operation, char* name);
void nu_vpe_print_AluOperationExtSwitch(AluOperationExtSwitch operation, char* name);
void nu_vpe_print_PoolingOperationSwitch(PoolingOperationSwitch operation, char* name);
void nu_vpe_print_DataType(DataType data_type, char* name);
void nu_vpe_print_DataTypeExt(DataTypeExt data_type, char* name);
void nu_vpe_print_DmaRamType(DmaRamType rt, char* name);
void nu_vpe_print_DmaDSizeType(DmaDSizeType st, char* name);
void nu_vpe_print_DmaDUseType(DmaDUseType ut, char* name);
void nu_vpe_print_DmaXYZDirectionType(DmaXYZDirectionType ut, char* name);
void nu_vpe_print_config(ConfigVPE* cfg);
void nu_print_config_dma(ConfigDMA * cfg_dma,char* name);

uint32_t nu_lut_log2(uint32_t a);
void nu_vpe_load_lut(uintptr_t base, void* lut1, void* lut2);

void nu_vpe_setup_op01(int dump, uintptr_t base_, Enable op_en, ConfigOp01* op_config);
void nu_vpe_setup_lut(int dump, uintptr_t base_, ConfigOp2* op_config, DataTypeExt in_data_type);
void nu_vpe_setup_op2(int dump, uintptr_t base_, Enable op_en, ConfigOp2* op_config, DataTypeExt in_data_type);
void nu_vpe_setup_dma(int dump, uintptr_t base_, ConfigDMA* dma_config, TraceMode trace_mode);
  
void nu_vpe_setup_or_dump(int dump, uintptr_t base, ConfigVPE* cfg);
void nu_vpe_setup(uintptr_t base, ConfigVPE* cfg);

void nu_vpe_decide_dma_cube_config(ConfigDMA* dma_cfg, TraceMode trace_mode, CubeMetrics* metrics);
void nu_vpe_decide_op01_rdma_config(ConfigOp01* op_config, ConfigDMA* op_rdma_config, TraceMode trace_mode, CubeMetrics* metrics);
void nu_vpe_decide_op2_rdma_config(ConfigOp2* op_config, ConfigDMA* op_rdma_config, TraceMode trace_mode, CubeMetrics* metrics) ;
void nu_vpe_decide_dma_config_trivial(ConfigVPE* cfg, CubeMetrics* metrics);

bool nu_vpe_mode_to_bool (Mode in_mode);

bool nu_vpe_op01_is_cube(ConfigOp01* op_config);
bool nu_vpe_op2_is_cube(ConfigOp2* op_config);
bool nu_vpe_op01_is_vec(ConfigOp01* op_config);
bool nu_vpe_op2_is_vec(ConfigOp2* op_config);

// void nu_calc_mpe2vpe_cube_metrics(CubeMetrics* mpe2vpe_metrics,CubeMetrics* cube_metrics,WarrMetrics* warr_metrics);

void nu_vpe_load_status_regs(StatusRegs* status_regs, void* status_regs_bin);
void nu_vpe_print_status_regs_etalon(StatusRegs* status_regs);
int nu_vpe_check_status_regs(uintptr_t base, StatusRegs* status_regs);// 0 - Ok

void nu_mpe_load_config(ConfigMPE* cfg, void* cfg_bin);
void nu_mpe_print_config(ConfigMPE* cfg);
void nu_mpe_print_config_dma(ConfigDMAMPE* cfg);

int  nu_mpe_get_size_in_partitions(int size_in_bytes);

void nu_mpe_rdma_setup(int dump, uintptr_t base, ConfigRDDMAMPE* cfg);
void nu_mpe_wdma_setup(int dump, uintptr_t base, ConfigWRDMAMPE* cfg);
void nu_mpe_dma_setup(int dump, uintptr_t base, ConfigDMAMPE* cfg);
void nu_mpe_ma_setup(int dump, uintptr_t base, ConfigMPE* cfg);
void nu_mpe_setup_or_dump(int dump, uintptr_t base, ConfigMPE* cfg);
void nu_mpe_setup(uintptr_t base, ConfigMPE* cfg);

int nu_mpe_look_up_dma_config(ConfigMPE* cfg, void* table);
int  nu_mpe_decide_dma_config_trivial(ConfigMPE* cfg, CubeMetrics* cube_metrics, WarrMetrics* warr_metrics);
uint32_t nu_mpe_get_warr_offset(void* cmd, MPECmdMetrics* metrics);

void nu_mpe_load_buf(uintptr_t base, void* data, int size);

void nu_ppe_load_config(ConfigPPE* cfg, void* cfg_bin);
int nu_ppe_reg_load_config (ConfigREGPPE* cfg_reg, void* cfg_reg_bin);

void nu_ppe_print_config(ConfigPPE* cfg);

void nu_ppe_setup_reg_or_dump(int dump, uintptr_t rbase, uintptr_t wbase, ConfigREGPPE* cfg) ;
void nu_ppe_setup_reg(uintptr_t rbase, uintptr_t wbase, ConfigREGPPE* cfg);
void nu_ppe_print_config_reg(ConfigREGPPE* cfg_reg);

int rndup_Kh_d_Sh_calc (int Kh, int Sh);
int Wout_splt_calc (int Kh, int Sh, int rdctn);
void nu_ppe_decide_dma_config_trivial(ConfigPPE* cfg, CubeMetrics* out_cube_metrics, ConfigREGPPE* cfg_reg);

uint32_t nu_ppe_status_done (uintptr_t wbase);

void nu_ppe_rdma_run(uintptr_t rbase, ConfigREGPPE* cfg);
void nu_ppe_wdma_run(uintptr_t wbase, ConfigREGPPE* cfg);
void nu_ppe_dma_wait_ready_and_run(uintptr_t base);

int  nu_mpe_run_cmd(uintptr_t base, void* cmd,MPECmdMetrics* cmd_metrics);

void nu_ppe_rdma_wait_complete(uintptr_t rbase);
void nu_ppe_wait_complete(uintptr_t wbase);

void nu_vpe_config_rd_main_channel(uintptr_t dma_base, void *addr, int size);
void nu_vpe_run_rd_main_channel(uintptr_t dma_base);
void nu_vpe_wait_rd_main_channel_complete(uintptr_t dma_base);

void nu_vpe_run (uintptr_t vpe_base, ConfigVPE* cfg);
void nu_vpe_wait(uintptr_t vpe_base,ConfigVPE* cfg);
void nu_vpe_wait_cntx_appl(uintptr_t vpe_base);
uint32_t nu_vpe_busy(uintptr_t vpe_base);


void nu_npe_vpe_set_int_mask(uintptr_t npe_base);

void nu_vpe_set_int_mask(uintptr_t vpe_base);
void nu_vpe_pause_next_cntx(uintptr_t vpe_base);
void nu_vpe_dev_resume(uintptr_t vpe_base);
void nu_vpe_dev_pause_resume(uintptr_t vpe_base);
void nu_vpe_wait_int_pause_cntx_appl(uintptr_t vpe_base);
void nu_vpe_wait_int_cntx_appl(uintptr_t vpe_base);
void nu_vpe_wait_int_dev_off(uintptr_t vpe_base);

void nu_vpe_dev_pause_norst_resume(uintptr_t vpe_base);
void nu_vpe_wait_int_pause_norst_cntx_appl(uintptr_t vpe_base);
void nu_vpe_soft_reset(uintptr_t vpe_base);
void nu_vpe_wait_marked_cube_complete(uintptr_t vpe_base);

void nu_na_vpe_dev_pause_norst_resume(uintptr_t npe_base);
void nu_na_vpe_pause(uintptr_t npe_base );
void nu_na_vpe_soft_reset(uintptr_t npe_base);
void nu_na_vpe_dev_pause_resume(uintptr_t npe_base);
void nu_na_vpe_wait_int_pause_cntx_appl(uintptr_t npe_base);
void nu_na_vpe_dev_resume(uintptr_t npe_base);
void nu_na_vpe_wait(uintptr_t vpe_base);
void nu_na_wait_int(uintptr_t npe_base);
void nu_na_vpe_wait_int_dev_off(uintptr_t npe_base);
void nu_na_vpe_wait_complete(uintptr_t npe_base);
void nu_na_vpe_wait_marked_cube_complete(uintptr_t npe_base);
 void nu_na_ppe_wait_marked_cube_complete(uintptr_t npe_base);

void nu_mpe_wait_ready(uintptr_t base);
void nu_mpe_run(uintptr_t mpe_base, ConfigMPE* cfg);
void nu_mpe_run_dmas_only(uintptr_t mpe_base, ConfigMPE* cfg);
void nu_mpe_run_ma_only(uintptr_t mpe_base, ConfigMPE* cfg);
void nu_mpe_wait(uintptr_t mpe_base, ConfigDMAMPE* cfg_dma);

void na_cu_set_units_direct_mode(uintptr_t base, uint32_t mask);
void na_cu_timer_reset_and_start(uintptr_t base, bool timer_vpe, bool timer_ppe);
uint64_t na_cu_timer_read(uintptr_t base);

void nu_vpe_config_wr_main_channel(uintptr_t dma_base, void *addr, int size);
void nu_vpe_run_wr_main_channel(uintptr_t dma_base);
void nu_vpe_wait_wr_main_channel_complete(uintptr_t dma_base);
 
void nu_mpe_config_wr_main_channel(uintptr_t dma_base, void *addr, int size);
void nu_mpe_run_wr_main_channel(uintptr_t dma_base);
void nu_mpe_wait_wr_main_channel_complete(uintptr_t dma_base);

void nu_ppe_config_rd_main_channel(uintptr_t dma_base, void *addr, int size);
void nu_ppe_run_rd_main_channel(uintptr_t dma_base);
void nu_ppe_wait_rd_main_channel_complete(uintptr_t dma_base);
void nu_ppe_set_wdma_int_mask(uintptr_t vpe_base);

void nu_npe_ppe_set_int_mask(uintptr_t npe_base);
void nu_na_ppe_pause(uintptr_t npe_base );
void nu_na_ppe_soft_reset(uintptr_t npe_base);
void nu_na_ppe_dev_pause_resume(uintptr_t npe_base);
void nu_na_ppe_wait_int_pause(uintptr_t npe_base);
void nu_na_ppe_rdma_wait_complete(uintptr_t npe_base);
void nu_na_ppe_wait_complete(uintptr_t npe_base);
void nu_na_ppe_dev_resume(uintptr_t npe_base);
void nu_na_ppe_dev_pause_norst_resume(uintptr_t npe_base);

void nu_na_mpe_dev_pause_norst_resume(uintptr_t npe_base);
void nu_npe_mpe_set_int_mask(uintptr_t npe_base);
void nu_na_mpe_soft_reset(uintptr_t npe_base); 
void nu_na_mpe_pause(uintptr_t npe_base);
void nu_na_mpe_dev_pause_resume(uintptr_t npe_base);
void nu_na_mpe_wait_int_pause(uintptr_t npe_base);
void nu_na_mpe_dev_resume(uintptr_t npe_base);
void nu_na_mpe_wait_int_dev_off(uintptr_t npe_base);
void nu_na_mpe_wait_complete(uintptr_t npe_base);

void nu_na_soft_reset(uintptr_t npe_base);

int out_dim_comp(int in_dim, int k_dim, int k_str);
void nu_calc_mpe_cube_out_metrics(ConfigMPE* cfg, CubeMetrics* out_m);
int nu_vpe_regs_check(uintptr_t base, int num, int iteration);
int nu_regs_check(uintptr_t base,int num, int iteration);
void nu_na_ppe_wait_int_dev_off(uintptr_t npe_base);

NPEReg* nu_mpe_add_diff_reg_map(NPEReg* cfg_diff_ptr,  uint32_t* curr_cfg_ptr, uint32_t* next_cfg_ptr);
NPEReg* nu_mpe_add_diff_start(NPEReg* cfg_diff_ptr, ConfigMPE* cfg_mpe);
NPEReg* nu_mpe_dma_add_diff_reg_map(uintptr_t base, uintptr_t mpe_dma_base, NPEReg* cfg_diff_ptr,  uint32_t* curr_cfg_ptr, uint32_t* next_cfg_ptr);
NPEReg* nu_mpe_ma_add_diff_reg_map(uintptr_t base, uintptr_t mpe_ma_base, NPEReg* cfg_diff_ptr,  uint32_t* curr_cfg_ptr, uint32_t* next_cfg_ptr);

NPEReg* nu_vpe_add_diff_reg_map(NPEReg* cfg_diff_ptr,  uint32_t* curr_cfg_ptr, uint32_t* next_cfg_ptr, Enable lut_en);
NPEReg* nu_vpe_add_diff_start(NPEReg* cfg_diff_ptr, ConfigVPE* cfg);
NPEReg* nu_vpe_dma_add_diff_reg_map(uintptr_t base, uintptr_t vpe_dma_base, NPEReg* cfg_diff_ptr,  uint32_t* curr_cfg_ptr, uint32_t* next_cfg_ptr);
NPEReg* nu_vpe_op01_add_diff_reg_map(uintptr_t base, uintptr_t vpe_op01_base, NPEReg* cfg_diff_ptr,  uint32_t* curr_cfg_ptr, uint32_t* next_cfg_ptr);
NPEReg* nu_vpe_op2_add_diff_reg_map(uintptr_t base, uintptr_t vpe_op2_base, NPEReg* cfg_diff_ptr,  uint32_t* curr_cfg_ptr, uint32_t* next_cfg_ptr, Enable lut_en);
NPEReg* nu_vpe_lut_add_diff_reg_map(uintptr_t base, uintptr_t vpe_lut_base, NPEReg* cfg_diff_ptr,  uint32_t* curr_cfg_ptr, uint32_t* next_cfg_ptr);

NPEReg* nu_ppe_rdma_add_diff_reg_map(NPEReg* cfg_diff,  uint32_t* curr_cfg_ptr, uint32_t* next_cfg_ptr);
NPEReg* nu_ppe_wdma_add_diff_reg_map(NPEReg* cfg_diff_ptr,  uint32_t* curr_cfg_ptr, uint32_t* next_cfg_ptr);
NPEReg* nu_ppe_wdma_add_diff_start(NPEReg* cfg_diff_ptr, ConfigREGPPE* cfg);

NPEReg* nu_add_diff_reg_map(uintptr_t base, uintptr_t device_base, NPEReg* cfg_diff_ptr,  uint32_t* curr_cfg_ptr, uint32_t* next_cfg_ptr, uint32_t start_shift, uint32_t end_shift);
NPEReg* nu_add_diff_start(NPEReg* cfg_diff_ptr, uintptr_t start, uint32_t depend_start);

void nu_npe_run(uintptr_t npe_base, NPEReg* cfg_diff_start_ptr, NPEReg* cfg_diff_end_ptr);

void nu_print_associative_regs_dump(NPEReg* start_ptr, NPEReg* end_ptr);

void nu_mpe_print_reg_map(uint32_t* mpe_reg_map_base);
void nu_mpe_dma_print_reg_map(uint32_t* mpe_dma_base);
void nu_mpe_ma_print_reg_map(uint32_t* mpe_ma_base);

void nu_vpe_print_reg_map(uint32_t* vpe_reg_map_base, Enable lut_en);
void nu_vpe_dma_print_reg_map(uint32_t* vpe_dma_base);
void nu_vpe_op01_print_reg_map(uint32_t* vpe_op01_base);
void nu_vpe_op2_print_reg_map(uint32_t* vpe_op2_base, Enable lut_en);
void nu_vpe_lut_print_reg_map(uint32_t* vpe_lut_base);

void nu_ppe_rdma_print_reg_map(uint32_t* ppe_base);
void nu_ppe_wdma_print_reg_map(uint32_t* ppe_base);

void print_part_of_memory(uint32_t* base, uint32_t start_shift, uint32_t end_shift);

void nu_print_cube_role(CubeRole role, char* name);
void nu_print_cube_metrics(CubeMetrics* metrics);
void nu_print_warr_metrics(WarrMetrics* metrics);
void nu_print_vec_metrics(VectorMetrics* metrics);

void nu_npe_cmd_dma_setup(uintptr_t npe_base, uint32_t cmd_dma_base_addr, uint32_t cmd_dma_page_size);
void nu_npe_cmd_dma_run(uintptr_t npe_base);
void nu_npe_cmd_dma_wait_page_complete(uintptr_t npe_base);
void nu_npe_cmd_dma_wait_cntx_appl(uintptr_t npe_base);

void nu_na_ppe_pause_set(uintptr_t base);
uint32_t nu_na_ppe_pause_status(uintptr_t base);
void nu_na_ppe_soft_reset_set (uintptr_t base);
uint32_t nu_na_ppe_soft_reset_status (uintptr_t base);
void nu_na_ppe_pause_clr(uintptr_t base);
uint32_t nu_ppe_page_cmpl_status(uintptr_t base);
void nu_ppe_page_cmpl_reset(uintptr_t base);

void nu_ppe_wdma_err_mask (uintptr_t base);
uint32_t nu_ppe_wdma_err_status(uintptr_t base);
void nu_ppe_wdma_err_reset (uintptr_t base);

void nu_ppe_rdma_err_mask (uintptr_t base);
uint32_t nu_ppe_rdma_err_status(uintptr_t base);
void nu_ppe_rdma_err_reset (uintptr_t base);
void na_rst(uintptr_t base );

uint32_t mask_N_M (int N, int M);

#endif
