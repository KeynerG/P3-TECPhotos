#include "TECPhotos.h"
#include "./ui_TECPhotos.h"

TECPhotos::TECPhotos(QWidget *parent) : QMainWindow(parent), ui(new Ui::TECPhotos) {
    ui->setupUi(this);
}

TECPhotos::~TECPhotos() {
    delete ui;
}