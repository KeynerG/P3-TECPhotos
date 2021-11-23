#ifndef P3_TECPHOTOS_TECPHOTOS_H
#define P3_TECPHOTOS_TECPHOTOS_H

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

    void displayMessage(const QString& type, const QString& message);

private slots:
    /**
     * @fn void on_accessButton_clicked();
     * @brief
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void on_accessButton_clicked();

private:
    Ui::TECPhotos *ui; /**< TECPhotos class interface. */
    QMessageBox *qMessageBox; /**< QMessageBox object reference. */
};

#endif // P3_TECPHOTOS_TECPHOTOS_H
