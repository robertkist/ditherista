#include "videotimeline.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStyle>

VideoTimeline::VideoTimeline(QWidget *parent)
    : QWidget(parent)
    , m_totalFrames(0)
    , m_fps(0.0)
    , m_visible(false)
    , m_isDragging(false)
{
    // Create layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(6, 4, 6, 4);
    mainLayout->setSpacing(4);
    
    // Top row: frame info and time
    QHBoxLayout* infoLayout = new QHBoxLayout();
    infoLayout->setSpacing(10);
    
    m_frameLabel = new QLabel("Frame: 0 / 0", this);
    m_frameLabel->setMinimumWidth(120);
    
    m_timeLabel = new QLabel("Time: 00:00.000", this);
    m_timeLabel->setMinimumWidth(130);
    
    infoLayout->addWidget(m_frameLabel);
    infoLayout->addWidget(m_timeLabel);
    infoLayout->addStretch();
    
    // Bottom row: controls
    QHBoxLayout* controlsLayout = new QHBoxLayout();
    controlsLayout->setSpacing(6);
    
    m_previousButton = new QPushButton(this);
    m_previousButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
    m_previousButton->setToolTip(tr("Previous frame"));
    m_previousButton->setMaximumWidth(32);
    
    m_slider = new QSlider(Qt::Horizontal, this);
    m_slider->setMinimum(0);
    m_slider->setMaximum(0);
    m_slider->setValue(0);
    m_slider->setTracking(true);
    m_slider->setToolTip(tr("Drag to navigate through video frames"));
    
    m_nextButton = new QPushButton(this);
    m_nextButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
    m_nextButton->setToolTip(tr("Next frame"));
    m_nextButton->setMaximumWidth(32);
    
    controlsLayout->addWidget(m_previousButton);
    controlsLayout->addWidget(m_slider);
    controlsLayout->addWidget(m_nextButton);
    
    mainLayout->addLayout(infoLayout);
    mainLayout->addLayout(controlsLayout);
    
    // Set fixed height for the timeline
    setFixedHeight(70);
    
    // Connect signals
    connect(m_slider, &QSlider::valueChanged, this, &VideoTimeline::onSliderValueChanged);
    connect(m_slider, &QSlider::sliderReleased, this, &VideoTimeline::onSliderReleased);
    connect(m_previousButton, &QPushButton::clicked, this, &VideoTimeline::onPreviousFrameClicked);
    connect(m_nextButton, &QPushButton::clicked, this, &VideoTimeline::onNextFrameClicked);
    
    // Initially hidden
    hide();
}

void VideoTimeline::setVideoInfo(int totalFrames, double fps) {
    m_totalFrames = totalFrames;
    m_fps = fps;
    
    m_slider->setMaximum(totalFrames - 1);
    m_slider->setValue(0);
    
    updateLabels();
}

void VideoTimeline::setCurrentFrame(int frameNumber) {
    if (frameNumber < 0 || frameNumber >= m_totalFrames) {
        return;
    }
    
    m_slider->blockSignals(true);
    m_slider->setValue(frameNumber);
    m_slider->blockSignals(false);
    
    updateLabels();
}

void VideoTimeline::reset() {
    m_totalFrames = 0;
    m_fps = 0.0;
    m_slider->setMaximum(0);
    m_slider->setValue(0);
    updateLabels();
    hide();
}

void VideoTimeline::setVisible(bool visible) {
    m_visible = visible;
    QWidget::setVisible(visible);
}

void VideoTimeline::onSliderValueChanged(int /*value*/) {
    // Only update labels while dragging, don't emit frameChanged yet
    updateLabels();
}

void VideoTimeline::onSliderReleased() {
    // Now that the user released the mouse, emit the frame change signal
    emit frameChanged(m_slider->value());
}

void VideoTimeline::onPreviousFrameClicked() {
    int currentValue = m_slider->value();
    if (currentValue > 0) {
        m_slider->setValue(currentValue - 1);
    }
}

void VideoTimeline::onNextFrameClicked() {
    int currentValue = m_slider->value();
    if (currentValue < m_totalFrames - 1) {
        m_slider->setValue(currentValue + 1);
    }
}

void VideoTimeline::updateLabels() {
    int currentFrame = m_slider->value();
    
    m_frameLabel->setText(QString("Frame: %1 / %2")
                          .arg(currentFrame + 1)
                          .arg(m_totalFrames));
    
    m_timeLabel->setText(QString("Time: %1").arg(formatTime(currentFrame)));
}

QString VideoTimeline::formatTime(int frameNumber) {
    if (m_fps <= 0) {
        return "00:00.000";
    }
    
    double timeSeconds = frameNumber / m_fps;
    int minutes = static_cast<int>(timeSeconds / 60.0);
    double seconds = timeSeconds - (minutes * 60.0);
    int wholeSecs = static_cast<int>(seconds);
    int millis = static_cast<int>((seconds - wholeSecs) * 1000.0);
    
    return QString("%1:%2.%3")
        .arg(minutes, 2, 10, QChar('0'))
        .arg(wholeSecs, 2, 10, QChar('0'))
        .arg(millis, 3, 10, QChar('0'));
}
