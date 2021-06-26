#include "inputwidget.h"
#include "ui_inputwidget.h"

#import <QFileDialog>
#import <QInputDialog>

InputWidget::InputWidget(QWidget* parent) :
    FWidgetBase(parent),
    ui(new Ui::InputWidget)
{
    ui->setupUi(this);

    m_formElements[0] = ui->btnEdit;
    m_formElements[1] = ui->btnBrowse;
    m_formElements[2] = ui->btnClear;
    m_formElements[3] = ui->checkboxRecursive;
    m_formElements[4] = ui->inputBox;

    showInfo();

    // TEST
    setPath("~/Desktop");
}

InputWidget::~InputWidget()
{
    delete ui;
}

bool InputWidget::isValid() const
{
    if (!m_scanner)
        return false;

    return m_scanner->isValid();
}

QString InputWidget::getPath() const
{
    return ui->inputBox->text();
}

bool InputWidget::hydrateScanner(unique_ptr<FScanner> scanner)
{
    if (!FWidgetBase::hydrateScanner(std::move(scanner)))
        return false;

    if (m_scanner) {
        auto scannerPath = m_scanner->getAbsolutePath();
        if (scannerPath != getPath()) {
            setPath(scannerPath);
        }
    } else {
        clearPath();
    }

    return true;
}

void InputWidget::updateCount(size_t count)
{
    showInfo(QString("%3canning: Found %L1 file%2")
            .arg(count)
            .arg(count == 1 ? "" : "s")
            .arg(isRecursive() ? "Recursively s" : "S"));
}

void InputWidget::scanFinished()
{
    if (!m_scanner->exists()) {
        showError("The specified path does not exist!");
    } else {
        ui->inputBox->setText(m_scanner->getAbsolutePath());

        auto count = m_scanner->getFileCount();
        if (count > 0) {
            showInfo(QString("%L1 file%2 applicable for rename").arg(count).arg(count == 1 ? "" : "s"));

        } else {
            showError(QString("No files to rename!"));
        }
    }

    enableForms();
    emit validityChanged();
}

void InputWidget::setPath(const QString& path)
{
    if (path.length() == 0) {
        emit validityChanged();
        return;
    }

    QString parsedPath = parsePath(path);

    m_scanner.reset(new FScanner(parsedPath, isRecursive()));
    ui->inputBox->setText(parsedPath);
    emit validityChanged();

    connect(m_scanner.get(), &FScanner::countUpdated, this, &InputWidget::updateCount);
    connect(m_scanner.get(), &FScanner::done, this, &InputWidget::scanFinished);

    disableForms();
    m_scanner->scan();
}

void InputWidget::clearPath()
{
    m_scanner.reset();
    ui->inputBox->setText("");
    showInfo();
    emit validityChanged();
}

QString InputWidget::parsePath(const QString& path) const
{
    if (path.at(0) == '~') {
        return QString(path).remove(0, 1).prepend(QDir::homePath());
    }

    return path;
}

void InputWidget::on_btnBrowse_clicked()
{
    QString dir = isValid() ? getPath() : QString();
    auto path = QFileDialog::getExistingDirectory(this, "Select rename source directory", dir);
    setPath(path);
}

void InputWidget::on_btnClear_clicked()
{
    clearError();
    clearPath();
}

void InputWidget::on_btnEdit_clicked()
{
    auto path = QInputDialog::getText(this, "Edit", "Source Directory", QLineEdit::Normal, getPath());
    setPath(path);
}

bool InputWidget::isRecursive() const
{
    return ui->checkboxRecursive->checkState() == Qt::Checked;
}

void InputWidget::showInfo()
{
    showInfo("Select a source directory then click Continue...");
}

void InputWidget::showInfo(const QString& msg)
{
    ui->labelSubtitle->setText(QString("<p style=\"color:gray;\">%1</p>").arg(msg));
}

void InputWidget::showError(const QString& msg)
{
    ui->labelSubtitle->setText(QString("<p style=\"color:red;\">%1</p>").arg(msg));
}

void InputWidget::clearError()
{
    showInfo();
}

void InputWidget::disableForms()
{
    for (auto* form : m_formElements) {
        form->setDisabled(true);
    }
}

void InputWidget::enableForms()
{
    for (auto* form : m_formElements) {
        form->setEnabled(true);
    }
}

void InputWidget::on_checkboxRecursive_stateChanged(int)
{
    if (m_scanner && m_scanner->exists())
        setPath(getPath());
}
