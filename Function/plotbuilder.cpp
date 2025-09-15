#include "plotbuilder.h"
#include <QThread>

PlotBuilder::PlotBuilder(QObject *parent) :
    QObject(parent),
    _isProcessing(false),
    _worker(new PlotBuilderWorker(this))
{
    QThread* thread = new QThread;
    _worker->moveToThread(thread);

    connect(this, SIGNAL(started()), _worker, SLOT(process()));
    connect(this, SIGNAL(resumed()), _worker, SLOT(process()));
    connect(_worker, SIGNAL(processed(double, double, double)), this, SIGNAL(processed(double, double, double)));
    connect(_worker, SIGNAL(finished()), this, SLOT(stop()));

    thread->start();
}

PlotBuilder::~PlotBuilder()
{
    delete _worker;
}

void PlotBuilder::setRange(double valueFrom, double valueTo)
{
    if(valueFrom > valueTo) {
        throw new std::runtime_error("End value must be greater than start value");
    }

    this->_worker->setValueFrom(valueFrom);
    this->_worker->setValueTo(valueTo);
}

void PlotBuilder::setStep(double step)
{
    if(step <= 0) {
        throw new std::runtime_error("Step value must be greater than zero");
    }

    _worker->setStep(step);
}

void PlotBuilder::setFunction(Formul *function)
{
    if(function == NULL) {
        throw new std::runtime_error("NULL function given");
    }

    _worker->setFunction(function);
}

void PlotBuilder::start()
{
    if(!isProcessing()) {
        _worker->reset();
        _isProcessing = true;

        emit started();
    }
}

void PlotBuilder::stop()
{
    _isProcessing = false;

    emit finished();
}

void PlotBuilder::pause()
{
    _isProcessing = false;

    emit paused();
}

void PlotBuilder::resume()
{
    _isProcessing = true;

    emit resumed();
}
