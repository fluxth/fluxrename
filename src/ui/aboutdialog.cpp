#include "aboutdialog.h"
#include "ui_aboutdialog.h"

#include "../config.h"

#include <QSysInfo>

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    QString versionStr = "v" APP_VERSION " (%1) build " APP_BUILD;
    ui->lbVersionInfo->setText(versionStr.arg(QSysInfo::buildCpuArchitecture()));
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
