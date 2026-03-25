#pragma once
#ifndef SPINSPINNER_HH_
#define SPINSPINNER_HH_

//Probablities for base spin qualities (think spin code)
#define CAMEL_CHANGEFOOTSPIN_PROB 0.5
#define CAMEL_FLYINGSPIN_PROB 0.5
#define SIT_CHANGEFOOTSPIN_PROB 0.5
#define SIT_FLYINGSPIN_PROB 0.5
#define UPRIGHT_CHANGEFOOTSPIN_PROB 0.5
#define UPRIGHT_FLYINGSPIN_PROB 0.01
#define LAYBACK_CHANGEFOOTSPIN_PROB 0.5
#define LAYBACK_FLYINGSPIN_PROB 0.01
#define COMBO_CHANGEFOOTSPIN_PROB 0.5
#define COMBO_FLYINGSPIN_PROB 0.5

//Weights for spin order in combo spins (sorta)
#define COMBO_START_CAMEL_PROB 0.6
#define COMBO_START_SIT_PROB 0.36
#define COMBO_START_UPRIGHT_PROB 0.04

//Probablities for spin bullets
#define ADD_VARIATION_PROB 0.3
#define ADD_SPIN_FEATURE_PROB 0.2
#define ADD_POSITION_FEATURE_PROB 0.2
#define ADD_INTERMEDIATE_POSITION_PROB 0.15 //only affects combo spins
#define ADD_CHANGE_OF_DIRECTION_PROB 0.15

#include <algorithm>
#include <string>
#include <vector>
#include "spin.hh"
#include "easyrandom.hh"

class SpinSpinner
{
    public:
        std::vector<Spin> spinHistory;

        std::string spin(); //as in the act of spinning a wheel, not as in a skating spin
        std::string spin(int level);
        std::string spin(char spinType, int level);
    private:
            std::string spin_spinInOnePosition(char spinType, int level);
            std::string spin_combo(int level);
                void spin_combo_addComboPosition(Spin& newSpin, char footness, int startPoint, int level);
                bool spin_combo_hasAllRequiredPositions(Spin& newSpin);
            void spin_decideRandomBaseQualities(Spin& newSpin);
            void spin_addLevelRandomly(Spin& newSpin);
                int spin_addLevelRandomly_pickRandomAddition(Spin& newSpin);
                int spin_addLevelRandomly_pickRandomPositionIndex(Spin& newSpin);
            bool spin_checkForDifficultChangeOfPosition(Spin& newSpin);

    public:
        std::string spinHistoryPrettyPrint();

};

#endif
