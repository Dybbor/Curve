#include "Ellipse.h"

Ellipse::Ellipse(double _radiusX, double _radiusY) {
    if (_radiusX <= 0 || _radiusY <= 0) {
        throw std::invalid_argument("Initialize circle with negative or 0 radius");
    }
    nameFigure = NameFigure::Ellipse;
    radiusX = _radiusX;
    radiusY = _radiusY;
    centerPoint = Point(0, 0, 0);
}

Ellipse::Ellipse(double _radiusX, double _radiusY, Point _centerPoint) {
    if (_radiusX <= 0 || _radiusY <= 0) {
        throw std::invalid_argument("Initialize circle with negative or 0 radius");
    }
    if (_centerPoint.z != 0) {
        throw std::invalid_argument("Z coordinate in ellipse must be zero");
    }
    nameFigure = NameFigure::Ellipse;
    radiusX = _radiusX;
    radiusY = _radiusY;
    centerPoint = _centerPoint;
}

void Ellipse::SetCenterPoint(const Point& newCenterPoint) {
    if (newCenterPoint.z != 0) {
        throw std::invalid_argument("Z coordinate in ellipse must be zero");
    }
    centerPoint = newCenterPoint;
}

Point Ellipse::GetCenterPoint() const {
    return centerPoint;
}

void Ellipse::SetRadiusX(double newRadiusX) {
    if (newRadiusX <= 0) {
        throw std::invalid_argument("RadiusX must be positive in ellipse");
    }
    radiusX = newRadiusX;
}

double Ellipse::GetRadiusX() const {
    return radiusX;
}

void Ellipse::SetRadiusY(double newRadiusY) {
    if (newRadiusY <= 0) {
        throw std::invalid_argument("RadiusY must be positive in ellipse");
    }
    radiusY = newRadiusY;
}

double Ellipse::GetRadiusY() const {
    return radiusY;
}

Point Ellipse::GetPoint(double t) const {
    if (t > M_PI * 2) {
        t = std::fmod(t, M_PI*2);
    }
    return Point(radiusX * cos(t) + centerPoint.x, radiusY * sin(t) + centerPoint.y, 0);
}

Point Ellipse::GetFirstDerivative(double t) const {
    if (t > M_PI * 2) {
        t = std::fmod(t, M_PI * 2);
    }
    return Point(-radiusX * sin(t), radiusY * cos(t), 0);
}
