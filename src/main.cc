#include <iostream>
#include <stdexcept>
#include <string>

#include "argparser.hh"
#include "easyrandom.hh"
#include "spinspinner.hh"

void validateInput(ap::argmap& args)
{
    //spin type
    bool spinTypeValid = false;
    std::vector<std::string> validSpinTypes = {"any","camel","sit","upright","layback","combo"};
    if(args["--type"].empty())
    {
        args["--type"] = "any";
        spinTypeValid = true;
    }
    else
    {
        for(int i=0;i<validSpinTypes.size();i++)
        {
            if(args["--type"]==validSpinTypes.at(i))
                spinTypeValid = true;
        }
    }

    //level
    bool levelValid = false;
    std::vector<std::string> validLevels = {"0","1","2","3","4"};
    if(args["--level"].empty())
    {
        args["--level"] = "0";
        levelValid = true;
    }
    else
    {
        for(int i=0;i<validLevels.size();i++)
        {
            if(args["--level"]==validLevels.at(i))
                levelValid = true;
        }
    }

    //number of spins spun
    bool numberValid = false;
    try
    {
        if(args["--number"].empty())
        {
            args["--number"] = "1";
            numberValid = true;
        }
        else
        {
           int tempCheck = std::stoi(args["--number"]);
           if(tempCheck>0 && tempCheck<=100)
               numberValid = true;
        }
    }
    catch (std::invalid_argument e)
    {/*Then by default the number is invalid*/}

    if(!spinTypeValid || !levelValid || !numberValid)
    {
        std::cout << "Invalid arguments. Use -h flag for more information.\n";
        exit(1);
    }
    return;
}

int main(int argc, char* argv[]) {

    ap::parser p(argc, argv);
    p.add("-l", "--level",  "Spin level (numeric 0-4)",     ap::mode::OPTIONAL);
    p.add("-t", "--type",   "Type of spin (any, camel, sit, upright, layback, combo)",      ap::mode::OPTIONAL);
    p.add("-n", "--number",    "Number spins spun (between 1-100)", ap::mode::OPTIONAL);

    ap::argmap args = p.parse();

    if (!args.parsed_successfully()) {
        std::cout << "Invalid arguments. Use -h flag for more information.\n";
        return 1;
    }

    validateInput(args);

    SpinSpinner spinSpinnerObj = SpinSpinner();
    int spinLevel = std::stoi(args["--level"]); //if empty it will be set to 0 by validateInput()
    std::string spinType = args["--type"];
    int numberOfSpins = std::stoi(args["--number"]);
    for(int i=0;i<numberOfSpins;i++)
    {
        if(spinType=="any")
        {
            spinSpinnerObj.spin(spinLevel);
        }
        else if(spinType=="combo")
        {
            spinSpinnerObj.spin('k', spinLevel);
        }
        else
        {
            spinSpinnerObj.spin(spinType.c_str()[0], spinLevel);
        }
    }
    std::cout << spinSpinnerObj.spinHistoryToString() << std::endl;
    spinSpinnerObj.spinHistory.clear();

    return 0;
}
