// #include <iostream>
// #include <fstream>
#include "ConverterJSON.h"
// #include <nlohmann/json.hpp>

#include "gtest/gtest.h"
TEST(ConverterJSON, GetTextDocuments)
{
    EXPECT_EQ(1, 1);
}


int main(){
    ConverterJSON converter;
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
        system("pause");
        exit(-1);
    }

    std::cout <<"Starting: " << jsonConfig["config"]["name"] <<std::endl;
    unsigned int updatePeriodMs =  jsonConfig["config"]["update_period_ms"];
    std::cout <<updatePeriodMs <<std::endl;



    std::vector<std::string> textFiles = converter.GetTextDocuments();
    for(auto it = textFiles.begin(); it!=textFiles.end(); ++it) {
        std::cout<<*it<<std::endl;
    }


    int maxResponses = converter.GetResponsesLimit();
    std::cout <<"maxResponses = " <<maxResponses <<std::endl;

    std::vector<std::string> listRequests = converter.GetRequests();
    for(auto it = listRequests.begin(); it!=listRequests.end(); ++it) {
        std::cout<<*it<<std::endl;
    }




    system("pause");
    return 0;
}

