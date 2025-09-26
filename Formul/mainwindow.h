#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "taskmodel.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_pushButton_Add_clicked();
    void on_pushButton_Delete_clicked();
    void on_Clear_All_clicked();

    // Новые слоты для обработки сигналов модели
    void updateView();
    void onItemChanged(QListWidgetItem *item);

    void on_pushButton_Up_toList_clicked();

    void on_pushButton_Down_toList_clicked();

private:
    Ui::MainWindow *ui;
    TaskModel *m_taskModel; // Указатель на нашу модель

    void setupConnections();
};

#endif // MAINWINDOW_H
