#include "ConcurrenciaGrafoConstructor.h" 
#include <algorithm>


ConcurrenciaGrafoConstructor::ConcurrenciaGrafoConstructor() {
    // constructor vacio
}

ConcurrenciaGrafoConstructor::~ConcurrenciaGrafoConstructor() {
    // destructor vacio
}

void ConcurrenciaGrafoConstructor::addQueryResultado(const LinkedList<int>* docIds) {
    if (!docIds || docIds->getSize() < 2 ) {
        return;
    }

    std::vector<int> actualDocIds;
    Node<int>* actual = docIds->getHead();
    while (actual != nullptr) {
        actualDocIds.push_back(actual->data);
        todosNodos.insert(actual->data);
        actual = actual->next;
    }

    std::sort(actualDocIds.begin(), actualDocIds.end());

    for (size_t i = 0; i < actualDocIds.size(); ++i) {
        for (size_t j = i + 1; j < actualDocIds.size(); ++j) {
            int doc1 = actualDocIds[i];
            int doc2 = actualDocIds[j];

            // aniadir arista de doc1 a doc2
            grafo[doc1].insert(doc2);
            // aniadir arista de doc2 a doc1 (grafo no dirigido)
            grafo[doc2].insert(doc1);

            // std::cout << "[DEBUG] Relacion agregada: " << doc1 << "<->" << doc2 << std::endl;
        }
    }
} 

void ConcurrenciaGrafoConstructor::printGrafo() const {
    std::cout << "\n--- Grafo de Co-ocurrencia ---" << std::endl;
    for (const auto& par : grafo) { 
        std::cout << "\nDocumento " << par.first << " conectado con: ";
        for (int vecino : par.second) { 
            std::cout << vecino << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "--------------------------------" << std::endl;
}