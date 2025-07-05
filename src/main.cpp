#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <chrono>
#include <algorithm>
#include <map>

#include "InvertedIndex.h"
#include "ConcurrenciaGrafoConstructor.h"
#include "LinkedList.h"
#include "DocumentProcessor.h"
#include "SearchEngine.h"
#include "PageRankCalculator.h"

#define STOPWORDS_FILE "data/stopwords_english.dat.txt"
#define DOCUMENTS_FILE "data/gov2_pages.dat"

std::string readFileContent(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "[ERROR] No se pudo abrir el archivo: " << filepath << std::endl;
            return "";
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
}

int main() {
    std::cout << "[MAIN] Iniciando motor de busqueda..." << std::endl;
    // 1) iniciar objetos
    DocumentProcessor procesadorDocumentos;
    InvertedIndex invertedIndex;
    SearchEngine searchEngine(&invertedIndex, &procesadorDocumentos);
    ConcurrenciaGrafoConstructor concurrenciaGrafoConstructor;
    
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
        LinkedList<int>* results = searchEngine.query(lineaQuery);
        end_time = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

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
            concurrenciaGrafoConstructor.addQueryResultado(results);
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

    if (lineaQuery == "salir") {
        std::cout << "\nFin del programa" << std::endl;
    }

    // 5) Fase 2: Calcular PageRank y preparar el motor de búsqueda
    std::cout << "\n-----Fase 2: Calculo de PageRank-----" << std::endl;
    // concurrenciaGrafoConstructor.printGrafo(); // Mostrar el grafo construido

    auto start_time_pagerank = std::chrono::high_resolution_clock::now();
    PageRankCalculator pageRankCalculator(concurrenciaGrafoConstructor.getGrafo(), concurrenciaGrafoConstructor.getTodosNodos());
    std::map<int, double> pageRankScores = pageRankCalculator.calculatePageRank();
    auto end_time_pagerank = std::chrono::high_resolution_clock::now();
    auto duration_pagerank = std::chrono::duration_cast<std::chrono::microseconds>(end_time_pagerank - start_time_pagerank);
    std::cout << "[MAIN] Tiempo de calculo de PageRank: " << duration_pagerank.count() << " microsegundos" << std::endl;

    // Pasar los scores de PageRank al SearchEngine
    // Necesitarás añadir un método setPageRankScores a la clase SearchEngine.
    // También necesitarás modificar el método SearchEngine::query para que use estos scores para ordenar los resultados.
    // Consulta SearchEngine.h para añadir el prototipo y SearchEngine.cpp para la implementación.
    // searchEngine.setPageRankScores(pageRankScores); // Descomentar cuando implementes esto en SearchEngine

    std::cout << "\n-----Scores de PageRank-----" << std::endl;
    for (const auto& pair : pageRankScores) {
        std::cout << "Doc-" << pair.first << ": " << pair.second << std::endl;
    }

    // 6) Fase 3: Realizar consultas con PageRank para reordenar resultados
    std::cout << "\n-----Fase 3: Busqueda de documentos con PageRank (ordenamiento)-----" << std::endl;
    std::cout << "Ingrese una nueva consulta: (o 'salir' para terminar el programa)" << std::endl;

    long long total_search_duration_phase3 = 0;
    int query_count_phase3 = 0;

    while (std::getline(std::cin, lineaQuery) && lineaQuery !="salir") {
        if (lineaQuery.empty()) {
            std::cout << "Por favor, ingrese una consulta..." << std::endl;
            continue;
        }

        std::cout << "\nBuscando " << lineaQuery << "..." << std::endl;
        auto start_time_search = std::chrono::high_resolution_clock::now();
        // Aquí, searchEngine.query() debería usar los scores de PageRank para ordenar los resultados
        LinkedList<int>* resultsWithPR = searchEngine.query(lineaQuery); // Suponiendo que query ahora usa PageRank
        auto end_time_search = std::chrono::high_resolution_clock::now();
        auto duration_search = std::chrono::duration_cast<std::chrono::microseconds>(end_time_search - start_time_search);
        total_search_duration_phase3 += duration_search.count();
        query_count_phase3++;

        if (resultsWithPR && resultsWithPR->getSize() > 0) {
            std::cout << "Documentos encontrados (con PageRank) (" << resultsWithPR->getSize() << "): [";
            Node<int>* actual = resultsWithPR->getHead();
            while (actual != nullptr) {
                std::cout << "Doc-" << actual->data;
                if (actual->next != nullptr) {
                    std::cout << ", ";
                }
                actual = actual->next;
            }
            std::cout << "]" << std::endl;
        } else {
            std::cout << "No se encontraron documentos para la consulta (con PageRank)." << std::endl;
        }

        std::cout << "Tiempo de busqueda (con PageRank): " << duration_search.count() << " microsegundos" << std::endl;

        if (resultsWithPR) {
            delete resultsWithPR;
            resultsWithPR = nullptr;
        }

        std::cout << "\nIngrese una nueva consulta: (o 'salir' para terminar el programa)" << std::endl;
    }

    std::cout << "\n[MAIN] Total de consultas en Fase 3: " << query_count_phase3 << std::endl;
    std::cout << "[MAIN] Tiempo total de busqueda en Fase 3: " << total_search_duration_phase3 << " microsegundos" << std::endl;
    std::cout << "\nFin del programa" << std::endl;

    return 0;
}