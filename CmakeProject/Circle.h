#pragma once
#include "Figure.h"

class Circle : public Figure {
private:
    double radius;
    Point centerPoint;
public:
    explicit Circle(double _radius);
    explicit Circle(double _radius, Point _centerPoint);
    void SetRadius(double newRadius);
    double GetRadius() const;
    void SetCenterPoint(const Point& newCenterPoint);
    Point GetCenterPoint() const;
    virtual Point GetPoint(double t) const override;
    virtual Point GetFirstDerivative(double t) const override;
};
