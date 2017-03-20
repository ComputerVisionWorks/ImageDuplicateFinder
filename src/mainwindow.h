#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProgressDialog>
#include <QStandardItemModel>

#include "imageduplicatefinder.h"
#include "imageduplicategroups.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void displayResults();

    void on_addFolderPushButton_clicked();

    void on_removeFolderPushButton_clicked();

    void on_scanPushButton_clicked();

    void currentDuplicateGroupsChanged(const QModelIndex &current, const QModelIndex &previous);

    void on_imageListWidget_itemSelectionChanged();

    void on_exportDuplicateGroupsPushButton_clicked();

private:
    Ui::MainWindow *ui;

    ImageDuplicateFinder *m_duplicateFinder;

    ImageDuplicateGroups m_duplicateGroups;

    int m_currentDuplicateGroup;

    QStandardItemModel* m_duplicateGroupsModel;
};

#endif // MAINWINDOW_H
