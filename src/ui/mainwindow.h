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
    void updateValidity();

    void on_btnContinue_clicked();
    void on_btnBack_clicked();

private:
    Ui::MainWindow* ui;

    const array<unique_ptr<FWidgetBase>, 3> m_pages;
    size_t m_currentPageIndex;

    bool switchWidget(size_t index);
    bool calculateRename();
};
#endif // MAINWINDOW_H

