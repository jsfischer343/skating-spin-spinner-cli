/*Copyright (C) 2026, John Fischer
*
*This program is free software: you can redistribute it and/or modify
*it under the terms of the GNU General Public License as published by
*the Free Software Foundation, either version 3 of the License, or
*(at your option) any later version.
*
*This program is distributed in the hope that it will be useful,
*but WITHOUT ANY WARRANTY; without even the implied warranty of
*MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*GNU General Public License for more details.
*
*You should have received a copy of the GNU General Public License
*along with this program.  If not, see <https://www.gnu.org/licenses/>.
*
* Version: 1.0
*/

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
        for(size_t i=0;i<validSpinTypes.size();i++)
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
        args["--level"] = easyRandom::pickFromVector(validLevels);
        levelValid = true;
    }
    else
    {
        for(size_t i=0;i<validLevels.size();i++)
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
    catch (std::invalid_argument e){}; //numberValid = false by default

    if(!spinTypeValid || !levelValid || !numberValid)
    {
        std::cout << "Invalid arguments. Use -h flag for more information.\n";
        exit(1);
    }
    return;
}

int main(int argc, char* argv[]) {

    ap::parser p(argc, argv);
    p.add("-l", "--level",  "Spin level (numeric 0-4)");
    p.add("-t", "--type",   "Type of spin (any, camel, sit, upright, layback, combo)");
    p.add("-n", "--number", "Number spins spun (between 1-100)");
    p.add("-r", "--reverse","Sets default direction to clockwise instead of counter-clockwise", ap::mode::BOOLEAN);
    p.add("-c", "--code",   "Prints spin as code rather than human readable", ap::mode::BOOLEAN);

    ap::argmap args = p.parse();

    if (!args.parsed_successfully()) {
        std::cout << "Invalid arguments. Use -h flag for more information.\n";
        return 1;
    }

    validateInput(args);

    SpinSpinner spinSpinnerObj = SpinSpinner(std::stoi(args["--reverse"]));
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
    if(std::stoi(args["--code"]))
    {
        std::cout << spinSpinnerObj.spinHistoryToCode() << std::endl;
    }
    else
    {
        std::cout << spinSpinnerObj.spinHistoryPrettyPrint() << std::endl;
    }
    spinSpinnerObj.spinHistory.clear();

    return 0;
}
