#include "DocumentProcessor.h"
#include "InvertedIndex.h"

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
    std::cout << "Cargadas" << stopWords.size() << "stopwords." << std::endl;
}

void DocumentProcessor::processDocumentContent(const std::string& linea, int doc_id, InvertedIndex& index) {
    size_t last_separator_pos = linea.rfind("||");
    if(last_separator_pos == std::string::npos || last_separator_pos + 2 >= linea.length()) {
        //std::cerr << "Advertencia: Línea mal formada (Doc ID: " << doc_id << "): " << line.substr(0, 50) << "..." << std::endl;
        return;
    }
    std::string contenido = linea.substr(last_separator_pos + 2); // +2 para saltar "||"

    std::istringstream iss(contenido);
    std::string raw_word;

    while (iss >> raw_word) {
        std::string cleaned_word = Utils::cleanWord(raw_word);

        if (!cleaned_word.empty()) {
            if (stopWords.find(cleaned_word) == stopWords.end()) {
                index.addDocumento(cleaned_word, doc_id);
            }
        }
    }
}

void DocumentProcessor::loadAndProcessDocuments(const std::string& filename, InvertedIndex& index) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo de documentos: " << filename << std::endl;
        return;
    }

    std::string linea;
    int doc_id = 0;
    while (std::getline(file, linea)) {
        // Asigna el ID unico a cada documento
        processDocumentContent(linea, doc_id, index);
        doc_id++;
    }
    file.close();
    std::cout << "Procesados " << doc_id << " documentos." << std::endl;
}