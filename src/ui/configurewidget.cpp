#include "configurewidget.h"
#include "ui_configurewidget.h"

ConfigureWidget::ConfigureWidget(QWidget* parent) :
    FWidgetBase(parent),
    ui(new Ui::ConfigureWidget)
{
    ui->setupUi(this);
    resetUi();

    m_groups[0] = ui->replaceGroup;
    m_groups[1] = ui->trimGroup;
    m_groups[2] = ui->addGroup;
    m_groups[3] = ui->stringGroup;

    //m_groups[3] = ui->numberingGroup;
    ui->numberingGroup->hide();
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
    return validate();
}

unique_ptr<FScanner> ConfigureWidget::takeScanner()
{
    serialize();
    return FWidgetBase::takeScanner();
}

bool ConfigureWidget::hydrateScanner(unique_ptr<FScanner> scanner)
{
    if (!FWidgetBase::hydrateScanner(std::move(scanner)))
        return false;

    if (m_scanner) {
        deserialize();
        return true;
    }

    return false;
}

template<size_t S>
inline void ConfigureWidget::batchSetEnabled(const array<QWidget*, S>& widgets, bool enabled)
{
    for (QWidget* widget : widgets)
        widget->setEnabled(enabled);
}

void ConfigureWidget::resetUi()
{
    ui->numSbPositionIndex->hide();
}

inline void ConfigureWidget::inputsChanged()
{
    emit validityChanged();
}

bool ConfigureWidget::validate() const
{
    if (!validateReplace())
        return false;

    if (!validateTrim())
        return false;

    if (!validateAdd())
        return false;

    if (!validateStringManipulation())
        return false;

    return true;
}

bool ConfigureWidget::validateReplace() const
{
    if (!ui->replaceGroup->isChecked())
        return true;

    if (checkbox2bool(ui->repCbClear->checkState()))
        return true;

    if (ui->repLeFind->text().count() > 0)
        return true;

    return false;
}

bool ConfigureWidget::validateTrim() const
{
    if (!ui->trimGroup->isChecked())
        return true;

    if (checkbox2bool(ui->trimCbRangeEnable->checkState())) {
        if (ui->trimSbRangeStart->value() < ui->trimSbRangeEnd->value())
            return true;
    } else {
        return true;
    }

    return false;
}

bool ConfigureWidget::validateAdd() const
{
    if (!ui->addGroup->isChecked())
        return true;

    if (!(checkbox2bool(ui->addCbPrefix->checkState())
            || checkbox2bool(ui->addCbInsert->checkState())
            || checkbox2bool(ui->addCbSuffix->checkState())))
        return false;

    bool result = true;
    if (checkbox2bool(ui->addCbPrefix->checkState()) && ui->addLePrefix->text().count() == 0)
        result = result && false;

    if (checkbox2bool(ui->addCbInsert->checkState()) && ui->addLeInsert->text().count() == 0)
        result = result && false;

    if (checkbox2bool(ui->addCbSuffix->checkState()) && ui->addLeSuffix->text().count() == 0)
        result = result && false;

    return result;
}

bool ConfigureWidget::validateStringManipulation() const
{
    return true;
}

// -- SERIALIZATION --

void ConfigureWidget::serialize()
{
    auto result = m_scanner->getMutRenameConfig();
    auto& [mutex, config] = result;
    QMutexLocker locker(&mutex);

    config.includeExtensions = checkbox2bool(ui->cbIncludeExtension->checkState());
    serializeReplace(config.replace);
    serializeTrim(config.trim);
    serializeAdd(config.add);
    serializeStringManipulation(config.string);
}

void ConfigureWidget::serializeReplace(RenameConfig::ReplaceConfig& config)
{
    config.enabled = ui->replaceGroup->isChecked();
    config.clearExisting = checkbox2bool(ui->repCbClear->checkState());
    config.useRegex = checkbox2bool(ui->repCbRegex->checkState());

    config.find = ui->repLeFind->text();
    config.replace = ui->repLeReplace->text();
}

void ConfigureWidget::serializeTrim(RenameConfig::TrimConfig& config)
{
    config.enabled = ui->trimGroup->isChecked();
    config.fromStartIndex = ui->trimSbStart->value();
    config.fromEndIndex = ui->trimSbEnd->value();

    config.rangeEnabled = checkbox2bool(ui->trimCbRangeEnable->checkState());
    config.rangeStartIndex = ui->trimSbRangeStart->value();
    config.rangeEndIndex = ui->trimSbRangeEnd->value();
}

void ConfigureWidget::serializeAdd(RenameConfig::AddConfig& config)
{
    config.enabled = ui->addGroup->isChecked();

    config.prefixEnabled = checkbox2bool(ui->addCbPrefix->checkState());
    config.prefix = ui->addLePrefix->text();

    config.insertEnabled = checkbox2bool(ui->addCbInsert->checkState());
    config.insert = ui->addLeInsert->text();
    config.insertIndex = ui->addSbInsertIndex->value();

    config.suffixEnabled = checkbox2bool(ui->addCbSuffix->checkState());
    config.suffix = ui->addLeSuffix->text();
}

void ConfigureWidget::serializeStringManipulation(RenameConfig::StringManipulationConfig& config)
{
    config.enabled = ui->stringGroup->isChecked();

    config.upperCase = static_cast<RenameConfig::StringManipulationTarget>(ui->strCbUpper->currentIndex());
    config.lowerCase = static_cast<RenameConfig::StringManipulationTarget>(ui->strCbLower->currentIndex());
}

void ConfigureWidget::deserialize()
{
    auto result = m_scanner->getRenameConfig();
    auto& [mutex, config] = result;
    QMutexLocker locker(&mutex);

    ui->cbIncludeExtension->setCheckState(bool2checkbox(config.includeExtensions));
    deserializeReplace(config.replace);
    deserializeTrim(config.trim);
    deserializeAdd(config.add);
    deserializeStringManipulation(config.string);
}

void ConfigureWidget::deserializeReplace(const RenameConfig::ReplaceConfig& config)
{
    ui->repCbRegex->setCheckState(bool2checkbox(config.useRegex));

    ui->repLeFind->setText(config.find);
    ui->repLeReplace->setText(config.replace);

    ui->repCbClear->setCheckState(bool2checkbox(config.clearExisting));
    ui->replaceGroup->setChecked(config.enabled);
}

void ConfigureWidget::deserializeTrim(const RenameConfig::TrimConfig& config)
{
    ui->trimSbStart->setValue(config.fromStartIndex);
    ui->trimSbEnd->setValue(config.fromEndIndex);

    ui->trimSbRangeStart->setValue(config.rangeStartIndex);
    ui->trimSbRangeEnd->setValue(config.rangeEndIndex);
    ui->trimCbRangeEnable->setCheckState(bool2checkbox(config.rangeEnabled));

    ui->trimGroup->setChecked(config.enabled);
}

void ConfigureWidget::deserializeAdd(const RenameConfig::AddConfig& config)
{

    ui->addLePrefix->setText(config.prefix);
    ui->addCbPrefix->setCheckState(bool2checkbox(config.prefixEnabled));

    ui->addLeInsert->setText(config.insert);
    ui->addSbInsertIndex->setValue(config.insertIndex);
    ui->addCbInsert->setCheckState(bool2checkbox(config.insertEnabled));

    ui->addLeSuffix->setText(config.suffix);
    ui->addCbSuffix->setCheckState(bool2checkbox(config.suffixEnabled));

    ui->addGroup->setChecked(config.enabled);
}

void ConfigureWidget::deserializeStringManipulation(const RenameConfig::StringManipulationConfig& config)
{
    ui->strCbLower->setCurrentIndex(static_cast<int>(config.lowerCase));
    ui->strCbUpper->setCurrentIndex(static_cast<int>(config.upperCase));

    ui->stringGroup->setChecked(config.enabled);
}

// -- SLOTS --

void ConfigureWidget::on_stringGroup_clicked()
{
    inputsChanged();
}

void ConfigureWidget::on_repLeFind_textChanged(const QString&)
{
    inputsChanged();
}

void ConfigureWidget::on_repLeReplace_textChanged(const QString&)
{
    inputsChanged();
}

void ConfigureWidget::on_repCbRegex_stateChanged(int)
{
    inputsChanged();
}

void ConfigureWidget::on_addCbPrefix_stateChanged(int state)
{
    const array<QWidget*, 1> widgets = {
        ui->addLePrefix,
    };

    batchSetEnabled(widgets, state == Qt::Checked);
    inputsChanged();
}

void ConfigureWidget::on_addCbInsert_stateChanged(int state)
{
    const array<QWidget*, 3> widgets = {
        ui->addLeInsert,
        ui->addLbInsertIndex,
        ui->addSbInsertIndex,
    };

    batchSetEnabled(widgets, state == Qt::Checked);
    inputsChanged();
}

void ConfigureWidget::on_addCbSuffix_stateChanged(int state)
{
    const array<QWidget*, 1> widgets = {
        ui->addLeSuffix,
    };

    batchSetEnabled(widgets, state == Qt::Checked);
    inputsChanged();
}

void ConfigureWidget::on_addGroup_clicked()
{
    inputsChanged();
}

void ConfigureWidget::on_replaceGroup_clicked()
{
    inputsChanged();
}

void ConfigureWidget::on_trimGroup_clicked()
{
    inputsChanged();
}

void ConfigureWidget::on_numberingGroup_clicked()
{
    inputsChanged();
}

void ConfigureWidget::on_trimCbRangeEnable_stateChanged(int state)
{
    const array<QWidget*, 4> widgets = {
        ui->trimSbRangeStart,
        ui->trimSbRangeEnd,
        ui->trimLbRangeStart,
        ui->trimLbRangeEnd,
    };

    batchSetEnabled(widgets, state == Qt::Checked);
    inputsChanged();
}

void ConfigureWidget::on_repCbClear_stateChanged(int state)
{
    const array<QWidget*, 5> widgets = {
        ui->repLeFind,
        ui->repLeReplace,
        ui->repLbFind,
        ui->repLbReplace,
        ui->repCbRegex,
    };

    batchSetEnabled(widgets, state != Qt::Checked);
    inputsChanged();
}

void ConfigureWidget::on_trimSbRangeStart_valueChanged(int)
{
    inputsChanged();
}

void ConfigureWidget::on_trimSbRangeEnd_valueChanged(int)
{
    inputsChanged();
}

void ConfigureWidget::on_trimSbStart_valueChanged(int)
{
    inputsChanged();
}

void ConfigureWidget::on_trimSbEnd_valueChanged(int)
{
    inputsChanged();
}

void ConfigureWidget::on_cbIncludeExtension_stateChanged(int)
{
    inputsChanged();
}

void ConfigureWidget::on_addLePrefix_textChanged(const QString&)
{
    inputsChanged();
}

void ConfigureWidget::on_addLeInsert_textChanged(const QString&)
{
    inputsChanged();
}

void ConfigureWidget::on_addLeSuffix_textChanged(const QString&)
{
    inputsChanged();
}

void ConfigureWidget::on_addSbInsertIndex_valueChanged(int)
{
    inputsChanged();
}
