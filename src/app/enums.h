#pragma once
#ifndef ENUMS_H
#define ENUMS_H

enum DitherType { ALL, DBS, DOT, ERR, GRD, LIP, ORD, PAT, RIM, THR, VAR };

enum SubDitherType {
    ERR_XOT, ERR_DIA, ERR_FYD, ERR_SF3, ERR_SF2, ERR_SF1, ERR_STK, ERR_D1D, ERR_D2D,
    ERR_FFS, ERR_JJN, ERR_ATK, ERR_BRK, ERR_SI3, ERR_SI2, ERR_SIL, ERR_SPG, ERR_RKI, ERR_SSA,
    DOT_KNU, DOT_MKN, DOT_OKN, DOT_M08, DOT_M16, DOT_G08, DOT_G16, DOT_SPR, DOT_ISP,
    GRD_GRD,
    DBS_DBS,
    ALL_ALL,
    LIP_LI1, LIP_LI2, LIP_LI3, LIP_GUO, LIP_MSE, LIP_KNU,
    ORD_BLU, ORD_BA2, ORD_BA3, ORD_BA4, ORD_BA8, ORD_B16, ORD_B32, ORD_DD1, ORD_DD2,
    ORD_VDD, ORD_NR1, ORD_NR2, ORD_NR3, ORD_NR4, ORD_ULY, ORD_UB5, ORD_CD1, ORD_CD2,
    ORD_CD3, ORD_CD4, ORD_CD5, ORD_CD6, ORD_CD7, ORD_CD8, ORD_CD9, ORD_CDA, ORD_CDB,
    ORD_CWP, ORD_BWP, ORD_DIO, ORD_UCD, ORD_M5C, ORD_M6C, ORD_M7C, ORD_M44, ORD_M64,
    ORD_M84, ORD_MA4, ORD_MA6, ORD_MA8, ORD_VA2, ORD_VA4, ORD_IGR,
    PAT_22A, PAT_33A, PAT_33B, PAT_33C, PAT_44A, PAT_52A,
    RIM_HIL, RIM_HIM, RIM_PEA, RIM_FS0, RIM_FS1, RIM_FS2, RIM_GOS, RIM_FSS,
    THR_THR,
    VAR_OST, VAR_ZHF
};

enum SettingKey {
    setting_serpentine,
    setting_threshold,
    setting_auto_threshold,
    setting_jitter,
    setting_step,
    setting_a,
    setting_b,
    setting_c,
    setting_alt_algorithm,
    setting_width,
    setting_height,
    setting_randomize,
    setting_formula,
    setting_min_pixels,
    setting_use_riemersma
};

#endif  // ENUMS_H
