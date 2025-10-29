#include "mainwindow.h"
#include "videoprocessor.h"
#include "videoprocessdialog.h"
#include "consts.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>
#include <iostream>

/* This file contains video processing functionality for MainWindow */

void MainWindow::loadVideoSlot(const QString &fileName) {
    /* loads a preview frame from the video file */
    
    // Store the video path for later export
    currentVideoPath = fileName;
    
    // Create video processor to extract a preview frame
    VideoProcessor* processor = new VideoProcessor(this);
    
    // Get video info
    int width, height;
    if (!processor->getVideoInfo(fileName, width, height, videoFps, videoTotalFrames)) {
        QMessageBox::critical(this, tr("Error"), 
                            tr("Failed to read video information. Make sure FFmpeg is installed."));
        delete processor;
        currentVideoPath.clear();
        return;
    }
    
    // Extract first frame for preview
    QImage previewFrame = processor->extractSingleFrame(fileName, 0.0);
    
    delete processor;
    
    if (previewFrame.isNull()) {
        QMessageBox::critical(this, tr("Error"), 
                            tr("Failed to extract preview frame from video."));
        currentVideoPath.clear();
        return;
    }
    
    // Load the preview frame as a normal image
    loadImage(&previewFrame);
    
    // Setup and show timeline
    videoTimeline->setVideoInfo(videoTotalFrames, videoFps);
    videoTimeline->setCurrentFrame(0);
    videoTimeline->setVisible(true);
    
    // Enable the Export Video action
    ui->actionExportVideo->setEnabled(true);
    
    // Show notification that this is a video preview
    QString videoInfo = QString(tr("Video loaded: %1\n%2x%3, %4 fps, %5 frames\n\nUse timeline to preview different frames.\nAdjust dithering settings, then use File → Export Video to process the entire video."))
        .arg(QFileInfo(fileName).fileName())
        .arg(width)
        .arg(height)
        .arg(QString::number(videoFps, 'f', 2))
        .arg(videoTotalFrames);
    
    notification->showText(videoInfo, 8000);
}

void MainWindow::exportVideoSlot() {
    /* exports the currently loaded video with the current dithering settings */
    
    if (currentVideoPath.isEmpty()) {
        QMessageBox::information(this, tr("No Video Loaded"), 
                               tr("Please load a video file first using File → Open Video or drag & drop."));
        return;
    }
    
    // Ask user for output location
    QString defaultOutput = QFileInfo(currentVideoPath).absolutePath() + QDir::separator() + 
                           QFileInfo(currentVideoPath).baseName() + "_dithered.mp4";
    
    QString outputPath = QFileDialog::getSaveFileName(this,
                                                      tr("Export Dithered Video"),
                                                      defaultOutput,
                                                      tr("Video Files (*.mp4 *.avi *.mov *.mkv)"));
    if (outputPath.isEmpty()) {
        return; // User cancelled
    }
    
    // Create and show progress dialog
    VideoProcessDialog* dialog = new VideoProcessDialog(this);
    
    // Create video processor
    VideoProcessor* processor = new VideoProcessor(this);
    
    // Get video info
    int width, height, frameCount;
    double fps;
    if (!processor->getVideoInfo(currentVideoPath, width, height, fps, frameCount)) {
        QMessageBox::critical(this, tr("Error"), 
                            tr("Failed to read video information."));
        delete processor;
        delete dialog;
        return;
    }
    
    dialog->setTotalFrames(frameCount);
    dialog->setStatus(tr("Extracting frames from video..."));
    
    // Connect signals
    connect(processor, &VideoProcessor::progressChanged, dialog, &VideoProcessDialog::setProgress);
    connect(processor, &VideoProcessor::frameProcessed, dialog, &VideoProcessDialog::setPreviewFrame);
    connect(processor, &VideoProcessor::finished, dialog, &VideoProcessDialog::processingComplete);
    connect(processor, &VideoProcessor::errorOccurred, dialog, &VideoProcessDialog::showError);
    connect(dialog, &VideoProcessDialog::cancelRequested, processor, &VideoProcessor::cancel);
    
    // Clean up when done
    connect(processor, &VideoProcessor::finished, [processor, dialog](bool success, const QString&) {
        processor->deleteLater();
        if (success) {
            QTimer::singleShot(2000, dialog, &QDialog::accept);
        }
    });
    
    // Preallocate buffers outside the callback to reuse them
    // This significantly reduces memory allocation overhead
    int videoWidth = 0, videoHeight = 0;
    double videoFps = 0;
    int videoFrameCount = 0;
    if (processor->getVideoInfo(currentVideoPath, videoWidth, videoHeight, videoFps, videoFrameCount)) {
        qDebug() << "Video info: " << videoWidth << "x" << videoHeight << " @ " << videoFps << " fps, " << videoFrameCount << " frames";
    }
    
    // Preallocate output buffer for the maximum frame size
    size_t bufferSize = static_cast<size_t>(videoWidth) * static_cast<size_t>(videoHeight);
    int* colorBuffer = static_cast<int*>(calloc(bufferSize, sizeof(int)));
    uint8_t* monoBuffer = static_cast<uint8_t*>(calloc(bufferSize, sizeof(uint8_t)));
    
    // CRITICAL: Create ONE ImageHash and reuse it for all frames
    // This avoids allocating 50+ MB per frame
    ImageHashMono* reusableHashMono = nullptr;
    ImageHashColor* reusableHashColor = nullptr;
    
    // OPTIMIZATION: Pre-allocate and cache dithering matrices to avoid reallocating them for every frame
    // These pointers will be reused across all frames
    ErrorDiffusionMatrix* cachedErrMatrix = nullptr;
    OrderedDitherMatrix* cachedOrdMatrix = nullptr;
    DotDiffusionMatrix* cachedDotDiffMatrix = nullptr;
    DotClassMatrix* cachedDotClassMatrix = nullptr;
    DotLippensCoefficients* cachedLippensCoef = nullptr;
    TilePattern* cachedPattern = nullptr;
    RiemersmaCurve* cachedCurve = nullptr;
    
    if (lastTabIndex == TAB_INDEX_MONO) {
        reusableHashMono = new ImageHashMono();
        reusableHashMono->brightness = imageHashMono.brightness;
        reusableHashMono->contrast = imageHashMono.contrast;
        reusableHashMono->gamma = imageHashMono.gamma;
        
        // Pre-allocate matrices for mono dithering based on current algorithm
        switch (current_dither_type) {
            case DOT: {
                switch(current_sub_dither_type) {
                    case DOT_KNU: cachedDotDiffMatrix = get_default_diffusion_matrix(); cachedDotClassMatrix = get_knuth_class_matrix(); break;
                    case DOT_MKN: cachedDotDiffMatrix = get_default_diffusion_matrix(); cachedDotClassMatrix = get_mini_knuth_class_matrix(); break;
                    case DOT_OKN: cachedDotDiffMatrix = get_default_diffusion_matrix(); cachedDotClassMatrix = get_optimized_knuth_class_matrix(); break;
                    case DOT_M08: cachedDotDiffMatrix = get_default_diffusion_matrix(); cachedDotClassMatrix = get_mese_8x8_class_matrix(); break;
                    case DOT_M16: cachedDotDiffMatrix = get_default_diffusion_matrix(); cachedDotClassMatrix = get_mese_16x16_class_matrix(); break;
                    case DOT_G08: cachedDotDiffMatrix = get_guoliu8_diffusion_matrix(); cachedDotClassMatrix = get_guoliu_8x8_class_matrix(); break;
                    case DOT_G16: cachedDotDiffMatrix = get_guoliu16_diffusion_matrix(); cachedDotClassMatrix = get_guoliu_16x16_class_matrix(); break;
                    case DOT_SPR: cachedDotDiffMatrix = get_guoliu8_diffusion_matrix(); cachedDotClassMatrix = get_spiral_class_matrix(); break;
                    case DOT_ISP: cachedDotDiffMatrix = get_guoliu8_diffusion_matrix(); cachedDotClassMatrix = get_spiral_inverted_class_matrix(); break;
                    default: break;
                }
            } break;
            case ERR: {
                switch(current_sub_dither_type) {
                    case ERR_XOT: cachedErrMatrix = get_xot_matrix(); break;
                    case ERR_DIA: cachedErrMatrix = get_diagonal_matrix(); break;
                    case ERR_FYD: cachedErrMatrix = get_floyd_steinberg_matrix(); break;
                    case ERR_SF3: cachedErrMatrix = get_shiaufan3_matrix(); break;
                    case ERR_SF2: cachedErrMatrix = get_shiaufan2_matrix(); break;
                    case ERR_SF1: cachedErrMatrix = get_shiaufan1_matrix(); break;
                    case ERR_STK: cachedErrMatrix = get_stucki_matrix(); break;
                    case ERR_D1D: cachedErrMatrix = get_diffusion_1d_matrix(); break;
                    case ERR_D2D: cachedErrMatrix = get_diffusion_2d_matrix(); break;
                    case ERR_FFS: cachedErrMatrix = get_fake_floyd_steinberg_matrix(); break;
                    case ERR_JJN: cachedErrMatrix = get_jarvis_judice_ninke_matrix(); break;
                    case ERR_ATK: cachedErrMatrix = get_atkinson_matrix(); break;
                    case ERR_BRK: cachedErrMatrix = get_burkes_matrix(); break;
                    case ERR_SI3: cachedErrMatrix = get_sierra_3_matrix(); break;
                    case ERR_SI2: cachedErrMatrix = get_sierra_2row_matrix(); break;
                    case ERR_SIL: cachedErrMatrix = get_sierra_lite_matrix(); break;
                    case ERR_SPG: cachedErrMatrix = get_steve_pigeon_matrix(); break;
                    case ERR_RKI: cachedErrMatrix = get_robert_kist_matrix(); break;
                    case ERR_SSA: cachedErrMatrix = get_stevenson_arce_matrix(); break;
                    default: break;
                }
            } break;
            case LIP: {
                switch(current_sub_dither_type) {
                    case LIP_LI1: cachedDotClassMatrix = get_dotlippens_class_matrix(); cachedLippensCoef = get_dotlippens_coefficients1(); break;
                    case LIP_LI2: cachedDotClassMatrix = get_dotlippens_class_matrix(); cachedLippensCoef = get_dotlippens_coefficients2(); break;
                    case LIP_LI3: cachedDotClassMatrix = get_dotlippens_class_matrix(); cachedLippensCoef = get_dotlippens_coefficients3(); break;
                    case LIP_GUO: cachedDotClassMatrix = get_guoliu_16x16_class_matrix(); cachedLippensCoef = get_dotlippens_coefficients1(); break;
                    case LIP_MSE: cachedDotClassMatrix = get_mese_16x16_class_matrix(); cachedLippensCoef = get_dotlippens_coefficients1(); break;
                    case LIP_KNU: cachedDotClassMatrix = get_knuth_class_matrix(); cachedLippensCoef = get_dotlippens_coefficients1(); break;
                    default: break;
                }
            } break;
            case ORD: {
                if (current_sub_dither_type == ORD_VA2) {
                    int step = ui->ORD_VAR_step->value();
                    cachedOrdMatrix = get_variable_2x2_matrix(step);
                } else if (current_sub_dither_type == ORD_VA4) {
                    int step = ui->ORD_VAR_step->value();
                    cachedOrdMatrix = get_variable_4x4_matrix(step);
                } else if (current_sub_dither_type == ORD_IGR) {
                    int step = ui->ORD_IGR_step->value();
                    double a = ui->ORD_IGR_a->value();
                    double b = ui->ORD_IGR_b->value();
                    double c = ui->ORD_IGR_c->value();
                    cachedOrdMatrix = get_interleaved_gradient_noise(step, a, b, c);
                } else {
                    switch(current_sub_dither_type) {
                        case ORD_BLU: cachedOrdMatrix = get_blue_noise_128x128(); break;
                        case ORD_BA2: cachedOrdMatrix = get_bayer2x2_matrix(); break;
                        case ORD_BA3: cachedOrdMatrix = get_bayer3x3_matrix(); break;
                        case ORD_BA4: cachedOrdMatrix = get_bayer4x4_matrix(); break;
                        case ORD_BA8: cachedOrdMatrix = get_bayer8x8_matrix(); break;
                        case ORD_B16: cachedOrdMatrix = get_bayer16x16_matrix(); break;
                        case ORD_B32: cachedOrdMatrix = get_bayer32x32_matrix(); break;
                        case ORD_DD1: cachedOrdMatrix = get_dispersed_dots_1_matrix(); break;
                        case ORD_DD2: cachedOrdMatrix = get_dispersed_dots_2_matrix(); break;
                        case ORD_VDD: cachedOrdMatrix = get_ulichney_void_dispersed_dots_matrix(); break;
                        case ORD_NR1: cachedOrdMatrix = get_non_rectangular_1_matrix(); break;
                        case ORD_NR2: cachedOrdMatrix = get_non_rectangular_2_matrix(); break;
                        case ORD_NR3: cachedOrdMatrix = get_non_rectangular_3_matrix(); break;
                        case ORD_NR4: cachedOrdMatrix = get_non_rectangular_4_matrix(); break;
                        case ORD_ULY: cachedOrdMatrix = get_ulichney_matrix(); break;
                        case ORD_UB5: cachedOrdMatrix = get_ulichney_bayer_5_matrix(); break;
                        case ORD_CD1: cachedOrdMatrix = get_bayer_clustered_dot_1_matrix(); break;
                        case ORD_CD2: cachedOrdMatrix = get_bayer_clustered_dot_2_matrix(); break;
                        case ORD_CD3: cachedOrdMatrix = get_bayer_clustered_dot_3_matrix(); break;
                        case ORD_CD4: cachedOrdMatrix = get_bayer_clustered_dot_4_matrix(); break;
                        case ORD_CD5: cachedOrdMatrix = get_bayer_clustered_dot_5_matrix(); break;
                        case ORD_CD6: cachedOrdMatrix = get_bayer_clustered_dot_6_matrix(); break;
                        case ORD_CD7: cachedOrdMatrix = get_bayer_clustered_dot_7_matrix(); break;
                        case ORD_CD8: cachedOrdMatrix = get_bayer_clustered_dot_8_matrix(); break;
                        case ORD_CD9: cachedOrdMatrix = get_bayer_clustered_dot_9_matrix(); break;
                        case ORD_CDA: cachedOrdMatrix = get_bayer_clustered_dot_10_matrix(); break;
                        case ORD_CDB: cachedOrdMatrix = get_bayer_clustered_dot_11_matrix(); break;
                        case ORD_CWP: cachedOrdMatrix = get_central_white_point_matrix(); break;
                        case ORD_BWP: cachedOrdMatrix = get_balanced_centered_point_matrix(); break;
                        case ORD_DIO: cachedOrdMatrix = get_diagonal_ordered_matrix_matrix(); break;
                        case ORD_UCD: cachedOrdMatrix = get_ulichney_clustered_dot_matrix(); break;
                        case ORD_M5C: cachedOrdMatrix = get_magic5x5_circle_matrix(); break;
                        case ORD_MA6: cachedOrdMatrix = get_magic6x6_matrix(); break;
                        case ORD_M7C: cachedOrdMatrix = get_magic7x7_circle_matrix(); break;
                        case ORD_MA4: cachedOrdMatrix = get_magic4x4_matrix(); break;
                        default: break;
                    }
                }
            } break;
            case PAT: {
                switch(current_sub_dither_type) {
                    case PAT_22A: cachedPattern = get_2x2_pattern(); break;
                    case PAT_33A: cachedPattern = get_3x3_v1_pattern(); break;
                    case PAT_33B: cachedPattern = get_3x3_v2_pattern(); break;
                    case PAT_33C: cachedPattern = get_3x3_v3_pattern(); break;
                    case PAT_44A: cachedPattern = get_4x4_pattern(); break;
                    case PAT_52A: cachedPattern = get_5x2_pattern(); break;
                    default: break;
                }
            } break;
            case RIM: {
                switch(current_sub_dither_type) {
                    case RIM_HIL: cachedCurve = get_hilbert_curve(); break;
                    case RIM_HIM: cachedCurve = get_hilbert_mod_curve(); break;
                    case RIM_PEA: cachedCurve = get_peano_curve(); break;
                    case RIM_FS0: cachedCurve = get_fass0_curve(); break;
                    case RIM_FS1: cachedCurve = get_fass1_curve(); break;
                    case RIM_FS2: cachedCurve = get_fass2_curve(); break;
                    case RIM_GOS: cachedCurve = get_gosper_curve(); break;
                    case RIM_FSS: cachedCurve = get_fass_spiral_curve(); break;
                    default: break;
                }
            } break;
            default: break;
        }
    } else {
        reusableHashColor = new ImageHashColor();
        reusableHashColor->brightness = imageHashColor.brightness;
        reusableHashColor->contrast = imageHashColor.contrast;
        reusableHashColor->gamma = imageHashColor.gamma;
        reusableHashColor->saturation = imageHashColor.saturation;
        
        // Pre-allocate matrices for color dithering
        if (current_dither_type == ERR_C) {
            switch(current_sub_dither_type) {
                case ERR_XOT_C: cachedErrMatrix = get_xot_matrix(); break;
                case ERR_DIA_C: cachedErrMatrix = get_diagonal_matrix(); break;
                case ERR_FYD_C: cachedErrMatrix = get_floyd_steinberg_matrix(); break;
                case ERR_SF3_C: cachedErrMatrix = get_shiaufan3_matrix(); break;
                case ERR_SF2_C: cachedErrMatrix = get_shiaufan2_matrix(); break;
                case ERR_SF1_C: cachedErrMatrix = get_shiaufan1_matrix(); break;
                case ERR_STK_C: cachedErrMatrix = get_stucki_matrix(); break;
                case ERR_D1D_C: cachedErrMatrix = get_diffusion_1d_matrix(); break;
                case ERR_D2D_C: cachedErrMatrix = get_diffusion_2d_matrix(); break;
                case ERR_FFS_C: cachedErrMatrix = get_fake_floyd_steinberg_matrix(); break;
                case ERR_JJN_C: cachedErrMatrix = get_jarvis_judice_ninke_matrix(); break;
                case ERR_ATK_C: cachedErrMatrix = get_atkinson_matrix(); break;
                case ERR_BRK_C: cachedErrMatrix = get_burkes_matrix(); break;
                case ERR_SI3_C: cachedErrMatrix = get_sierra_3_matrix(); break;
                case ERR_SI2_C: cachedErrMatrix = get_sierra_2row_matrix(); break;
                case ERR_SIL_C: cachedErrMatrix = get_sierra_lite_matrix(); break;
                case ERR_SPG_C: cachedErrMatrix = get_steve_pigeon_matrix(); break;
                case ERR_RKI_C: cachedErrMatrix = get_robert_kist_matrix(); break;
                case ERR_SSA_C: cachedErrMatrix = get_stevenson_arce_matrix(); break;
                default: break;
            }
        } else if (current_dither_type == ORD_C) {
            switch(current_sub_dither_type) {
                case ORD_BLU_C: cachedOrdMatrix = get_blue_noise_128x128(); break;
                case ORD_BA2_C: cachedOrdMatrix = get_bayer2x2_matrix(); break;
                case ORD_BA3_C: cachedOrdMatrix = get_bayer3x3_matrix(); break;
                case ORD_BA4_C: cachedOrdMatrix = get_bayer4x4_matrix(); break;
                case ORD_BA8_C: cachedOrdMatrix = get_bayer8x8_matrix(); break;
                case ORD_B16_C: cachedOrdMatrix = get_bayer16x16_matrix(); break;
                case ORD_B32_C: cachedOrdMatrix = get_bayer32x32_matrix(); break;
                case ORD_DD1_C: cachedOrdMatrix = get_dispersed_dots_1_matrix(); break;
                case ORD_DD2_C: cachedOrdMatrix = get_dispersed_dots_2_matrix(); break;
                case ORD_VDD_C: cachedOrdMatrix = get_ulichney_void_dispersed_dots_matrix(); break;
                case ORD_NR1_C: cachedOrdMatrix = get_non_rectangular_1_matrix(); break;
                case ORD_NR2_C: cachedOrdMatrix = get_non_rectangular_2_matrix(); break;
                case ORD_NR3_C: cachedOrdMatrix = get_non_rectangular_3_matrix(); break;
                case ORD_NR4_C: cachedOrdMatrix = get_non_rectangular_4_matrix(); break;
                case ORD_ULY_C: cachedOrdMatrix = get_ulichney_matrix(); break;
                default: break;
            }
        }
    }
    
    // Create dither callback based on current settings
    auto ditherCallback = [this, colorBuffer, monoBuffer, reusableHashMono, reusableHashColor,
                           cachedErrMatrix, cachedOrdMatrix, cachedDotDiffMatrix, cachedDotClassMatrix,
                           cachedLippensCoef, cachedPattern, cachedCurve](const QImage& frame, int frameNum) -> QImage {
        // Apply dithering to the frame and return dithered image
        QImage ditheredFrame;
        
        // Debug: print which algorithm is being used (first frame only)
        if (frameNum == 0) {
            std::cerr << "=== VIDEO EXPORT DEBUG ===" << std::endl;
            std::cerr << "Tab Index: " << lastTabIndex << " (0=mono, 1=color)" << std::endl;
            std::cerr << "Dither Type: " << current_dither_type << std::endl;
            std::cerr << "Sub Dither Type: " << current_sub_dither_type << std::endl;
            std::cerr << "Frame size: " << frame.width() << "x" << frame.height() << std::endl;
            std::cerr << "=========================" << std::endl;
        }
        
        if (lastTabIndex == TAB_INDEX_MONO) {
            // Mono dithering - REUSE the same ImageHash
            int w = frame.width();
            int h = frame.height();
            
            // Update the ImageHash with the new frame
            if (frameNum == 0) {
                // setSourceImage() resets brightness/contrast/gamma, so save and restore them
                int savedBrightness = reusableHashMono->brightness;
                int savedContrast = reusableHashMono->contrast;
                int savedGamma = reusableHashMono->gamma;
                
                reusableHashMono->setSourceImage(&frame);
                
                // Restore adjustments and reapply
                reusableHashMono->brightness = savedBrightness;
                reusableHashMono->contrast = savedContrast;
                reusableHashMono->gamma = savedGamma;
                reusableHashMono->adjustSource();
            } else {
                // OPTIMIZED: Direct memory access instead of pixel-by-pixel
                // This is 10-20x faster than using pixel() and setPixel()
                DitherImage* origLinear = reusableHashMono->getOrigLinear();
                
                // Ensure frame is in RGB888 format for fast scanLine access
                QImage frameRGB = (frame.format() == QImage::Format_RGB888) 
                    ? frame 
                    : frame.convertToFormat(QImage::Format_RGB888);
                
                for(int y = 0; y < h; y++) {
                    const uint8_t* scanLine = frameRGB.constScanLine(y);
                    for(int x = 0; x < w; x++) {
                        int offset = x * 3;
                        uint8_t r = scanLine[offset];
                        uint8_t g = scanLine[offset + 1];
                        uint8_t b = scanLine[offset + 2];
                        DitherImage_set_pixel_rgba(origLinear, x, y, r, g, b, 255, true);
                    }
                }
                // Reapply adjustments from origLinear to sourceImg
                reusableHashMono->adjustSource();
            }
            
            // REUSE preallocated buffer instead of allocating new one
            uint8_t *out_buf = monoBuffer;
            // Clear the buffer
            memset(out_buf, 0, static_cast<size_t>(w * h) * sizeof(uint8_t));
            
            // Get the source image (with adjustments applied)
            DitherImage* sourceImg = reusableHashMono->getSourceImage();
            
            // Get dot size and spacing from UI
            int dot_size = ui->dotSizeSpinBox->value();
            int dot_spacing = ui->dotSpacingSpinBox->value();
            
            // Apply current dither algorithm synchronously
            switch (current_dither_type) {
                case ALL: {
                    bool randomize = ui->ALL_randomize->isChecked();
                    kallebach_dither(sourceImg, randomize, dot_size, dot_spacing, out_buf);
                } break;
                case GRD: {
                    bool altAlgorithm = ui->GRD_altAlgorithm->isChecked();
                    int width = ui->GRD_width->value();
                    int height = ui->GRD_height->value();
                    int minPixels = ui->GRD_minPixels->value();
                    grid_dither(sourceImg, width, height, minPixels, altAlgorithm, dot_size, dot_spacing, out_buf);
                } break;
                case DBS: {
                    int formula = ui->DBS_formula->currentIndex();
                    dbs_dither(sourceImg, formula, dot_size, dot_spacing, out_buf);
                } break;
                case THR: {
                    double threshold;
                    if (ui->THR_autoThreshold->isChecked()) {
                        threshold = auto_threshold(sourceImg);
                    } else {
                        threshold = ui->THR_threshold->value();
                    }
                    double jitter = ui->THR_jitter->value();
                    threshold_dither(sourceImg, threshold, jitter, dot_size, dot_spacing, out_buf);
                } break;
                case DOT: {
                    // Use cached matrices instead of reallocating
                    if (cachedDotDiffMatrix != nullptr && cachedDotClassMatrix != nullptr) {
                        dot_diffusion_dither(sourceImg, cachedDotDiffMatrix, cachedDotClassMatrix, dot_size, dot_spacing, out_buf);
                    }
                } break;
                case ERR: {
                    // Use cached matrix instead of reallocating
                    if (cachedErrMatrix != nullptr) {
                        bool serpentine = ui->ERR_serpentine->isChecked();
                        double jitter = ui->ERR_jitter->value();
                        error_diffusion_dither(sourceImg, cachedErrMatrix, serpentine, jitter, dot_size, dot_spacing, out_buf);
                    }
                } break;
                case LIP: {
                    // Use cached matrices instead of reallocating
                    if (cachedDotClassMatrix != nullptr && cachedLippensCoef != nullptr) {
                        dotlippens_dither(sourceImg, cachedDotClassMatrix, cachedLippensCoef, dot_size, dot_spacing, out_buf);
                    }
                } break;
                case ORD: {
                    // Use cached matrix instead of reallocating
                    if (cachedOrdMatrix != nullptr) {
                        double jitter;
                        if (current_sub_dither_type == ORD_VA2 || current_sub_dither_type == ORD_VA4) {
                            jitter = ui->ORD_VAR_jitter->value();
                        } else if (current_sub_dither_type == ORD_IGR) {
                            jitter = ui->ORD_IGR_jitter->value();
                        } else {
                            jitter = ui->ORD_jitter->value();
                        }
                        ordered_dither(sourceImg, cachedOrdMatrix, jitter, dot_size, dot_spacing, out_buf);
                    }
                } break;
                case PAT: {
                    // Use cached pattern instead of reallocating
                    if (cachedPattern != nullptr) {
                        pattern_dither(sourceImg, cachedPattern, dot_size, dot_spacing, out_buf);
                    }
                } break;
                case RIM: {
                    // Use cached curve instead of reallocating
                    if (cachedCurve != nullptr) {
                        bool modRiemersma = ui->RIM_modRiemersma->isChecked();
                        riemersma_dither(sourceImg, cachedCurve, modRiemersma, dot_size, dot_spacing, out_buf);
                    }
                } break;
                case VAR: {
                    bool serpentine = ui->VAR_serpentine->isChecked();
                    switch(current_sub_dither_type) {
                        case VAR_OST:
                            variable_error_diffusion_dither(sourceImg, Ostromoukhov, serpentine, dot_size, dot_spacing, out_buf);
                            break;
                        case VAR_ZHF:
                            variable_error_diffusion_dither(sourceImg, Zhoufang, serpentine, dot_size, dot_spacing, out_buf);
                            break;
                        default: break;
                    }
                } break;
                default:
                    break;
            }
            
            // OPTIMIZED: Convert buffer to QImage with custom colors using direct memory access
            ditheredFrame = QImage(w, h, QImage::Format_RGB32);
            QRgb color1Rgb = ui->monoColorOneButton->getColor().rgb();
            QRgb color2Rgb = ui->monoColorTwoButton->getColor().rgb();
            
            for (int y = 0; y < h; y++) {
                QRgb* scanLine = reinterpret_cast<QRgb*>(ditheredFrame.scanLine(y));
                const uint8_t* bufLine = out_buf + (y * w);
                for (int x = 0; x < w; x++) {
                    // out_buf contains 0 (black) or 255 (white)
                    // black (0) -> use monoColorOne, white (255) -> use monoColorTwo
                    scanLine[x] = (bufLine[x] == 0) ? color1Rgb : color2Rgb;
                }
            }
            
            // DON'T free - buffer is reused
        } else {
            // Color dithering - REUSE the same ImageHash
            int w = frame.width();
            int h = frame.height();
            
            // Update the ImageHash with the new frame
            if (frameNum == 0) {
                // setSourceImage() resets brightness/contrast/gamma/saturation, so save and restore them
                int savedBrightness = reusableHashColor->brightness;
                int savedContrast = reusableHashColor->contrast;
                int savedGamma = reusableHashColor->gamma;
                int savedSaturation = reusableHashColor->saturation;
                
                reusableHashColor->setSourceImage(&frame);
                
                // Restore adjustments and reapply
                reusableHashColor->brightness = savedBrightness;
                reusableHashColor->contrast = savedContrast;
                reusableHashColor->gamma = savedGamma;
                reusableHashColor->saturation = savedSaturation;
                reusableHashColor->adjustSource();
            } else {
                // OPTIMIZED: Direct memory access instead of pixel-by-pixel
                QImage* origImg = reusableHashColor->getSourceQImage();
                
                // Ensure frame is in ARGB32 format for fast scanLine access
                QImage frameARGB = (frame.format() == QImage::Format_ARGB32 || frame.format() == QImage::Format_RGB32) 
                    ? frame 
                    : frame.convertToFormat(QImage::Format_ARGB32);
                
                for(int y = 0; y < h; y++) {
                    const QRgb* srcLine = reinterpret_cast<const QRgb*>(frameARGB.constScanLine(y));
                    QRgb* dstLine = reinterpret_cast<QRgb*>(origImg->scanLine(y));
                    memcpy(dstLine, srcLine, static_cast<size_t>(w) * sizeof(QRgb));
                }
                // Reapply adjustments from origQImage to sourceImg
                reusableHashColor->adjustSource();
            }
            
            // REUSE preallocated buffer instead of allocating new one
            int* out_buf = colorBuffer;
            // Clear the buffer
            memset(out_buf, 0, static_cast<size_t>(w * h) * sizeof(int));
            
            // Get the source image (with adjustments applied)
            ColorImage* sourceImg = reusableHashColor->getSourceImage();
            
            // Get dot size and spacing from UI
            int dot_size = ui->dotSizeSpinBox->value();
            int dot_spacing = ui->dotSpacingSpinBox->value();
            
            // Apply current dither algorithm synchronously
            if (current_dither_type == ERR_C) {
                // Use cached error diffusion matrix for color
                if (cachedErrMatrix != nullptr) {
                    bool serpentine = ui->ERR_C_serpentine->isChecked();
                    error_diffusion_dither_color(sourceImg, cachedErrMatrix, cachedPalette, serpentine, dot_size, dot_spacing, out_buf);
                }
            } else if (current_dither_type == ORD_C) {
                // Use cached ordered dithering matrix for color
                if (cachedOrdMatrix != nullptr) {
                    ordered_dither_color(sourceImg, cachedPalette, cachedOrdMatrix, dot_size, dot_spacing, out_buf);
                }
            }
            
            // OPTIMIZED: Convert buffer to QImage using current palette with direct memory access
            ditheredFrame = QImage(w, h, QImage::Format_RGB32);
            BytePalette* palette = cachedPalette->target_palette;
            
            for (int y = 0; y < h; y++) {
                QRgb* scanLine = reinterpret_cast<QRgb*>(ditheredFrame.scanLine(y));
                const int* bufLine = out_buf + (y * w);
                for (int x = 0; x < w; x++) {
                    int colorIdx = bufLine[x];
                    if (colorIdx >= 0 && colorIdx < static_cast<int>(palette->size)) {
                        ByteColor* color = BytePalette_get(palette, colorIdx);
                        scanLine[x] = qRgb(color->r, color->g, color->b);
                    }
                }
            }
            
            // DON'T free - buffer is reused
        }
        
        return ditheredFrame;
    };
    
    // Start processing
    dialog->show();
    processor->processVideo(currentVideoPath, outputPath, ditherCallback);
    
    // Free buffers after processing is complete
    free(colorBuffer);
    free(monoBuffer);
    
    // Free cached matrices
    if (cachedErrMatrix) ErrorDiffusionMatrix_free(cachedErrMatrix);
    if (cachedOrdMatrix) OrderedDitherMatrix_free(cachedOrdMatrix);
    if (cachedDotDiffMatrix) DotDiffusionMatrix_free(cachedDotDiffMatrix);
    if (cachedDotClassMatrix) DotClassMatrix_free(cachedDotClassMatrix);
    if (cachedLippensCoef) DotLippensCoefficients_free(cachedLippensCoef);
    if (cachedPattern) TilePattern_free(cachedPattern);
    if (cachedCurve) RiemersmaCurve_free(cachedCurve);
    
    // Clean up reusable hash objects
    delete reusableHashMono;
    delete reusableHashColor;
}

void MainWindow::fileOpenVideoSlot() {
    /* opens file dialog for loading a video file */
    QString fileName;
    if (fileManager.getOpenVideoName(&fileName)) {
        loadVideoSlot(fileName);
    }
}

void MainWindow::videoFrameChangedSlot(int frameNumber) {
    /* user moved the timeline slider to view a different frame */
    
    if (currentVideoPath.isEmpty()) {
        return;
    }
    
    // Calculate time in seconds for this frame
    double timeSeconds = 0.0;
    if (videoFps > 0) {
        timeSeconds = frameNumber / videoFps;
    }
    
    // Extract the requested frame
    VideoProcessor* processor = new VideoProcessor(this);
    QImage frame = processor->extractSingleFrame(currentVideoPath, timeSeconds);
    delete processor;
    
    if (!frame.isNull()) {
        // Update the frame without resetting UI state
        updateVideoFrame(&frame);
    }
}

void MainWindow::updateVideoFrame(const QImage* image) {
    /* Update video frame without resetting the entire UI state */
    
    if(image->width() * image->height() > IMAGE_MAX_SIZE) {
        return;
    }
    
    // Update resolution label
    ui->resolutionLabel->setText(QString("%1 \u00D7 %2").arg(image->width()).arg(image->height()));
    
    // Update the current tab's image hash and re-apply current dithering
    if (ui->imageSettingsStackedWidget->currentIndex() == 0) {
        // Mono tab - save current adjustment values
        double brightness = imageHashMono.brightness;
        double contrast = imageHashMono.contrast;
        double gamma = imageHashMono.gamma;
        
        // Update source image
        imageHashMono.setSourceImage(image);
        
        // Restore adjustment values
        imageHashMono.brightness = brightness;
        imageHashMono.contrast = contrast;
        imageHashMono.gamma = gamma;
        
        // Apply adjustments and update view
        ui->graphicsView->setSourceImageMono(imageHashMono.getSourceQImage());
        adjustImageMono();
        
    } else {
        // Color tab - save current adjustment values
        double brightness = imageHashColor.brightness;
        double contrast = imageHashColor.contrast;
        double gamma = imageHashColor.gamma;
        double saturation = imageHashColor.saturation;
        
        // Update source image
        imageHashColor.setSourceImage(image);
        
        // Restore adjustment values
        imageHashColor.brightness = brightness;
        imageHashColor.contrast = contrast;
        imageHashColor.gamma = gamma;
        imageHashColor.saturation = saturation;
        
        // Apply adjustments and update view
        ui->graphicsView->setSourceImageColor(imageHashColor.getSourceQImage());
        adjustImageColor();
    }
}

