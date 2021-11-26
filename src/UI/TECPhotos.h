#ifndef P3_TECPHOTOS_TECPHOTOS_H
#define P3_TECPHOTOS_TECPHOTOS_H

#include "../Communication/DataManager.h"

#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QMovie>
#include <QPixmap>
#include <QTime>

QT_BEGIN_NAMESPACE
namespace Ui { class TECPhotos; }
QT_END_NAMESPACE

/**
 * @class TECPhotos
 * @brief
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
     * @brief
     * @param type
     * @param message
     */
    void displayMessage(const QString& type, const QString &message);

    /**
     * @fn std::string converterQStringToStdString(const QString &qString)
     * @brief
     * @param qString
     * @return string
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    std::string converterQStringToStdString(const QString &qString);

    /**
     * @fn QString converterStdStringToString(const std::string &String
     * @brief
     * @param String
     * @return
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    QString converterStdStringToQString(const std::string &String);

    /**
     * @fn void getUserAlbums()
     * @brief
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void getUserAlbums();

    /**
     * @fn void update()
     * @brief
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void update();

private slots:
    /**
     * @fn void on_accessButton_clicked();
     * @brief
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void on_accessButton_clicked();

    /**
     * @fn void on_signUpButton_clicked()
     * @brief
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void on_signUpButton_clicked();

    /**
     * @fn void on_createButton_clicked()
     * @brief
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void on_createButton_clicked();

    /**
     * @fn void on_loginButton_clicked()
     * @brief
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void on_loginButton_clicked();

    /**
     * @fn void on_logOutButton_clicked()
     * @brief
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void on_logOutButton_clicked();

    /**
     * @fn void on_optionsTab_currentChanged(int index)
     * @brief
     * @param index
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void on_optionsTab_currentChanged(int index);

    /**
     * @fn void on_openAlbumButton_clicked()
     * @brief
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void on_openAlbumButton_clicked();

    /**
     * @fn void on_browseButton_clicked()
     * @brief
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void on_browseButton_clicked();

    /**
     * @fn void on_uploadPhotoButton_clicked()
     * @brief
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void on_uploadPhotoButton_clicked();

    /**
     * @fn void on_deleteAlbumButton_clicked()
     * @brief
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void on_deleteAlbumButton_clicked();

    /**
     * @fn void on_saveButton_clicked()
     * @brief
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void on_saveButton_clicked();

    /**
     * @fn void on_galleryPButton_clicked()
     * @brief
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void on_galleryPButton_clicked();

    /**
     * @fn void on_propertiesButton_clicked()
     * @brief
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void on_propertiesButton_clicked();

    /**
     * @fn void on_deletePhotoButton_clicked()
     * @brief
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void on_deletePhotoButton_clicked();

    /**
     * @fn void on_previousButton_clicked()
     * @brief
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
     * @brief
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void on_galleryIButton_clicked();

    /**
     * @fn void on_editButton_clicked()
     * @brief
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void on_editButton_clicked();

    /**
     * @fn void on_okButton_clicked()
     * @brief
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void on_okButton_clicked();

private:
    Ui::TECPhotos *ui; /**< TECPhotos class interface. */
    QMessageBox *qMessageBox; /**< QMessageBox object reference. */
    QStringList albumsList; /**< List with the names of the albums of the current user. */
    QImage image; /**< */
};

#endif // P3_TECPHOTOS_TECPHOTOS_H
