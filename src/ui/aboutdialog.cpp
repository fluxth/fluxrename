#include "aboutdialog.h"
#include "ui_aboutdialog.h"

#include "../config.h"

#import <QDateTime>

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    QString versionStr = "v" APP_VERSION " build %1";
    ui->lbVersionInfo->setText(versionStr.arg(QDateTime::currentDateTimeUtc().toString("yyyyMMdd")));
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
