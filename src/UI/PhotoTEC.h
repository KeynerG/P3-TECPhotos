#ifndef P3_TECPHOTOS_PHOTOTEC_H
#define P3_TECPHOTOS_PHOTOTEC_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class PhotoTEC; }
QT_END_NAMESPACE

/**
 * @class PhotoTEC
 * @brief
 * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
 */
class PhotoTEC : public QMainWindow {
Q_OBJECT

public:
    PhotoTEC(QWidget *parent = nullptr);

    ~PhotoTEC();

private:
    Ui::PhotoTEC *ui;
};

#endif // P3_TECPHOTOS_PHOTOTEC_H