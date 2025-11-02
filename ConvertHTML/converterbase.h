#ifndef CONVERTERBASE_H
#define CONVERTERBASE_H

#include <QObject>
#include <QString>

class ConverterBase : public QObject
{
    Q_OBJECT

public:
    enum ConverterType {
        Basic,
        Advanced,
    };

    explicit ConverterBase(QObject *parent = nullptr) : QObject(parent) {}
    virtual ~ConverterBase() = default;

    virtual ConverterType type() const = 0;
    virtual QString name() const = 0;
    virtual QString description() const = 0;

public slots:
    virtual void convert(const QString &htmlContent, const QString &outputPath) = 0;

signals:
    void conversionFinished(bool success, const QString &message);
    void progressChanged(int percent);
};

#endif
