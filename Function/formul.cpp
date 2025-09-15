#include "formul.h"
#include <stdexcept>

Formul::Formul(int parametersCount, QString functionString)
{
    _parameters.fill(0, parametersCount);
    _functionString = functionString;
}


void Formul::setParameter(int parameterNumber, double value){

    if(parameterNumber > _parameters.size() || parameterNumber < 0) {
        throw new std::out_of_range("Invalid parameter number");
    }

    _parameters[parameterNumber] = value;

}

void Formul::setParameters(const QVector<double> & parameters){

    if(_parameters.size() != parameters.size()) {
        throw new std::runtime_error("Unsupported number of function prameters provided");
    }

    for(int i = 0; i < _parameters.size(); i++)
    {
        setParameter(i, parameters[i]);
    }

}
