#include <conio.h>
#include <gtest/gtest.h>

#include "ConverterJSON.h"


 TEST(ConverterJSON, GetTextDocuments) {
     EXPECT_EQ(1, 1);
 }

extern std::vector<std::string> listRequests;
extern int maxResponses;
int main(int argc, char **argv){

     ::testing::InitGoogleTest(&argc, argv);

    ConverterJSON converter;
    InvertedIndex invertedIndex;

    auto fileAnswers = new std::ofstream("answers.json",std::ios_base::trunc | std::ios_base::out);
    delete fileAnswers;

    std::string versionEngine = "0.1.001";
    nlohmann::json jsonConfig;
    std::ifstream configFile("config.json", std::ios::in);

    try
    {
        if (!configFile.is_open())
            throw "Config file is missing";
        configFile >> jsonConfig;
        configFile.close();
        if (!jsonConfig["config"].is_object())
            throw "Config file is empty";
        if (jsonConfig["config"]["version"]!=versionEngine)
            throw "\"config.json\" has incorrect file version.";

    }
    catch (const char* errorMessage)
    {
        std::cerr << errorMessage << std::endl;
        exit(-1);
    }

    unsigned int updatePeriodMs =  jsonConfig["config"]["update_period_ms"];

    std::vector<std::string> textFiles = converter.GetTextDocuments();
    maxResponses = converter.GetResponsesLimit();
    listRequests = converter.GetRequests();
    invertedIndex.UpdateDocumentBase(textFiles);
    SearchServer searchServer(invertedIndex);
    auto listAnswers = searchServer.search(listRequests);
    converter.putAnswers(listAnswers);

    return RUN_ALL_TESTS();
}

