#pragma once
#include "Figure.h"

class Ellipse:public Figure {
private:
    double radiusX;
    double radiusY;
    Point centerPoint;
public:
    explicit Ellipse(double _radiusX, double _radiusY);
    explicit Ellipse(double _radiusX, double _radiusY, Point _centerPoint);
    void SetCenterPoint(const Point& newCenterPoint);
    Point GetCenterPoint() const;
    void SetRadiusX(double newRadiusX);
    double GetRadiusX() const;
    void SetRadiusY(double newRadiusY);
    double GetRadiusY() const;
    virtual Point GetPoint(double t) const override;
    virtual Point GetFirstDerivative(double t) const override;
};
