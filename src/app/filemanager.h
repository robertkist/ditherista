#pragma once
#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>

class FileManager : public QObject {
    Q_OBJECT
public:
    FileManager();
    bool getOpenFileName(QString* fileName);
    QString fileSave(bool saveAs, QString suggestedFileName);
private:
    QString fileIoLocation = "";
    QString currentFileName = "";
};

#endif // FILEMANAGER_H
