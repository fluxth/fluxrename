#ifndef CONFIGUREWIDGET_H
#define CONFIGUREWIDGET_H

#include "fwidgetbase.h"

#include <QGroupBox>

#define bool2checkbox(x) x ? Qt::Checked : Qt::Unchecked
#define checkbox2bool(x) x == Qt::Checked

namespace Ui {
class ConfigureWidget;
}

class ConfigureWidget : public FWidgetBase
{
    Q_OBJECT

public:
    explicit ConfigureWidget(QWidget* parent = nullptr);
    ~ConfigureWidget();

    bool isValid() const override;
    const char* getTitle() const override {
        return "Rename Configuration";
    }

    unique_ptr<FScanner> takeScanner() override;
    bool hydrateScanner(unique_ptr<FScanner> scanner) override;

private:
    Ui::ConfigureWidget* ui;

    array<QGroupBox*, 4> m_groups;

    void resetUi();
    inline void inputsChanged();

    bool validate() const;
    bool validateReplace() const;
    bool validateTrim() const;
    bool validateAdd() const;
    bool validateStringManipulation() const;

    void serialize();
    void serializeReplace(RenameConfig::ReplaceConfig& config);
    void serializeTrim(RenameConfig::TrimConfig& config);
    void serializeAdd(RenameConfig::AddConfig& config);
    void serializeStringManipulation(RenameConfig::StringManipulationConfig& config);

    void deserialize();
    void deserializeReplace(const RenameConfig::ReplaceConfig& config);
    void deserializeTrim(const RenameConfig::TrimConfig& config);
    void deserializeAdd(const RenameConfig::AddConfig& config);
    void deserializeStringManipulation(const RenameConfig::StringManipulationConfig& config);

    template<size_t S>
    static inline void batchSetEnabled(const array<QWidget*, S>& widgets, bool enabled);

private slots:
    void on_addGroup_clicked();
    void on_replaceGroup_clicked();
    void on_trimGroup_clicked();
    void on_numberingGroup_clicked();
    void on_stringGroup_clicked();
    void on_trimCbRangeEnable_stateChanged(int state);
    void on_repCbClear_stateChanged(int state);
    void on_addCbPrefix_stateChanged(int state);
    void on_addCbInsert_stateChanged(int state);
    void on_addCbSuffix_stateChanged(int state);
    void on_repLeFind_textChanged(const QString&);
    void on_repLeReplace_textChanged(const QString&);
    void on_repCbRegex_stateChanged(int state);
    void on_trimSbRangeStart_valueChanged(int);
    void on_trimSbRangeEnd_valueChanged(int);
    void on_trimSbStart_valueChanged(int);
    void on_trimSbEnd_valueChanged(int);
    void on_cbIncludeExtension_stateChanged(int state);
    void on_addLePrefix_textChanged(const QString&);
    void on_addLeInsert_textChanged(const QString&);
    void on_addLeSuffix_textChanged(const QString&);
    void on_addSbInsertIndex_valueChanged(int);
};

#endif // CONFIGUREWIDGET_H
