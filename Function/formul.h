#ifndef FORMUL_H
#define FORMUL_H


#include <QString>
#include <QVector>

class Formul
{
public:
    Formul(int parametersCount, QString functionString);

    virtual double calculate (double x) = 0;

    const inline QVector<double> &getParams() const { return this->_parameters; }
    const inline QString getFunctionString() const { return this->_functionString; }

    void setParameter(int parameterNumber, double value);

    void setParameters(const QVector<double> & parameters);

protected:
    QVector<double> _parameters;    // function's parameters (A, B, C, etc)
    QString _functionString;
};

#endif // FORMUL_H
