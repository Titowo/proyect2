#include "PageRankCalculator.h"

#include <cmath>
#include <limits>
#include <iostream>

PageRankCalculator::PageRankCalculator(const std::map<int, std::set<int>>& grafo, const std::set<int>& todosNodos) 
    : grafo(grafo), todosNodos(todosNodos), FACTOR_AMORTIGUACION(0.85), UMBRAL_CONVERGENCIA(1e-6), MAX_ITERACIONES(100) {
        
}

int PageRankCalculator::getGradoSalida(int nodeId) const {
    auto it = grafo.find(nodeId);
    if (it != grafo.end()) {
        return it->second.size(); //el tamano de adyacencia es el gradjo
    }
    return 0;
}

// implementacion de algoritmo de pagerank
std::map<int, double> PageRankCalculator::calculatePageRank() {
    std::map<int, double> pageRankScores;
    std::map<int, double> newPageRankScores;
    int N = todosNodos.size();
    
    if (N == 0) {
        std::cerr << "Advertencia: No se puede calcular el PageRank sin nodos" << std::endl;
        return pageRankScores;
    }

    // 1) inicializacion = asignar un pagerank inicial uniforme a todos los nodos (1/N)
    double initialPR = 1.0 / N;
    for (int nodeId : todosNodos) {
        pageRankScores[nodeId] = initialPR;
    }

    int iteraciones = 0; // contador de iteraciones
    double delta = std::numeric_limits<double>::max(); // diferencia entre pagerank actual y nuevo

    while (iteraciones < MAX_ITERACIONES && delta > UMBRAL_CONVERGENCIA) {
        iteraciones++;
        delta = 0.0;
        double colgantesPageRankScores = 0.0;

        for (int nodeId : todosNodos) {
            if (getGradoSalida(nodeId) == 0) {
                colgantesPageRankScores += pageRankScores[nodeId];
            }
        }

        for (int i : todosNodos) {
            double nuevoPRsum = 0.0;

            for (int j : todosNodos) {
                auto it_j = grafo.find(j);
                if (it_j != grafo.end() && it_j->second.count(i)) {
                    int grado_j = getGradoSalida(j);
                    if (grado_j > 0) {
                        nuevoPRsum += pageRankScores[j] / grado_j;
                    }
                }
            }
            
            nuevoPRsum += colgantesPageRankScores / N;

            newPageRankScores[i] = (1.0 - FACTOR_AMORTIGUACION) / N + FACTOR_AMORTIGUACION * nuevoPRsum;
        }

        for (int nodeId : todosNodos) {
            delta += std::abs(newPageRankScores[nodeId] - pageRankScores[nodeId]);
            pageRankScores[nodeId] = newPageRankScores[nodeId];
        }
    }

    std::cout << "PageRank convergio en " << iteraciones << " iteraciones. Delta final: " << delta << std::endl;
    return pageRankScores;
}