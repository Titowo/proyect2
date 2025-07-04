#ifndef DOCUMENT_PROCESSOR_H
#define DOCUMENT_PROCESSOR_H

#include <string>
#include <vector>
#include <unordered_set>
#include <fstream>
#include <iostream>
#include <sstream>

#include "Utils.h"

class InvertedIndex;

struct Document {
    int id;
    std::string url;
    std::string contenido;
};

class DocumentProcessor {
public:
    DocumentProcessor();
    ~DocumentProcessor();

    void loadStopWords(const std::string& filename);
    void processDocumentContent(const std::string& linea, int doc_id, InvertedIndex& index);

    void loadAndProcessDocuments(const std::string& filename, class InvertedIndex& index);
private:
    std::unordered_set<std::string> stopWords;
};

#endif