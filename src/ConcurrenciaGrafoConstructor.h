#ifndef CONCURRENCIA_GRAFO_CONSTRUCTOR_H
#define CONCURRENCIA_GRAFO_CONSTRUCTOR_H

#include "LinkedList.h"
#include <vector>
#include <set>
#include <map>
#include <iostream>

class ConcurrenciaGrafoConstructor {
public:
    ConcurrenciaGrafoConstructor();
    ~ConcurrenciaGrafoConstructor();

    void addQueryResultado(const LinkedList<int>* docIds);

    void printGrafo() const;

    const std::map<int, std::set<int>>& getGrafo() const { return grafo; }
    const std::set<int>& getTodosNodos() const { return todosNodos; }

private:
    // representacion del grafo: lista de adyacencia
    // map<ID_documento, set<ID_documento_adyacente>>
    std::map<int, std::set<int>> grafo;

    // conjunto de los id del documento que han aparecido en alguna consulta
    std::set<int> todosNodos;

};

#endif