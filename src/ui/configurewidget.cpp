#include "configurewidget.h"
#include "ui_configurewidget.h"

ConfigureWidget::ConfigureWidget(QWidget* parent) :
    FWidgetBase(parent),
    ui(new Ui::ConfigureWidget)
{
    ui->setupUi(this);

    m_groups[0] = ui->addGroup;
    m_groups[1] = ui->replaceGroup;
    m_groups[2] = ui->trimGroup;
    m_groups[3] = ui->numberingGroup;
}

ConfigureWidget::~ConfigureWidget()
{
    delete ui;
}

bool ConfigureWidget::isValid() const
{
    bool isEnabled = false;
    for (const auto* group : m_groups)
        isEnabled = isEnabled || group->isChecked();

    if (!isEnabled)
        return false;

    // further checks
    return true;
}

bool ConfigureWidget::hydrateScanner(unique_ptr<FScanner> scanner)
{
    if (!FWidgetBase::hydrateScanner(std::move(scanner)))
        return false;

    if (m_scanner) {

        return true;
    }

    return false;
}

void ConfigureWidget::on_addGroup_clicked()
{
    emit validityChanged();
}

void ConfigureWidget::on_replaceGroup_clicked()
{
    emit validityChanged();
}

void ConfigureWidget::on_trimGroup_clicked()
{
    emit validityChanged();
}

void ConfigureWidget::on_numberingGroup_clicked()
{
    emit validityChanged();
}
