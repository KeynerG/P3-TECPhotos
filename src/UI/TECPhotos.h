#ifndef P3_TECPHOTOS_TECPHOTOS_H
#define P3_TECPHOTOS_TECPHOTOS_H

#include "../Communication/DataManager.h"

#include <QMainWindow>
#include <QMessageBox>

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
    QString converterStdStringToString(const std::string &String);

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

private:
    Ui::TECPhotos *ui; /**< TECPhotos class interface. */
    QMessageBox *qMessageBox; /**< QMessageBox object reference. */
};

#endif // P3_TECPHOTOS_TECPHOTOS_H
