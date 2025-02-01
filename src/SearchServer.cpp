#include "SearchServer.h"
#include <algorithm>
#include <iterator>
#include <vector>
#include <string>
#include <gtest/gtest.h>

std::vector<std::vector<RelativeIndex>>
            SearchServer::search
            (const std::vector<std::string> &queriesInput)
{

    std::vector<std::vector<RelativeIndex>> vecRelativeIndex;
    std::multimap<int, std::string> mаpSorting;

    auto itQueries = queriesInput.begin();

    while(itQueries != queriesInput.end()){
        std::string strQueries = *itQueries;
        std::vector<std::string> listWords;
        size_t pos{0}, posNew{0};
        while(pos<itQueries->size()){
            std::string word;
            posNew = itQueries->find(' ',pos);
            if(posNew>(pos+1) || posNew == itQueries->size()){
                word = itQueries->substr(pos,posNew-pos);
              }
              if (std::find(std::begin(listWords),std::end(listWords),word) ==
                  std::end(listWords)){
                  listWords.push_back(word);
                }
            pos = posNew;
            if(posNew<std::string::npos) ++pos;
          }
        std::map <int, size_t> mapCount;
        for(auto & listWord : listWords){
            auto listEntry = index.GetWordCount(listWord);
            for(auto & itListEntry : listEntry){
                if(auto itMapCount = mapCount.find(itListEntry.docId); itMapCount != mapCount.end()){
                  itMapCount->second +=itListEntry.count;
                }else{
                    mapCount.insert(std::pair<int, size_t>(itListEntry.docId,itListEntry.count));
                }
            }
        }
        size_t  maxAbsRelev = 0;
        for(auto & itMapCount : mapCount){
            if(itMapCount.second > maxAbsRelev) maxAbsRelev = itMapCount.second;
        }
        std::multimap<float,int> mapRelativeIndex;
        for(auto & itMapCount : mapCount){
            float tempRelative = (float)itMapCount.second / (float)maxAbsRelev;
            mapRelativeIndex.insert(std::pair<float, int>(tempRelative,itMapCount.first));
        }

        std::vector<RelativeIndex> listRelativeIndexRequest;
        std::vector<RelativeIndex> rere;
        std::vector<RelativeIndex> tempVec;
        float tempRank = 10.f;
        int countResponses =1;

        for(auto key_value = mapRelativeIndex.rbegin(); key_value != mapRelativeIndex.rend() ; ++key_value) {
            if(key_value->first<tempRank){
                for(auto it = tempVec.rbegin(); it!= tempVec.rend(); ++it ){
                  listRelativeIndexRequest.push_back(*it);
                  if(++countResponses >maxResponses) {
                      tempVec.clear();
                      break;}
                  }
                tempVec.clear();
                tempRank = key_value->first;
              }
            RelativeIndex relativeIndex{};
            relativeIndex.doc_id = key_value->second;
            relativeIndex.rank = key_value->first;
            tempVec.push_back(relativeIndex);
        }

        for(auto it = tempVec.rbegin(); it!= tempVec.rend(); ++it ){
            listRelativeIndexRequest.push_back(*it);
            if(++countResponses >maxResponses) {
                tempVec.clear();
                break;
            }
        }

        int countRequest = 0;
        for(int i  = rere.size()-1; i>=0 && countRequest<5 ; --i,++countRequest) {
            listRelativeIndexRequest.push_back(rere.at(i));
          }

          vecRelativeIndex.push_back(listRelativeIndexRequest);
          ++itQueries;
    }
    return vecRelativeIndex;
}
