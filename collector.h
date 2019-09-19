#include <fstream>
#include <string>
#include "nlohmann/json.hpp"

using json = nlohmann::json;
#pragma once


class FColector{
    public:

        // call to read json data, and store it in internal "list"
        // function to return entries that match two beginning letters

        json ReadJson(std::string FileName);
        void EventNamesAlphabetic(std::string FileName);

    private:
        // "list" to hold the event names (in alphabetical order)
        // "list" to hold ocurrences (and make suggestions based on that)
        // two indexes to get the indexes of entries that will have to be returned
        int i1=0;
        int i2=0;

        // function to read a Json file

}