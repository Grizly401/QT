#ifndef BASICCONVERTER_H
#define BASICCONVERTER_H

#include "converterbase.h"
#include <QTextDocument>

class basicconverter : public ConverterBase
{
    Q_OBJECT
public:

    explicit basicconverter(QObject *parent = nullptr);

    ConverterType type() const override { return Basic; }
    QString name() const override { return "Быстрый конвертер"; }
    QString description() const override {
        return "Использует QTextDocument. Быстрая конвертация с ограниченной поддержкой CSS.";
    }

public slots:
    void convert(const QString &htmlContent, const QString &outputPath) override;

private:
    QTextDocument *m_document;

};

#endif
