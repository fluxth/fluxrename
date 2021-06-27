#ifndef INPUTWIDGET_H
#define INPUTWIDGET_H

#include "fwidgetbase.h"

#import <QScopedPointer>

namespace Ui {
class InputWidget;
}

class InputWidget : public FWidgetBase
{
    Q_OBJECT

public:
    explicit InputWidget(QWidget* parent = nullptr);
    ~InputWidget();

    const char* getTitle() const override {
        return "Select Source Directory";
    }

    bool isValid() const override;
    QString getPath() const;

    bool hydrateScanner(unique_ptr<FScanner> scanner) override;

private slots:
    void updateCount(size_t count);
    void scanFinished();
    void on_btnBrowse_clicked();
    void on_btnClear_clicked();
    void on_btnEdit_clicked();
    void on_checkboxRecursive_stateChanged(int state);

private:
    Ui::InputWidget* ui;
    array<QWidget*, 5> m_formElements;

    void setPath(const QString& path);
    void clearPath();
    QString parsePath(const QString& path) const;

    bool isRecursive() const;

    void showInfo();
    void showInfo(const QString& msg);
    void showError(const QString& msg);
    void clearError();

    void disableForms();
    void enableForms();
};

#endif // INPUTWIDGET_H
