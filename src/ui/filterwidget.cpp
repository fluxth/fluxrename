#include "filterwidget.h"
#include "ui_filterwidget.h"

#include <QRegularExpression>

FilterWidget::FilterWidget(QWidget* parent) :
    FWidgetBase(parent),
    ui(new Ui::FilterWidget),
    m_debounceTimer()
{
    ui->setupUi(this);
    ui->extensionListWidget->setSelectionMode(QAbstractItemView::NoSelection);

    m_debounceTimer.setSingleShot(true);
    m_debounceTimer.setInterval(100);
    m_debounceTimer.callOnTimeout(this, &FilterWidget::update);
}

bool FilterWidget::hydrateScanner(unique_ptr<FScanner> scanner)
{
    if (!FWidgetBase::hydrateScanner(std::move(scanner)))
        return false;

    if (m_scanner) {
        populateExtensionList(true);
        showStatus();

        return true;
    }

    return false;
}

bool FilterWidget::isValid() const
{
    return m_matchIndices.count() > 0;
}

unique_ptr<FScanner> FilterWidget::takeScanner()
{
    m_scanner->setFilteredIndices(m_matchIndices);
    return FWidgetBase::takeScanner();
}

void FilterWidget::populateExtensionList(bool clear)
{
    if (!clear && ui->extensionListWidget->count() > 0 && m_extensionListLUT.count() > 0)
        return;

    if (clear) {
        clearExtensionList();
        m_extensionListLUT.clear();
    }

    auto [mutex, extList] = m_scanner->getExtensionList();
    QMutexLocker locker(&mutex);

    size_t i = 0;
    bool checked = true;
    for (const auto& ext : extList) {
        // TODO: remember checked extensions
        addExtensionListItem(ext, checked);
        m_extensionListLUT.insert(ext.first, i);
        i++;
    }
}

void FilterWidget::addExtensionListItem(const ExtensionPair& data, bool checked)
{
    auto text = QString("%2%1  (%L3 item%4)")
            .arg(data.first == "" ? "[no extension]" : data.first)
            .arg(data.first == "" ? "" : ".")
            .arg(data.second)
            .arg(data.second == 1 ? "": "s");
    auto* item = new QListWidgetItem(text, ui->extensionListWidget);
    item->setCheckState(checked ? Qt::Checked : Qt::Unchecked);
    ui->extensionListWidget->addItem(item);
}

void FilterWidget::clearExtensionList()
{
    ui->extensionListWidget->clear();
}

void FilterWidget::showStatus()
{
    if (!processFilter()) {
        emit validityChanged();
        return;
    }

    auto matchCount = m_matchIndices.count();
    if (matchCount > 0)
        showStatus(QString("%L1 file%2 matched this filter").arg(matchCount).arg(matchCount == 1 ? "" : "s"));
    else
        showError("No files matched this filter!");

    emit validityChanged();
}

void FilterWidget::showStatus(const QString& msg)
{
    ui->labelStatus->setText(QString("<p style=\"color:grey\">%1</p>").arg(msg));
}

void FilterWidget::showError(const QString& msg)
{
    m_matchIndices.clear();
    ui->labelStatus->setText(QString("<p style=\"color:red\">%1</p>").arg(msg));
}

bool FilterWidget::processFilter()
{
    auto [mutex, fileList] = m_scanner->getFileList();
    QMutexLocker locker(&mutex);

    m_matchIndices.clear();

    bool pass = false;
    for (qsizetype i = 0; i < fileList.count(); ++i) {
        const auto& fileInfo = fileList.at(i);
        pass = false;

        // check name include
        auto name = fileInfo.completeBaseName();
        if (ui->checkboxRegex->checkState() == Qt::Checked) {
            QRegularExpression::PatternOptions options = QRegularExpression::NoPatternOption;
            if (ui->checkboxCaseSensitive->checkState() == Qt::Unchecked)
                options |= QRegularExpression::CaseInsensitiveOption;

            auto regex = QRegularExpression(ui->lineEditInclude->text(), options);
            if (!regex.isValid()) {
                showError("Invalid regular expression syntax!");
                return false;
            }

            pass = name.contains(regex);
        } else {
            pass = name.contains(
                    ui->lineEditInclude->text(),
                    ui->checkboxCaseSensitive->checkState() == Qt::Checked ? Qt::CaseSensitive : Qt::CaseInsensitive);
        }

        if (!pass) continue;
        pass = false;

        // check name exclude
        if (ui->checkboxRegex->checkState() == Qt::Checked) {
            QRegularExpression::PatternOptions options = QRegularExpression::NoPatternOption;
            if (ui->checkboxCaseSensitive->checkState() == Qt::Unchecked)
                options |= QRegularExpression::CaseInsensitiveOption;

            auto regex = QRegularExpression(ui->lineEditExclude->text(), options);
            if (!regex.isValid()) {
                showError("Invalid regular expression syntax!");
                return false;
            }

            pass = ui->lineEditExclude->text().count() == 0 || !name.contains(regex);
        } else {
            pass = ui->lineEditExclude->text().count() == 0 || !name.contains(
                        ui->lineEditExclude->text(),
                        ui->checkboxCaseSensitive->checkState() == Qt::Checked ? Qt::CaseSensitive : Qt::CaseInsensitive);
        }

        if (!pass) continue;
        pass = false;

        // check extension
        auto ext = fileInfo.suffix().toLower();
        if (m_extensionListLUT.contains(ext)) {
            auto* item = ui->extensionListWidget->item(m_extensionListLUT.value(ext));
            if (item->checkState() == Qt::Checked) {
                pass = true;
            }
        }

        if (!pass) continue;

        // check succeeded
        m_matchIndices.insert(i);
    }

    return true;
}

FilterWidget::~FilterWidget()
{
    delete ui;
}

void FilterWidget::on_btnExtSelectAll_clicked()
{
    for (int i = 0; i < ui->extensionListWidget->count(); ++i) {
        auto* item = ui->extensionListWidget->item(i);
        item->setCheckState(Qt::Checked);
    }
}

void FilterWidget::on_btnExtClearAll_clicked()
{
    for (int i = 0; i < ui->extensionListWidget->count(); ++i) {
        auto* item = ui->extensionListWidget->item(i);
        item->setCheckState(Qt::Unchecked);
    }
}

void FilterWidget::on_btnExtInvert_clicked()
{
    for (int i = 0; i < ui->extensionListWidget->count(); ++i) {
        auto* item = ui->extensionListWidget->item(i);
        item->setCheckState(item->checkState() == Qt::Checked ? Qt::Unchecked : Qt::Checked);
    }
}

void FilterWidget::on_extensionListWidget_itemChanged(QListWidgetItem*)
{
    if (!m_debounceTimer.isActive())
        m_debounceTimer.start();
}

void FilterWidget::update()
{
    showStatus();
}

void FilterWidget::on_lineEditInclude_textChanged(const QString&)
{
    if (!m_debounceTimer.isActive())
        m_debounceTimer.start();
}

void FilterWidget::on_checkboxRegex_stateChanged(int)
{
    if ((!m_debounceTimer.isActive()) && ui->lineEditInclude->text().count() > 0)
        m_debounceTimer.start();
}

void FilterWidget::on_checkboxCaseSensitive_stateChanged(int)
{
    if ((!m_debounceTimer.isActive()) && ui->lineEditInclude->text().count() > 0)
        m_debounceTimer.start();
}

void FilterWidget::on_lineEditExclude_textChanged(const QString&)
{
    if (!m_debounceTimer.isActive())
        m_debounceTimer.start();
}
