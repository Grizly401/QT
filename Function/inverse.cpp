#include "inverse.h"
#include <limits>
#include <cmath>

Inverse::Inverse():
    Formul(2, QString("f(x) = A / ( sin(x*x) * B )"))
{}

double Inverse::calculate (double x){
    double divisor = sin(x * x) * this->_parameters[1];

    if (divisor == 0) {
        double inf = std::numeric_limits<double>::infinity();
        return this->_parameters[0] >= 0    // that's weird... a bit. I don't know what 0/0 should mean in current context
                   ? inf
                   : -inf;

    } else {
        return this->_parameters[0] / divisor;
    }
}
