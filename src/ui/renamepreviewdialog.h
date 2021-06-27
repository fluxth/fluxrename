#ifndef RENAMEPREVIEWDIALOG_H
#define RENAMEPREVIEWDIALOG_H

#include <QDialog>

#include "../models/frenameitem.h"

namespace Ui {
class RenamePreviewDialog;
}

class RenamePreviewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RenamePreviewDialog(QWidget *parent, const QList<FRenameItem>& renameItems);
    ~RenamePreviewDialog();

private:
    Ui::RenamePreviewDialog *ui;

    const QList<FRenameItem>& m_renameItems;

    void populateItems();
    void addItem(const FRenameItem& item);
};

#endif // RENAMEPREVIEWDIALOG_H
