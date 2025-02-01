#ifndef INVERTEDINDEX_H
#define INVERTEDINDEX_H
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <thread>

inline std::vector<std::string> listRequests;

struct Entry {
    size_t docId, count;
    // Данный оператор необходим для проведения тестовых сценариев
    bool operator ==(const Entry& other) const {
    return (docId == other.docId && count == other.count);
    }
};

class InvertedIndex {
    std::vector<std::string> docs;
    std::map<std::string, std::vector<Entry>> freqDictionary;

public:
    InvertedIndex() = default;
    std::vector<Entry> GetWordCount(const std::string& word);
    void UpdateDocumentBase(const std::vector<std::string>& inputDocs);
};
#endif //INVERTEDINDEX_H
