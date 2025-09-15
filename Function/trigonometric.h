#ifndef TRIGONOMETRIC_H
#define TRIGONOMETRIC_H

#include "formul.h"

class Trigonometric : public Formul
{
public:
    Trigonometric();
    double calculate (double x) override;
};

#endif // TRIGONOMETRIC_H
