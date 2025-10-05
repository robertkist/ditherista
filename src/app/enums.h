#pragma once
#ifndef ENUMS_H
#define ENUMS_H

#include <QHash>
#include <QString>

#define COLOR_DITHER_START 1000  // use this to tell mono and color ditherers apart
#define MONO_ERR_START 0
#define MONO_DOT_START 50
#define MONO_GRD_START 60
#define MONO_DBS_START 70
#define MONO_ALL_START 80
#define MONO_LIP_START 90
#define MONO_ORD_START 100
#define MONO_PAT_START 150
#define MONO_RIM_START 160
#define MONO_THR_START 170
#define MONO_VAR_START 180

enum DitherType {
    // Mono ditherer types
    ALL = 0, DBS, DOT, ERR, GRD, LIP, ORD, PAT, RIM, THR, VAR,
    // Color ditherer types (not all may be implemented)
    ALL_C = COLOR_DITHER_START, DBS_C, DOT_C, ERR_C, GRD_C, LIP_C, ORD_C, PAT_C, RIM_C, THR_C, VAR_C
};

enum SubDitherType {
    // Mono ditherers
    ERR_XOT = MONO_ERR_START, ERR_DIA, ERR_FYD, ERR_SF3, ERR_SF2, ERR_SF1, ERR_STK, ERR_D1D, ERR_D2D, ERR_SSA,
                              ERR_FFS, ERR_JJN, ERR_ATK, ERR_BRK, ERR_SI3, ERR_SI2, ERR_SIL, ERR_SPG, ERR_RKI,
    DOT_KNU = MONO_DOT_START, DOT_MKN, DOT_OKN, DOT_M08, DOT_M16, DOT_G08, DOT_G16, DOT_SPR, DOT_ISP,
    GRD_GRD = MONO_GRD_START,
    DBS_DBS = MONO_DBS_START,
    ALL_ALL = MONO_ALL_START,
    LIP_LI1 = MONO_LIP_START, LIP_LI2, LIP_LI3, LIP_GUO, LIP_MSE, LIP_KNU,
    ORD_BLU = MONO_ORD_START, ORD_BA2, ORD_BA3, ORD_BA4, ORD_BA8, ORD_B16, ORD_B32, ORD_DD1, ORD_DD2,
                              ORD_VDD, ORD_NR1, ORD_NR2, ORD_NR3, ORD_NR4, ORD_ULY, ORD_UB5, ORD_CD1, ORD_CD2,
                              ORD_CD3, ORD_CD4, ORD_CD5, ORD_CD6, ORD_CD7, ORD_CD8, ORD_CD9, ORD_CDA, ORD_CDB,
                              ORD_CWP, ORD_BWP, ORD_DIO, ORD_UCD, ORD_M5C, ORD_M6C, ORD_M7C, ORD_M44, ORD_M64,
                              ORD_M84, ORD_MA4, ORD_MA6, ORD_MA8, ORD_VA2, ORD_VA4, ORD_IGR,
    PAT_22A = MONO_PAT_START, PAT_33A, PAT_33B, PAT_33C, PAT_44A, PAT_52A,
    RIM_HIL = MONO_RIM_START, RIM_HIM, RIM_PEA, RIM_FS0, RIM_FS1, RIM_FS2, RIM_GOS, RIM_FSS,
    THR_THR = MONO_THR_START,
    VAR_OST = MONO_VAR_START, VAR_ZHF,
    // Color ditherers
    ERR_XOT_C = COLOR_DITHER_START + MONO_ERR_START, ERR_DIA_C, ERR_FYD_C, ERR_SF3_C, ERR_SF2_C, ERR_SF1_C, ERR_STK_C,
                   ERR_D1D_C, ERR_D2D_C, ERR_SSA_C, ERR_FFS_C, ERR_JJN_C, ERR_ATK_C, ERR_BRK_C, ERR_SI3_C,
                   ERR_SI2_C, ERR_SIL_C, ERR_SPG_C, ERR_RKI_C,
    ORD_BLU_C = COLOR_DITHER_START + MONO_ORD_START, ORD_BA2_C, ORD_BA3_C, ORD_BA4_C, ORD_BA8_C, ORD_B16_C, ORD_B32_C,
                   ORD_DD1_C, ORD_DD2_C, ORD_VDD_C, ORD_NR1_C, ORD_NR2_C, ORD_NR3_C, ORD_NR4_C, ORD_ULY_C,
                   ORD_UB5_C, ORD_CD1_C, ORD_CD2_C, ORD_CD3_C, ORD_CD4_C, ORD_CD5_C, ORD_CD6_C, ORD_CD7_C,
                   ORD_CD8_C, ORD_CD9_C, ORD_CDA_C, ORD_CDB_C, ORD_CWP_C, ORD_BWP_C, ORD_DIO_C, ORD_UCD_C,
                   ORD_M5C_C, ORD_M6C_C, ORD_M7C_C, ORD_M44_C, ORD_M64_C, ORD_M84_C, ORD_MA4_C, ORD_MA6_C,
                   ORD_MA8_C, ORD_VA2_C, ORD_VA4_C, ORD_IGR_C,
    RIM_HIL_C = COLOR_DITHER_START + MONO_RIM_START, RIM_HIM_C, RIM_PEA_C, RIM_FS0_C, RIM_FS1_C, RIM_FS2_C, RIM_GOS_C, RIM_FSS_C,
    THR_THR_C = COLOR_DITHER_START + MONO_THR_START,
    VAR_OST_C = COLOR_DITHER_START + MONO_VAR_START, VAR_ZHF_C
};

const QHash<int, QString> ditherTypeName = {  // dither type mapping for file-name generation for saving in treewidget.cpp
        // mono
        {ALL, "allebach"}, {DBS, "dbs"}, {DOT, "dotdiff"}, {ERR, "errordiff"},
        {LIP, "dotlippens"}, {ORD, "ordered"}, {PAT, "pattern"}, {RIM, "riemersma"},
        {THR, "threshold"}, {VAR, "variable"},
        // color (not all may be implemented)
        {ALL_C, "allebach"}, {DBS_C, "dbs"}, {DOT_C, "dotdiff"}, {ERR_C, "errordiff"},
        {LIP_C, "dotlippens"}, {ORD_C, "ordered"}, {PAT_C, "pattern"}, {RIM_C, "riemersma"},
        {THR_C, "threshold"}, {VAR_C, "variable"}
};

const QHash<int, QString> subDitherTypeName = { // dither type mapping for file-name generation for saving in treewidget.cpp
    {ORD_BLU_C, "blue-noise"}, {ORD_BA2_C, "bayer2x2"},
    {ORD_BA3_C, "bayer3x3"}, {ORD_BA4_C, "bayer4x4"}, {ORD_BA8_C, "bayer8x8"}, {ORD_B16_C, "bayer16x16"},
    {ORD_B32_C, "bayer32x32"}, {ORD_DD1_C, "dispersed-dots1"}, {ORD_DD2_C, "dispersed-dots2"},
    {ORD_VDD_C, "void-dispersed-dots"}, {ORD_NR1_C, "non-Rectangular1"}, {ORD_NR2_C, "non-Rectangular2"},
    {ORD_NR3_C, "non-Rectangular3"}, {ORD_NR4_C, "non-Rectangular4"}, {ORD_ULY_C, "ulichney"},
    {ORD_UB5_C, "ulichney-bayer5x5"}, {ORD_CD1_C, "clustered-dot1"}, {ORD_CD2_C, "clustered-dot2"},
    {ORD_CD3_C, "clustered-dot3"}, {ORD_CD4_C, "clustered-dot4"}, {ORD_CD5_C, "clustered-dot5"},
    {ORD_CD6_C, "clustered-dot6"}, {ORD_CD7_C, "clustered-dot7"}, {ORD_CD8_C, "clustered-dot8"},
    {ORD_CD9_C, "clustered-dot9"}, {ORD_CDA_C, "clustered-dot10"}, {ORD_CDB_C, "clustered-dot11"},
    {ORD_CWP_C, "central-white-point"}, {ORD_BWP_C, "balanced-white-point"}, {ORD_DIO_C, "diagonal-ordered"},
    {ORD_UCD_C, "ulichney-cluster-dot"}, {ORD_M5C_C, "magic-5x5circle"}, {ORD_M6C_C, "magic-6x6circle"},
    {ORD_M7C_C, "magic-7x7circle"}, {ORD_M44_C, "magic-4x4-45deg"}, {ORD_M64_C, "magic-6x6-45deg"},
    {ORD_M84_C, "magic-8x8-45deg"}, {ORD_MA4_C, "magic-4x4"}, {ORD_MA6_C, "magic-6x6"}, {ORD_MA8_C, "magic-8x8"},
    {ORD_VA2_C, "variable2x2"}, {ORD_VA4_C, "variable4x4"}, {ORD_IGR_C, "interleaved-gradient"}
};

enum SettingKey {  // for TreeWidget (i.e. ditherer list)
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

enum ReturnStatus {  // for FileExistsDialog
    RS_CANCEL,
    RS_SKIP,
    RS_SKIP_ALL,
    RS_OVERWRITE_ALL,
    RS_OVERWRITE,
    RS_CREATE
};

enum BatchDitherResult {  // for DitherResults dialog
    DR_IMAGE_READ_ERROR,  // file read error or unknown/faulty format
    DR_IMAGE_WRITE_ERROR, // file write error
    DR_RESOLUTION_ERROR,  // resolution > max supported resolution
    DR_OK,                // no error
};

#endif  // ENUMS_H
