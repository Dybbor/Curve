#if 1
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

void GenerateRandomFigures(std::vector<std::shared_ptr<Figure>>& figures, size_t countFigures) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distForTypeFigure(0, 2);
    std::uniform_real_distribution<> distForArgs(0.5, 10.0);
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

void PrintFigures(const std::vector<std::shared_ptr<Figure>>& figures) {
    for (const auto& figure : figures) {
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

void PrintTimeSum(const std::vector<std::shared_ptr<Circle>>& circles) {
    double sum = 0.0;
    auto startSum = std::chrono::high_resolution_clock::now();
    for (const auto& circle : circles) {
        sum += circle->GetRadius();
    }
    auto finishSum = std::chrono::high_resolution_clock::now();

    double reduce = 0.0;
    auto startReduce = std::chrono::high_resolution_clock::now();
    std::reduce(circles.cbegin(), circles.cend(), 0.0,
        [](double prevSum, std::shared_ptr<Circle> circle) -> double { return prevSum + circle->GetRadius(); });
    auto finishReduce = std::chrono::high_resolution_clock::now();

    double accaumulate = 0.0;
    auto startAccumulate = std::chrono::high_resolution_clock::now();
    std::accumulate(circles.cbegin(), circles.cend(), 0.0,
        [](double prevSum, std::shared_ptr<Circle> circle) -> double { return prevSum + circle->GetRadius(); });
    auto finishAccumulate = std::chrono::high_resolution_clock::now();

    omp_set_num_threads(12);
    double sumOMP = 0.0;
    auto startOMP = std::chrono::high_resolution_clock::now();
#pragma omp parallel for
    for (int i = 0; i < circles.size(); ++i) {
        sumOMP += circles[i]->GetRadius();
    }
    auto finishOMP = std::chrono::high_resolution_clock::now();


    auto duration = [](auto start, auto finish) {
        return std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();
    };
    std::cout << "count " << circles.size() << std::endl;
    std::cout << "sum : " << sum << " " << reduce << " " << accaumulate << " " << sumOMP << std::endl;
    std::cout << "time sum:  " << duration(startSum, finishSum) << std::endl;
    std::cout << "reduce     " << duration(startReduce, finishReduce) << std::endl;
    std::cout << "accumulate " << duration(startAccumulate, finishAccumulate) << std::endl;
    std::cout << "Openmp     " << duration(startOMP, finishOMP) << std::endl;
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
    /* for (const auto& figure : figures) {
         PrintInfoFigure(figure);
         std::cout << "point from t = PI/4: " << figure->GetPoint(M_PI / 4) << std::endl;
         std::cout << "first derivative t = PI/4: " << figure->GetFirstDerivative(M_PI / 4) << std::endl;
         std::cout << std::endl;
     }*/
    std::vector<std::shared_ptr<Circle>> circles;
    for (const auto figure : figures) {
        std::shared_ptr<Circle> circle = dynamic_pointer_cast<Circle>(figure);
        if (circle.get() != nullptr) {
            circles.push_back(circle);
        }
    }
    // or we can use copy_if, but then need to add a lot of cast figure->circle in future
    // std::copy_if(figures.begin(), figures.end(), std::back_inserter(circles), [](std::shared_ptr<Figure>& figure) -> bool {
    //    std::shared_ptr<Circle> temp = dynamic_pointer_cast<Circle>(figure);
    //    return temp.get() != nullptr;
    //});
    /* if (circles.size() > 0) {
         std::cout << "Before sort " << std::endl;
         for (const auto& circle : circles) {
             std::cout << "Circle:  radius " << circle->GetRadius() << "\n";
         }
         std::sort(circles.begin(), circles.end(),
             [](std::shared_ptr<Circle> first, std::shared_ptr<Circle> second) -> bool {
                 return first->GetRadius() < second->GetRadius();
             });
         std::cout << "After sort " << std::endl;
         for (const auto& circle : circles) {
             std::cout << "Circle:  radius " << circle->GetRadius() << "\n";
         }
     } else {
         std::cout << "circles does't exist in first container" << std::endl;
     }*/
    // double sum = 0;
    // for (const auto& circle : circles) {
    //     sum += circle->GetRadius();
    // }
    // double next_sum = std::reduce(circles.cbegin(), circles.cend(), 0.0, [](double prevSum, std::shared_ptr<Circle> circle) -> double {
    //     return prevSum + circle->GetRadius();
    //     });
    // std::cout << "sum radiuses of circles: " << next_sum << std::endl;
    PrintTimeSum(circles);
    return 0;
}

#endif

// test shared_ptr
#if 0
#include <iostream>
#include <vector>
#include <memory>

class MyClassA{
private:
    int a;

public:
    MyClassA(int _a) {
        a = _a;
        std::cout << "I was created " << a << std::endl;
    }
    ~MyClassA() { std::cout << "Now i am destroyed" << a <<std::endl;
    }
    void Print() { std::cout << "Print " << a << std::endl;
    }
    static void Except() { throw std::exception("asdf"); }
};

int main() {
    std::vector<MyClassA*> v;
    v.push_back(new MyClassA(3));
    //std::shared_ptr<MyClassA> test(a);
    //std::vector<std::shared_ptr<MyClassA>> v;
    //v.push_back(std::make_unique<MyClassA>(2));
    //v.push_back(std::make_unique<MyClassA>(1));
    //std::vector<std::shared_ptr<MyClassA>> another;
    //for (const auto& el : v) {
    //    another.push_back(el);
    //}
    //try {
    //    MyClassA::Except();
    //} catch (std::exception ex) {
    //    std::cout << ex.what() << std::endl;
    //    return -1;
    //}
    //std::swap(another[0], another[1]);
    //for (const auto& el : another) {
    //    el->Print();
    //}
    return 0;
}
#endif
