#ifndef CONFIGUREWIDGET_H
#define CONFIGUREWIDGET_H

#include "fwidgetbase.h"

#include <QGroupBox>

namespace Ui {
class ConfigureWidget;
}

class ConfigureWidget : public FWidgetBase
{
    Q_OBJECT

public:
    explicit ConfigureWidget(QWidget* parent = nullptr);
    ~ConfigureWidget();

    bool isValid() const override;
    const char* getTitle() const override {
        return "Rename Configuration";
    }

    bool hydrateScanner(unique_ptr<FScanner> scanner) override;

private slots:
    void on_addGroup_clicked();

    void on_replaceGroup_clicked();

    void on_trimGroup_clicked();

    void on_numberingGroup_clicked();

private:
    Ui::ConfigureWidget *ui;

    QGroupBox* m_groups[4];
};

#endif // CONFIGUREWIDGET_H
