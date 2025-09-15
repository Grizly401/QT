#include "quadratic.h"
#include <cmath>

Quadratic::Quadratic() :
    Formul(3, QString("f(x) = A*(x*x) + B*x + C"))
{}


double Quadratic::calculate(double x) {
    return this->_parameters[0] * x * x
           + this->_parameters[1] * x
           + this->_parameters[2];
}
