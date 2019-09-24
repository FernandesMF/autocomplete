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
    
    ImportJsonData(FileName, OUT JsonDataRef);
    std::cout << "EvenNamesAlphabetic: JsonData size: " << JsonData.size() << std::endl;
    std::cout << "----------------------------------\n";
    for(int r = 0; r<JsonData.size() ; r++){
        EventName = GetEventName(JsonData, r);      // get event name
        MakeInsertion(EventName);                   // insert at the appropriate position
    }
    for(int i=0; i<OrderedEventNames.size(); i++){ std::cout << OrderedEventNames[i] << std::endl;}
    for(int i=0; i<OrderedEventNames.size(); i++){ std::cout << Ocurrences[i] << std::endl;}

    return;
}

void FAutoCompleteData::ImportJsonData(FString FileName, json &JsonData)
{
    std::ifstream EventsFile(FileName, std::ifstream::binary);
    if( EventsFile.fail() ){ throw std::runtime_error("Error opening input file."); }
    std::cout << "Reading json data.\n";
    EventsFile >> JsonData;
    EventsFile.close();
    if( EventsFile.fail() ){ throw std::runtime_error("Error closing input file."); }

    return;
}

FString FAutoCompleteData::GetEventName(json &JsonData, int index) { return JsonData[index]["event"].get<FString>(); }

void FAutoCompleteData::MakeInsertion(FString EventName )
{
    if(OrderedEventNames.empty()){                      // if OrderedEventnames is empty
        OrderedEventNames.emplace_front(EventName);     // insert EventName at the first position
        Ocurrences.emplace_front(1);                    // insert 1 at first position of Ocurrences
        return;
    }

    std::deque<FString>::iterator R = OrderedEventNames.begin();    // pointer to elements of OrderedEventNames
    std::deque<int>::iterator S = Ocurrences.begin();               // pointer to  elements of Ocurrences
    int StrCmp = 0;
    int s = 0;                                 
    
    while(R!=OrderedEventNames.end()){                                    
        StrCmp = strcmp(EventName.c_str(),R->c_str());                                    
        if(StrCmp == 0){                                    
            Ocurrences[s]++;            //increase ocurrence by 1
            break;
        } else if(StrCmp > 0){                                     
            R++; S++; s++;
        } else {
            OrderedEventNames.emplace(R,EventName);
            Ocurrences.emplace(S,1);
            break;
        }
    }
    if(R==OrderedEventNames.end()){         // if the end of OrderedEventNames was reached
        OrderedEventNames.emplace_back(EventName);      // insert event name at end
        Ocurrences.emplace_back(1);                     // insert a 1 at end
    }
    return;
}

EInput FAutoCompleteData::CheckInputValidity(FString Input)
{
    if( strcmp(Input.c_str(),":q") == 0 ){
        return EInput::SC_Quit;
    } else if( strcmp(Input.c_str(),":l") == 0 ) {
        return EInput::SC_Show_ACData;
    } else if ( strcmp(Input.c_str(),":h") == 0 ){
        return EInput::SC_Help;
    } else if ( Input.length()<2 ){
        return EInput::ERR_Too_Short;
    } else {
        return EInput::Valid;
    }
}

void FAutoCompleteData::ShowAccumulatedData()
{
    std::cout << "Event Names\t\tOcurences\n";
    for(int i=0; i<OrderedEventNames.size(); i++){
        std::cout << OrderedEventNames[i] << "\t\t" << Ocurrences[i] << std::endl;
    }
    return;
}

void FAutoCompleteData::PrintSpecialCommands()
{
    // list special commands
    std::cout << "The special commands to interface with this program are the following>\n";
    std::cout << ":q --> quit\n";
    std::cout << ":l --> list the data accumulated for suggestions\n";
    std::cout << ":h --> display special command list\n\n";
    return;
}

std::deque<FString> FAutoCompleteData::Autocomplete(FString Input)
{
    // Strategy: search for a compatible entry via bissection, then "explode" forward
    // and backward from it to find boundaries of the compatible subset

    bool bSearching = true;                 // "still searching" flag
    std::deque<FString> Suggestions;        // deque to hold completion suggestions
    int i1 = 0;                             // indexes to explore the events deque
    int i2 = OrderedEventNames.size()-1;
    int ic;
    int n = Input.length();

    // Finding a compatible entry (bissection)
    if(strncmp(Input.c_str(),OrderedEventNames.front().c_str(),n)<0 || 
       strncmp(Input.c_str(),OrderedEventNames.back().c_str(),n)>0) {
        return Suggestions;     // return empty deque if Input is not present on the list
    }
     
    // knowing the entries at begin/end positions are not compatible is
    // important to later determine if the indexes collapsed and skip to
    // the "explosion" phase if they are compatible with the input
    if( strncmp(Input.c_str(),OrderedEventNames.front().c_str(),n) == 0 ){ 
        bSearching = false;
        ic = 0;
    } else if ( strncmp(Input.c_str(),OrderedEventNames.back().c_str(),n) == 0 ){
        bSearching = false;
        ic = OrderedEventNames.size()-1;
    }

    int StrCmp;
    while(bSearching && (i2-i1)>1){       // while compatible entry is not found and bissection indexes are not collapsed
        ic = ceil((i1+i2)/2);
        StrCmp = strncmp(Input.c_str(),OrderedEventNames[ic].c_str(),n);
        if( StrCmp == 0 ){ bSearching = false; }
        else if (StrCmp < 0){ i2 = ic; }
        else { i1 = ic;}
    }

    // Finding boundaries of the compatible subset (explosion) and building Suggestion list
    if( (i2-i1)>1 ){
        Suggestions.push_front(OrderedEventNames[ic]);
        i1 = ic-1; i2 = ic+1;
        while( i1>=0 && strncmp(Input.c_str(),OrderedEventNames[i1].c_str(),n)==0 ) {
            Suggestions.push_front(OrderedEventNames[i1]);
            i1--;
        }
        while( i2<=OrderedEventNames.size()-1 && strncmp(Input.c_str(),OrderedEventNames[i2].c_str(),n)==0 ) {
            Suggestions.push_back(OrderedEventNames[i2]);
            i2++;
        }
    }

    return Suggestions;
}

// TODO Order suggestions by ocurrences