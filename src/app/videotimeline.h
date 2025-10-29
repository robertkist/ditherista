#ifndef VIDEOTIMELINE_H
#define VIDEOTIMELINE_H

#include <QWidget>
#include <QSlider>
#include <QLabel>
#include <QPushButton>

/* Video timeline widget for navigating through video frames */

class VideoTimeline : public QWidget {
    Q_OBJECT

public:
    explicit VideoTimeline(QWidget *parent = nullptr);
    
    void setVideoInfo(int totalFrames, double fps);
    void setCurrentFrame(int frameNumber);
    void reset();
    bool isVisible() const { return m_visible; }
    void setVisible(bool visible);

signals:
    void frameChanged(int frameNumber);
    void playPauseClicked();

private slots:
    void onSliderValueChanged(int value);
    void onSliderReleased();
    void onPreviousFrameClicked();
    void onNextFrameClicked();

private:
    QSlider* m_slider;
    QLabel* m_frameLabel;
    QLabel* m_timeLabel;
    QPushButton* m_previousButton;
    QPushButton* m_nextButton;
    
    int m_totalFrames;
    double m_fps;
    bool m_visible;
    bool m_isDragging;
    
    void updateLabels();
    QString formatTime(int frameNumber);
};

#endif // VIDEOTIMELINE_H
