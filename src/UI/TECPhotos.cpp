#include "TECPhotos.h"
#include "./ui_TECPhotos.h"

TECPhotos::TECPhotos(QWidget *parent) : QMainWindow(parent), ui(new Ui::TECPhotos) {
    ui->setupUi(this);
}

TECPhotos::~TECPhotos() {
    delete ui;
}

void TECPhotos::displayMessage(const QString &type, const QString &message) {
    qMessageBox = new QMessageBox(QMessageBox::NoIcon, type, message, QMessageBox::StandardButton::Ok, this);
    qMessageBox->setIconPixmap((QPixmap(QString::fromUtf8(":/icon/TECPhotos.png"))).scaled(50,50, Qt::AspectRatioMode::KeepAspectRatio,Qt::TransformationMode::SmoothTransformation));
    qMessageBox->exec();
}

// --- Log In Screen ---
void TECPhotos::on_accessButton_clicked() {
    if (!ui->usernameLLineEdit->text().isEmpty() && !ui->passwordLLineEdit->text().isEmpty()) {

    } else {
        if (ui->usernameLLineEdit->text().isEmpty() && ui->passwordLLineEdit->text().isEmpty()) {
            displayMessage("Warning", "Please enter your username and password.");
        } else if (ui->usernameLLineEdit->text().isEmpty()) {
            displayMessage("Warning", "Please enter your username.");
        } else if (ui->passwordLLineEdit->text().isEmpty()) {
            displayMessage("Warning", "Please enter your password.");
        }
    }
}