#include "ConverterJSON.h"

std::vector<std::string> ConverterJSON::GetRequests() {
    const int maxNumRequests = 1000;
    const int maxWord = 10;

    std::vector<std::string> outListRequests;
    std::vector<std::string> outListWordRequests;
    nlohmann::json jsonFiles;
    std::ifstream fileRequests("requests.json");
    if (fileRequests.is_open()) {
        fileRequests>>jsonFiles;
        fileRequests.close();
        if (jsonFiles["requests"].is_array()) {
            if (jsonFiles["requests"].size()>maxNumRequests) {
                std::cerr<<"Max number of requests reached"<<std::endl;
                outListRequests.reserve(maxNumRequests);
                auto it = outListRequests.cbegin();
                outListRequests.insert(it,jsonFiles["requests"].begin(),jsonFiles["requests"].begin()+maxNumRequests);
            }else {
                outListRequests.reserve(jsonFiles["requests"].size());
                outListRequests = jsonFiles["requests"];
            }
        }
    }
    else{
        std::cerr<<"The \"requests.json\" file was not found."<<std::endl;
    }
    for (auto it = outListRequests.begin(); it != outListRequests.end(); ++it) {
        int count=0;
        std::string tempStr;
        std::size_t  posFound=0 ;
        std::size_t posFoundNew;
        while (posFound<it->size()) {
            posFoundNew = it->find(' ',posFound);
            if ((posFoundNew-posFound)<=1 && posFoundNew!=std::string::npos) {
                posFound = posFoundNew+1;
            }else if(posFoundNew>(posFound+1)) {
                if (tempStr.size()>0) {
                    tempStr += ' ';
                }
                if (posFoundNew>=it->size()) posFoundNew = it->size();
                tempStr+=(it->substr(posFound,posFoundNew-posFound));
                if (posFoundNew!=std::string::npos) posFound = posFoundNew+1;
                count++;
            }
            if (count==maxWord) break;
        }
        outListWordRequests.push_back(tempStr);
    }
    return outListWordRequests;
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
            std::cerr << "The file on the path: \""<<
            pathFile<<"\" - does not exist" << std::endl;
        }else{
            int count = 0;
            std::string tempStr;
            while (!file.eof()){
                ++count;
                if(count>1000){
                    std::cerr<<"Exceeded the number of "
                    "words in the file "
                    "along the path: \""<<pathFile<<"\""<<std::endl;
                    break;
                }
                file>>tempStr;
                if(tempStr.size()>100){
                    std::cerr<<"The word contains more than 100 characters"<<std::endl;
                }
                tempStrFile+=(tempStr+" ");
            }
        }
        outListTextFiles.push_back(tempStrFile);
    }
    return outListTextFiles;
}


void ConverterJSON::putAnswers(const std::vector<std::vector<RelativeIndex>>&  answers) {
    nlohmann::json answersJSON;
    std::string strAnswers = "answers";
    std::string strRequest = "request";
    std::string strRelevance = "relevance";
    std::string strActualRequest;

    auto stringMod = [&](const std::string &inStrRequest, const int &inIdDoc, std::string &outStrRequest){
        outStrRequest.clear();
        outStrRequest = inStrRequest;
        if(inIdDoc<100) outStrRequest+="0";
        if(inIdDoc<10)  outStrRequest+="0";
        outStrRequest+=std::to_string(inIdDoc+1);
    };

    for(int i = 0; i<answers.size(); ++i){
        bool result = answers.at(i).size()>0;
        stringMod(strRequest,i,strActualRequest);
        answersJSON[strAnswers][strActualRequest]["result"] = result;
        if(result){
            for(auto it = answers.at(i).begin(); it!=answers.at(i).end(); ++it){
                nlohmann::json temp;
                temp["docid"] = it->doc_id;
                temp["rank"] = round(it->rank*1000)/1000;
                answersJSON[strAnswers][strActualRequest][strRelevance]+=temp;
              }
          }
      }

    std::ofstream configFile("answers.json");

    if (configFile.is_open()) {
        configFile << std::setw(2) <<answersJSON;
        configFile.close();
    }
}
