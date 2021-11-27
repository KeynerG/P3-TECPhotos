#ifndef P3_TECPHOTOS_TECPHOTOS_H
#define P3_TECPHOTOS_TECPHOTOS_H

#include "../Communication/DataManager.h"

#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QMovie>
#include <QPair>
#include <QPixmap>
#include <QTime>

QT_BEGIN_NAMESPACE
namespace Ui { class TECPhotos; }
QT_END_NAMESPACE

/**
 * @class TECPhotos
 * @brief Class to display the graphic user interface.
 * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
 */
class TECPhotos : public QMainWindow {
Q_OBJECT

public:
    TECPhotos(QWidget *parent = nullptr); /**< TECPhotos class constructor. */

    ~TECPhotos(); /**< TECPhotos class destructor. */

protected:
    /**
     * @fn void displayMessage(const QString& type, const QString &message)
     * @brief Function to display a graphic message.
     * @param type
     * @param message
     */
    void displayMessage(const QString &type, const QString &message);

    /**
     * @fn std::string converterQStringToStdString(const QString &qString)
     * @brief Function to convert a QString into an std::String.
     * @param qString
     * @return string
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    std::string converterQStringToStdString(const QString &qString);

    /**
     * @fn QString converterStdStringToString(const std::string &String
     * @brief Function to convert an std::String into a QString.
     * @param String
     * @return QString
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    QString converterStdStringToQString(const std::string &String);

    /**
     * @fn void getUserAlbums()
     * @brief Function to load all user albums from DataManager.
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void getUserAlbums();

    /**
     * @fn void update()
     * @brief Function to update graphic elements content.
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void update();

private slots:
    /**
     * @fn void on_accessButton_clicked();
     * @brief Function to access into a user's account.
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void on_accessButton_clicked();

    /**
     * @fn void on_signUpButton_clicked()
     * @brief Function to switch to signup screen.
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void on_signUpButton_clicked();

    /**
     * @fn void on_createButton_clicked()
     * @brief Function to create a new user's account.
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void on_createButton_clicked();

    /**
     * @fn void on_loginButton_clicked()
     * @brief Function to switch to login screen.
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void on_loginButton_clicked();

    /**
     * @fn void on_logOutButton_clicked()
     * @brief Function to log out from a user's account.
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void on_logOutButton_clicked();

    /**
     * @fn void on_optionsTab_currentChanged(int index)
     * @brief Function to update the album options section.
     * @param index
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void on_optionsTab_currentChanged(int index);

    /**
     * @fn void on_openAlbumButton_clicked()
     * @brief Function to open the album selected by the user.
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void on_openAlbumButton_clicked();

    /**
     * @fn void on_browseButton_clicked()
     * @brief Function to display the image selection interface.
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void on_browseButton_clicked();

    /**
     * @fn void on_uploadPhotoButton_clicked()
     * @brief Function to upload the photo and metadata specified by the user.
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void on_uploadPhotoButton_clicked();

    /**
     * @fn void on_deleteAlbumButton_clicked()
     * @brief Function to delete an album and all its images.
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void on_deleteAlbumButton_clicked();

    /**
     * @fn void on_saveButton_clicked()
     * @brief Function to save the new photo metadata.
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void on_saveButton_clicked();

    /**
     * @fn void on_galleryPButton_clicked()
     * @brief Function to switch to albums screen.
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void on_galleryPButton_clicked();

    /**
     * @fn void on_propertiesButton_clicked()
     * @brief Function to switch to image properties (metadata) screen.
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void on_propertiesButton_clicked();

    /**
     * @fn void on_deletePhotoButton_clicked()
     * @brief Function to delete the currently displayed photo.
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void on_deletePhotoButton_clicked();

    /**
     * @fn void on_previousButton_clicked()
     * @brief Function to switch to previous photo.
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void on_previousButton_clicked();

    /**
     * @fn void on_nextButton_clicked()
     * @brief
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void on_nextButton_clicked();

    /**
     * @fn void on_galleryIButton_clicked()
     * @brief Function to switch to next photo.
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void on_galleryIButton_clicked();

    /**
     * @fn void on_editButton_clicked()
     * @brief Function to enable the properties fields to update metadata.
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void on_editButton_clicked();

    /**
     * @fn void on_okButton_clicked()
     * @brief Function to update an image metadata.
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void on_okButton_clicked();

private:
    Ui::TECPhotos *ui; /**< TECPhotos class interface. */
    QMessageBox *qMessageBox; /**< QMessageBox object reference. */
    QStringList albumsList; /**< List with the names of the albums of the current user. */
    QPair<QImage, bool> imageObj; /**< QPair<QImage, bool> with QImage as image to display and bool as existance of the files on disk. */
    QImage image; /**< Image displayed. */
};

#endif // P3_TECPHOTOS_TECPHOTOS_H