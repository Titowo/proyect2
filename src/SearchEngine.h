#ifndef SEARCH_ENGINE_H
#define SEARCH_ENGINE_H

#include <string>
#include <vector>
#include <sstream>

#include "InvertedIndex.h"
#include "DocumentProcessor.h"
#include "LinkedList.h"


class SearchEngine {
public:
    SearchEngine(InvertedIndex* index, DocumentProcessor* docProcessor);

    LinkedList<int>* query(const std::string& queryString) const;

private:
    InvertedIndex* invertedIndex;
    DocumentProcessor* documentProcessor;

    std::vector<std::string> procesarQueryString(const std::string& queryString) const;
};

#endif