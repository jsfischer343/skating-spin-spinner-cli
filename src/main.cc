#include <iostream>
#include <stdexcept>
#include <string>

#include "argparser.hh"
#include "easyrandom.hh"
#include "spinspinner.hh"

int main(int argc, char* argv[]) {

    ap::parser p(argc, argv);
    p.add("-l", "--level",  "Spin level",     ap::mode::OPTIONAL);
    p.add("-t", "--type",   "Type of spin (any, camel, sit, upright, layback, combo)",      ap::mode::OPTIONAL);
    p.add("-n", "--number",    "Number iterations", ap::mode::OPTIONAL);

    ap::argmap args = p.parse();

    if (!args.parsed_successfully()) {
        std::cerr << "Arguments unsuccessfully parsed\n";
        return -1;
    }

    int numberOfSpins;
    int spinLevel;
    std::string spinType;
    //argument validation for numberOfSpins, spinLevel, and spinType
    if(args["--type"].empty())
        spinType = "any";
    else
        spinType = args["--type"];
    try
    {
        if(args["--number"].empty())
            numberOfSpins = 1;
        else
            numberOfSpins = std::stoi(args["--number"]);
    }
    catch (std::invalid_argument e)
    {
       std::cout << "Number of iterations must be a valid number.\n";
    }
    try
    {
        if(args["--level"].empty())
            spinLevel = 0;
        else
            spinLevel = std::stoi(args["--level"]);
    }
    catch (std::invalid_argument e)
    {
        std::cout << "Level must be 0-4 (0 for base).\n";
    }


    SpinSpinner spinSpinnerObj = SpinSpinner();
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
    std::cout << spinSpinnerObj.spinHistoryToString() << std::endl;
    spinSpinnerObj.spinHistory.clear();

    return 0;
}
