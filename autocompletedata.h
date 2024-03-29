#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <deque>
#include <math.h>
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using FString = std::string;
#define OUT     // useful output reminder

enum class EInput
{
    SC_Quit,
    SC_Show_ACData,
    SC_Help,
    ERR_Too_Short,
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
        std::deque<FString> Autocomplete(FString Input);

    private:
        // We chose to store the event names in a deque structure, and not in a vector or set.
        // This was because deques do not use contiguous memory adresses, which would be interesting
        // if we wanted to add more suggestion data on the fly without risking to reallocate a lot
        // of other things in the memory. A set could be the most interesting structure to use, if
        // we wanted to use a tree to speed up the searching. I am not very familiar with implementing
        // trees though, so I chose to use deques.
        std::deque<FString> OrderedEventNames;  // "list" to hold the event names (in alphabetical order)
        std::deque<int> Ocurrences;             // "list" to hold ocurrences (and make suggestions based on that)
        int BegIndex;                           // indexes of first and last entries that will have to be returned
        int FinIndex;
        
        void ImportJsonData(FString FileName, json &JsonData);  // function to read a Json file
        FString GetEventName(json &JsonData, int index);        // reads name of event at index in JsonData
        void MakeInsertion(FString EventName );                 // inserts EventName at the correct position
};