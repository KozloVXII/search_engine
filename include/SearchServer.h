#ifndef SEARCHSERVER_H
#define SEARCHSERVER_H
#include "InvertedIndex.h"

inline int  maxResponses;

struct RelativeIndex{
  size_t doc_id;
  float rank;

  bool operator ==(const RelativeIndex& other) const {
    return (doc_id == other.doc_id && rank == other.rank);
  }
};

class SearchServer {
public:
/**
* @param idx в конструктор класса передаётся ссылка на класс
InvertedIndex,
* чтобы SearchServer мог узнать частоту слов встречаемых в
запросе
*/
    SearchServer(InvertedIndex& inIndex) : index(inIndex){};
/**
* Метод обработки поисковых запросов
* @param queriesInput поисковые запросы взятые из файла
requests.json
* @return возвращает отсортированный список релевантных ответов для
заданных запросов
*/
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queriesInput);
private:
    InvertedIndex index;
};

#endif // SEARCHSERVER_H
