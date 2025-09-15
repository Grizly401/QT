#ifndef PLOTBUILDERWORKER_H
#define PLOTBUILDERWORKER_H

#include <QObject>

class PlotBuilder;
class Formul;

class PlotBuilderWorker : public QObject
{
    Q_OBJECT
public:
    PlotBuilderWorker(PlotBuilder * manager);
    void setValueFrom(double valueFrom) { _valueFrom = valueFrom; }
    void setValueTo(double valueTo) { _valueTo = valueTo; }
    void setStep(double step) { _step = step; }
    void reset() { _valueCurrent = _valueFrom; }
    void setFunction(Formul *function) { _function = function; }

signals:
    void processed(double, double, double);
    void finished();

public slots:
    void process();

private:
    PlotBuilder * _manager;
    Formul * _function;

    double _valueFrom;
    double _valueTo;
    double _valueCurrent;
    double _step;

    double getProgress();
};

#endif // PLOTBUILDERWORKER_H
