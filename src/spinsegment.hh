#pragma once
#ifndef SPINSEGMENT_HH_
#define SPINSEGMENT_HH_

#define DEFAULT_DIRECTION 'r'

#include <vector>
#include "spinposition.hh"

class SpinSegment
{
    public:
        /* Valid Settings
         * direction: ccw -> 'r'; cw -> 'l'
         * footness: backspin -> 'b'; forwardspin -> 'f'
         */
        char direction = DEFAULT_DIRECTION; //rotational direction: ccw -> 'r', cw -> 'l'
        char footness = -1; //footness: spinning on dominant (backspin) or non-dominant (forwardspin) foot (depends on rotational direction)
        std::vector<SpinPosition> spinPositions;

        //segment specific features
        typedef struct {
            bool difficultChangeOfPosition = false; //sit or upright to camel spin
            bool laybackTransition = false; //core transition backwards->sideways or reverse
            bool biellmannAfterLayback = false;
        } SpinSegmentFeatures;
        SpinSegmentFeatures features;


        SpinSegment(char direction);

        void swapDirection();

        int getBullets() const; //Difficult variations + features + other. Needed because there is a maximum of 2 bullets per foot in change foot spins.
        std::string getDirectionString() const;
        std::string getFootnessString() const;
};
#endif
