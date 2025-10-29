#include "videoprocessor.h"
#include <QFileInfo>
#include <QDir>
#include <QRegularExpression>
#include <QImageReader>
#include <QImageWriter>
#include <QDebug>
#include <QThread>
#include <QStandardPaths>
#include <QCoreApplication>
#include <QFile>
#include <QPixmapCache>

VideoProcessor::VideoProcessor(QObject* parent) : QObject(parent) {
    // Use .cache/ditherista directory instead of temporary directory
    QString cacheDir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    cachePath = cacheDir + "/ditherista";
    
    QDir dir;
    if (!dir.mkpath(cachePath)) {
        qWarning() << "Failed to create cache directory:" << cachePath;
    } else {
        qDebug() << "VideoProcessor: Using cache directory:" << cachePath;
    }
}

VideoProcessor::~VideoProcessor() {
    if (ffmpegProcess) {
        ffmpegProcess->kill();
        ffmpegProcess->waitForFinished();
        delete ffmpegProcess;
    }
    cleanupCache();
}

void VideoProcessor::cleanupCache() {
    // Clean up processed frames from cache
    QDir cacheDir(cachePath);
    if (cacheDir.exists()) {
        QStringList filters;
        filters << "dithered_frame_*.png";
        QStringList files = cacheDir.entryList(filters, QDir::Files);
        for (const QString& file : files) {
            cacheDir.remove(file);
        }
        qDebug() << "VideoProcessor: Cleaned up" << files.count() << "cached frames";
    }
}

bool VideoProcessor::checkFFmpegAvailable() {
    QProcess process;
    process.start("ffmpeg", QStringList() << "-version");
    process.waitForFinished(3000);
    return process.exitCode() == 0;
}

bool VideoProcessor::getVideoInfo(const QString& videoPath, int& width, int& height, 
                                  double& fps, int& frameCount) {
    if (!checkFFmpegAvailable()) {
        emit errorOccurred(tr("FFmpeg not found. Please install FFmpeg to process videos."));
        return false;
    }

    QProcess process;
    QStringList args;
    args << "-i" << videoPath;
    
    process.start("ffprobe", args);
    process.waitForFinished();
    
    QString output = process.readAllStandardError();
    
    // Parse resolution
    QRegularExpression resRegex(R"((\d{2,5})x(\d{2,5}))");
    QRegularExpressionMatch resMatch = resRegex.match(output);
    if (resMatch.hasMatch()) {
        width = resMatch.captured(1).toInt();
        height = resMatch.captured(2).toInt();
    } else {
        return false;
    }
    
    // Parse FPS
    QRegularExpression fpsRegex(R"((\d+(?:\.\d+)?)\s*fps)");
    QRegularExpressionMatch fpsMatch = fpsRegex.match(output);
    if (fpsMatch.hasMatch()) {
        fps = fpsMatch.captured(1).toDouble();
    } else {
        fps = 30.0; // default
    }
    
    // Parse duration to estimate frame count
    QRegularExpression durRegex(R"(Duration:\s*(\d{2}):(\d{2}):(\d{2})\.(\d{2}))");
    QRegularExpressionMatch durMatch = durRegex.match(output);
    if (durMatch.hasMatch()) {
        int hours = durMatch.captured(1).toInt();
        int minutes = durMatch.captured(2).toInt();
        int seconds = durMatch.captured(3).toInt();
        double totalSeconds = hours * 3600 + minutes * 60 + seconds;
        frameCount = static_cast<int>(totalSeconds * fps);
    } else {
        frameCount = 0;
    }
    
    return true;
}

QImage VideoProcessor::extractSingleFrame(const QString& videoPath, double timeSeconds) {
    /* Extracts a single frame from the video at the specified time */
    if (!checkFFmpegAvailable()) {
        return QImage();
    }

    // Create a temporary file for the extracted frame
    QString framePath = cachePath + "/preview_frame.png";
    
    // Remove old preview frame if exists
    QFile::remove(framePath);
    
    QProcess process;
    QStringList args;
    args << "-ss" << QString::number(timeSeconds)
         << "-i" << videoPath
         << "-vframes" << "1"
         << "-f" << "image2"
         << framePath;

    process.start("ffmpeg", args);
    process.waitForFinished(10000); // 10 second timeout
    
    if (process.exitCode() != 0 || !QFile::exists(framePath)) {
        return QImage();
    }

    QImage frame(framePath);
    return frame;
}

bool VideoProcessor::extractAllFrames() {
    /* Extract all frames from video in one go */
    state = ExtractingFrames;
    
    // Clean up any old frames
    cleanupCache();
    QDir cacheDir(cachePath);
    QStringList oldFrames = cacheDir.entryList(QStringList() << "frame_*.png", QDir::Files);
    for (const QString& file : oldFrames) {
        cacheDir.remove(file);
    }
    
    QProcess extractProcess;
    QStringList extractArgs;
    extractArgs << "-i" << currentInputPath
                << "-vf" << QString("fps=%1").arg(videoFps)
                << cachePath + "/frame_%06d.png";
    
    extractProcess.start("ffmpeg", extractArgs);
    if (!extractProcess.waitForStarted()) {
        emit errorOccurred(tr("Failed to start frame extraction"));
        return false;
    }
    
    // Wait for extraction to complete
    if (!extractProcess.waitForFinished(-1)) {  // No timeout
        emit errorOccurred(tr("Frame extraction failed"));
        return false;
    }
    
    if (extractProcess.exitCode() != 0) {
        emit errorOccurred(tr("FFmpeg extraction error"));
        return false;
    }
    
    qDebug() << "VideoProcessor: All frames extracted to cache";
    return true;
}

bool VideoProcessor::processFramesSequentially() {
    /* Process frames one at a time to minimize memory usage */
    state = ProcessingFrames;
    isCancelled = false;
    processedFrames = 0;
    
    // Get list of extracted frames
    QDir cacheDir(cachePath);
    QStringList frameFiles = cacheDir.entryList(QStringList() << "frame_*.png", QDir::Files, QDir::Name);
    
    if (frameFiles.isEmpty()) {
        emit errorOccurred(tr("No frames found to process"));
        state = Idle;
        return false;
    }
    
    totalFrames = frameFiles.count();
    emit progressChanged(0, totalFrames);
    
    qDebug() << "VideoProcessor: Processing" << totalFrames << "frames sequentially";
    
    // Process each frame one at a time
    for (int i = 0; i < totalFrames; ++i) {
        if (isCancelled) {
            emit finished(false, tr("Processing cancelled by user"));
            state = Idle;
            return false;
        }
        
        QString framePath = cachePath + "/" + frameFiles[i];
        QString ditheredPath = cachePath + QString("/dithered_frame_%1.png").arg(i + 1, 6, 10, QChar('0'));
        
        // Strict scope to ensure immediate deallocation
        {
            // Load ONE frame at a time with explicit format to avoid conversions
            QImageReader reader(framePath);
            reader.setAutoTransform(true);
            QImage frame = reader.read();
            
            if (frame.isNull()) {
                qWarning() << "Failed to load frame:" << framePath;
                QFile::remove(framePath);
                continue;
            }
            
            // Convert to RGB888 format to reduce memory if needed
            if (frame.format() != QImage::Format_RGB888 && 
                frame.format() != QImage::Format_ARGB32) {
                frame = frame.convertToFormat(QImage::Format_RGB888);
            }
            
            // Apply dithering to this single frame
            QImage ditheredFrame = currentDitherCallback(frame, i);
            
            // IMMEDIATELY destroy original frame
            frame = QImage();
            
            // Save dithered frame immediately
            QImageWriter writer(ditheredPath, "PNG");
            writer.setCompression(1); // Fast compression
            if (!writer.write(ditheredFrame)) {
                qWarning() << "Failed to save dithered frame:" << i;
                QFile::remove(framePath);
                continue;
            }
            
            // Emit preview occasionally (but don't keep in memory)
            if ((i + 1) % 30 == 0 || (i + 1) == totalFrames) {
                emit frameProcessed(ditheredFrame, i);
            }
            
            // IMMEDIATELY destroy dithered frame
            ditheredFrame = QImage();
            
        } // Scope ends here - all QImage objects destroyed
        
        // Delete the original extracted frame to save disk space
        QFile::remove(framePath);
        
        // Update progress
        processedFrames = i + 1;
        if ((i + 1) % 5 == 0 || (i + 1) == totalFrames) {
            emit progressChanged(i + 1, totalFrames);
        }
        
        // Every 10 frames, aggressively clean up memory
        if ((i + 1) % 10 == 0) {
            // Force Qt to process events and clean up
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
            
            // Force cleanup of Qt's internal image cache
            QPixmapCache::clear();
        }
    }
    
    if (isCancelled) {
        emit finished(false, tr("Processing cancelled by user"));
        state = Idle;
        return false;
    }
    
    qDebug() << "VideoProcessor: All frames processed, starting encoding";
    
    // Start encoding
    if (!encodeVideo(currentOutputPath)) {
        emit errorOccurred(tr("Failed to start video encoding"));
        emit finished(false, tr("Encoding failed to start"));
        state = Idle;
        return false;
    }
    
    return true;
}



bool VideoProcessor::encodeVideo(const QString& outputPath) {
    state = EncodingVideo;
    
    if (ffmpegProcess) {
        disconnect(ffmpegProcess, nullptr, this, nullptr);
        ffmpegProcess->deleteLater();
    }
    
    ffmpegProcess = new QProcess(this);
    connect(ffmpegProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &VideoProcessor::onFFmpegEncodeFinished);
    connect(ffmpegProcess, &QProcess::readyReadStandardError,
            this, &VideoProcessor::onFFmpegReadyReadStandardError);

    QString framesPattern = cachePath + "/dithered_frame_%06d.png";
    QStringList args;
    args << "-y"  // overwrite output file
         << "-framerate" << QString::number(videoFps)
         << "-i" << framesPattern
         << "-c:v" << "libx264"
         << "-pix_fmt" << "yuv420p"
         << "-preset" << "veryfast"
         << "-crf" << "18"
         << outputPath;

    ffmpegProcess->start("ffmpeg", args);
    return ffmpegProcess->waitForStarted();
}

void VideoProcessor::onFFmpegEncodeFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    if (isCancelled) {
        emit finished(false, tr("Processing cancelled by user"));
        state = Idle;
        return;
    }

    if (exitStatus != QProcess::NormalExit || exitCode != 0) {
        emit errorOccurred(tr("Failed to encode video"));
        emit finished(false, tr("Video encoding failed"));
    } else {
        emit finished(true, tr("Video processed successfully"));
    }
    
    state = Idle;
}

void VideoProcessor::onFFmpegReadyReadStandardError() {
    if (ffmpegProcess) {
        QString output = ffmpegProcess->readAllStandardError();
        // Parse progress if needed
        // qDebug() << "FFmpeg output:" << output;
    }
}

bool VideoProcessor::processVideo(const QString& inputPath, 
                                  const QString& outputPath,
                                  std::function<QImage(const QImage&, int)> ditherCallback) {
    if (state != Idle) {
        emit errorOccurred(tr("Video processor is busy"));
        return false;
    }

    if (!checkFFmpegAvailable()) {
        emit errorOccurred(tr("FFmpeg is not installed or not in PATH. Please install FFmpeg to process videos."));
        return false;
    }

    if (!getVideoInfo(inputPath, videoWidth, videoHeight, videoFps, totalFrames)) {
        emit errorOccurred(tr("Failed to get video information"));
        return false;
    }

    currentInputPath = inputPath;
    currentOutputPath = outputPath;
    currentDitherCallback = ditherCallback;

    // First extract all frames
    if (!extractAllFrames()) {
        return false;
    }
    
    // Then process frames sequentially to minimize memory usage
    return processFramesSequentially();
}

void VideoProcessor::cancel() {
    isCancelled = true;
    if (ffmpegProcess && ffmpegProcess->state() == QProcess::Running) {
        ffmpegProcess->kill();
    }
}
