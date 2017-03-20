#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_duplicateFinder = new ImageDuplicateFinder();

    QObject::connect(m_duplicateFinder, &ImageDuplicateFinder::finished, this, &MainWindow::displayResults);

    m_duplicateGroupsModel = new QStandardItemModel(this);
    m_duplicateGroupsModel->setColumnCount(2);
    QStringList horizontalHeaderLabels = {"Path of the first image in the group", "Number of images in the group"};
    m_duplicateGroupsModel->setHorizontalHeaderLabels(horizontalHeaderLabels);

    ui->duplicateGroupsTableView->setModel(m_duplicateGroupsModel);
    ui->duplicateGroupsTableView->resizeColumnToContents(0);

    QObject::connect(ui->duplicateGroupsTableView->selectionModel(), &QItemSelectionModel::currentChanged, this, &MainWindow::currentDuplicateGroupsChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_duplicateFinder;
}

void MainWindow::displayResults() {
    m_duplicateGroups = m_duplicateFinder->duplicateGroups();

    // Update the duplicate group table view.
    m_duplicateGroupsModel->removeRows(0, m_duplicateGroupsModel->rowCount());
    for (int i = 0; i < m_duplicateGroups.duplicateGroups.size(); i++) {
        QList<QStandardItem*> row;

        row.append(new QStandardItem(m_duplicateGroups.groupImage(i, 0)));
        row.append(new QStandardItem(QString::number(m_duplicateGroups.groupSize(i))));

        m_duplicateGroupsModel->appendRow(row);
    }
    ui->duplicateGroupsTableView->resizeColumnToContents(0);
    ui->duplicateGroupsTableView->sortByColumn(0, Qt::AscendingOrder);

    m_duplicateFinder->reset();

    QMessageBox::information(this, "Duplicates", QString::number(m_duplicateGroups.size()) + " groups of duplicates found.");
}

void MainWindow::on_addFolderPushButton_clicked()
{
    // Let the user choose a directory to add.
    QString dir_path = QFileDialog::getExistingDirectory(this);

    ui->foldersListWidget->addItem(dir_path);
}

void MainWindow::on_removeFolderPushButton_clicked()
{
    QList<QListWidgetItem*> selection = ui->foldersListWidget->selectedItems();

    for (QList<QListWidgetItem*>::const_iterator it = selection.begin(); it != selection.end(); ++it) {
        ui->foldersListWidget->removeItemWidget(*it);
        delete *it;
    }
}

void MainWindow::on_scanPushButton_clicked()
{
    QStringList directories;
    for (int i = 0; i < ui->foldersListWidget->count(); i++) {
        directories << ui->foldersListWidget->item(i)->text();
    }

    if (directories.count() > 0)
    {
        QProgressDialog *progressDialog = new QProgressDialog(this);
        progressDialog->setAttribute(Qt::WA_DeleteOnClose);

        QObject::connect(m_duplicateFinder, &ImageDuplicateFinder::finished, progressDialog, &QProgressDialog::reset);
        QObject::connect(progressDialog, &QProgressDialog::canceled, m_duplicateFinder, &ImageDuplicateFinder::cancel);
        QObject::connect(m_duplicateFinder, &ImageDuplicateFinder::progressTextChanged, progressDialog, &QProgressDialog::setLabelText);
        QObject::connect(m_duplicateFinder, &ImageDuplicateFinder::progressRangeChanged, progressDialog, &QProgressDialog::setRange);
        QObject::connect(m_duplicateFinder, &ImageDuplicateFinder::progressValueChanged, progressDialog, &QProgressDialog::setValue);

        m_duplicateFinder->start(directories, ui->subfolderCheckBox->isChecked());

        // Display the dialog and start the event loop.
        progressDialog->exec();
    }
    else
    {
        QMessageBox::warning(this, "No directory selected", "You should select at least one directory in which to search for duplicates.");
    }
}

void MainWindow::currentDuplicateGroupsChanged(const QModelIndex &current, const QModelIndex &)
{
    int row = current.row();
    QStandardItem* item = m_duplicateGroupsModel->item(row, 0);

    if (item != 0) {
        QString groupPath = item->text();

        ui->imageLabel->setPixmap(QPixmap(groupPath));

        m_currentDuplicateGroup = m_duplicateGroups.findGroup(groupPath);

        // Update the list of photos.
        ui->imageListWidget->clear();
        QStringList currentGroup = m_duplicateGroups.group(m_currentDuplicateGroup);
        for (QStringList::const_iterator itImage = currentGroup.begin(); itImage != currentGroup.end(); ++itImage) {
            QFileInfo fileInfo(*itImage);
            ui->imageListWidget->addItem(new QListWidgetItem(QIcon(fileInfo.filePath()), fileInfo.fileName()));
        }
    }
}

void MainWindow::on_imageListWidget_itemSelectionChanged()
{
    QModelIndexList indexes = ui->imageListWidget->selectionModel()->selectedIndexes();

    if (indexes.size() > 0) {
        int imageIndex = indexes.first().row();
        ui->imageLabel->setPixmap(QPixmap(m_duplicateGroups.groupImage(m_currentDuplicateGroup, imageIndex)));
    }
}

void MainWindow::on_exportDuplicateGroupsPushButton_clicked()
{
    QFileDialog::getSaveFileName(this, "Save duplicate groups");
}
