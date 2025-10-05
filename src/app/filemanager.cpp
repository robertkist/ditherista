#include "filemanager.h"
#include "consts.h"
#include <QFileDialog>
#include <QStandardPaths>

FileManager::FileManager() {
    /* Constructor */
    if(QStringList dir = QStandardPaths::standardLocations(QStandardPaths::HomeLocation); dir.count() > 0) {
        fileIoLocation = dir[0];
    } else {
        qDebug()<<"WARNING: could not determine default load/save location";
    }
}

void FileManager::setDirectory(const QString& directory) {
    fileIoLocation = directory;
}

bool FileManager::getOpenFileName(QString* fileName) {
    const QString filter = tr("Images") + " (" + FILE_FILTERS.join(" ") + ")";
    *fileName = QFileDialog::getOpenFileName((QWidget*)parent(),
                                                    tr("Open Image File"), fileIoLocation,
                                                    filter);
    if(fileName->isEmpty() || fileName->isNull() || !QFile::exists(*fileName)) {
        return false; // cancelled or file does not exist
    }
    fileIoLocation = QFileInfo(*fileName).absolutePath(); // remember file location
    defaultDirectory = false;
    return(true);
}

QString FileManager::fileSave(const bool saveAs, QString suggestedFileName) {
    /* Displays a file "Save-as" dialog when user chooses "Save As" and returns the file-name.
     * Displays a file "Save-as" dialog when the user chooses "Save" for the first time, then returns
     * the file name.
     * Returns an empty file-name if cancelled */
    if(!currentFileName.isEmpty() && !saveAs) {
        return currentFileName;
    }
    if (currentFileName.isEmpty()) {
        suggestedFileName += ".png";
        suggestedFileName = fileIoLocation + QDir::separator() + suggestedFileName;
    } else {
        suggestedFileName = currentFileName;
    }
    const QString filter = tr("Images") + " (" + FILE_FILTERS.join(" ") + ")";
    const QString fileName = QFileDialog::getSaveFileName((QWidget*)parent(),
                                                    tr("Save Image File"), suggestedFileName,
                                                    filter);
    if(!fileName.isEmpty()) {
        currentFileName = fileName;
        fileIoLocation = QFileInfo(fileName).absolutePath(); // remember file location
        defaultDirectory = false;
    }
    return fileName;
}

bool FileManager::isDefaultDirectory() const {
    return defaultDirectory;
}

void FileManager::clearCurrentFileName() {
    currentFileName = "";
}