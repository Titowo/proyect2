#ifndef DOCUMENT_PROCESSOR_H
#define DOCUMENT_PROCESSOR_H

#include <string>
#include <vector>
#include <unordered_set>
#include <fstream>       

class InvertedIndex;

class DocumentProcessor {
public:
    DocumentProcessor();
    ~DocumentProcessor();

    void loadStopWords(const std::string& filename);

    int processDocumentContent(const std::string& content, int documentId, InvertedIndex& index);

    std::vector<std::string> getCleanWords(const std::string& text) const;

    void loadAndProcessDocuments(const std::string& filename, InvertedIndex& index);

private:
    std::unordered_set<std::string> stopWords;
};

#endif