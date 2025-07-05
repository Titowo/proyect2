#ifndef PAGERANKCALCULATOR_H
#define PAGERANKCALCULATOR_H

#include <map>
#include <set>
#include <vector>

class PageRankCalculator {
public:
    PageRankCalculator(const std::map<int, std::set<int>>& grafo, const std::set<int>& todosNodso);
    std::map<int, double> calculatePageRank();

private:
    const std::map<int, std::set<int>>& grafo;
    const std::set<int>& todosNodos;

    const double FACTOR_AMORTIGUACION;
    const double UMBRAL_CONVERGENCIA;
    const int MAX_ITERACIONES;

    int getGradoSalida(int nodeId) const;
};

#endif