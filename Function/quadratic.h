#ifndef QUADRATIC_H
#define QUADRATIC_H

#include "formul.h"

class Quadratic : public Formul
{
public:
    Quadratic();
    double calculate (double x) override;
};

#endif // QUADRATIC_H
