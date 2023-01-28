#pragma once
#include "Figure.h"

class Helix : public Figure {
private:
    double radius;
    int step;
public:
    Helix(double _radius, int _step);
    void SetRadius(double newRadius);
    double GetRadius() const;
    void SetStep(int step);
    int GetStep() const;
    /**
    * One round - 2*PI 
    * So any point on helix satisfies the condition C(t + 2*PI) = C(t) + {0, 0, step},
    * where 0 <= t <= 2 * PI
    */
    virtual Point GetPoint(double t) const override;
    virtual Point GetFirstDerivative(double t) const override;
};
