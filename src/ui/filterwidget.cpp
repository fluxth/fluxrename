#include "filterwidget.h"
#include "ui_filterwidget.h"

FilterWidget::FilterWidget(QWidget* parent) :
    FWidgetBase(parent),
    ui(new Ui::FilterWidget)
{
    ui->setupUi(this);
}

FilterWidget::~FilterWidget()
{
    delete ui;
}
