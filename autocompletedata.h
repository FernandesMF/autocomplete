//#pragma once
#include <fstream>
#include <string>
//#include <vector>
#include <deque>
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using FString = std::string;


class FAutoCompleteData
{
    public:

        FAutoCompleteData();    // constructor(s)
        void Constructor();

        void EventNamesAlphabetic(std::string FileName); // call to read json data, and store it in internal "list"

        // function to return entries that match two beginning letters

    private:
         
        std::deque<FString> OrderedEventNames; // "list" to hold the event names (in alphabetical order)
        std::deque<int> Ocurrences;    // "list" to hold ocurrences (and make suggestions based on that)
        int BegIndex;       // indexes of first and last entries that will have to be returned
        int FinIndex;
        
        void ReadJson(FString FileName, json &JsonData);  // function to read a Json file

};