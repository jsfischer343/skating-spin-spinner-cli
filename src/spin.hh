#pragma once
#ifndef SPIN_HH_
#define SPIN_HH_

#include <string>
#include <vector>
#include <utility>
#include "spinposition.hh"

class Spin
{
    public:
        //Base spin qualities
        char baseType = -1; //camel -> 'c'; sit -> 's'; upright -> 'u'; layback -> 'l'; combo -> 'k'
        bool isFlying = false;
        bool isChangeFoot = false;
        int level = 0;
        std::vector<SpinPosition> spinPositions;

        //spin features
        typedef struct {
            bool changeFootByJump = false;
            bool difficultChangeOfPosition = false;
            bool difficultEntrance = false;
            bool difficultExit = false;
            bool laybackTransition = false;
            bool biellmannAfterLayback = false;
        } SpinFeatures;
        SpinFeatures spinFeatures;

        //flags
        bool hasIntermediatePositionFlag = false;
        bool hasTwoVariationsFlag = false; //only the first two difficult variations count towards levels
        bool hasChangeDirectionFlag = false;

        Spin(char baseType);

        bool positionVariationUsed(char positionChar, char variationChar) const; //the same variation can't be used on the same position type (i.e. camel side twice, sit front twice, ...)
        bool positionFeatureUsed(char featureChar) const;
        bool hasTwoVariations() const;
        int getChangeOfFootIndex() const;
        std::string toString() const;
    private:
            std::pair<std::string,int> toString_part(int startIndex) const;
};

#endif
