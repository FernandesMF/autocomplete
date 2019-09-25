#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include "nlohmann/json.hpp"
#include "autocompletedata.h"

using json = nlohmann::json;
#define OUT         // useful tag to remind of output references

bool ArgCountProblem(int, char**);
void ReadExternalJson(FString);
void CreateMockJson(FString);
void WriteJson(FString,json);
FString GetUserInput();

FAutoCompleteData ACData;   // start an instance of the autocomplete class (autocompletedata.h) globally

int main(int argc, char* argv[])
{    
    if(ArgCountProblem(argc,argv)){ return 1; } // check command line inputs

    // Preparatory steps
    try{ ReadExternalJson(argv[1]); }     // create json file with events
    catch(const std::runtime_error& e){ 
        std::cerr << e.what() << '\n';
        return 1;
    }    
    try{ ACData.EventNamesAlphabetic(argv[1]); }   // collect that data in the collector class
    catch(const std::runtime_error& e){ 
        std::cerr << e.what() << '\n';     
       return 1;
    }    

    ACData.PrintSpecialCommands();

    // User input loop (while quit command is not given)
    FString UserInput = "";
    std::deque<FString> Suggestions;
    do{
        UserInput = GetUserInput();
        switch (ACData.CheckInputValidity(UserInput))
        {
        case EInput::ERR_Too_Short :
            std::cout << "Input was too short. Give at least two characters.";
            break;
        case EInput::SC_Show_ACData :
            std::cout << "Showing accumulated data for autocomplete suggestions:\n";
            ACData.ShowAccumulatedData();
            break;
        case EInput::SC_Help :
            ACData.PrintSpecialCommands();
            break;
        case EInput::Valid :
            std::cout << "Fetching suggestions for \"" << UserInput << "\"...";
            Suggestions = ACData.Autocomplete(UserInput);
            std::cout << " found " << Suggestions.size() << ":\n";
            for(int i=0; i<Suggestions.size(); i++){ std::cout << Suggestions[i] << std::endl;}
        }
        std::cout << std::endl << std::endl;
    } while(UserInput != ":q");

    return 0;
}

bool ArgCountProblem(int argc, char* argv[])
{
    if(argc!=2){
        std::cout << "Wrong number of input arguments. You should enter";
        std::cout << " (1) the name of the file to use as mock Json data ";
        std::cout << " Input list:\n";
        for(int i=1; i<argc; i++){ 
            std::cout << "(" << i << ") = " << argv[i] << std::endl;
        }
        return true;
    }
    return false;
}

void ReadExternalJson(FString FileName)
{
    struct stat buf;
    if( stat(FileName.c_str(), &buf) == 0 ){
        std::cout << "Found external json file.\n";
    } else {
        std::cout << "Could not find this external file. Creating mock json data in a file with that name.\n";
        CreateMockJson(FileName);
    }
    return;
}

void CreateMockJson(FString FileName)
{
    json MockJson;

    std::cout << "Creating mock Json file.\n";
    MockJson = {
        {
            {"event","buy"},
            {"timestamp","2016-09-22T13:57:31.2311892-04:00"}
        },
        {
            {"event","buy"},
            {"timestamp","2016-09-22T18:57:31.2311892-04:00"}
        },
        {
            {"event","click-ad"},
            {"timestamp","2016-09-22T19:57:31.2311892-04:00"}
        },
        {
            {"event","click-promotion"},
            {"timestamp","2016-09-23T18:57:31.2311892-04:00"}
        },
        {
            {"event","click-promotion"},
            {"timestamp","2016-09-23T18:58:31.2311892-04:00"}
        },
        {
            {"event","buy-promotion"},
            {"timestamp","2016-09-23T18:58:31.2311892-04:00"}
        },
        {
            {"event","click"},
            {"timestamp","2016-09-23T18:58:31.2311892-04:00"}
        },
    };
    WriteJson(FileName,MockJson);
    return;
}

// Writes a Json file from a a json variable
void WriteJson(FString FileName,json JsonData)
{
    std::ofstream TimelineFile;
    struct stat buf;

    if( stat(FileName.c_str(), &buf) != -1 ){ std::cout << "Warning: output file already exists. Overwriting.\n"; }
    TimelineFile.open(FileName);  
    if( TimelineFile.fail() ){ throw std::runtime_error("Error opening output file."); }
    TimelineFile << JsonData;
    TimelineFile.close();
    if( TimelineFile.fail() ){ throw std::runtime_error("Error closing output file."); }

    return;
}

FString GetUserInput()
{
    FString Input;
    std::cout << "Please enter: a string to autocomplete or a special command (:h for help):\n";
    std::cin >> Input;
    return Input;
}