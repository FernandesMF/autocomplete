#include "autocompletedata.h"
#define OUT     // useful output reminder

FAutoCompleteData::FAutoCompleteData(){ Constructor(); }

void FAutoCompleteData::Constructor()
{
    BegIndex = 0;
    FinIndex = 0;
    // will leave OrderedEventNames and Ocurrences empty for now
    return;
}

void FAutoCompleteData::EventNamesAlphabetic(FString FileName)
{
    json JsonData;
    json &JsonDataRef = JsonData;

    ReadJson(FileName, OUT JsonDataRef);
    for(int r = 0; r<JsonData.size() ; r++){
        // get event name and insertion index
        // if event name is already in the list, just increase the ocurrence counter
        // else, insert it and make a new ocurrence marker with value 1
    }       

    return;
}

void FAutoCompleteData::ReadJson(FString FileName, json &JsonData)
{
    std::ifstream EventsFile(FileName, std::ifstream::binary);
    if( EventsFile.fail() ){ throw std::runtime_error("Error opening input file."); }
    EventsFile >> JsonData;
    EventsFile.close();
    if( EventsFile.fail() ){ throw std::runtime_error("Error closing input file."); }

    return;
}