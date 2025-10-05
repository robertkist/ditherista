#include "mainwindow.h"
#include "consts.h"
#include <QtConcurrent>

/* This file contains:
 * - methods related to color dithering
 */

/*********************************
 * COLOR DITHER OPTION UI SLOTS  *
 *********************************/

void MainWindow::serpentineColorToggledSlot(bool serpentine) {
    /* Serpentine toggle checkbox for Error Diffusion and Variable Error Diffusion */
    ui->treeWidgetColor->setValue(current_sub_dither_type, setting_serpentine, serpentine);
    forceReDitherSlot();
}

void MainWindow::ORD_stepColorValueChangedSlot(int step) {
    ui->treeWidgetColor->setValue(current_sub_dither_type, setting_step, step);
    forceReDitherSlot();
}

void MainWindow::ORD_IGR_aColorValueChangedSlot(double a) {
    ui->treeWidgetColor->setValue(current_sub_dither_type, setting_a, a);
    forceReDitherSlot();
}

void MainWindow::ORD_IGR_bColorValueChangedSlot(double b) {
    ui->treeWidgetColor->setValue(current_sub_dither_type, setting_b, b);
    forceReDitherSlot();
}

void MainWindow::ORD_IGR_cColorValueChangedSlot(double c) {
    ui->treeWidgetColor->setValue(current_sub_dither_type, setting_c, c);
    forceReDitherSlot();
}

/*********************************
 * COLOR DITHERER METHODS        *
 *********************************/

void MainWindow::ERR_C_dither(int* out_buf, const SubDitherType n) {
    /* Runs the color Error Diffusion ditherer */
    ErrorDiffusionMatrix* matrix = nullptr;
    switch(n) {
        case ERR_XOT_C: matrix = get_xot_matrix(); break;
        case ERR_DIA_C: matrix = get_diagonal_matrix(); break;
        case ERR_FYD_C: matrix = get_floyd_steinberg_matrix(); break;
        case ERR_SF3_C: matrix = get_shiaufan3_matrix(); break;
        case ERR_SF2_C: matrix = get_shiaufan2_matrix(); break;
        case ERR_SF1_C: matrix = get_shiaufan1_matrix(); break;
        case ERR_STK_C: matrix = get_stucki_matrix(); break;
        case ERR_D1D_C: matrix = get_diffusion_1d_matrix(); break;
        case ERR_D2D_C: matrix = get_diffusion_2d_matrix(); break;
        case ERR_FFS_C: matrix = get_fake_floyd_steinberg_matrix(); break;
        case ERR_JJN_C: matrix = get_jarvis_judice_ninke_matrix(); break;
        case ERR_ATK_C: matrix = get_atkinson_matrix(); break;
        case ERR_BRK_C: matrix = get_burkes_matrix(); break;
        case ERR_SI3_C: matrix = get_sierra_3_matrix(); break;
        case ERR_SI2_C: matrix = get_sierra_2row_matrix(); break;
        case ERR_SIL_C: matrix = get_sierra_lite_matrix(); break;
        case ERR_SPG_C: matrix = get_steve_pigeon_matrix(); break;
        case ERR_RKI_C: matrix = get_robert_kist_matrix(); break;
        case ERR_SSA_C: matrix = get_stevenson_arce_matrix(); break;
        default: qDebug() << "WARNING: requested ERR ditherer " << n << " not found"; break;
    }
    if(matrix != nullptr) {
        bool serpentine = ui->ERR_C_serpentine->isChecked();
        fthread = QtConcurrent::run(error_diffusion_dither_color, imageHashColor.getSourceImage(),
                                    matrix, cachedPalette, serpentine, out_buf);
        runDitherThread();
        ErrorDiffusionMatrix_free(matrix);
    }
}

void MainWindow::ORD_C_dither(int* out_buf, const SubDitherType n) {
    /* Runs the Ordered ditherer */
    OrderedDitherMatrix* matrix = nullptr;
    switch(n) {
        case ORD_BLU_C: matrix = get_blue_noise_128x128(); break;
        case ORD_BA2_C: matrix = get_bayer2x2_matrix(); break;
        case ORD_BA3_C: matrix = get_bayer3x3_matrix(); break;
        case ORD_BA4_C: matrix = get_bayer4x4_matrix(); break;
        case ORD_BA8_C: matrix = get_bayer8x8_matrix(); break;
        case ORD_B16_C: matrix = get_bayer16x16_matrix(); break;
        case ORD_B32_C: matrix = get_bayer32x32_matrix(); break;
        case ORD_DD1_C: matrix = get_dispersed_dots_1_matrix(); break;
        case ORD_DD2_C: matrix = get_dispersed_dots_2_matrix(); break;
        case ORD_VDD_C: matrix = get_ulichney_void_dispersed_dots_matrix(); break;
        case ORD_NR1_C: matrix = get_non_rectangular_1_matrix(); break;
        case ORD_NR2_C: matrix = get_non_rectangular_2_matrix(); break;
        case ORD_NR3_C: matrix = get_non_rectangular_3_matrix(); break;
        case ORD_NR4_C: matrix = get_non_rectangular_4_matrix(); break;
        case ORD_ULY_C: matrix = get_ulichney_matrix(); break;
        case ORD_UB5_C: matrix = get_ulichney_bayer_5_matrix(); break;
        case ORD_CD1_C: matrix = get_bayer_clustered_dot_1_matrix(); break;
        case ORD_CD2_C: matrix = get_bayer_clustered_dot_2_matrix(); break;
        case ORD_CD3_C: matrix = get_bayer_clustered_dot_3_matrix(); break;
        case ORD_CD4_C: matrix = get_bayer_clustered_dot_4_matrix(); break;
        case ORD_CD5_C: matrix = get_bayer_clustered_dot_5_matrix(); break;
        case ORD_CD6_C: matrix = get_bayer_clustered_dot_6_matrix(); break;
        case ORD_CD7_C: matrix = get_bayer_clustered_dot_7_matrix(); break;
        case ORD_CD8_C: matrix = get_bayer_clustered_dot_8_matrix(); break;
        case ORD_CD9_C: matrix = get_bayer_clustered_dot_9_matrix(); break;
        case ORD_CDA_C: matrix = get_bayer_clustered_dot_10_matrix(); break;
        case ORD_CDB_C: matrix = get_bayer_clustered_dot_11_matrix(); break;
        case ORD_CWP_C: matrix = get_central_white_point_matrix(); break;
        case ORD_BWP_C: matrix = get_balanced_centered_point_matrix(); break;
        case ORD_DIO_C: matrix = get_diagonal_ordered_matrix_matrix(); break;
        case ORD_UCD_C: matrix = get_ulichney_clustered_dot_matrix(); break;
        case ORD_M5C_C: matrix = get_magic5x5_circle_matrix(); break;
        case ORD_M6C_C: matrix = get_magic6x6_circle_matrix(); break;
        case ORD_M7C_C: matrix = get_magic7x7_circle_matrix(); break;
        case ORD_M44_C: matrix = get_magic4x4_45_matrix(); break;
        case ORD_M64_C: matrix = get_magic6x6_45_matrix(); break;
        case ORD_M84_C: matrix = get_magic8x8_45_matrix(); break;
        case ORD_MA4_C: matrix = get_magic4x4_matrix(); break;
        case ORD_MA6_C: matrix = get_magic6x6_matrix(); break;
        case ORD_MA8_C: matrix = get_magic8x8_matrix(); break;
        case ORD_VA2_C: matrix = get_variable_2x2_matrix(ui->ORD_VAR_C_step->value()); break; // 50
        case ORD_VA4_C: matrix = get_variable_4x4_matrix(ui->ORD_VAR_C_step->value()); break;
        case ORD_IGR_C: {
            const double a = ui->ORD_IGR_C_a->value();
            const double b = ui->ORD_IGR_C_b->value();
            const double c = ui->ORD_IGR_C_c->value();
            matrix = get_interleaved_gradient_noise(ui->ORD_IGR_C_step->value(), a, b, c);
        } break;
        default: qDebug() << "WARNING: requested ORD_C ditherer " << n << " not found"; break;
    }
    if(matrix != nullptr) {
        fthread = QtConcurrent::run(ordered_dither_color, imageHashColor.getSourceImage(), cachedPalette, matrix, out_buf);
        runDitherThread();
        OrderedDitherMatrix_free(matrix);
    }
}
