#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUrl>
#include "converterbase.h"
#include "basicconverter.h"

class BasicConverter;
class advancedconverter;

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

private slots:
    void on_btnBrowseHtml_clicked();
    void on_btnConvert_clicked();
    void on_radioSimple_toggled(bool checked);
    void on_radioAdvanced_toggled(bool checked);

    void onConversionFinished(bool success, const QString &message);
    void onProgressChanged(int percent);

private:
    Ui::MainWindow *ui;
    ConverterBase *m_currentConverter;
    basicconverter *m_basicConverter;

#ifdef WEBENGINE_AVAILABLE
    advancedconverter *m_advancedConverter;
#endif

    void updateModeInfo();
    void updateStatus(const QString &message);
    void setupConverters();
    QString getHtmlContent(const QString &input);
    void updateConverterSelection();
    void setUiEnabled(bool enabled);
    void startConversion();
    void finishConversion();
    bool isUrl(const QString &input);
    QString getUrlContent(const QString &url);
    QString getFileContent(const QString &filePath);
};
#endif
