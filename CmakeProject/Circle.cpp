#include "Circle.h"

Circle::Circle(double _radius) {
    if (_radius <= 0) {
        throw std::invalid_argument("Initialize circle with negative or 0 radius");
    }
    nameFigure = NameFigure::Circle;
    radius = _radius;
    centerPoint.x = 0;
    centerPoint.y = 0;
    centerPoint.z = 0;
}

Circle::Circle(double _radius, Point _center) {
    if (_radius <= 0) {
        throw std::invalid_argument("Initialize circle with negative or 0 radius");
    }
    if (_center.z != 0) {
        throw std::invalid_argument("Z coordinate must be zero in circle");
    }
    nameFigure = NameFigure::Circle;
    radius = _radius;
    centerPoint = _center;
}

void Circle::SetRadius(double newRadius) {
    if (newRadius <= 0) {
        throw std::invalid_argument("Radius must be positive in circle");
    }
    radius = newRadius;
}

double Circle::GetRadius() const {
    return radius;
}

void Circle::SetCenterPoint(const Point& newCenterPoint) {
    if (newCenterPoint.z != 0) {
        throw std::invalid_argument("Z coordinate must be zero in circle");
    }
    centerPoint = newCenterPoint;
}

Point Circle::GetCenterPoint() const {
    return centerPoint;
}

Point Circle::GetPoint(double t) const {
    if (t > 2 * M_PI) {
        t = std::fmod(t, 2 * M_PI);
    }
    return Point(radius * cos(t) + centerPoint.x, radius * sin(t) + centerPoint.y, 0);
}

Point Circle::GetFirstDerivative(double t) const {
    if (t > 2 * M_PI) {
        t = std::fmod(t, 2 * M_PI);
    }
    return Point(-radius * sin(t), radius * cos(t), 0);
}