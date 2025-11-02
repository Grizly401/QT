#include "advancedconverter.h"
#include <QDebug>

#ifdef WEBENGINE_AVAILABLE
#include <QWebEngineSettings>
#include <QWebEngineProfile>
#include <QPageLayout>
#include <QPageSize>
#include <QFile>
#endif

advancedconverter::advancedconverter(QObject *parent)
    : ConverterBase(parent)
{
#ifdef WEBENGINE_AVAILABLE
    m_webPage = new QWebEnginePage(this);
    m_loadTimer = new QTimer(this);
    m_loadTimer->setSingleShot(true);
    m_retryCount = 0;

    m_webPage->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
    m_webPage->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    m_webPage->settings()->setAttribute(QWebEngineSettings::ScrollAnimatorEnabled, true);
    m_webPage->settings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
    m_webPage->settings()->setAttribute(QWebEngineSettings::FullScreenSupportEnabled, true);

    connect(m_webPage, &QWebEnginePage::loadFinished,
            this, &advancedconverter::onLoadFinished);
#endif
}

advancedconverter::~advancedconverter()
{

}

void advancedconverter::convert(const QString &htmlContent, const QString &outputPath)
{
#ifdef WEBENGINE_AVAILABLE
    m_outputPath = outputPath;
    m_retryCount = 0;

    emit progressChanged(10);
    updateStatus("Загрузка HTML контента...");

    m_webPage->setHtml(htmlContent, QUrl("file:///"));

#else
    // Если QWebEngine недоступен, сообщаем об ошибке
    emit conversionFinished(false,
                            "Продвинутый конвертер недоступен в этой сборке. "
                            "Соберите приложение с поддержкой QWebEngine.");
#endif
}


#ifdef WEBENGINE_AVAILABLE
void advancedconverter::onLoadFinished(bool success)
{
    if (!success) {
        if (m_retryCount < 2) {
            m_retryCount++;
            m_loadTimer->singleShot(500, [this]() {
                emit progressChanged(20 + m_retryCount * 10);
                updateStatus(QString("Повторная загрузка (%1/2)...").arg(m_retryCount));
                m_webPage->triggerAction(QWebEnginePage::Reload);
            });
            return;
        }

        emit conversionFinished(false, "Не удалось загрузить HTML контент");
        return;
    }

    emit progressChanged(50);
    updateStatus("HTML загружен, подготовка к конвертации...");

    m_loadTimer->singleShot(1500, this, [this]() {
        emit progressChanged(70);
        updateStatus("Генерация PDF...");

        // Настройка layout для PDF
        QPageLayout pageLayout(QPageSize(QPageSize::A4),
                               QPageLayout::Portrait,
                               QMarginsF(10, 10, 10, 10));

        // Печатаем в PDF
        m_webPage->printToPdf([this](const QByteArray &pdfData) {
            this->onPdfPrintingFinished(pdfData, !pdfData.isEmpty());
        }, pageLayout);
    });
}


void advancedconverter::onPdfPrintingFinished(const QByteArray &pdfData, bool success)
{
    if (success && !pdfData.isEmpty()) {
        QFile file(m_outputPath);
        if (file.open(QIODevice::WriteOnly)) {
            qint64 bytesWritten = file.write(pdfData);
            file.close();

            if (bytesWritten > 0) {
                emit progressChanged(100);
                emit conversionFinished(true,
                                        QString("PDF успешно создан: %1\nРазмер: %2 байт")
                                            .arg(m_outputPath)
                                            .arg(bytesWritten));
            } else {
                emit conversionFinished(false, "Ошибка записи PDF файла");
            }
        } else {
            emit conversionFinished(false,
                                    QString("Не удалось открыть файл для записи: %1").arg(m_outputPath));
        }
    } else {
        emit conversionFinished(false, "Ошибка генерации PDF");
    }
}

void advancedconverter::updateStatus(const QString &message)
{
    qDebug() << "AdvancedConverter:" << message;
}
#endif
