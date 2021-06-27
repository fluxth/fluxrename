#include "frenamer.h"

#include <QProgressDialog>
#include <QRegularExpression>

#include "ui/renamepreviewdialog.h"

FRenamer::FRenamer(unique_ptr<FScanner> scanner) :
    QObject(nullptr),
    m_scanner(std::move(scanner))
{

}

FRenamer::~FRenamer()
{

}

void FRenamer::calculate(QWidget* parent)
{
    size_t filteredCount = m_scanner->m_scannerData.filteredIndices.count();
    m_renameItems.reserve(filteredCount);

    QProgressDialog progress("Calculating new filenames...", "Cancel", 0, filteredCount, parent);
    progress.setWindowModality(Qt::WindowModal);
    if (filteredCount > 2000)
        progress.show();

    size_t count = 0;
    for (qsizetype i = 0; i < m_scanner->m_scannerData.targets.count(); ++i) {
        if (!m_scanner->m_scannerData.filteredIndices.contains(i))
            continue;

        const auto& fileInfo = m_scanner->m_scannerData.targets.at(i);

        if (!processItem(fileInfo)) {
            qWarning("Process fail!"); // show error dialog
        }

        count++;
        progress.setValue(count);
    }
}

void FRenamer::previewChanges(QWidget* parent)
{
    RenamePreviewDialog dialog(parent, m_renameItems);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.exec();
}

unique_ptr<FScanner> FRenamer::takeScanner()
{
    return std::move(m_scanner);
}

bool FRenamer::processItem(const QFileInfo& fileInfo)
{
    auto item = FRenameItem();
    item.fileInfo = fileInfo;

    auto result = processFilename(fileInfo);
    if (!result.has_value())
        return false;

    item.newFilename = result.value();
    //qDebug() << item.newFilename;

    m_renameItems.emplaceBack(item);
    return true;
}

optional<QString> FRenamer::processFilename(const QFileInfo& fileInfo)
{
    const auto& config = m_scanner->m_scannerData.renameConfig;
    QString newFilename = config.includeExtensions ? fileInfo.fileName() : fileInfo.completeBaseName();
    bool trailingDot = fileInfo.fileName().last(1) == ".";

    if (config.replace.enabled && !processFilenameReplace(config.replace, newFilename))
        return {};

    if (config.trim.enabled && !processFilenameTrim(config.trim, newFilename))
        return {};

    if (config.add.enabled && !processFilenameAdd(config.add, newFilename))
        return {};

    // Add extension to newFilename for the next process
    if (!config.includeExtensions)
        newFilename.append(QString("%1%2").arg(fileInfo.suffix().count() > 0 || trailingDot ? "." : "", fileInfo.suffix()));

    if (config.string.enabled && !processFilenameStringManipulation(config.string, newFilename))
        return {};

    return newFilename;
}

inline bool FRenamer::processFilenameReplace(const RenameConfig::ReplaceConfig& config, QString& filename)
{
    if (config.clearExisting) {
        filename.clear();
        return true;
    }

    if (config.useRegex) {
        QRegularExpression regex(config.find);
        if (!regex.isValid())
            return false;

        filename.replace(regex, config.replace);
    } else {
        filename.replace(config.find, config.replace, Qt::CaseSensitive);
    }

    return true;
}

inline bool FRenamer::processFilenameTrim(const RenameConfig::TrimConfig& config, QString& filename)
{
    if (config.fromStartIndex > 0) {
        filename.remove(0, config.fromStartIndex);
    }

    if (config.fromEndIndex > 0) {
        filename.chop(config.fromEndIndex);
    }

    if (config.rangeEnabled) {
        qsizetype length = config.rangeEndIndex - config.rangeStartIndex;
        if (length > 0) {
            filename.remove(config.rangeStartIndex, length);
        }
    }

    return true;
}

inline bool FRenamer::processFilenameAdd(const RenameConfig::AddConfig& config, QString& filename)
{
    if (config.insertEnabled) {
        qsizetype index = config.insertIndex;
        if (index >= filename.count()) index = filename.count();
        if (index < 0) index = 0;

        filename.insert(index, config.insert);
    }

    if (config.prefixEnabled) {
        filename.prepend(config.prefix);
    }

    if (config.suffixEnabled) {
        filename.append(config.suffix);
    }

    return true;
}

inline bool FRenamer::processFilenameStringManipulation(const RenameConfig::StringManipulationConfig& config, QString& filename)
{
    // Very temporary: GUI changes coming soon

    auto dotIndex = filename.lastIndexOf('.');
    auto filePrefix = QString(filename).left(dotIndex);
    auto ext = dotIndex < 0 ? QString() : QString(filename).sliced(dotIndex);

    switch (config.upperCase) {
    case RenameConfig::StringManipulationTarget::Filename:
        filePrefix = filePrefix.toUpper();
        break;
    case RenameConfig::StringManipulationTarget::Extension:
        ext = ext.toUpper();
        break;
    case RenameConfig::StringManipulationTarget::NameAndExtension:
        filePrefix = filePrefix.toUpper();
        ext = ext.toUpper();
        break;
    default:
        break;
    }

    switch (config.lowerCase) {
    case RenameConfig::StringManipulationTarget::Filename:
        filePrefix = filePrefix.toLower();
        break;
    case RenameConfig::StringManipulationTarget::Extension:
        ext = ext.toLower();
        break;
    case RenameConfig::StringManipulationTarget::NameAndExtension:
        filePrefix = filePrefix.toLower();
        ext = ext.toLower();
        break;
    default:
        break;
    }

    filename = filePrefix + ext;
    return true;
}
