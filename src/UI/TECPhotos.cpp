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
    qMessageBox->setIconPixmap((QPixmap(QString::fromUtf8(":/icon/TECPhotos.png"))).scaled(50, 50, Qt::AspectRatioMode::KeepAspectRatio, Qt::TransformationMode::SmoothTransformation));
    qMessageBox->exec();
}

std::string TECPhotos::converterQStringToStdString(const QString &qString) {
    std::string string = qString.toStdString();
    return string;
}

QString TECPhotos::converterStdStringToQString(const std::string &String) {
    QString qString = QString::fromStdString(String);
    return qString;
}

void TECPhotos::getUserAlbums() {
    albumsList.clear();
    QMap <std::string, QVector<std::string>> albums = DataManager::getInstance()->getCurrentUserMap();
    QMapIterator <std::string, QVector<std::string>> albumsIterator(albums);
    while (albumsIterator.hasNext()) {
        albumsIterator.next();
        albumsList.append(converterStdStringToQString(albumsIterator.key()));
    }
}

// --- Log In Screen (ScreenView 0) ---

void TECPhotos::on_accessButton_clicked() {
    if (!ui->usernameLLineEdit->text().isEmpty() && !ui->passwordLLineEdit->text().isEmpty()) {
        if (DataManager::getInstance()->login(converterQStringToStdString(ui->usernameLLineEdit->text()), converterQStringToStdString(ui->passwordLLineEdit->text()))) {
            // Switch to Album Screen
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
    ui->usernameSULineEdit->clear();
    ui->passwordSULineEdit->clear();
    // Switch to Sign Up Screen
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
    ui->usernameLLineEdit->clear();
    ui->passwordLLineEdit->clear();
    // Switch to Login Screen
    ui->ScreenView->setCurrentIndex(0);
}

// --- Album Screen (ScreenView 2) ---

void TECPhotos::on_logOutButton_clicked() {
    std::cout << "DATABASE LOG - " << DataManager::getInstance()->getCurrentUsername() << " LOGGED OUT.\n" << std::endl;
    DataManager::getInstance()->setCurrentUsername("");
    DataManager::getInstance()->getCurrentUserMap().clear();
    ui->usernameLLineEdit->clear();
    ui->passwordLLineEdit->clear();
    // Switch to Login Screen
    ui->ScreenView->setCurrentIndex(0);
}

void TECPhotos::on_editAlbumButton_clicked() {
    // Edit Album tab option
    ui->editAlbumSelectorComboBox->clear();
    ui->editAlbumSelectorComboBox->clear();
    ui->photoPathLineEdit->clear();
    // Delete Album tab option
    ui->albumSelectorComboBox->clear();
    ui->albumSelectorComboBox->clear();
    // Setup current user albums
    getUserAlbums();
    ui->editAlbumSelectorComboBox->addItems(albumsList);
    ui->albumSelectorComboBox->addItems(albumsList);
    // Switch to Album Options Screen
    ui->ScreenView->setCurrentIndex(3);
}

// --- Album Options Screen (ScreenView 3) ---

void TECPhotos::on_galleryAOButton_clicked() {
    getUserAlbums();
    // Display current user albums
    // Switch to Album Screen
    ui->ScreenView->setCurrentIndex(2);
}

void TECPhotos::on_browseButton_clicked() {
    ui->photoPathLineEdit->clear();
    ui->photoLabel->clear();
    QString imagePath = "";
    bool validImage = false;
    imagePath = QFileDialog::getOpenFileName(this, "TECPhotos - Select your image", "",
                                             "PNG (*.png);;JPG (*.jpg);;JPEG (*.jpeg)");
    if (QString::compare(imagePath, QString()) != 0) {
        QImage imageSelected;
        validImage = imageSelected.load(imagePath);
        if (validImage) {
            ui->photoPathLineEdit->setText(imagePath);
        }
    } else {
        displayMessage("Alert", "The selected file is not a valid image file.");
    }
}

void TECPhotos::on_uploadPhotoButton_clicked() {
    if (!ui->editAlbumSelectorComboBox->currentText().isEmpty() && !ui->photoPathLineEdit->text().isEmpty()) {
        QImage image(ui->photoPathLineEdit->text());
        ui->photoLabel->setPixmap(QPixmap::fromImage(
                image.scaled(ui->photoLabel->size(), Qt::AspectRatioMode::KeepAspectRatio,
                             Qt::TransformationMode::SmoothTransformation)));
        ui->albumNameNPLineEdit->setText(ui->editAlbumSelectorComboBox->currentText());
        ui->photoSizeNPLineEdit->setText(
                converterStdStringToQString(std::to_string(image.sizeInBytes() / 1000)) + " kB");
        ui->photoWidthNPLineEdit->setText(converterStdStringToQString(std::to_string(image.width())));
        ui->photoHeightNPLineEdit->setText(converterStdStringToQString(std::to_string(image.height())));
        // Switch to New Photo Screen
        ui->ScreenView->setCurrentIndex(5);
    } else {
        if (ui->editAlbumSelectorComboBox->currentText().isEmpty() && ui->photoPathLineEdit->text().isEmpty()) {
            displayMessage("Warning", "Please select the album and the photo you want to update.");
        } else if (ui->editAlbumSelectorComboBox->currentText().isEmpty()) {
            displayMessage("Warning", "Please select the album you want to edit.");
        } else if (ui->photoPathLineEdit->text().isEmpty()) {
            displayMessage("Warning", "Please select the photo you want to upload.");
        }
    }
}