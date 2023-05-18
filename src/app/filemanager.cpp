#include "filemanager.h"
#include <QFileDialog>
#include <QDebug>
#include <QStandardPaths>

FileManager::FileManager() {
    QStringList dir = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
    if(dir.count() > 0)
        fileIoLocation = dir[0];
    else
        qDebug()<<"WARNING: could not determine default load/save location";
}

bool FileManager::getOpenFileName(QString* fileName) {
    *fileName = QFileDialog::getOpenFileName((QWidget*)parent(),
                                                    tr("Open Image File"), fileIoLocation,
                                                    tr("Images (*.png *.jpg *.jpeg *.bmp *.gif)"));
    if(*fileName == "" || !QFile::exists(*fileName))
        return(false);
    fileIoLocation = QFileInfo(*fileName).absolutePath(); // remember file location
    return(true);
}

QString FileManager::fileSave(bool saveAs, QString suggestedFileName) {
    if(currentFileName != "" && !saveAs)
        return currentFileName;
    else {
        if (currentFileName == "") {
            suggestedFileName += ".png";
            suggestedFileName = fileIoLocation + QDir::separator() + suggestedFileName;
        } else
            suggestedFileName = currentFileName;
        QString fileName = QFileDialog::getSaveFileName((QWidget*)parent(),
                                                        tr("Save Image File"), suggestedFileName,
                                                        tr("Images (*.png *.jpg *.jpeg *.bmp)"));
        if(fileName != "") {
            currentFileName = fileName;
            fileIoLocation = QFileInfo(fileName).absolutePath(); // remember file location
        }
        return fileName;
    }
}
