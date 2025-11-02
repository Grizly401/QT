#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "basicconverter.h"

#ifdef WEBENGINE_AVAILABLE
#include "advancedconverter.h"
#endif

#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>
#include <QFile>
#include <QTextStream>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_currentConverter(nullptr)
    , m_basicConverter(new basicconverter(this))
#ifdef WEBENGINE_AVAILABLE
    , m_advancedConverter(new advancedconverter(this))
#endif
{
    ui->setupUi(this);

    setupConverters();
    updateModeInfo();
    updateStatus("Готов к работе");

    ui->radioSimple->setChecked(true);

#ifdef WEBENGINE_AVAILABLE
    ui->radioAdvanced->setEnabled(true);
    ui->radioAdvanced->setToolTip("Полная поддержка CSS3 и JavaScript через QWebEngine");
#else
    ui->radioAdvanced->setEnabled(false);
    ui->radioAdvanced->setToolTip("QWebEngine недоступен");
#endif

    ui->lineHtmlPath->setPlaceholderText("Введите путь к HTML файлу или URL веб-страницы...");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupConverters()
{
    m_currentConverter = m_basicConverter;

    connect(m_basicConverter, &ConverterBase::conversionFinished,
            this, &MainWindow::onConversionFinished);
    connect(m_basicConverter, &ConverterBase::progressChanged,
            this, &MainWindow::onProgressChanged);

#ifdef WEBENGINE_AVAILABLE
    connect(m_advancedConverter, &ConverterBase::conversionFinished,
            this, &MainWindow::onConversionFinished);
    connect(m_advancedConverter, &ConverterBase::progressChanged,
            this, &MainWindow::onProgressChanged);
#endif
}

bool MainWindow::isUrl(const QString &input)
{
    QString trimmed = input.trimmed();

    if (trimmed.startsWith("http://") ||
        trimmed.startsWith("https://") ||
        trimmed.startsWith("ftp://") ||
        trimmed.startsWith("file://")) {
        return true;
    }

    if (trimmed.contains(".") &&
        (trimmed.contains("/") || trimmed.contains("www.")) &&
        !trimmed.contains("\\") && // Исключаем пути Windows
        !QFile::exists(trimmed)) { // Исключаем существующие локальные файлы
        return true;
    }

    return false;
}

QString MainWindow::getUrlContent(const QString &url)
{
    updateStatus("Загрузка веб-страницы...");

    QNetworkAccessManager manager;
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36");

    QNetworkReply *reply = manager.get(request);

    QTimer timer;
    timer.setSingleShot(true);

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);

    timer.start(15000);

    loop.exec();

    if (timer.isActive()) {
        timer.stop();

        if (reply->error() == QNetworkReply::NoError) {
            QByteArray data = reply->readAll();
            QString content = QString::fromUtf8(data);

            if (content.contains("<!DOCTYPE", Qt::CaseInsensitive) ||
                content.contains("<html", Qt::CaseInsensitive)) {
                updateStatus("Веб-страница успешно загружена");
                reply->deleteLater();
                return content;
            } else {
                updateStatus("Ошибка: Загруженный контент не является HTML");
                reply->deleteLater();
                return QString();
            }
        } else {
            updateStatus("Ошибка загрузки: " + reply->errorString());
            reply->deleteLater();
            return QString();
        }
    } else {
        updateStatus("Ошибка: Превышено время ожидания загрузки (15 секунд)");
        reply->abort();
        reply->deleteLater();
        return QString();
    }
}

QString MainWindow::getFileContent(const QString &filePath)
{
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        QString content = stream.readAll();
        file.close();
        updateStatus("Файл успешно загружен: " + filePath);
        return content;
    } else {
        updateStatus("Ошибка: Не удалось открыть файл");
        return QString();
    }
}

QString MainWindow::getHtmlContent(const QString &input)
{
    QString trimmedInput = input.trimmed();

    if (trimmedInput.isEmpty()) {
        updateStatus("Ошибка: Пустой ввод");
        return QString();
    }

    if (isUrl(trimmedInput)) {
        // Добавляем https:// если не указана схема
        if (!trimmedInput.startsWith("http://") && !trimmedInput.startsWith("https://") &&
            !trimmedInput.startsWith("ftp://") && !trimmedInput.startsWith("file://")) {
            trimmedInput = "https://" + trimmedInput;
        }

        return getUrlContent(trimmedInput);
    } else {
        // Локальный файл
        return getFileContent(trimmedInput);
    }
}

void MainWindow::setUiEnabled(bool enabled)
{

    ui->btnConvert->setEnabled(enabled);
    ui->btnBrowseHtml->setEnabled(enabled);

    ui->radioSimple->setEnabled(enabled);

    ui->lineHtmlPath->setEnabled(enabled);

#ifdef WEBENGINE_AVAILABLE
    ui->radioAdvanced->setEnabled(enabled);
#else
    ui->radioAdvanced->setEnabled(false);
#endif

    if (enabled) {
        ui->btnConvert->setText("Конвертировать в PDF");
        ui->btnConvert->setStyleSheet("");
        ui->progressBar->setVisible(false);
    } else {
        ui->btnConvert->setText("Конвертация...");
        ui->btnConvert->setStyleSheet(
            "QPushButton {"
            "   background-color: #f39c12;"
            "   color: white;"
            "   font-weight: bold;"
            "}"
            );
        ui->progressBar->setVisible(true);
    }
}

void MainWindow::startConversion()
{
    setUiEnabled(false);
    ui->progressBar->setValue(0);
    updateStatus("Начата конвертация...");
    setCursor(Qt::BusyCursor);
}

void MainWindow::finishConversion()
{
    setUiEnabled(true);

    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(false);

    setCursor(Qt::ArrowCursor);
}

void MainWindow::updateConverterSelection()
{
    if (ui->radioSimple->isChecked()) {
        m_currentConverter = m_basicConverter;
        updateStatus("Выбран быстрый режим конвертации");
    } else {
#ifdef WEBENGINE_AVAILABLE
        m_currentConverter = m_advancedConverter;
        updateStatus("Выбран продвинутый режим конвертации (QWebEngine)");
#else
        // Защита - если вдруг кнопка активна, но WebEngine недоступен
        ui->radioSimple->setChecked(true);
        m_currentConverter = m_basicConverter;
        updateStatus("Продвинутый режим недоступен. Используется базовый.");
#endif
    }
}

void MainWindow::on_btnBrowseHtml_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Выберите HTML файл",
                                                    QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                                    "HTML Files (*.html *.htm);;All Files (*)");

    if (!fileName.isEmpty()) {
        ui->lineHtmlPath->setText(fileName);
        updateStatus("Файл выбран: " + fileName);
    }
}

void MainWindow::on_btnConvert_clicked()
{
    QString inputPath = ui->lineHtmlPath->text().trimmed();

    if (inputPath.isEmpty()) {
        QMessageBox::warning(this, "Внимание",
                             "Пожалуйста, выберите HTML файл или введите URL веб-страницы");
        return;
    }

    if (isUrl(inputPath)) {
#ifdef WEBENGINE_AVAILABLE
        if (ui->radioSimple->isChecked()) {
            QMessageBox::StandardButton reply = QMessageBox::question(this, "Рекомендация",
                                                                      "Для конвертации веб-страниц рекомендуется использовать Продвинутый режим (QWebEngine).\n\n"
                                                                      "Базовый режим может не поддерживать:\n"
                                                                      "• Загрузку внешних ресурсов (CSS, изображения)\n"
                                                                      "• Выполнение JavaScript\n"
                                                                      "• Современные CSS стили\n\n"
                                                                      "Переключиться на Продвинутый режим?",
                                                                      QMessageBox::Yes | QMessageBox::No);

            if (reply == QMessageBox::Yes) {
                ui->radioAdvanced->setChecked(true);
            }
        }
#else
        // Если WebEngine недоступен, предупреждаем
        QMessageBox::warning(this, "Ограничение",
                             "Для конвертации веб-страниц требуется Продвинутый режим (QWebEngine), который currently недоступен.\n\n"
                             "Будет выполнена базовая конвертация загруженного HTML.");
#endif
    }

    updateConverterSelection();

    QString htmlContent = getHtmlContent(inputPath);
    if (htmlContent.isEmpty()) {
        QMessageBox::critical(this, "Ошибка", "Не удалось загрузить HTML контент");
        return;
    }

    QString defaultName = "converted.pdf";
    QString input = ui->lineHtmlPath->text().trimmed();

    if (isUrl(input)) {
        QUrl url(input);
        QString host = url.host();
        if (!host.isEmpty()) {
            defaultName = host + ".pdf";
            if (defaultName.startsWith("www.")) {
                defaultName = defaultName.mid(4);
            }
        }
    } else if (input.endsWith(".html", Qt::CaseInsensitive) ||
               input.endsWith(".htm", Qt::CaseInsensitive)) {
        defaultName = QFileInfo(input).completeBaseName() + ".pdf";
    }

    QString outputPath = QFileDialog::getSaveFileName(this,
                                                      "Сохранить PDF как",
                                                      QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/" + defaultName,
                                                      "PDF Files (*.pdf)");

    if (outputPath.isEmpty()) {
        return; // Пользователь отменил
    }

    startConversion();
    m_currentConverter->convert(htmlContent, outputPath);
}

void MainWindow::onConversionFinished(bool success, const QString &message)
{
    finishConversion();
    updateStatus(message);

    if (success) {
        QMessageBox::information(this, "Успех", message);
    } else {
        QMessageBox::critical(this, "Ошибка", message);
    }
}

void MainWindow::onProgressChanged(int percent)
{
    ui->progressBar->setValue(percent);
    updateStatus(QString("Конвертация: %1%").arg(percent));
}

void MainWindow::on_radioSimple_toggled(bool checked)
{
    if (checked) {
        updateConverterSelection();
        updateModeInfo();
    }
}

void MainWindow::on_radioAdvanced_toggled(bool checked)
{
    if (checked) {
        updateConverterSelection();
        updateModeInfo();
    }
}

void MainWindow::updateModeInfo()
{
    QString info;

    if (ui->radioSimple->isChecked()) {
        info = "Быстрый режим (QTextDocument):\n"
               "• Мгновенная конвертация\n"
               "• Базовая поддержка HTML\n"
               "• Минимальное использование ресурсов\n"
               "• Идеален для простых документов";
    } else {
        info = "Продвинутый режим (QWebEngine):\n"
               "• Полная поддержка CSS3 и JavaScript\n"
               "• Современные веб-стандарты\n"
               "• Загрузка внешних ресурсов\n"
               "• Движок Chromium\n"
               "• Требует больше памяти и времени";
    }

    ui->groupBox->setToolTip(info);
    updateStatus(info.split('\n').first());
}

void MainWindow::updateStatus(const QString &message)
{
    ui->labelStatus->setText("Статус: " + message);
}
