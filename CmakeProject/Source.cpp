#include <iostream>
#include <memory>
#include <vector>
#include <random>
#include <algorithm>
#include <numeric>
#include <chrono>
#include "omp.h"
#include "Circle.h"
#include "Ellipse.h"
#include "Helix.h"

using Time = std::chrono::high_resolution_clock;

void GenerateRandomFigures(std::vector<std::shared_ptr<Figure>>& figures, size_t countFigures) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distForTypeFigure(0, 2);
    std::uniform_real_distribution<> distForArgs(0.5, 5.0);
    for (size_t i = 0; i < countFigures; ++i) {
        int typeFigure = distForTypeFigure(gen);
        switch (typeFigure) {
            case 0: figures.push_back(std::make_shared<Circle>(distForArgs(gen))); break;
            case 1: figures.push_back(std::make_shared<Ellipse>(distForArgs(gen), distForArgs(gen))); break;
            case 2: figures.push_back(std::make_shared<Helix>(distForArgs(gen), static_cast<int>(distForArgs(gen)))); break;
            default: break;
        }
    }
}

void PrintInfoFigure(const std::shared_ptr<Figure>& figure) {
    NameFigure typeFigure = figure->GetNameFigure();
    if (typeFigure == NameFigure::Circle) {
        auto temp = static_pointer_cast<Circle>(figure);
        std::cout << "Circle:  radius " << temp->GetRadius() << "\n";
    } else if (typeFigure == NameFigure::Ellipse) {
        auto temp = static_pointer_cast<Ellipse>(figure);
        std::cout << "Ellipse: radiusX " << temp->GetRadiusX() << " radiusY " << temp->GetRadiusY() << "\n";
    } else if (typeFigure == NameFigure::Helix) {
        auto temp = static_pointer_cast<Helix>(figure);
        std::cout << "Helix:   radius " << temp->GetRadius() << " step " << temp->GetStep() << "\n";
    }
}

int main() {
    size_t countFigures = 10000000;
    std::vector<std::shared_ptr<Figure>> figures;
    try {
        GenerateRandomFigures(figures, countFigures);
    } catch (std::invalid_argument ex) {
        std::cerr << ex.what() << std::endl;
        std::abort();
    } catch (std::exception ex) {
        std::cerr << ex.what() << std::endl;
    }
    if (figures.size() < 30) {
        for (const auto& figure : figures) {
            PrintInfoFigure(figure);
            std::cout << "point from t = PI/4: " << figure->GetPoint(M_PI / 4) << std::endl;
            std::cout << "first derivative t = PI/4: " << figure->GetFirstDerivative(M_PI / 4) << std::endl;
            std::cout << std::endl;
        }
    }
    std::vector<std::shared_ptr<Circle>> circles;
    for (const auto& figure : figures) {
        std::shared_ptr<Circle> circle = dynamic_pointer_cast<Circle>(figure);
        if (circle.get() != nullptr) {
            circles.push_back(circle);
        }
    }
    std::sort(circles.begin(), circles.end(),
        [](std::shared_ptr<Circle> first, std::shared_ptr<Circle> second) -> bool { return first->GetRadius() < second->GetRadius(); });

    auto duration = [](auto start, auto finish) { return std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count(); };
    double sumSeq = 0.0;
    auto startSeq = Time::now();
    for (const auto& circle : circles) {
        sumSeq += circle->GetRadius();
    }
    auto finishSeq = Time::now();

    int countThreads;
    double sumOMP = 0.0;
    double partSum = 0.0;
#pragma omp parallel
    {
#pragma omp master
        { countThreads = omp_get_num_threads(); }
    }
    omp_set_num_threads(countThreads);

    auto startOMP = Time::now();
#pragma omp parallel private(partSum)
    {
#pragma omp for
        for (int i = 0; i < circles.size(); ++i) {
            partSum += circles[i]->GetRadius();
        }
#pragma omp critical
        { 
            sumOMP += partSum;
        }
    }
    auto finishOMP = Time::now();
    std::cout << "\nCount circles: " << circles.size() << std::endl;
    std::cout << "Count threads: " << countThreads << std::endl;
    std::cout << "Time seq sum    " << duration(startSeq, finishSeq) << " microseconds"<< std::endl;
    std::cout << "Time openMP sum " << duration(startOMP, finishOMP) << " microseconds" << std::endl;
    std::cout << "Sum seq:    " << sumSeq << std::endl;
    std::cout << "Sum OpenMP: " << sumOMP << std::endl;
    return 0;
}