#ifndef CONVERTERJSON_H
#define CONVERTERJSON_H
#include <iostream>
#include <fstream>
#include <vector>
#include  <string>
#include  <nlohmann/json.hpp>



//Класс для работы с JSON-файлами
class ConverterJSON {
public:
ConverterJSON() = default;

std::vector<std::string> GetTextDocuments();

int GetResponsesLimit();

std::vector<std::string> GetRequests();

void putAnswers(std::vector<std::vector<std::pair<int, float>>>  answers){};
};






#endif // CONVERTERJSON_H
