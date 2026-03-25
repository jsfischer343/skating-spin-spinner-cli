#pragma once
#ifndef SPINPOSITION_HH_
#define SPINPOSITION_HH_

#include <algorithm>
#include <string>
#include <vector>
#include "easyrandom.hh"

class SpinPosition
{
    public:
        /* --Valid Settings--
         * position: camel -> 'c'; sit -> 's'; upright -> 'u'; layback -> 'l'; intermediate -> 'i'
         * variation: up -> 'u'; front -> 'f'; behind -> 'b'; side -> 's'
         * features: blade -> 'b'; coe -> 'c'; jump -> 'j'; 8 revs -> '8'; speed -> 's'
         */
        char position = -1; //basic position: camel, sit, upright, layback, intermediate
        std::vector<char> variations = {}; //change of body making spin position more difficult
        std::vector<char> features = {}; //features that are associated with a specific position (see spin features)


        SpinPosition(char position);

        std::string getPositionString() const;
        std::string getVariationString() const;
        std::string getFeatureString() const;
        bool hasVariation() const;
        bool hasFeature() const;
        bool pickRandomFeature() const;
        bool pickRandomVariation() const;
};

#endif
