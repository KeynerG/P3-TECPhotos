#include "TECPhotos.h"
#include "./ui_TECPhotos.h"

TECPhotos::TECPhotos(QWidget *parent) : QMainWindow(parent), ui(new Ui::TECPhotos) {
    ui->setupUi(this);
    // setup widgets screen
    ui->optionsTab->setCurrentIndex(0);
    // display Login Screen
    ui->ScreenView->setCurrentIndex(0);
    // loading animation
    QMovie *loadingIcon = new QMovie(":/icon/loading.gif");
    ui->loadingIconLabel->setMovie(loadingIcon);
    ui->decompressingIconLabel->setVisible(true);
    ui->decompressingIconLabel->setMovie(loadingIcon);
    loadingIcon->start();
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

void TECPhotos::update() {
    // open album tab option
    ui->openAlbumSelectorComboBox->clear();
    // edit album tab option
    ui->editAlbumSelectorComboBox->setCurrentText("");
    ui->editAlbumSelectorComboBox->clear();
    ui->photoPathLineEdit->clear();
    // delete album tab option
    ui->deleteAlbumSelectorComboBox->clear();
    // setup current user albums
    getUserAlbums();
    ui->openAlbumSelectorComboBox->addItems(albumsList);
    ui->editAlbumSelectorComboBox->addItems(albumsList);
    ui->deleteAlbumSelectorComboBox->addItems(albumsList);
}

// --- Log In Screen (ScreenView 0) ---

void TECPhotos::on_accessButton_clicked() {
    ui->accessButton->setEnabled(false);
    if (!ui->usernameLLineEdit->text().isEmpty() && !ui->passwordLLineEdit->text().isEmpty()) {
        if (DataManager::getInstance()->login(converterQStringToStdString(ui->usernameLLineEdit->text()), converterQStringToStdString(ui->passwordLLineEdit->text()))) {
            // retrieve user information from database
            update();
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
            // retrieve user information from database
            update();
            // switch to Album Screen
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
    ui->optionsTab->setCurrentIndex(0);
    ui->usernameLLineEdit->clear();
    ui->passwordLLineEdit->clear();
    // switch to Login Screen
    ui->ScreenView->setCurrentIndex(0);
}

void TECPhotos::on_optionsTab_currentChanged(int index) {
    update();
}

void TECPhotos::on_openAlbumButton_clicked() {
    ui->decompressingIconLabel->setVisible(true);
    ui->openAlbumButton->setEnabled(false);
    if (!ui->openAlbumSelectorComboBox->currentText().isEmpty()) {
        // retrieve album information from database
        DataManager::getInstance()->openAlbum(converterQStringToStdString(ui->openAlbumSelectorComboBox->currentText()));
        // setup screen widgets
        imageObj = DataManager::getInstance()->loadImage(0);
        image = imageObj.first;
        ui->photoNameLabel->setText(converterStdStringToQString(DataManager::getInstance()->getCurrentImageName()));
        ui->albumNameLabel->setText(converterStdStringToQString(DataManager::getInstance()->getCurrentAlbumName()));
        if (!imageObj.first.isNull()) {
            ui->displayPhotoLabel->setPixmap(QPixmap(QPixmap::fromImage(image).scaled(ui->displayPhotoLabel->size(), Qt::AspectRatioMode::KeepAspectRatio, Qt::TransformationMode::SmoothTransformation)));
            if (imageObj.second) {
                ui->decompressingIconLabel->setVisible(false);
            }
        }
        // disables next and previous buttons when there's only one image on the album
        if (DataManager::getInstance()->getCurrentUserMap().value(converterQStringToStdString(ui->albumNameLabel->text())).size() == 1) {
            ui->nextButton->setEnabled(false);
            ui->previousButton->setEnabled(false);
        } else {
            ui->nextButton->setEnabled(true);
            ui->previousButton->setEnabled(true);
        }
        // switch to Photo Screen
        ui->ScreenView->setCurrentIndex(5);
    } else {
        displayMessage("Warning", "Please select the album you want to open.");
    }
    ui->openAlbumButton->setEnabled(true);
}

void TECPhotos::on_browseButton_clicked() {
    ui->photoPathLineEdit->clear();
    ui->photoLabel->clear();
    QString imagePath = "";
    bool validImage = false;
    imagePath = QFileDialog::getOpenFileName(this, "TECPhotos - Select your image", "", "PNG (*.png);;JPG (*.jpg);;JPEG (*.jpeg)");
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
        ui->photoDateNPEdit->setDate(QDate::fromString("01/01/2022", "MM/dd/yyyy"));
        // setup screen widgets
        QImage image(ui->photoPathLineEdit->text());
        ui->photoLabel->setPixmap(QPixmap::fromImage(image.scaled(ui->photoLabel->size(), Qt::AspectRatioMode::KeepAspectRatio, Qt::TransformationMode::SmoothTransformation)));
        ui->albumNameNPLineEdit->setText(ui->editAlbumSelectorComboBox->currentText());
        ui->photoSizeNPLineEdit->setText(converterStdStringToQString(std::to_string(image.sizeInBytes())) + " B");
        ui->photoWidthNPLineEdit->setText(converterStdStringToQString(std::to_string(image.width())));
        ui->photoHeightNPLineEdit->setText(converterStdStringToQString(std::to_string(image.height())));
        // switch to New Photo Screen
        ui->ScreenView->setCurrentIndex(3);
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
    if (!ui->deleteAlbumSelectorComboBox->currentText().isEmpty()) {
        if (DataManager::getInstance()->deleteAlbum(converterQStringToStdString(ui->deleteAlbumSelectorComboBox->currentText()))) {
            displayMessage("Notification", QString(ui->deleteAlbumSelectorComboBox->currentText() + " album successfully deleted."));
            ui->openAlbumSelectorComboBox->removeItem(ui->deleteAlbumSelectorComboBox->currentIndex());
            ui->editAlbumSelectorComboBox->removeItem(ui->deleteAlbumSelectorComboBox->currentIndex());
            ui->deleteAlbumSelectorComboBox->removeItem(ui->deleteAlbumSelectorComboBox->currentIndex());
        } else {
            displayMessage("Notification", QString(ui->deleteAlbumSelectorComboBox->currentText() + " album deletion was not successful."));
        }
    } else {
        displayMessage("Warning", "Please select the album you want to remove.");
    }
    ui->deleteAlbumButton->setEnabled(true);
}

// --- New Photo Screen (ScreenView 3) ---

void TECPhotos::on_saveButton_clicked() {
    ui->saveButton->setEnabled(false);
    if (!ui->photoNameNPLineEdit->text().isEmpty()) {
        // switch to Loading Screen
        ui->ScreenView->setCurrentIndex(4);
        // timer 1 sec
        QTime dieTime = QTime::currentTime().addMSecs(1000);
        while (QTime::currentTime() < dieTime) { QCoreApplication::processEvents(QEventLoop::AllEvents, 100); }
        // image Processing
        if (DataManager::getInstance()->saveImage(QImage(ui->photoPathLineEdit->text()),
                                                  converterQStringToStdString(ui->photoNameNPLineEdit->text()),
                                                  converterQStringToStdString(ui->albumNameNPLineEdit->text()),
                                                  converterQStringToStdString(ui->photoDescriptionNPLineEdit->text()),
                                                  converterQStringToStdString(ui->photoAuthorNPLineEdit->text()),
                                                  converterQStringToStdString(ui->photoSizeNPLineEdit->text()),
                                                  converterQStringToStdString(ui->photoWidthNPLineEdit->text()),
                                                  converterQStringToStdString(ui->photoHeightNPLineEdit->text()),
                                                  converterQStringToStdString(ui->photoDateNPEdit->date().toString("MM/dd/yyyy"))))
        {
            // switch to Album Screen
            update();
            ui->optionsTab->setCurrentIndex(0);
            ui->ScreenView->setCurrentIndex(2);
        } else {
            displayMessage("Notification", "The image \"" + QString(ui->photoNameNPLineEdit->text() + "\" was not successfully upload."));
        }
    } else {
        displayMessage("Warning", "Please enter the photo name.");
    }
    ui->saveButton->setEnabled(true);
}

// --- Photo Screen (ScreenView 5) ---

void TECPhotos::on_galleryPButton_clicked() {
    ui->albumNameLabel->clear();
    ui->photoNameLabel->clear();
    update();
    // switch to Album Screen
    ui->optionsTab->setCurrentIndex(0);
    ui->ScreenView->setCurrentIndex(2);
}

void TECPhotos::on_propertiesButton_clicked() {
    // clear screen widgets
    ui->photoNameILineEdit->clear();
    ui->albumNameILineEdit->clear();
    ui->photoDescriptionILineEdit->clear();
    ui->photoAuthorILineEdit->clear();
    ui->photoSizeILineEdit->clear();
    ui->photoWidthILineEdit->clear();
    ui->photoHeightILineEdit->clear();
    ui->photoDateIEdit->clear();
    // setup screen widgets
    ui->photoNameILineEdit->setText(converterStdStringToQString(DataManager::getInstance()->getCurrentImageName()));
    ui->albumNameILineEdit->setText(converterStdStringToQString(DataManager::getInstance()->getCurrentAlbumName()));
    ui->photoDescriptionILineEdit->setText(converterStdStringToQString(DataManager::getInstance()->getCurrentImageDescription()));
    ui->photoAuthorILineEdit->setText(converterStdStringToQString(DataManager::getInstance()->getCurrentImageAuthor()));
    ui->photoSizeILineEdit->setText(converterStdStringToQString(DataManager::getInstance()->getCurrentImageSize()));
    ui->photoWidthILineEdit->setText(converterStdStringToQString(DataManager::getInstance()->getCurrentImageWidthX()));
    ui->photoHeightILineEdit->setText(converterStdStringToQString(DataManager::getInstance()->getCurrentImageHeightY()));
    ui->photoDateIEdit->setDate(QDate::fromString(converterStdStringToQString(DataManager::getInstance()->getCurrentImageCreationDate()), "MM/dd/yyyy"));
    // switch to Photo Information Screen
    ui->ScreenView->setCurrentIndex(6);
}

void TECPhotos::on_deletePhotoButton_clicked() {
    ui->displayPhotoLabel->clear();
    DataManager::getInstance()->deleteImageMetadata();
    if (DataManager::getInstance()->getCurrentUserMap().value(converterQStringToStdString(ui->albumNameLabel->text())).size() == 0) {
        update();
        // switch to Albums Screen
        ui->ScreenView->setCurrentIndex(2);
    } else if (DataManager::getInstance()->getCurrentUserMap().value(converterQStringToStdString(ui->albumNameLabel->text())).size() == 1) {
        on_nextButton_clicked();
        ui->nextButton->setEnabled(false);
        ui->previousButton->setEnabled(false);
    } else {
        on_nextButton_clicked();
        ui->nextButton->setEnabled(true);
        ui->previousButton->setEnabled(true);
    }
}

void TECPhotos::on_previousButton_clicked() {
    ui->decompressingIconLabel->setVisible(true);
    ui->previousButton->setEnabled(false);
    ui->nextButton->setEnabled(false);
    ui->displayPhotoLabel->clear();
    imageObj = DataManager::getInstance()->loadImage(-1);
    image = imageObj.first;
    if (!imageObj.first.isNull()) {
        ui->displayPhotoLabel->setPixmap(QPixmap(QPixmap::fromImage(image).scaled(ui->displayPhotoLabel->size(), Qt::AspectRatioMode::KeepAspectRatio, Qt::TransformationMode::SmoothTransformation)));
        if (imageObj.second) {
            ui->decompressingIconLabel->setVisible(false);
        }
    }
    ui->photoNameLabel->setText(converterStdStringToQString(DataManager::getInstance()->getCurrentImageName()));
    ui->previousButton->setEnabled(true);
    ui->nextButton->setEnabled(true);
}

void TECPhotos::on_nextButton_clicked() {
    ui->decompressingIconLabel->setVisible(true);
    ui->previousButton->setEnabled(false);
    ui->nextButton->setEnabled(false);
    ui->displayPhotoLabel->clear();
    imageObj = DataManager::getInstance()->loadImage(1);
    image = imageObj.first;
    if (!imageObj.first.isNull()) {
        ui->displayPhotoLabel->setPixmap(QPixmap(QPixmap::fromImage(image).scaled(ui->displayPhotoLabel->size(), Qt::AspectRatioMode::KeepAspectRatio, Qt::TransformationMode::SmoothTransformation)));
        if (imageObj.second) {
            ui->decompressingIconLabel->setVisible(false);
        }
    }
    ui->photoNameLabel->setText(converterStdStringToQString(DataManager::getInstance()->getCurrentImageName()));
    ui->previousButton->setEnabled(true);
    ui->nextButton->setEnabled(true);
}

// --- Photo Information Screen (ScreenView 6) ---

void TECPhotos::on_galleryIButton_clicked() {
    // setup screen widgets
    ui->photoNameLabel->setText(ui->photoNameILineEdit->text());
    ui->albumNameLabel->setText(ui->albumNameILineEdit->text());
    // switch to Photo Screen
    if (ui->editButton->isEnabled() && !ui->okButton->isEnabled()) {
        ui->ScreenView->setCurrentIndex(5);
    } else {
        displayMessage("Warning", "Please press OK before continue.");
    }
}

void TECPhotos::on_editButton_clicked() {
    // disable edit button
    ui->editButton->setEnabled(false);
    // enable name, description, author, date to update
    ui->photoNameILineEdit->setReadOnly(false);
    ui->photoDescriptionILineEdit->setReadOnly(false);
    ui->photoAuthorILineEdit->setReadOnly(false);
    ui->photoDateIEdit->setEnabled(true);
    // enable ok button
    ui->okButton->setEnabled(true);
}

void TECPhotos::on_okButton_clicked() {
    // disable ok button
    ui->okButton->setEnabled(false);
    // update image information
    if (!ui->photoNameILineEdit->text().isEmpty()) {
        if (DataManager::getInstance()->updateImageMetadata(converterQStringToStdString(ui->photoNameILineEdit->text()),
                                                            converterQStringToStdString(ui->photoDescriptionILineEdit->text()),
                                                            converterQStringToStdString(ui->photoAuthorILineEdit->text()),
                                                            converterQStringToStdString(ui->photoDateIEdit->date().toString("MM/dd/yyyy")))) {
            // disable name, description, author, date
            ui->photoNameILineEdit->setReadOnly(true);
            ui->photoDescriptionILineEdit->setReadOnly(true);
            ui->photoAuthorILineEdit->setReadOnly(true);
            ui->photoDateIEdit->setEnabled(false);
            // enable edit button
            ui->editButton->setEnabled(true);
        } else {
            displayMessage("Notification", "The image \"" + QString(ui->photoNameNPLineEdit->text() + "\" was not successfully update."));
        }
    } else {
        displayMessage("Warning", "Please enter the photo name.");
    }
}