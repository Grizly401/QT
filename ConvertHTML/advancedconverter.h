#ifndef ADVANCEDCONVERTER_H
#define ADVANCEDCONVERTER_H

#include "converterbase.h"

#ifdef WEBENGINE_AVAILABLE
#include <QWebEnginePage>
#include <QTimer>
#endif


class advancedconverter : public ConverterBase
{
    Q_OBJECT
public:
    explicit advancedconverter(QObject *parent = nullptr);
    ~advancedconverter();

    ConverterType type() const override { return Advanced; }
    QString name() const override { return "Продвинутый конвертер"; }
    QString description() const override {
        return "Использует QWebEngine. Полная поддержка CSS, JavaScript и современных веб-стандартов.";
    }

public slots:
    void convert(const QString &htmlContent, const QString &outputPath) override;

private slots:
#ifdef WEBENGINE_AVAILABLE
    void onLoadFinished(bool success);
    void onPdfPrintingFinished(const QByteArray &pdfData, bool success);
#endif

private:
#ifdef WEBENGINE_AVAILABLE
    QWebEnginePage *m_webPage;
    QString m_outputPath;
    QTimer *m_loadTimer;
    int m_retryCount;
    void updateStatus(const QString &message);
#endif
};

#endif
