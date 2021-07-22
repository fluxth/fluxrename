#include "aboutdialog.h"
#include "ui_aboutdialog.h"

#include "../config.h"

#import <QDateTime>

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    ui->lbVersionInfo->setText("v" APP_VERSION " build " APP_BUILD);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
