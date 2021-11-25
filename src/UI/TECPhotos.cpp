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
    ui->accessButton->setEnabled(false);
    if (!ui->usernameLLineEdit->text().isEmpty() && !ui->passwordLLineEdit->text().isEmpty()) {
        if (DataManager::getInstance()->login(converterQStringToStdString(ui->usernameLLineEdit->text()), converterQStringToStdString(ui->passwordLLineEdit->text()))) {
            // switch to Album Screen
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
    ui->accessButton->setEnabled(true);
}

void TECPhotos::on_signUpButton_clicked() {
    ui->usernameSULineEdit->clear();
    ui->passwordSULineEdit->clear();
    // switch to Sign Up Screen
    ui->ScreenView->setCurrentIndex(1);
}

// --- Sign Up Screen (ScreenView 1) ---

void TECPhotos::on_createButton_clicked() {
    ui->createButton->setEnabled(false);
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
    ui->createButton->setEnabled(true);
}

void TECPhotos::on_loginButton_clicked() {
    ui->usernameLLineEdit->clear();
    ui->passwordLLineEdit->clear();
    // switch to Login Screen
    ui->ScreenView->setCurrentIndex(0);
}

// --- Album Screen (ScreenView 2) ---

void TECPhotos::on_logOutButton_clicked() {
    std::cout << "DATABASE LOG - " << DataManager::getInstance()->getCurrentUsername() << " LOGGED OUT.\n" << std::endl;
    DataManager::getInstance()->setCurrentUsername("");
    DataManager::getInstance()->getCurrentUserMap().clear();
    ui->usernameLLineEdit->clear();
    ui->passwordLLineEdit->clear();
    // switch to Login Screen
    ui->ScreenView->setCurrentIndex(0);
}

void TECPhotos::on_editAlbumButton_clicked() {
    // edit Album tab option
    ui->optionsTab->setCurrentIndex(0);
    ui->editAlbumSelectorComboBox->clear();
    ui->editAlbumSelectorComboBox->clear();
    ui->photoPathLineEdit->clear();
    // delete Album tab option
    ui->albumSelectorComboBox->clear();
    ui->albumSelectorComboBox->clear();
    // setup current user albums
    getUserAlbums();
    ui->editAlbumSelectorComboBox->addItems(albumsList);
    ui->albumSelectorComboBox->addItems(albumsList);
    // switch to Album Options Screen
    ui->ScreenView->setCurrentIndex(3);
}

// --- Album Options Screen (ScreenView 3) ---

void TECPhotos::on_galleryAOButton_clicked() {
    getUserAlbums();
    // display current user albums
    // switch to Album Screen
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
    ui->uploadPhotoButton->setEnabled(false);
    if (!ui->editAlbumSelectorComboBox->currentText().isEmpty() && !ui->photoPathLineEdit->text().isEmpty()) {
        // clear screen widgets
        ui->photoLabel->clear();
        ui->photoNameNPLineEdit->clear();
        ui->albumNameNPLineEdit->clear();
        ui->photoDescriptionNPLineEdit->clear();
        ui->photoAuthorNPLineEdit->clear();
        ui->photoWidthNPLineEdit->clear();
        ui->photoHeightNPLineEdit->clear();
        ui->photoDateNPEdit->clear();
        // setup screen widgets
        QImage image(ui->photoPathLineEdit->text());
        ui->photoLabel->setPixmap(QPixmap::fromImage(image.scaled(ui->photoLabel->size(), Qt::AspectRatioMode::KeepAspectRatio, Qt::TransformationMode::SmoothTransformation)));
        ui->albumNameNPLineEdit->setText(ui->editAlbumSelectorComboBox->currentText());
        ui->photoSizeNPLineEdit->setText(converterStdStringToQString(std::to_string(image.sizeInBytes())) + " B");
        ui->photoWidthNPLineEdit->setText(converterStdStringToQString(std::to_string(image.width())));
        ui->photoHeightNPLineEdit->setText(converterStdStringToQString(std::to_string(image.height())));
        // switch to New Photo Screen
        ui->ScreenView->setCurrentIndex(4);
    } else {
        if (ui->editAlbumSelectorComboBox->currentText().isEmpty() && ui->photoPathLineEdit->text().isEmpty()) {
            displayMessage("Warning", "Please select the album and the photo you want to update.");
        } else if (ui->editAlbumSelectorComboBox->currentText().isEmpty()) {
            displayMessage("Warning", "Please select the album you want to edit.");
        } else if (ui->photoPathLineEdit->text().isEmpty()) {
            displayMessage("Warning", "Please select the photo you want to upload.");
        }
    }
    ui->uploadPhotoButton->setEnabled(true);
}

void TECPhotos::on_deleteAlbumButton_clicked() {
    ui->deleteAlbumButton->setEnabled(false);
    if (!ui->albumSelectorComboBox->currentText().isEmpty()) {
        if (DataManager::getInstance()->deleteAlbum(
                converterQStringToStdString(ui->albumSelectorComboBox->currentText()))) {
            displayMessage("Notification", QString(ui->albumSelectorComboBox->currentText() + " album successfully deleted."));
            ui->editAlbumSelectorComboBox->removeItem(ui->albumSelectorComboBox->currentIndex());
            ui->albumSelectorComboBox->removeItem(ui->albumSelectorComboBox->currentIndex());
        } else {
            displayMessage("Notification", QString(ui->albumSelectorComboBox->currentText() + " album deletion was not successful."));
        }
    } else {
        displayMessage("Warning", "Please select the album you want to remove.");
    }
    ui->deleteAlbumButton->setEnabled(true);
}

// --- New Photo Screen (ScreenView 4) ---

void TECPhotos::on_saveButton_clicked() {
    ui->loadingIconLabel->setPixmap(QPixmap(":/icon/metadataOn.png"));
    ui->saveButton->setEnabled(false);
    if (!ui->photoNameNPLineEdit->text().isEmpty()) {
        // switch to Loading Screen
        ui->ScreenView->setCurrentIndex(5);
        // timer 1 sec
        QTime dieTime = QTime::currentTime().addMSecs(1000);
        while (QTime::currentTime() < dieTime) { QCoreApplication::processEvents(QEventLoop::AllEvents, 100); }
        // loading animation
        std::thread animation([this]() {
            int degrees = 0;
            while(true) {
                if (degrees < 360) {
                        QPixmap icon(":/icon/metadataOn.png");
                        QPixmap rotatedIcon(icon.size());
                        rotatedIcon.fill(QColor::fromRgb(0, 0, 0, 0));

                        QPainter *painter = new QPainter(&rotatedIcon);

                        QSize size = icon.size();
                        painter->setRenderHint(QPainter::Antialiasing);
                        painter->setRenderHint(QPainter::SmoothPixmapTransform);
                        painter->translate(size.height() / 2, size.height() / 2);
                        painter->rotate(degrees);
                        painter->translate(-size.height() / 2, -size.height() / 2);
                        painter->drawPixmap(0, 0, icon);
                        painter->end();

                        ui->loadingIconLabel->setPixmap(rotatedIcon);
                        QTime dieTime = QTime::currentTime().addMSecs(10);
                        while (QTime::currentTime() < dieTime) { QCoreApplication::processEvents(QEventLoop::AllEvents, 100); }
                        degrees+=3;
                } else {
                    degrees = 0;
                }
            }
        });
        // run animation
        animation.detach();
        // image Processing
        if (DataManager::getInstance()->saveImage(QImage(ui->photoPathLineEdit->text()),
                                                              converterQStringToStdString(ui->photoNameNPLineEdit->text()),
                                                              converterQStringToStdString(ui->albumNameNPLineEdit->text()),
                                                              converterQStringToStdString(ui->photoDescriptionNPLineEdit->text()),
                                                              converterQStringToStdString(ui->photoAuthorNPLineEdit->text()),
                                                              converterQStringToStdString(ui->photoSizeNPLineEdit->text()),
                                                              converterQStringToStdString(ui->photoWidthNPLineEdit->text()),
                                                              converterQStringToStdString(ui->photoHeightNPLineEdit->text()),""))
        {
            // switch to Album Screen
            ui->ScreenView->setCurrentIndex(2);
        } else {
            displayMessage("Notification", "The image \"" + QString(ui->photoNameNPLineEdit->text() + "\" was not successfully upload."));
        }
    } else {
        displayMessage("Warning", "Please enter the photo name.");
    }
    ui->saveButton->setEnabled(true);
}
