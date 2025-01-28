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
      size_t pos{0}, posNew{0}, wordLength{0};
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
    for(auto itListWords = listWords.begin(); itListWords!=listWords.end(); ++itListWords){
        auto listEntry = index.GetWordCount(*itListWords);
        for(auto itListEntry = listEntry.begin(); itListEntry!=listEntry.end(); ++itListEntry){
            auto itMapCount = mapCount.find(itListEntry->docId);
            if(itMapCount != mapCount.end()){
              itMapCount->second +=itListEntry->count;
            }else{
                mapCount.insert(std::pair<int, size_t>(itListEntry->docId,itListEntry->count));
              }

        }
      }
    size_t  maxAbsRelev = 0;
    for(auto itMapCount = mapCount.begin(); itMapCount!=mapCount.end(); ++itMapCount){
        if(itMapCount->second > maxAbsRelev) maxAbsRelev = itMapCount->second;
      }
    std::multimap<float,int> mapRelativeIndex;
    for(auto itMapCount = mapCount.begin(); itMapCount!=mapCount.end(); ++itMapCount){
        float tempRelative = (float)itMapCount->second / (float)maxAbsRelev;
        mapRelativeIndex.insert(std::pair<float, int>(tempRelative,itMapCount->first));
      }

  std::vector<RelativeIndex> listRelativeIndexRequest;
  std::vector<RelativeIndex> rere;
  std::vector<RelativeIndex> tempVec;
  float tempRank = 100.f;
  int count =1;
  for(auto key_value = mapRelativeIndex.rbegin(); key_value != mapRelativeIndex.rend() ; ++key_value) {

      if(key_value->first<tempRank){
          for(auto it = tempVec.rbegin(); it!= tempVec.rend(); ++it ){
            listRelativeIndexRequest.push_back(*it);
            if(++count >maxResponses) {
                tempVec.clear();
                break;}
            }
          tempVec.clear();
          tempRank = key_value->first;
        }
      RelativeIndex relativeIndex;
      relativeIndex.doc_id = key_value->second;
      relativeIndex.rank = key_value->first;
      tempVec.push_back(relativeIndex);
    }
  for(auto it = tempVec.rbegin(); it!= tempVec.rend(); ++it ){
    listRelativeIndexRequest.push_back(*it);
    if(++count >maxResponses) {
        tempVec.clear();
        break;}
    }
    int c = 0;
    for(int i  = rere.size()-1; i>=0 && c<5 ; --i,++c) {
        std::cout <<"   +++    --- : "<< rere.at(i).doc_id << " - " << rere.at(i).rank << std::endl;
        listRelativeIndexRequest.push_back(rere.at(i));
      }
    vecRelativeIndex.push_back(listRelativeIndexRequest);
    ++itQueries;
  }
  return vecRelativeIndex;
}


TEST(TestCaseSearchServer, TestSimple) {
const std::vector<std::string> docs = {
"milk milk milk milk water water water",
"milk water water",
"milk milk milk milk milk water water water water water",
"americano cappuccino"
};
const std::vector<std::string> request = {"milk water", "sugar"};
const std::vector<std::vector<RelativeIndex>> expected = {
{
{2, 1},
{0, 0.7},
{1, 0.3}
},
{
}
};
InvertedIndex idx;
idx.UpdateDocumentBase(docs);
SearchServer srv(idx);
std::vector<std::vector<RelativeIndex>> result = srv.search(request);
ASSERT_EQ(result, expected);
}
TEST(TestCaseSearchServer, TestTop5) {
    const std::vector<std::string> docs = {
    "london is the capital of great britain",
    "paris is the capital of france",
    "berlin is the capital of germany",
    "rome is the capital of italy",
    "madrid is the capital of spain",
    "lisboa is the capital of portugal",
    "bern is the capital of switzerland",
    "moscow is the capital of russia",
    "kiev is the capital of ukraine",
    "minsk is the capital of belarus",
    "astana is the capital of kazakhstan",
    "beijing is the capital of china",
    "tokyo is the capital of japan",
    "bangkok is the capital of thailand",
    "welcome to moscow the capital of russia the third rome",
    "amsterdam is the capital of netherlands",
    "helsinki is the capital of finland",
    "oslo is the capital of norway",
    "stockholm is the capital of sweden",
    "riga is the capital of latvia",
    "tallinn is the capital of estonia",
    "warsaw is the capital of poland"
  };
  const std::vector<std::string> request = {"moscow is the capital of russia"};
  const std::vector<std::vector<RelativeIndex>> expected = {
    {
      {7, 1},
      {14, 1},
      {0, 0.666666687},
      {1, 0.666666687},
      {2, 0.666666687}
    }
  };
  InvertedIndex idx;
  idx.UpdateDocumentBase(docs);
  SearchServer srv(idx);
  std::vector<std::vector<RelativeIndex>> result = srv.search(request);
  ASSERT_EQ(result, expected);
  }
