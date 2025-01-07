#include "ConverterJSON.h"


std::vector<std::string> ConverterJSON::GetRequests() {
    std::vector<std::string> outListRequests;
    nlohmann::json jsonFiles;
    std::ifstream fileRequests("requests.json");
    if (fileRequests.is_open()) {
        fileRequests>>jsonFiles;
        fileRequests.close();
        if (jsonFiles["requests"].is_array()) {
            outListRequests = jsonFiles["requests"];
        }
    }
    return outListRequests;
}


int ConverterJSON::GetResponsesLimit() {
    int maxResponses=0;
    nlohmann::json jsonResponses;
    std::ifstream configFile("config.json", std::ios::in);
    if (configFile.is_open()) {
        configFile>>jsonResponses;
        configFile.close();
        if (jsonResponses["config"]["max_responses"].is_number_integer())
            maxResponses=jsonResponses["config"]["max_responses"];
        if (maxResponses<=0) maxResponses = 5;
    }
    return maxResponses;
}

std::vector<std::string> ConverterJSON::GetTextDocuments() {
    std::vector<std::string> nameFiles;
    std::vector<std::string> outListTextFiles;
    nlohmann::json jsonFiles;
    std::ifstream configFile("config.json");
    if (configFile.is_open()) {
        configFile>>jsonFiles;
        configFile.close();
        if (jsonFiles["files"].is_array()) {
            nameFiles = jsonFiles["files"];
        }
    }
        std::string tempStrFile;
        for(auto it = nameFiles.begin(); it!=nameFiles.end(); ++it) {
            tempStrFile.clear();
            std::string pathFile = *it;
            std::ifstream file(pathFile.c_str());
            if(!file.is_open()){
                std::cout << "The file on the path: \""<<
                    pathFile<<"\" - does not exist" << std::endl;
            }else{
                char b;
                int count = 0;
                std::string tempStr;
                while (!file.eof())
                {
                    ++count;
                    if(count>1000){
                        std::cout<<"Exceeded the number of "
                                     "words in the file "
                                     "along the path: \""<<pathFile<<"\""<<std::endl;
                        break;
                    }
                    file>>tempStr;
                    if(tempStr.size()>100)
                    {
                        std::cout<<"The word contains more than 100 characters"<<std::endl;
                    }
                    tempStrFile+=(tempStr+" ");
                }
                std::cout << "count = " <<count<< std::endl;
            }
            outListTextFiles.push_back(tempStrFile);
        }
    return outListTextFiles;
}
