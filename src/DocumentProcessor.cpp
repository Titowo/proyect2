#include "DocumentProcessor.h"
#include "InvertedIndex.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <sstream> 
#include <algorithm>

DocumentProcessor::DocumentProcessor() {
    // Constructor
}

DocumentProcessor::~DocumentProcessor() {
    // Destructor
}

void DocumentProcessor::loadStopWords(const std::string& filename) {
    std::ifstream file(filename);
    if(!file.is_open()) {
        std::cerr << "Error al abrir el archivo de stopwords: " << filename << std::endl;
        return;
    }

    std::string word;
    while (file >> word) {
        stopWords.insert(Utils::toLower(word));
    }
    file.close();
    std::cout << "Cargadas " << stopWords.size() << " stopwords." << std::endl;
}

void DocumentProcessor::processDocumentContent(const std::string& linea, int doc_id, InvertedIndex& index) {

    std::cout << "VERBOSE: Iniciando procesamiento de Doc ID: " << doc_id << std::endl;

    size_t last_separator_pos = linea.rfind("||");
    if(last_separator_pos == std::string::npos || last_separator_pos + 2 >= linea.length()) {
        std::cerr << "Advertencia: Línea mal formada (Doc ID: " << doc_id << "): " << linea.substr(0, 50) << "..." << std::endl;
        return;
    }
    std::string contenido = linea.substr(last_separator_pos + 2); // +2 para saltar "||"

    std::istringstream iss(contenido);
    std::string raw_word;
    int words_added_count = 0; // Para contar palabras añadidas por documento

    while (iss >> raw_word) {
        std::string cleaned_word = Utils::cleanWord(raw_word);

        if (!cleaned_word.empty()) {
            if (stopWords.find(cleaned_word) == stopWords.end()) {

                index.addDocumento(cleaned_word, doc_id);
                words_added_count++;
            }
        }
    }
    // Mensaje de finalización de procesamiento para cada documento
    std::cout << "VERBOSE: Doc ID " << doc_id << " procesado. " << words_added_count << " palabras añadidas al índice." << std::endl;
}

void DocumentProcessor::loadAndProcessDocuments(const std::string& filename, InvertedIndex& index) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo de documentos: " << filename << std::endl;
        return;
    }

    std::cout << "VERBOSE: Iniciando la carga y procesamiento de documentos desde: " << filename << std::endl;
    std::string linea;
    int doc_id = 0;
    int processed_docs_count = 0;
    const int VERBOSE_DOC_STEP = 100; // Muestra un mensaje cada N documentos

    while (std::getline(file, linea)) {
        processDocumentContent(linea, doc_id, index); // Procesa la línea y añade palabras al índice
        doc_id++; // Incrementa el ID para el siguiente documento
        processed_docs_count++;

        if (processed_docs_count % VERBOSE_DOC_STEP == 0) {
            std::cout << "VERBOSE: " << processed_docs_count << " documentos procesados hasta ahora." << std::endl;
        }
    }
    file.close();
    std::cout << "VERBOSE: Finalizado el procesamiento de " << processed_docs_count << " documentos." << std::endl;
    std::cout << "Procesados " << processed_docs_count << " documentos en total." << std::endl;
}