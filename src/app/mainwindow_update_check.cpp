#include "mainwindow.h"
#include "consts.h"
#include <QRandomGenerator>
#include <QNetworkReply>

/********************
 * UPDATE CHECK     *
 ********************/

void MainWindow::updateCheckSlot() {
    /* check for update */
    // generate random string to avoid HTTP caching
    QRandomGenerator r(QTime::currentTime().msec());
    QString randomString;
    for (int i = 0; i < 12; i++) {
        randomString.append(QChar('A' + (r.bounded(25))));
    }
    // get current version number
    networkAccessManager = new QNetworkAccessManager(this);
    bool status = connect(networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(networkRequestFinishedSlot(QNetworkReply*)));
    if (status) {
        updateCheck->show();
        updateCheck->setModal(true);
        auto request = QNetworkRequest(QUrl(UPDATE_URL + randomString));
        request.setTransferTimeout(HTTP_TIMEOUT);
        networkAccessManager->get(request);
    } else {
        updateCheck->setNetworkError();
    }
}

void MainWindow::networkRequestFinishedSlot(QNetworkReply* reply) const {
    /* called when the update check completed (either successfully or timeout) */
    const QString remoteVersion = reply->readAll();
    if (reply->error() == QNetworkReply::NoError) {
        if (remoteVersion != updateCheck->getCurrentVersion()) {
            QString remoteUrl = remoteVersion;
            updateCheck->setUpdateAvailable(DOWNLOAD_URL + remoteUrl.replace(".", "-"),
                                            remoteVersion);
        } else {
            updateCheck->setUpdateToDate();
        }
    } else {
        updateCheck->setNetworkError();
    }
}
