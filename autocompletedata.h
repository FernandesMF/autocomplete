#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <deque>
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using FString = std::string;
#define OUT // useful output reminder

enum class EInput
{
    SC_Quit,
    SC_Show_ACData,
    SC_Help,
    ERR_Too_Short,
//    ERR_Invalid_Character, // I guess every character will be valid for searching...
    Valid,
};

class FAutoCompleteData
{
    public:
        
        FAutoCompleteData();    // constructor(s)
        void Constructor();

        void EventNamesAlphabetic(std::string FileName); // read json data and store it in internal "list"
        EInput CheckInputValidity(FString Input);
        void PrintSpecialCommands();
        void ShowAccumulatedData();

    private:
        //TODO comment on why deque and not set/map or vector
        std::deque<FString> OrderedEventNames; // "list" to hold the event names (in alphabetical order)
        std::deque<int> Ocurrences;    // "list" to hold ocurrences (and make suggestions based on that)
        int BegIndex;       // indexes of first and last entries that will have to be returned
        int FinIndex;
        
        void ImportJsonData(FString FileName, json &JsonData);  // function to read a Json file
        FString GetEventName(json &JsonData, int index); // reads name of event at index in JsonData
        void MakeInsertion(FString EventName ); // inserts EventName at the correct position
 //       bool IsAlphaOnly(FString Input);

};