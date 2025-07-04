#include <iostream>
#include <chrono>

#include "InvertedIndex.h"
#include "DocumentProcessor.h"
#include "SearchEngine.h"

#define STOPWORDS_FILE "data/stopwords_english.dat.txt"
#define DOCUMENTS_FILE "data/gov2_pages.dat"

int main() {
    std::cout << "[MAIN] Iniciando motor de busqueda..." << std::endl;
    // 1) iniciar objetos
    DocumentProcessor procesadorDocumentos;
    InvertedIndex invertedIndex;
    SearchEngine searchEngine(&invertedIndex, &procesadorDocumentos);
    
    // 2) cargar stopwrods
    std::cout << "[MAIN] Cargando stopwords..." << std::endl;
    procesadorDocumentos.loadStopWords(STOPWORDS_FILE);

    // 3) cargar documetnos
    std::cout << "[MAIN] Cargando y procesando documentos (" << DOCUMENTS_FILE << ")..." << std::endl;
    auto start_time = std::chrono::high_resolution_clock::now();
    procesadorDocumentos.loadAndProcessDocuments(DOCUMENTS_FILE, invertedIndex);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "[MAIN] Tiempo de carga y procesamiento: " << duration.count() << " ms" << std::endl;

    // 4) hacer consultas
    std::cout << "-----Busqueda de documentos-----" << std::endl;
    std::cout << "Ingrese una consulta: (o 'salir' para terminar)" << std::endl;

    std::string lineaQuery;
    while (std::getline(std::cin, lineaQuery) && lineaQuery !="salir") {
        if (lineaQuery.empty()) {
            std::cout << "Porfavor, ingrese una consulta..." << std::endl;
            continue;
        }

        std::cout << "\nBuscando " << lineaQuery << "..." << std::endl;
        start_time = std::chrono::high_resolution_clock::now();
        LinkedList<int>* results = searchEngine.performQuery(lineaQuery);
        end_time = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

        if (results && results->getSize() > 0) {
            std::cout << "Documentos encontrados: (" << results->getSize() << "): [";
            Node<int>* actual = results->getHead();
            while (actual != nullptr) {
                std::cout << "Doc-" << actual->data;
                if (actual->next != nullptr) {
                    std::cout << ", ";
                }
                actual = actual->next;
            }
            std::cout << "]" << std::endl;
        } else {
            std::cout << "No se encontraron documentos para la consulta" << std::endl;
        }

        std::cout << "Tiempo de busqueda: " << duration.count() << " microsegundos" << std::endl;

        if(results) {
            delete results;
            results = nullptr;
        }

        std::cout << "\nIngrese una consulta: (o 'salir' para terminar)" << std::endl;
    }

    std::cout << "\nFin del programa" << std::endl;
    return 0;
}