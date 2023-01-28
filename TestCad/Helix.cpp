#include "Helix.h"

Helix::Helix(double _radius, int _step) {
    if (_radius <= 0) {
        throw std::invalid_argument("Initialize helix with negative or 0 radius ");
    }
    if (_step < 0) {
        throw std::invalid_argument("Initialize helix with negative");
    }
    nameFigure = NameFigure::Helix;
    radius = _radius;
    step = _step;
}

void Helix::SetRadius(double newRadius) {
    if (newRadius <= 0) {
        throw std::invalid_argument("Radius must be positive in helix");
    }
    nameFigure = NameFigure::Helix;
    radius = newRadius;
}

double Helix::GetRadius() const {
    return radius;
}

void Helix::SetStep(int newStep) {
    if (newStep < 0) {
        throw std::invalid_argument("Step must'n be negative in helix");
    }
    step = newStep;
}

int Helix::GetStep() const {
    return step;
}

Point Helix::GetPoint(double t) const {
    if (t >= 2 * M_PI) {
        t = std::fmod(t, M_PI * 2);
    }
    return Point(radius * cos(t), radius * sin(t), step * 2 * M_PI + t);
}

Point Helix::GetFirstDerivative(double t) const {
    if (t >= 2 * M_PI) {
        t = std::fmod(t, M_PI * 2);
    }
    return Point(-radius * sin(t), radius * cos(t), 1);
}
