#include "configurewidget.h"
#include "ui_configurewidget.h"

ConfigureWidget::ConfigureWidget(QWidget* parent) :
    FWidgetBase(parent),
    ui(new Ui::ConfigureWidget)
{
    ui->setupUi(this);
}

ConfigureWidget::~ConfigureWidget()
{
    delete ui;
}
