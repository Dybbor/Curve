#pragma once
#include <math.h>
#include <ostream>

#define M_PI 3.14159265358979323846

struct Point {
    double x = 0;
    double y = 0;
    double z = 0;
    friend std::ostream& operator<<(std::ostream& os, const Point& point) {
        os << "(" << point.x << ", " << point.y << ", " << point.z << ")";
        return os;
    }
};

enum class NameFigure {
    Circle,
    Ellipse,
    Helix
};

class Figure {
protected:
    NameFigure nameFigure;
public:
    NameFigure GetNameFigure() { return nameFigure; }
    virtual Point GetPoint(double t) const = 0;
    virtual Point GetFirstDerivative(double t) const = 0;
};
