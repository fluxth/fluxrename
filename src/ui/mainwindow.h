#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "fwidgetbase.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnContinue_clicked();
    void on_btnBack_clicked();

    void updateValidity();

private:
    Ui::MainWindow* ui;

    FWidgetBase* m_pages[3];
    size_t m_currentPageIndex;

    bool switchWidget(size_t index);
};
#endif // MAINWINDOW_H

