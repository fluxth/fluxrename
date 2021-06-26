#ifndef FILTERWIDGET_H
#define FILTERWIDGET_H

#include "fwidgetbase.h"

namespace Ui {
class FilterWidget;
}

class FilterWidget : public FWidgetBase
{
    Q_OBJECT

public:
    explicit FilterWidget(QWidget* parent = nullptr);
    ~FilterWidget();

    const char* getTitle() const override {
        return "Filter Input Items";
    }

private:
    Ui::FilterWidget *ui;
};

#endif // FILTERWIDGET_H
