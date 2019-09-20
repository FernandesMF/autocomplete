#include "autocompletedata.h"
#define OUT     // useful output reminder

FAutoCompleteData::FAutoCompleteData(){ Constructor(); }

void FAutoCompleteData::Constructor()
{
    // will leave OrderedEventNames and Ocurrences empty for now
    std::deque<FString> OrderedEventNames;
    std::deque<int> Ocurrences;
    BegIndex = 0;
    FinIndex = 0;
    
    return;
}

void FAutoCompleteData::EventNamesAlphabetic(FString FileName)
{
    json JsonData;
    json &JsonDataRef = JsonData;
    FString EventName = "";
    int InsertionIndex = -1;

    ReadJson(FileName, OUT JsonDataRef);
    for(int r = 0; r<JsonData.size() ; r++){
        EventName = GetEventName(JsonData, r);  // get event name
        MakeInsertion(EventName); // insert at the appropriate position
    }       

    std::cout << "EventNamesAlphabetic: list of event names after ordering call:\n";    //FiXME remove these prints
    for(int i=0; i<OrderedEventNames.size(); i++){ std::cout << OrderedEventNames[i] << std::endl;}

    return;
}

// TODO change namne to ImportJsonData?
void FAutoCompleteData::ReadJson(FString FileName, json &JsonData)
{
    std::ifstream EventsFile(FileName, std::ifstream::binary);
    if( EventsFile.fail() ){ throw std::runtime_error("Error opening input file."); }
    std::cout << "Reading json data.\n";
    EventsFile >> JsonData;
    EventsFile.close();
    if( EventsFile.fail() ){ throw std::runtime_error("Error closing input file."); }

    return;
}

// FIXME use just the line of code, instead of a separate function?
FString FAutoCompleteData::GetEventName(json &JsonData, int index) { return JsonData[index]["event"].get<FString>(); }

void FAutoCompleteData::MakeInsertion(FString EventName )
{
    if(OrderedEventNames.empty()){  // if OrderedEventnames is empty
        OrderedEventNames.emplace_front(EventName); // insert EventName at the first position
        Ocurrences.emplace_front(1);    // insert 1 at first position of Ocurrences
        return;
    }
    
    int r = 0;  //start index to access elements of OrderedEventNames
    int StrCmp = 0;
    while(r<OrderedEventNames.size()){  //while the index is within the bounds of OrderedEventNames
        StrCmp = strcmp(EventName.c_str(),OrderedEventNames[r].c_str);
        if(StrCmp == 0){
            Ocurrences[r]++;    // increase ocurrence by 1
            break;              // finish loop (break)
        } else if(StrCmp > 0){
            r++;                // increase index
        } else {
            //HERE!! OrderedEventNames.// insert event name before the current element
            // insert a 1 at Ocurrences before the current element
            // finish the loop (break)
        }
    }
    return;
}