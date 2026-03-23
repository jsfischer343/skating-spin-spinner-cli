#ifndef SPINPOSITION_HH_
#define SPINPOSITION_HH_

#define DEFAULT_DIRECTION 'r'

#include <string>
#include <vector>
#include "easyrandom.hh"

class SpinPosition
{
    public:
        //Data
        char position = -1; //basic position: camel, sit, upright, layback
        char direction = DEFAULT_DIRECTION; //rotational direction: ccw, cw
        char footness = -1; //footness: spinning on dominant (backspin) or non-dominant (forwardspin) foot (depends on rotational direction)
        char variation = -1; //change of body making spin position more difficult
        std::vector<char> features; //features that are associated with a specific position (see spin features)

        /* --Valid Settings--
         * position: camel -> 'c'; sit -> 's'; upright -> 'u'; layback -> 'l'; intermediate -> 'i'
         * direction: ccw -> 'r'; cw -> 'l'
         * footness: backspin -> 'b'; forwardspin -> 'f'
         * variation: up -> 'u'; front -> 'f'; behind -> 'b'; side -> 's'
         * features: blade -> 'b'; coe -> 'c'; jump -> 'j'; 8 revs -> '8'; speed -> 's'
         */


        SpinPosition(char position, char footness);

        std::string getPositionString() const;
        std::string getDirectionString() const;
        std::string getFootnessString() const;
        std::string getVariationString() const;
        std::string getFeatureString() const;
        bool hasVariation() const;
        bool hasFeature() const;

        static char pickRandomFeature();
        static char pickRandomVariation(char position);
};

#endif
