#pragma once
#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>

class FileManager final : public QObject {
    Q_OBJECT
public:
    /* methods */
    FileManager();
    bool getOpenFileName(QString* fileName);
    void setDirectory(const QString &directory);
    [[nodiscard]] bool isDefaultDirectory() const;
    void clearCurrentFileName();
    QString fileSave(bool saveAs, QString suggestedFileName);
private:
    /* attributes */
    bool defaultDirectory = true;
    QString fileIoLocation;
    QString currentFileName;
};

#endif // FILEMANAGER_H
