#ifndef PLOTBUILDER_H
#define PLOTBUILDER_H

#include <QObject>
#include "plotbuilderworker.h"

class Formul;

class PlotBuilder : public QObject
{
    Q_OBJECT
public:
    explicit PlotBuilder(QObject *parent = 0);
    ~PlotBuilder();

    void setRange(double valueFrom, double valueTo);
    void setStep(double step);
    void setFunction(Formul* function);
    bool isProcessing() {return _isProcessing;}

signals:
    void started();
    void finished();
    void paused();
    void resumed();
    void processed(double, double, double);

public slots:
    void start();
    void pause();
    void resume();
    void stop();

private:
    bool _isProcessing;
    PlotBuilderWorker* _worker;

};

#endif // PLOTBUILDER_H
