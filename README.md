# autocomplete
Emulation of an autocomplete API. This is a terminal application that accumulates data about
the navigatin of users in a website and uses it to suggest completions to strings that the user
enters.

The example of data that I received was a json file that looked like this:
```
{
    {"event","buy"},
    {"timestamp","2016-09-22T13:57:31.2311892-04:00"}
}
```
which led me to think that the users of this API will be the people who are analysing the
traffic in some brand website (possibly workers for that brand itself).

## How to use this emulation
To interface with this API, simply input special commands or the string you want to be
completed. The API will then display the completion suggestions or execute the command.

There are three special commands to use: `:q` to quit the application, `:l` to display the
accumulated data and `:h` to display help.

To use the API, call it from from the command line and give the name of the json file you want
to use as an argument. If you don't have such a file, just give it any name and it will create
one with mock data.


## Compilation
You will need to use the [JSON for Modern C++ library by Niels Lohmann](https://github.com/nlohmann/json/releases)
in order to compile. Just add `nlohmann/json.hpp` to the directory with main.cpp and autocomplete.cpp.
