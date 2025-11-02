#include "basicconverter.h"
#include <QFile>
#include <QTextStream>
#include <QTextDocument>
#include <QPrinter>
#include <QPageSize>
#include <QMarginsF>

basicconverter::basicconverter(QObject *parent)
    : ConverterBase(parent)
    , m_document(new QTextDocument(this))
{
}

void basicconverter::convert(const QString &htmlContent, const QString &outputPath)
{
    try {
        emit progressChanged(10);

        m_document->setHtml(htmlContent);

        emit progressChanged(50);

        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(outputPath);
        printer.setPageSize(QPageSize(QPageSize::A4));
        printer.setPageMargins(QMarginsF(15, 15, 15, 15));

        emit progressChanged(80);

        m_document->print(&printer);

        emit progressChanged(100);
        emit conversionFinished(true, QString("PDF успешно создан: %1").arg(outputPath));

    } catch (const std::exception &e) {
        emit conversionFinished(false, QString("Ошибка конвертации: %1").arg(e.what()));
    }
}
