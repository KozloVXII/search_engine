#include "InvertedIndex.h"

#include <fstream>
#include <iostream>
#include <algorithm>
#include <mutex>
#include <gtest/gtest.h>

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> inputDocs) {
    std::mutex mtx;
    docs = inputDocs;
    auto readDoc = [&](int i, std::string doc) {
        std::string wordRequests;
        size_t pos{0}, posNew{0};
        std::string wordFromDoc;
        bool usl = true;
        while (usl){
            posNew = doc.find(' ',pos);
            if(posNew==std::string::npos && pos!=doc.size()-2){
                usl = false;
                posNew = doc.size();
              }
            if (posNew>(pos+1)){
                wordFromDoc = doc.substr(pos,posNew-(pos));
                mtx.lock();
                auto itFreqDict = freqDictionary.find(wordFromDoc);
                if (itFreqDict == freqDictionary.end() ){
                    Entry tempEntry;
                    std::vector<Entry> vecEntry;
                    tempEntry.docId = i;
                    tempEntry.count = 1;
                    vecEntry.push_back(tempEntry);
                    freqDictionary.insert(std::pair<std::string,std::vector<Entry>>(wordFromDoc,vecEntry));
                }
                else{
                  for(int l = 0; l<itFreqDict->second.size(); ++l){
                      if(itFreqDict->second.at(l).docId == i){
                          itFreqDict->second.at(l).count++;;
                          break;
                        }
                      else if(l == itFreqDict->second.size()-1 && itFreqDict->second.size()>0){
                          Entry entry;
                          entry.docId = i;
                          entry.count = 1;
                          itFreqDict->second.push_back(entry);
                          break;
                        }
                    }
                }
                mtx.unlock();
            }
            pos = posNew + 1;
        }
    };


    freqDictionary.clear();
    std::vector<std::thread *> listThreads;
    for (int i = 0; i < inputDocs.size(); i++) {
        auto tempThread = new std::thread(readDoc, i, inputDocs[i]);
        listThreads.push_back(tempThread);
    }

    for (int i = 0; i < listThreads.size(); ++i) {
        listThreads.at(i)->join();
    }
    for (int i = 0; i < listThreads.size(); ++i) {
        delete listThreads.at(i);
    }
}


std::vector<Entry> InvertedIndex::GetWordCount(const std::string & word) {
  auto itFindWord =  freqDictionary.find(word);
    if(itFindWord != freqDictionary.end()){
        std::map<size_t,size_t> tempMap;
        std::vector<Entry> tempVecEntry;
        for(auto& entry : itFindWord->second){
            tempMap.insert(std::pair<size_t,size_t>(entry.docId,entry.count));
          }
        for(auto& entry : tempMap){
            Entry tempEntry;
            tempEntry.docId = entry.first;
            tempEntry.count = entry.second;
            tempVecEntry.push_back(tempEntry);
          }
        return tempVecEntry;
      }else{
        std::vector<Entry> tempVecEntry;
        return  tempVecEntry;
      }
}

 void TestInvertedIndexFunctionality(
     const std::vector<std::string>& docs,
     const std::vector<std::string>& requests,
     const std::vector<std::vector<Entry>>& expected
     ) {
     std::vector<std::vector<Entry>> result;
     InvertedIndex idx;
     idx.UpdateDocumentBase(docs);
     for(auto& request : requests) {
         std::vector<Entry> word_count = idx.GetWordCount(request);
         result.push_back(word_count);
     }
     ASSERT_EQ(result, expected);
 }

 TEST(TestCaseInvertedIndex, TestBasic) {
     const std::vector<std::string> docs = {
         "london is the capital of great britain",
         "big ben is the nickname for the Great bell of the striking clock"
     };
     const std::vector<std::string> requests = {"london", "the"};
     const std::vector<std::vector<Entry>> expected = {
         {
             {0, 1}
         }, {
             {0, 1}, {1, 3}
         }
     };
     TestInvertedIndexFunctionality(docs, requests, expected);
 }
 TEST(TestCaseInvertedIndex, TestBasic2) {
     const std::vector<std::string> docs = {
         "milk milk milk milk water water water",
         "milk water water",
         "milk milk milk milk milk water water water water water",
         "americano cappuccino"
         };
     const std::vector<std::string> requests = {"milk", "water", "cappuccino"};
     const std::vector<std::vector<Entry>> expected = {
         {
             {0, 4}, {1, 1}, {2, 5}
         }, {
             {0, 3}, {1, 2}, {2, 5}
         }, {
             {3, 1}
         }
     };
     TestInvertedIndexFunctionality(docs, requests, expected);
 }
 TEST(TestCaseInvertedIndex, TestInvertedIndexMissingWord) {
     const std::vector<std::string> docs = {
         "a b c d e f g h i j k l",
         "statement"
         };
     const std::vector<std::string> requests = {"m", "statement"};
     const std::vector<std::vector<Entry>> expected = {
         {
         }, {
             {1, 1}
         }
     };
     TestInvertedIndexFunctionality(docs, requests, expected);
 }



