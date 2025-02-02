#include <gtest/gtest.h>
#include <iostream>
#include "ConverterJSON.h"

extern std::vector<std::string> listRequests;
extern int maxResponses;

int main(int argc, char **argv){
    bool gTest = true;
    for (int i = 1; i < argc; ++i)
    {
        if      (std::string(argv[i]) == "gtest-")     gTest = false;
    }

    ConverterJSON converter;
    InvertedIndex invertedIndex;
    std::vector<std::string> textFiles;

    auto fileAnswers = new std::ofstream("answers.json",std::ios_base::trunc | std::ios_base::out);
    delete fileAnswers;

    std::string versionEngine = "0.1.001";
    nlohmann::json jsonConfig;
    std::ifstream configFile("config.json");
    try
    {
        if (!configFile.is_open()) {
            throw "Config file is missing";
          }
        configFile >> jsonConfig;
        configFile.close();
        if (!jsonConfig["config"].is_object()) {
            throw "Config file is empty";
          }
        if (jsonConfig["config"]["version"]!=versionEngine){
            throw "\"config.json\" has incorrect file version.";
          }
    }
    catch (const char* errorMessage)
    {
        std::cerr << errorMessage << std::endl;
        exit(-1);
    }
    textFiles =     converter.GetTextDocuments();
    maxResponses =  converter.GetResponsesLimit();
    listRequests =  converter.GetRequests();
    invertedIndex.UpdateDocumentBase(textFiles);

    SearchServer searchServer(invertedIndex);
    auto listAnswers = searchServer.search(listRequests);
    converter.putAnswers(listAnswers);


    if(gTest) {
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
      }
    return 0;
}

