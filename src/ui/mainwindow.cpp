#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "inputwidget.h"
#include "filterwidget.h"
#include "configurewidget.h"

#include "fwidgetbase.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_currentPageIndex(0)
{
    m_pages[0] = new InputWidget();
    m_pages[1] = new FilterWidget();
    m_pages[2] = new ConfigureWidget();

    ui->setupUi(this);

    connect(m_pages[0], &FWidgetBase::validityChanged, this, &MainWindow::updateValidity);

    switchWidget(m_currentPageIndex);
}

MainWindow::~MainWindow()
{
    delete ui;

    for (auto* page : m_pages) {
        delete page;
    }
}

bool MainWindow::switchWidget(size_t index) {
    size_t size = sizeof(m_pages) / sizeof(m_pages[0]);
    if (index >= size)
        return false;

    if (ui->rootLayout->replaceWidget(ui->widget, m_pages[index]) == nullptr)
        return false;

    ui->widget = m_pages[index];

    m_pages[m_currentPageIndex]->hide();
    m_pages[index]->show();

    m_currentPageIndex = index;
    ui->label->setText(QString("Step %1: %2").arg(m_currentPageIndex + 1).arg(m_pages[index]->getTitle()));

    if (index == 0)
        ui->btnBack->hide();
    else
        ui->btnBack->show();

    if (index + 1 == size)
        ui->btnContinue->setText("Preview Renames");
    else
        ui->btnContinue->setText("Continue");

    updateValidity();

    return true;
}

void MainWindow::on_btnContinue_clicked()
{
    switchWidget(m_currentPageIndex + 1);
}

void MainWindow::on_btnBack_clicked()
{
    switchWidget(m_currentPageIndex - 1);
}

void MainWindow::updateValidity()
{
    if (m_pages[m_currentPageIndex]->isValid())
        ui->btnContinue->setEnabled(true);
    else
        ui->btnContinue->setEnabled(false);
}
