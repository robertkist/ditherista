#include "mainwindow.h"
#include "consts.h"
#include <QtConcurrent>

/* This file contains:
 * - methods related to mono dithering
 */

void MainWindow::setDitherImageMono() {
    /* applies the user defined light/dark colors to the mono dithered image */
    QImage *monoImage = imageHashMono.getDitheredImage(current_dither_number);
    if (monoImage != nullptr) {
        QColor color1 = ui->monoColorOneButton->getColor();
        QColor color2 = ui->monoColorTwoButton->getColor();
        if (color1 != DEFAULT_DARK_MONO_COLOR || color2 != DEFAULT_LIGHT_MONO_COLOR) {
            QImage *newImage = new QImage(monoImage->width(), monoImage->height(), monoImage->format());
            for (int y = 0; y < newImage->height(); y++) {
                for (int x = 0; x < newImage->width(); x++) {
                    newImage->setPixel(x, y, qRed(monoImage->pixel(x, y)) == 0 ? color1.rgb() : color2.rgb());
                }
            }
            ui->graphicsView->setDitherImageMono(newImage, ui->treeWidgetMono->getCurrentDitherFileName());
            delete newImage;
        } else {
            ui->graphicsView->setDitherImageMono(monoImage, ui->treeWidgetMono->getCurrentDitherFileName());
        }
    }
}

/*********************************
 * MONO DITHER OPTION UI SLOTS   *
 *********************************/

void MainWindow::DBS_setFormulaSlot(int formula) {
    /* DBS formula changed in ComboBox */
    ui->treeWidgetMono->setValue(current_sub_dither_type, setting_formula, formula);
    forceReDitherSlot();
}

void MainWindow::ALL_randomizeToggleSlot(bool value) {
    /* randomize checkbox toggle */
    ui->treeWidgetMono->setValue(current_sub_dither_type, setting_randomize, value);
    forceReDitherSlot();
}

void MainWindow::GRD_altAlgorithmToggleSlot(bool value) {
    /* Grid dither checkbox toggle */
    ui->treeWidgetMono->setValue(current_sub_dither_type, setting_alt_algorithm, value);
    forceReDitherSlot();
}

void MainWindow::GRD_widthValueChangedSlot(int value) {
    /* Grid dither width spinBox */
    ui->treeWidgetMono->setValue(current_sub_dither_type, setting_width, value);
    forceReDitherSlot();
}

void MainWindow::GRD_heightValueChangedSlot(int value) {
    /* Grid dither height spinbox */
    ui->treeWidgetMono->setValue(current_sub_dither_type, setting_height, value);
    forceReDitherSlot();
}

void MainWindow::GRD_minPixelsValueChangedSlot(int value) {
    /* Grid dither min-pixels spinBox */
    ui->treeWidgetMono->setValue(current_sub_dither_type, setting_min_pixels, value);
    forceReDitherSlot();
}

void MainWindow::serpentineToggledSlot(bool serpentine) {
    /* Serpentine toggle checkbox for Error Diffusion and Variable Error Diffusion */
    ui->treeWidgetMono->setValue(current_sub_dither_type, setting_serpentine, serpentine);
    forceReDitherSlot();
}

void MainWindow::jitterValueChangedSlot(double jitter) {
    /* Jitter spinBox changed */
    ui->treeWidgetMono->setValue(current_sub_dither_type, setting_jitter, jitter);
    forceReDitherSlot();
}

void MainWindow::RIM_modRiemersmaToggledSlot(bool value) {
    /* checkbox toggle for modified Riemersma algorithm */
    ui->treeWidgetMono->setValue(current_sub_dither_type, setting_use_riemersma, value);
    forceReDitherSlot();
}

void MainWindow::THR_thresholdValueChangedSlot(double threshold) {
    /* Thresholding mid-value spinBox */
    ui->treeWidgetMono->setValue(current_sub_dither_type, setting_threshold, threshold);
    forceReDitherSlot();
}

void MainWindow::THR_toggleAutoThresholdSlot(bool autoThreshold) {
    /* Thresholding auto-threshold checkbox toggle */
    ui->treeWidgetMono->setValue(current_sub_dither_type, setting_auto_threshold, autoThreshold);
    ui->THR_threshold->setEnabled(ui->THR_autoThreshold->checkState() != Qt::Checked);
    forceReDitherSlot();
}

void MainWindow::ORD_stepValueChangedSlot(int step) {
    ui->treeWidgetMono->setValue(current_sub_dither_type, setting_step, step);
    forceReDitherSlot();
}

void MainWindow::ORD_IGR_aValueChangedSlot(double a) {
    ui->treeWidgetMono->setValue(current_sub_dither_type, setting_a, a);
    forceReDitherSlot();
}

void MainWindow::ORD_IGR_bValueChangedSlot(double b) {
    ui->treeWidgetMono->setValue(current_sub_dither_type, setting_b, b);
    forceReDitherSlot();
}

void MainWindow::ORD_IGR_cValueChangedSlot(double c) {
    ui->treeWidgetMono->setValue(current_sub_dither_type, setting_c, c);
    forceReDitherSlot();
}

/*********************************
 * MONO DITHERER METHODS         *
 *********************************/

void MainWindow::ALL_dither(uint8_t* out_buf) {
    /* Runs the Allebach ditherer */
    bool randomize = ui->ALL_randomize->isChecked();
    fthread = QtConcurrent::run(kallebach_dither, imageHashMono.getSourceImage(), randomize, out_buf);
    runDitherThread();
}

void MainWindow::GRD_dither(uint8_t* out_buf) {
    /* Runs the Grid ditherer */
    bool altAlgorithm = ui->GRD_altAlgorithm->isChecked();
    int width = ui->GRD_width->value();
    int height = ui->GRD_height->value();
    int minPixels = ui->GRD_minPixels->value();
    DitherImage* image = imageHashMono.getSourceImage();
    // use std::bind as work-around for Qt5, where QtConcurrent::run only supports up to 5 arguments
    fthread = QtConcurrent::run(std::bind(&grid_dither, image, width, height,
                                          minPixels, altAlgorithm, out_buf));
    runDitherThread();
}

void MainWindow::DBS_dither(uint8_t* out_buf) {
    /* Runs the DBS ditherer */
    DitherImage* image = imageHashMono.getSourceImage();
    if(image->width * image->height > DBS_PROGRESS_TRIGGER_SIZE) {
        notification->showText(tr("please wait..."), TIMEOUT_FOREVER); // only show if image is bigger than a certain size
    }
    int formula = ui->DBS_formula->currentIndex();
    fthread = QtConcurrent::run(dbs_dither, image, formula, out_buf);
    runDitherThread();
    if(image->width * image->height > DBS_PROGRESS_TRIGGER_SIZE) {
        notification->cancel();
    }
}

void MainWindow::DOT_dither(uint8_t* out_buf, const SubDitherType n) {
    /* Runs the Dot Diffusion ditherer */
    DotClassMatrix* cm = nullptr;
    DotDiffusionMatrix* dm = nullptr;
    switch(n) {
        case DOT_KNU: dm = get_default_diffusion_matrix(); cm = get_knuth_class_matrix(); break;
        case DOT_MKN: dm = get_default_diffusion_matrix(); cm = get_mini_knuth_class_matrix(); break;
        case DOT_OKN: dm = get_default_diffusion_matrix(); cm = get_optimized_knuth_class_matrix(); break;
        case DOT_M08: dm = get_default_diffusion_matrix(); cm = get_mese_8x8_class_matrix(); break;
        case DOT_M16: dm = get_default_diffusion_matrix(); cm = get_mese_16x16_class_matrix(); break;
        case DOT_G08: dm = get_guoliu8_diffusion_matrix(); cm = get_guoliu_8x8_class_matrix(); break;
        case DOT_G16: dm = get_guoliu16_diffusion_matrix(); cm = get_guoliu_16x16_class_matrix(); break;
        case DOT_SPR: dm = get_guoliu8_diffusion_matrix(); cm = get_spiral_class_matrix(); break;
        case DOT_ISP: dm = get_guoliu8_diffusion_matrix(); cm = get_spiral_inverted_class_matrix(); break;
        default: qDebug() << "WARNING: requested DOT ditherer " << n << " not found"; break;
    }
    if(dm != nullptr) {
        fthread = QtConcurrent::run(dot_diffusion_dither, imageHashMono.getSourceImage(),
                                    dm, cm, out_buf);
        runDitherThread();
        DotClassMatrix_free(cm);
        DotDiffusionMatrix_free(dm);
    }
}

void MainWindow::ERR_dither(uint8_t* out_buf, const SubDitherType n) {
    /* Runs the mono Error Diffusion ditherer */
    ErrorDiffusionMatrix* matrix = nullptr;
    switch(n) {
        case ERR_XOT: matrix = get_xot_matrix(); break;
        case ERR_DIA: matrix = get_diagonal_matrix(); break;
        case ERR_FYD: matrix = get_floyd_steinberg_matrix(); break;
        case ERR_SF3: matrix = get_shiaufan3_matrix(); break;
        case ERR_SF2: matrix = get_shiaufan2_matrix(); break;
        case ERR_SF1: matrix = get_shiaufan1_matrix(); break;
        case ERR_STK: matrix = get_stucki_matrix(); break;
        case ERR_D1D: matrix = get_diffusion_1d_matrix(); break;
        case ERR_D2D: matrix = get_diffusion_2d_matrix(); break;
        case ERR_FFS: matrix = get_fake_floyd_steinberg_matrix(); break;
        case ERR_JJN: matrix = get_jarvis_judice_ninke_matrix(); break;
        case ERR_ATK: matrix = get_atkinson_matrix(); break;
        case ERR_BRK: matrix = get_burkes_matrix(); break;
        case ERR_SI3: matrix = get_sierra_3_matrix(); break;
        case ERR_SI2: matrix = get_sierra_2row_matrix(); break;
        case ERR_SIL: matrix = get_sierra_lite_matrix(); break;
        case ERR_SPG: matrix = get_steve_pigeon_matrix(); break;
        case ERR_RKI: matrix = get_robert_kist_matrix(); break;
        case ERR_SSA: matrix = get_stevenson_arce_matrix(); break;
        default: qDebug() << "WARNING: requested ERR ditherer " << n << " not found"; break;
    }
    if(matrix != nullptr) {
        double jitter = ui->ERR_jitter->value();
        bool serpentine = ui->ERR_serpentine->isChecked();
        fthread = QtConcurrent::run(error_diffusion_dither, imageHashMono.getSourceImage(),
                                    matrix, serpentine, jitter, out_buf);
        runDitherThread();
        ErrorDiffusionMatrix_free(matrix);
    }
}

void MainWindow::LIP_dither(uint8_t* out_buf, const SubDitherType n) {
    /* Runs the Lippens & Philips Dot ditherer */
    DotClassMatrix* matrix = nullptr;
    DotLippensCoefficients* coe = nullptr;
    switch(n) {
        case LIP_LI1:
            matrix = get_dotlippens_class_matrix();
            coe = get_dotlippens_coefficients1();
            break;
        case LIP_LI2:
            matrix = get_dotlippens_class_matrix();
            coe = get_dotlippens_coefficients2();
            break;
        case LIP_LI3:
            matrix = get_dotlippens_class_matrix();
            coe = get_dotlippens_coefficients3();
            break;
        case LIP_GUO:
            matrix = get_guoliu_16x16_class_matrix();
            coe = get_dotlippens_coefficients1();
            break;
        case LIP_MSE:
            matrix = get_mese_16x16_class_matrix();
            coe = get_dotlippens_coefficients1();
            break;
        case LIP_KNU:
            matrix = get_knuth_class_matrix();
            coe = get_dotlippens_coefficients1();
            break;
        default: qDebug() << "WARNING: requested VAR ditherer " << n << " not found"; break;
    }
    if(matrix != nullptr && coe != nullptr) {
        fthread = QtConcurrent::run(dotlippens_dither, imageHashMono.getSourceImage(), matrix, coe, out_buf);
        runDitherThread();
        DotLippensCoefficients_free(coe);
        DotClassMatrix_free(matrix);
    }
}

void MainWindow::ORD_dither(uint8_t* out_buf, const SubDitherType n) {
    /* Runs the Ordered ditherer */
    OrderedDitherMatrix* matrix = nullptr;
    switch(n) {
        case ORD_BLU: matrix = get_blue_noise_128x128(); break;
        case ORD_BA2: matrix = get_bayer2x2_matrix(); break;
        case ORD_BA3: matrix = get_bayer3x3_matrix(); break;
        case ORD_BA4: matrix = get_bayer4x4_matrix(); break;
        case ORD_BA8: matrix = get_bayer8x8_matrix(); break;
        case ORD_B16: matrix = get_bayer16x16_matrix(); break;
        case ORD_B32: matrix = get_bayer32x32_matrix(); break;
        case ORD_DD1: matrix = get_dispersed_dots_1_matrix(); break;
        case ORD_DD2: matrix = get_dispersed_dots_2_matrix(); break;
        case ORD_VDD: matrix = get_ulichney_void_dispersed_dots_matrix(); break;
        case ORD_NR1: matrix = get_non_rectangular_1_matrix(); break;
        case ORD_NR2: matrix = get_non_rectangular_2_matrix(); break;
        case ORD_NR3: matrix = get_non_rectangular_3_matrix(); break;
        case ORD_NR4: matrix = get_non_rectangular_4_matrix(); break;
        case ORD_ULY: matrix = get_ulichney_matrix(); break;
        case ORD_UB5: matrix = get_ulichney_bayer_5_matrix(); break;
        case ORD_CD1: matrix = get_bayer_clustered_dot_1_matrix(); break;
        case ORD_CD2: matrix = get_bayer_clustered_dot_2_matrix(); break;
        case ORD_CD3: matrix = get_bayer_clustered_dot_3_matrix(); break;
        case ORD_CD4: matrix = get_bayer_clustered_dot_4_matrix(); break;
        case ORD_CD5: matrix = get_bayer_clustered_dot_5_matrix(); break;
        case ORD_CD6: matrix = get_bayer_clustered_dot_6_matrix(); break;
        case ORD_CD7: matrix = get_bayer_clustered_dot_7_matrix(); break;
        case ORD_CD8: matrix = get_bayer_clustered_dot_8_matrix(); break;
        case ORD_CD9: matrix = get_bayer_clustered_dot_9_matrix(); break;
        case ORD_CDA: matrix = get_bayer_clustered_dot_10_matrix(); break;
        case ORD_CDB: matrix = get_bayer_clustered_dot_11_matrix(); break;
        case ORD_CWP: matrix = get_central_white_point_matrix(); break;
        case ORD_BWP: matrix = get_balanced_centered_point_matrix(); break;
        case ORD_DIO: matrix = get_diagonal_ordered_matrix_matrix(); break;
        case ORD_UCD: matrix = get_ulichney_clustered_dot_matrix(); break;
        case ORD_M5C: matrix = get_magic5x5_circle_matrix(); break;
        case ORD_M6C: matrix = get_magic6x6_circle_matrix(); break;
        case ORD_M7C: matrix = get_magic7x7_circle_matrix(); break;
        case ORD_M44: matrix = get_magic4x4_45_matrix(); break;
        case ORD_M64: matrix = get_magic6x6_45_matrix(); break;
        case ORD_M84: matrix = get_magic8x8_45_matrix(); break;
        case ORD_MA4: matrix = get_magic4x4_matrix(); break;
        case ORD_MA6: matrix = get_magic6x6_matrix(); break;
        case ORD_MA8: matrix = get_magic8x8_matrix(); break;
        case ORD_VA2: matrix = get_variable_2x2_matrix(ui->ORD_VAR_step->value()); break; // 50
        case ORD_VA4: matrix = get_variable_4x4_matrix(ui->ORD_VAR_step->value()); break;
        case ORD_IGR: {
            const double a = ui->ORD_IGR_a->value();
            const double b = ui->ORD_IGR_b->value();
            const double c = ui->ORD_IGR_c->value();
            matrix = get_interleaved_gradient_noise(ui->ORD_IGR_step->value(), a, b, c);
        } break;
        default: qDebug() << "WARNING: requested ORD ditherer " << n << " not found"; break;
    }
    if(matrix != nullptr) {
        double jitter;
        switch(n) {
            case ORD_VA2:
            case ORD_VA4: jitter = ui->ORD_VAR_jitter->value(); break;
            case ORD_IGR: jitter = ui->ORD_IGR_jitter->value(); break;
            default: jitter = ui->ORD_jitter->value(); break;
        }
        fthread = QtConcurrent::run(ordered_dither, imageHashMono.getSourceImage(), matrix, jitter, out_buf);
        runDitherThread();
        OrderedDitherMatrix_free(matrix);
    }
}

void MainWindow::PAT_dither(uint8_t* out_buf, const SubDitherType n) {
    /* Runs the Pattern ditherer */
    TilePattern *pattern = nullptr;
    switch(n) {
        case PAT_22A: pattern = get_2x2_pattern(); break;
        case PAT_33A: pattern = get_3x3_v1_pattern(); break;
        case PAT_33B: pattern = get_3x3_v2_pattern(); break;
        case PAT_33C: pattern = get_3x3_v3_pattern(); break;
        case PAT_44A: pattern = get_4x4_pattern(); break;
        case PAT_52A: pattern = get_5x2_pattern(); break;
        default: qDebug() << "WARNING: requested PAT ditherer " << n << " not found"; break;
    }
    if(pattern != nullptr) {
        fthread = QtConcurrent::run(pattern_dither, imageHashMono.getSourceImage(), pattern, out_buf);
        runDitherThread();
        TilePattern_free(pattern);
    }
}

void MainWindow::THR_dither(uint8_t* out_buf) {
    /* Runs the Threshold ditherer */
    double threshold;
    if (ui->THR_autoThreshold->isChecked()) {
        threshold = auto_threshold(imageHashMono.getSourceImage());
    } else {
        threshold = ui->THR_threshold->value();
    }
    double jitter = ui->THR_jitter->value();
    fthread = QtConcurrent::run(threshold_dither, imageHashMono.getSourceImage(), threshold, jitter, out_buf);
    runDitherThread();
}

void MainWindow::VAR_dither(uint8_t* out_buf, const SubDitherType n) {
    /* Runs the Variable Error Diffusion ditherer */
    bool serpentine = ui->VAR_serpentine->isChecked();
    switch((SubDitherType)n) {
        case VAR_OST:
            fthread = QtConcurrent::run(variable_error_diffusion_dither,
                                        imageHashMono.getSourceImage(), Ostromoukhov, serpentine, out_buf);
            runDitherThread();
            break;
        case VAR_ZHF:
            fthread = QtConcurrent::run(variable_error_diffusion_dither,
                                        imageHashMono.getSourceImage(), Zhoufang, serpentine, out_buf);
            runDitherThread();
            break;
        default: qDebug() << "WARNING: requested VAR ditherer " << n << " not found"; break;
    }
}

void MainWindow::RIM_dither(uint8_t* out_buf, const SubDitherType n) {
    /* Runs the Riemersma ditherer */
    RiemersmaCurve* curve = nullptr;
    switch(n) {
        case RIM_HIL: curve = get_hilbert_curve(); break;
        case RIM_HIM: curve = get_hilbert_mod_curve(); break;
        case RIM_PEA: curve = get_peano_curve(); break;
        case RIM_FS0: curve = get_fass0_curve(); break;
        case RIM_FS1: curve = get_fass1_curve(); break;
        case RIM_FS2: curve = get_fass2_curve(); break;
        case RIM_GOS: curve = get_gosper_curve(); break;
        case RIM_FSS: curve = get_fass_spiral_curve(); break;
        default: qDebug() << "WARNING: requested RIM ditherer " << n << " not found"; break;
    }
    if(curve != nullptr) {
        const bool mod = ui->RIM_modRiemersma->isChecked();
        fthread = QtConcurrent::run(riemersma_dither, imageHashMono.getSourceImage(), curve, !mod, out_buf);
        runDitherThread();
        RiemersmaCurve_free(curve);
    }
}
