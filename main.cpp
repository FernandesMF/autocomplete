#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include "nlohmann/json.hpp"
#include "autocompletedata.h"

using json = nlohmann::json;
#define OUT         // useful tag to remind of output references

bool ArgCountProblem(int, char**);
void CreateMockJson(std::string);
void WriteJson(std::string,json);

FAutoCompleteData ACData;   // start an instance of the autocomplete class (autocompletedata.h)

int main(int argc, char* argv[])
{    
    if(ArgCountProblem(argc,argv)){ return 1; } // check command line inputs

    // Preparatory steps
    try{ CreateMockJson(argv[1]); }     // create json file with events
    catch(const std::runtime_error& e){ 
        std::cerr << e.what() << '\n';
        return 1;
    }
    
    /*
    try{ ACData.EventNamesAlphabetic(argv[1]); }   // collect that data in the collector class
    catch(const std::runtime_error& e){ 
        std::cerr << e.what() << '\n';
        return 1;
    }
    */

    // Print starting message (special commands)

    // User input loop (while quit command is not given)
        // Ask user input
        // Check for special command
        // Check input validity (two letters)
        // Call autocomplete function
        // print back autocomplete loop

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

void CreateMockJson(std::string FileName)
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
        }
    };
    WriteJson(FileName,MockJson);
    return;
}

// Writes a Json file from a a json variable
void WriteJson(std::string FileName,json JsonData)
{
    std::ofstream TimelineFile;
    struct stat buf;

	// FIXME confirm overwriting with the user
    if( stat(FileName.c_str(), &buf) != -1 ){ std::cout << "Warning: output file already exists. Overwriting.\n"; }
    TimelineFile.open(FileName);  
    if( TimelineFile.fail() ){ throw std::runtime_error("Error opening output file."); }
    TimelineFile << JsonData;
    TimelineFile.close();
    if( TimelineFile.fail() ){ throw std::runtime_error("Error closing output file."); }

    return;
}