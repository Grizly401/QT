#ifndef INVERSE_H
#define INVERSE_H
#include "formul.h"

class Inverse : public Formul
{
public:
    Inverse();

    double calculate (double x) override;
};

#endif // INVERSE_H
