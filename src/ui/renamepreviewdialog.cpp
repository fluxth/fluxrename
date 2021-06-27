#include "renamepreviewdialog.h"
#include "ui_renamepreviewdialog.h"

RenamePreviewDialog::RenamePreviewDialog(QWidget *parent, const QList<FRenameItem>& renameItems) :
    QDialog(parent),
    ui(new Ui::RenamePreviewDialog),
    m_renameItems(renameItems)
{
    ui->setupUi(this);
    populateItems();

    ui->treeWidget->resizeColumnToContents(2);
}

RenamePreviewDialog::~RenamePreviewDialog()
{
    delete ui;
}

void RenamePreviewDialog::populateItems()
{
    for (const auto& item : m_renameItems) {
        addItem(item);
    }

}

void RenamePreviewDialog::addItem(const FRenameItem& item)
{
    auto* i = new QTreeWidgetItem(ui->treeWidget, {item.fileInfo.path(), item.fileInfo.fileName(), ">>", item.newFilename});
    ui->treeWidget->addTopLevelItem(i);
}
