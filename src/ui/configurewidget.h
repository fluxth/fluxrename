#ifndef CONFIGUREWIDGET_H
#define CONFIGUREWIDGET_H

#include "fwidgetbase.h"

namespace Ui {
class ConfigureWidget;
}

class ConfigureWidget : public FWidgetBase
{
    Q_OBJECT

public:
    explicit ConfigureWidget(QWidget* parent = nullptr);
    ~ConfigureWidget();

    const char* getTitle() const override {
        return "Rename Configuration";
    }

private:
    Ui::ConfigureWidget *ui;
};

#endif // CONFIGUREWIDGET_H
