#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "formul.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}

class Function;
class PlotBuilder;
class PointsCollection;

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void startProcessing();
    void pauseProcessing();
    void resumeProcessing();
    void stopProcessing();

public slots:

    void startClicked();
    void pauseClicked();
    void valueProcessed(double, double, double);
    void processingFinished();
    void saveClicked();
    void loadClicked();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;

    PointsCollection *_points;
    PlotBuilder *_plotBuilder;

    static const QString LABEL_PAUSE;
    static const QString LABEL_START;
    static const QString LABEL_CONTINUE;
    static const QString LABEL_PROCESSING;

    void setupConnections();

    QVector<Function *> * loadFunctions();
};
#endif // MAINWINDOW_H
