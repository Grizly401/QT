#ifndef LOGARITHMIC_H
#define LOGARITHMIC_H
#include "formul.h"

class Logarithmic : public Formul
{
public:
    Logarithmic();
    double calculate (double x) override;
};

#endif // LOGARITHMIC_H
