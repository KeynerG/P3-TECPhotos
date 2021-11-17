#ifndef P3_TECPHOTOS_TECPHOTOS_H
#define P3_TECPHOTOS_TECPHOTOS_H

#include <QMainWindow>

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

private:
    Ui::TECPhotos *ui; /**< TECPhotos class interface. */
};

#endif // P3_TECPHOTOS_TECPHOTOS_H