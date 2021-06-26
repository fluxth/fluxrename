#ifndef FILTERWIDGET_H
#define FILTERWIDGET_H

#include "fwidgetbase.h"

#include <QListWidgetItem>
#include <QTimer>
#include <QSet>

namespace Ui {
class FilterWidget;
}

class FilterWidget : public FWidgetBase
{
    Q_OBJECT

public:
    explicit FilterWidget(QWidget* parent = nullptr);
    ~FilterWidget();

    bool isValid() const override;
    const char* getTitle() const override {
        return "Filter Input Files";
    }

    unique_ptr<FScanner> takeScanner() override;
    bool hydrateScanner(unique_ptr<FScanner> scanner) override;


private slots:
    void on_btnExtSelectAll_clicked();
    void on_btnExtClearAll_clicked();
    void on_btnExtInvert_clicked();
    void on_extensionListWidget_itemChanged(QListWidgetItem *item);
    void on_lineEditInclude_textChanged(const QString &arg1);
    void on_checkboxRegex_stateChanged(int arg1);
    void update();

    void on_checkboxCaseSensitive_stateChanged(int arg1);

    void on_lineEditExclude_textChanged(const QString &arg1);

private:
    Ui::FilterWidget* ui;
    QList<ExtensionPair> m_extensionListCache;
    QHash<QString, size_t> m_extensionListLUT;
    QTimer m_debounceTimer;
    QSet<size_t> m_matchIndices;

    void populateExtensionList();

    void addExtensionListItem(const ExtensionPair& data, bool checked);
    void clearExtensionList();

    void showStatus();
    void showStatus(const QString& msg);
    void showError(const QString& msg);

    bool processFilter();

};

#endif // FILTERWIDGET_H
