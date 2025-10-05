#include <QMessageBox>

int showErrorBox(QWidget* parent, const QString& message) {
    QMessageBox msgBox = QMessageBox(parent);
    msgBox.setText(message);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    return msgBox.exec();
}
