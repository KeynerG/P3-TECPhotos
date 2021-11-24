#include "TECPhotos.h"
#include "./ui_TECPhotos.h"

TECPhotos::TECPhotos(QWidget *parent) : QMainWindow(parent), ui(new Ui::TECPhotos) {
    ui->setupUi(this);
    ui->ScreenView->setCurrentIndex(0);
}

TECPhotos::~TECPhotos() {
    delete ui;
}

void TECPhotos::displayMessage(const QString &type, const QString &message) {
    qMessageBox = new QMessageBox(QMessageBox::NoIcon, type, message, QMessageBox::StandardButton::Ok, this);
    qMessageBox->setIconPixmap((QPixmap(QString::fromUtf8(":/icon/TECPhotos.png"))).scaled(50,50, Qt::AspectRatioMode::KeepAspectRatio,Qt::TransformationMode::SmoothTransformation));
    qMessageBox->exec();
}

std::string TECPhotos::converterQStringToStdString(const QString& qString) {
    std::string string = qString.toStdString();
    return string;
}

QString TECPhotos::converterStdStringToString(const std::string &String) {
    QString qString = QString::fromStdString(String);
    return qString;
}

// --- Log In Screen (ScreenView 0) ---
void TECPhotos::on_accessButton_clicked() {
    if (!ui->usernameLLineEdit->text().isEmpty() && !ui->passwordLLineEdit->text().isEmpty()) {
        if (DataManager::getInstance()->login(converterQStringToStdString(ui->usernameLLineEdit->text()), converterQStringToStdString(ui->passwordLLineEdit->text()))) {
            ui->ScreenView->setCurrentIndex(2);
        } else {
            displayMessage("Warning", "Invalid username or password.");
        }
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

void TECPhotos::on_signUpButton_clicked() {
    ui->usernameSULineEdit->setText("");
    ui->passwordSULineEdit->setText("");
    ui->ScreenView->setCurrentIndex(1);
}

// --- Sign Up Screen (ScreenView 1) ---
void TECPhotos::on_createButton_clicked() {
    if (!ui->usernameSULineEdit->text().isEmpty() && !ui->passwordSULineEdit->text().isEmpty()) {
        if (DataManager::getInstance()->signUp(converterQStringToStdString(ui->usernameSULineEdit->text()), converterQStringToStdString(ui->passwordSULineEdit->text()))) {
            ui->ScreenView->setCurrentIndex(2);
        } else {
            displayMessage("Warning", "This username is not available.");
        }
    } else {
        if (ui->usernameSULineEdit->text().isEmpty() && ui->passwordSULineEdit->text().isEmpty()) {
            displayMessage("Warning", "Please enter your username and password.");
        } else if (ui->usernameSULineEdit->text().isEmpty()) {
            displayMessage("Warning", "Please enter your username.");
        } else if (ui->passwordSULineEdit->text().isEmpty()) {
            displayMessage("Warning", "Please enter your password.");
        }
    }
}

void TECPhotos::on_loginButton_clicked() {
    ui->usernameLLineEdit->setText("");
    ui->passwordLLineEdit->setText("");
    ui->ScreenView->setCurrentIndex(0);
}

// --- Album Screen (ScreenView 2) ---

void TECPhotos::on_logOutButton_clicked() {

}