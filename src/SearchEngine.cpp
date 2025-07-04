#include "SearchEngine.h"

SearchEngine::SearchEngine(InvertedIndex* index, DocumentProcessor* docProcessor)
    : invertedIndex(index), documentProcessor(docProcessor) {
    // Los punteros se inicializan en la lista de inicialización.
}

std::vector<std::string> SearchEngine::procesarQueryString(const std::string& queryString) const {
    std::vector<std::string> terminosProcesados;
    std::istringstream iss(queryString);
    std::string raw_word;

    while (iss >> raw_word) {
        std::string cleaned_word = Utils::cleanWord(raw_word);

        if (!cleaned_word.empty()) {
            terminosProcesados.push_back(cleaned_word);
        }
    }
    return terminosProcesados;
}

LinkedList<int>* SearchEngine::query(const std::string& queryString) const {
    std::vector<std::string> terminosQuery = procesarQueryString(queryString);

    if (terminosQuery.empty()) {
        std::cout << "No se encontraron terminos validos para la consulta" << std::endl;
        return new LinkedList<int>();
    }

    if (terminosQuery.size() == 1) {
        const LinkedList<int>* resultado = invertedIndex->search(terminosQuery[0]);
        if (resultado) {
            LinkedList<int>* resultadoCopia = new LinkedList<int>();
            Node<int>* actual = resultado->getHead();
            while (actual != nullptr) {
                resultadoCopia->add(actual->data);
                actual = actual->next;
            }
            return resultadoCopia;
        } else {
            return new LinkedList<int>();
        }
    } else {
        return invertedIndex->search(terminosQuery);
    }
}