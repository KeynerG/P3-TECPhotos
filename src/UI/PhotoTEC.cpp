#include "PhotoTEC.h"
#include "./ui_PhotoTEC.h"

PhotoTEC::PhotoTEC(QWidget *parent) : QMainWindow(parent), ui(new Ui::PhotoTEC) {
    ui->setupUi(this);
}

PhotoTEC::~PhotoTEC() {
    delete ui;
}