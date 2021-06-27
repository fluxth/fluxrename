#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "inputwidget.h"
#include "filterwidget.h"
#include "configurewidget.h"

#include "fwidgetbase.h"
#include"../frenamer.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pages({
        std::make_unique<InputWidget>(),
        std::make_unique<FilterWidget>(),
        std::make_unique<ConfigureWidget>()
    }),
    m_currentPageIndex(0)
{
//    m_pages[0] = std::make_unique<InputWidget>();
//    m_pages[1] = std::make_unique<FilterWidget>();
//    m_pages[2] = std::make_unique<ConfigureWidget>();

    ui->setupUi(this);

    switchWidget(m_currentPageIndex);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::switchWidget(size_t index) {
    size_t size = m_pages.size();
    if (index > size)
        return false;

    if (index == size) {
        // preview rename
        return calculateRename();
    }

    auto oldHeight = height();

    auto* prevWidget = m_pages[m_currentPageIndex].get();
    auto* nextWidget = m_pages[index].get();

    prevWidget->widgetWillDisappear();
    disconnect(prevWidget, &FWidgetBase::validityChanged, this, &MainWindow::updateValidity);
    prevWidget->hide();

    if (ui->rootLayout->replaceWidget(ui->widget, nextWidget) == nullptr)
        return false;

    ui->widget = nextWidget;
    connect(nextWidget, &FWidgetBase::validityChanged, this, &MainWindow::updateValidity);

    if (!nextWidget->hydrateScanner(prevWidget->takeScanner()))
        // TODO: Show dialog and quit
        qWarning("Scanner failed to hydrate!");

    nextWidget->widgetWillAppear();
    nextWidget->show();

    m_currentPageIndex = index;

    ui->label->setText(QString("Step %1: %2").arg(m_currentPageIndex + 1).arg(nextWidget->getTitle()));

    if (index == 0) {
        ui->btnBack->hide();
        if (height() > 270 && height() < 640)
            resize({ width(), 270 });
    } else {
        ui->btnBack->show();
    }
    if (index + 1 == size)
        ui->btnContinue->setText("Preview Renames");
    else
        ui->btnContinue->setText("Continue");

    auto deltaHeight = (height() - oldHeight) / 2;
    if (deltaHeight > 0 && y() - deltaHeight >= 20) {
        move({ x(), y() - deltaHeight });
    } else if (deltaHeight < 0) {
        move({ x(), y() - deltaHeight });
    }

    updateValidity();
    return true;
}

bool MainWindow::calculateRename()
{
    auto scanner = m_pages[m_currentPageIndex]->takeScanner();
    auto renamer = FRenamer(std::move(scanner));

    renamer.calculate(this);
    renamer.previewChanges(this);

    m_pages[m_currentPageIndex]->hydrateScanner(renamer.takeScanner());
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
