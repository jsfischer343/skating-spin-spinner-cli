#include "spinspinner.hh"


std::string SpinSpinner::spin()
{
    int tempRandomSelect = easyRandom::range(0,4);
    int tempRandomLevel = easyRandom::range(0,4);
    switch(tempRandomSelect)
    {
        case 0:
            return spin_spinInOnePosition('c',tempRandomLevel);
        case 1:
            return spin_spinInOnePosition('s',tempRandomLevel);
        case 2:
            return spin_spinInOnePosition('u',tempRandomLevel);
        case 3:
            return spin_spinInOnePosition('l',tempRandomLevel);
        case 4:
            return spin_combo(tempRandomLevel);
        default:
            throw;
    }
}
std::string SpinSpinner::spin(int level)
{
    int tempRandomSelect = easyRandom::range(0,4);
    switch(tempRandomSelect)
    {
        case 0:
            return spin_spinInOnePosition('c',level);
        case 1:
            return spin_spinInOnePosition('s',level);
        case 2:
            return spin_spinInOnePosition('u',level);
        case 3:
            return spin_spinInOnePosition('l',level);
        case 4:
            return spin_combo(level);
        default:
            throw;
    }
}
std::string SpinSpinner::spin(char type, int level)
{
    switch(type)
    {
        case 'c':
            return spin_spinInOnePosition('c',level);
        case 's':
            return spin_spinInOnePosition('s',level);
        case 'u':
            return spin_spinInOnePosition('u',level);
        case 'l':
            return spin_spinInOnePosition('l',level);
        case 'k':
            return spin_combo(level);
        default:
            throw;
    }
}

std::string SpinSpinner::spin_spinInOnePosition(char type, int level)
{
    Spin newSpin = Spin(type);
    spin_decideRandomBaseQualities(newSpin);

    //randomly decide if it will be a forward to back spin or back spin to forward spin
    if(newSpin.isChangeFoot)
    {
        if(easyRandom::range(0,1))
        {
            newSpin.spinPositions.push_back(SpinPosition(type,'b'));
            newSpin.spinPositions.push_back(SpinPosition(type,'f'));
        }
        else
        {
            newSpin.spinPositions.push_back(SpinPosition(type,'f'));
            newSpin.spinPositions.push_back(SpinPosition(type,'b'));
        }
    }
    else
    {
        if(easyRandom::range(0,1))
        {
            newSpin.spinPositions.push_back(SpinPosition(type,'b'));
        }
        else
        {
            newSpin.spinPositions.push_back(SpinPosition(type,'f'));
        }
    }
    //add levels
    for(int i=0;i<level;i++)
        spin_addLevelRandomly(newSpin);

    //record spin
    spinHistory.push_back(newSpin);
    //get string representation of spin
    return newSpin.toString();
}
std::string SpinSpinner::spin_combo(int level)
{
    Spin newSpin = Spin('k');
    spin_decideRandomBaseQualities(newSpin);

    //pick first position
    char tempRandomStartPosition;
    int tempRandomSelect;
    if(level<1)
        tempRandomSelect = 2;
    else
        tempRandomSelect = easyRandom::pickFromVectorWeighted(std::vector<int>{0,1,2},std::vector<double>{COMBO_START_UPRIGHT_PROB,COMBO_START_SIT_PROB,COMBO_START_CAMEL_PROB});
    if(tempRandomSelect==0) //upright start (unlikely)
        tempRandomStartPosition = 'u';
    else if(tempRandomSelect==1) //sit start
        tempRandomStartPosition = 's';
    else if(tempRandomSelect==2) //camel start
        tempRandomStartPosition = 'c';

    //pick backwards or forward start
    char tempRandomFootness;
    if(easyRandom::range(0,1))
        tempRandomFootness = 'b';
    else
        tempRandomFootness = 'f';

    //push first position
    newSpin.spinPositions.push_back(SpinPosition(tempRandomStartPosition,tempRandomFootness));

    if(newSpin.isChangeFoot)
    {
        bool endSpinBool = false; //randomly turned true to end the spin early (i.e. less than 6 positions)
        bool swappedFeet = false;
        char firstFoot = tempRandomFootness;
        char secondFoot;
        int secondFootStartPoint;
        int secondFootCounter = 0;
        if(firstFoot=='b')
            secondFoot = 'f';
        else
            secondFoot = 'b';

        while(newSpin.spinPositions.size()<6 && !endSpinBool)
        {
            if(!swappedFeet)
            {
                //after each position, randomly need to decide if the change of foot will take place
                if(easyRandom::weightedTruth(0.4) || newSpin.spinPositions.size()==3) //if spin already has 3 positions on one foot the next position must be on the other foot
                {
                    secondFootStartPoint = newSpin.spinPositions.size();
                    swappedFeet = true;
                    spin_combo_addComboPosition(newSpin,secondFoot,secondFootStartPoint,level);
                    secondFootCounter++;
                }
                else
                {
                    spin_combo_addComboPosition(newSpin,firstFoot,0,level);
                }
            }
            else
            {
                spin_combo_addComboPosition(newSpin,secondFoot,secondFootStartPoint,level);
                secondFootCounter++;
                if(spin_combo_hasAllRequiredPositions(newSpin))
                {
                    if(secondFootCounter==3) //must end if on second foot and done three positions
                    {
                        endSpinBool=true;
                    }
                    else if(easyRandom::weightedTruth(0.6))
                    {
                        endSpinBool=true;
                    }
                }
            }
        }
    }
    else
    {
        spin_combo_addComboPosition(newSpin,tempRandomFootness,0,level); //add two last positions on same foot
        spin_combo_addComboPosition(newSpin,tempRandomFootness,0,level);
    }
    //check for difficult change of position
    if(level>=1)
    {
        if(spin_checkForDifficultChangeOfPosition(newSpin))
        {
            newSpin.level++;
            level--;
        }
    }
    //add levels
    for(int i=0;i<level;i++)
        spin_addLevelRandomly(newSpin);

    //record spin
    spinHistory.push_back(newSpin);
    //get string representation of spin
    return newSpin.toString();
}
void SpinSpinner::spin_combo_addComboPosition(Spin& newSpin, char footness, int startPoint, int level)
{
    if(startPoint+3<=newSpin.spinPositions.size()) //if this is true then it would imply that we are trying to add a 4th position to the same foot which wouldn't make sense
    {
        throw;
    }

    if(level>=1) //combo spins that are level 1 or higher can have a difficult change of position so the logic is more lax (i.e. there is more rolling of random numbers)
    {
        std::vector<char> validPositions = {'c','s','u'};
        std::vector<char> usedPositions = {};
        std::vector<char> unusedPositions;
        char nextPosition;
        int i=0;
        while(usedPositions.size()<newSpin.spinPositions.size()-startPoint)
        {
            usedPositions.push_back(newSpin.spinPositions.at(i+startPoint).position);
            i++;
        }
        std::sort(usedPositions.begin(),usedPositions.end());
        std::set_difference(validPositions.begin(),validPositions.end(),usedPositions.begin(),usedPositions.end(),std::back_inserter(unusedPositions));
        std::vector<double> positionWeightsForUnusedPositions;
        for(int i=0;i<unusedPositions.size();i++)
        {
            if(unusedPositions.at(i)=='c')
                positionWeightsForUnusedPositions.push_back(COMBO_START_CAMEL_PROB);
            else if(unusedPositions.at(i)=='s')
                positionWeightsForUnusedPositions.push_back(COMBO_START_SIT_PROB);
            else if(unusedPositions.at(i)=='u')
                positionWeightsForUnusedPositions.push_back(COMBO_START_UPRIGHT_PROB);
        }
        nextPosition = easyRandom::pickFromVectorWeighted(unusedPositions,positionWeightsForUnusedPositions);
        newSpin.spinPositions.push_back(SpinPosition(nextPosition,footness));
    }
    else //there is a small amount of base combos that can be rolled so the logic is a bit simpler. Note: intermediate positions aren't valid for a base spin
    {
        char nextPosition;
        if(startPoint==newSpin.spinPositions.size() && easyRandom::range(0,1)) //First param implies this position will be a change of foot. Second is because logic only changes if the position type is repeated when transitioning to the other foot (which can be randomly decided).
        {
            if(newSpin.spinPositions.at(newSpin.spinPositions.size()-1).position=='c')
                nextPosition = 'c';
            else if(newSpin.spinPositions.at(newSpin.spinPositions.size()-1).position=='s')
                nextPosition = 's';
            else if(newSpin.spinPositions.at(newSpin.spinPositions.size()-1).position=='u')
                nextPosition = 'u';
        }
        else //this position is on the same foot as the previous position
        {
            if(newSpin.spinPositions.at(newSpin.spinPositions.size()-1).position=='c')
                nextPosition = 's';
            else if(newSpin.spinPositions.at(newSpin.spinPositions.size()-1).position=='s')
                nextPosition = 'u';
        }
        newSpin.spinPositions.push_back(SpinPosition(nextPosition,footness));
    }
}
bool SpinSpinner::spin_combo_hasAllRequiredPositions(Spin& newSpin)
{
    std::vector<bool> tempTruthVector = {0,0,0};
    for(int i=0;i<newSpin.spinPositions.size();i++)
    {
        if(newSpin.spinPositions.at(i).position=='c')
        {
            tempTruthVector.at(0)=1;
        }
        else if(newSpin.spinPositions.at(i).position=='s')
        {
            tempTruthVector.at(1)=1;
        }
        else if(newSpin.spinPositions.at(i).position=='u')
        {
            tempTruthVector.at(2)=1;
        }
    }
    for(int i=0;i<3;i++)
    {
        if(!tempTruthVector.at(i))
            return false;
    }
    return true;
}
void SpinSpinner::spin_decideRandomBaseQualities(Spin& newSpin)
{
    double changeFootSpinProb;
    double flyingFootSpinProb;
    if(newSpin.baseType=='c')
    {
        changeFootSpinProb = CAMEL_CHANGEFOOTSPIN_PROB;
        flyingFootSpinProb = CAMEL_FLYINGSPIN_PROB;
    }
    else if(newSpin.baseType=='s')
    {
        changeFootSpinProb = SIT_CHANGEFOOTSPIN_PROB;
        flyingFootSpinProb = SIT_FLYINGSPIN_PROB;
    }
    else if(newSpin.baseType=='u')
    {
        changeFootSpinProb = UPRIGHT_CHANGEFOOTSPIN_PROB;
        flyingFootSpinProb = UPRIGHT_FLYINGSPIN_PROB;
    }
    else if(newSpin.baseType=='l')
    {
        changeFootSpinProb = LAYBACK_CHANGEFOOTSPIN_PROB;
        flyingFootSpinProb = LAYBACK_FLYINGSPIN_PROB;
    }
    else if(newSpin.baseType=='k')
    {
        changeFootSpinProb = COMBO_CHANGEFOOTSPIN_PROB;
        flyingFootSpinProb = COMBO_FLYINGSPIN_PROB;
    }

    if(easyRandom::weightedTruth(changeFootSpinProb))
        newSpin.isChangeFoot = true;
    else
        newSpin.isChangeFoot = false;

    if(easyRandom::weightedTruth(flyingFootSpinProb))
        newSpin.isFlying = true;
    else
        newSpin.isFlying = false;
}
void SpinSpinner::spin_addLevelRandomly(Spin& newSpin)
{
    if(newSpin.level==0) //increasing from level 1->2
    {
        int tempRandomSelect = spin_addLevelRandomly_pickRandomAddition(newSpin);
        if(tempRandomSelect==0)
        {
            //add variation
            int tempRandomPositionIndex = easyRandom::range(0,newSpin.spinPositions.size()-1);
            char randomVariation = SpinPosition::pickRandomVariation(newSpin.spinPositions.at(tempRandomPositionIndex).position);
            newSpin.spinPositions.at(tempRandomPositionIndex).variation = randomVariation;
            newSpin.level++;
            return;
        }
        else if(tempRandomSelect==1)
        {
            //add spin feature
            int tempRandomSelect;
            int tempLowerRandomRange = 1;
            int tempUpperRandomRange = 2;
            if(newSpin.isChangeFoot)
                tempLowerRandomRange--;
            if(newSpin.baseType=='l')
                tempUpperRandomRange = 4;

            tempRandomSelect = easyRandom::range(tempLowerRandomRange,tempUpperRandomRange);
            if(tempRandomSelect==0) //changeFootByJump
            {
                newSpin.spinFeatures.changeFootByJump = true;
                newSpin.level++;
                return;
            }
            else if(tempRandomSelect==1) //difficultEntrance
            {
                newSpin.spinFeatures.difficultEntrance = true;
                newSpin.level++;
                return;
            }
            else if(tempRandomSelect==2) //difficultExit
            {
                newSpin.spinFeatures.difficultExit = true;
                newSpin.level++;
                return;
            }
            else if(tempRandomSelect==3) //laybackTransition
            {
                newSpin.spinFeatures.laybackTransition = true;
                newSpin.level++;
                return;
            }
            else if(tempRandomSelect==4) //biellmann after layback
            {
                newSpin.spinFeatures.biellmannAfterLayback = true;
                newSpin.level++;
                return;
            }

        }
        else if(tempRandomSelect==2)
        {
            //add position feature
            int tempRandomPositionIndex = easyRandom::range(0,newSpin.spinPositions.size()-1);
            char randomFeature = SpinPosition::pickRandomFeature();
            newSpin.spinPositions.at(tempRandomPositionIndex).features.push_back(randomFeature);
            newSpin.level++;
            return;
        }
        else if(tempRandomSelect==3) //only occurs in combo spins (see pickRandomAddition sub-function for logic)
        {
            //add intermediate position
            int tempRandomPositionIndex = easyRandom::range(0,newSpin.spinPositions.size()-1);
            newSpin.spinPositions.insert(newSpin.spinPositions.begin()+tempRandomPositionIndex+1,SpinPosition('i',newSpin.spinPositions.at(tempRandomPositionIndex).footness));
            newSpin.hasIntermediatePositionFlag = true;
            newSpin.level++;
            return;
        }
        else
            throw; //implies that tempRandomSelect is outside of possible spin additions (this would be a bug)
    }
    else if(newSpin.level==1 || newSpin.level==2) //increasing to level 2 and 3 is relatively similar but requires checks for conflicts (current implementation of "checks" just means re-roll until it doesn't conflict)
    {
        while(true) //keep looping until the rolled "spin addition" (variation/features) doesn't conflict with any other additions (lazy implementation)
        {
            int tempRandomSelect = spin_addLevelRandomly_pickRandomAddition(newSpin);
            if(tempRandomSelect==0)
            {
                //add variation
                if(newSpin.hasTwoVariationsFlag)
                    continue;
                else
                {
                    int tempRandomPositionIndex = easyRandom::range(0,newSpin.spinPositions.size()-1);
                    char randomVariation = SpinPosition::pickRandomVariation(newSpin.spinPositions.at(tempRandomPositionIndex).position);
                    if(newSpin.positionVariationUsed(newSpin.spinPositions.at(tempRandomPositionIndex).position,randomVariation)) //if it is already used then re-roll selection of type of "spin addition"
                        continue;
                    else
                    {
                        newSpin.spinPositions.at(tempRandomPositionIndex).variation = randomVariation;
                        if(newSpin.hasTwoVariations())
                            newSpin.hasTwoVariationsFlag = true;
                        newSpin.level++;
                        return;
                    }
                }

            }
            else if(tempRandomSelect==1)
            {
                //add spin feature
                int tempRandomSelect2;
                int tempLowerRandomRange = 1;
                int tempUpperRandomRange = 2;
                if(newSpin.isChangeFoot)
                    tempLowerRandomRange--;
                if(newSpin.baseType=='l')
                    tempUpperRandomRange = 4;

                tempRandomSelect2 = easyRandom::range(tempLowerRandomRange,tempUpperRandomRange);
                if(tempRandomSelect2==0 && !newSpin.spinFeatures.changeFootByJump) //changeFootByJump
                {
                    newSpin.spinFeatures.changeFootByJump = true;
                    newSpin.level++;
                    return;
                }
                else if(tempRandomSelect2==1 &&
                !newSpin.spinFeatures.difficultExit &&
                !newSpin.spinFeatures.difficultEntrance) //difficultEntrance
                {
                    newSpin.spinFeatures.difficultEntrance = true;
                    newSpin.level++;
                    return;
                }
                else if(tempRandomSelect2==2 &&
                !newSpin.spinFeatures.difficultExit &&
                !newSpin.spinFeatures.difficultEntrance) //difficultExit
                {
                    newSpin.spinFeatures.difficultExit = true;
                    newSpin.level++;
                    return;
                }
                else if(tempRandomSelect2==3 && !newSpin.spinFeatures.laybackTransition) //laybackTransition
                {
                    newSpin.spinFeatures.laybackTransition = true;
                    newSpin.level++;
                    return;
                }
                else if(tempRandomSelect2==4 && !newSpin.spinFeatures.biellmannAfterLayback) //biellmann after layback
                {
                    newSpin.spinFeatures.biellmannAfterLayback = true;
                    newSpin.level++;
                    return;
                }
                else
                    continue;
            }
            else if(tempRandomSelect==2)
            {
                //add position feature
                char randomFeature = SpinPosition::pickRandomFeature();
                if(newSpin.positionFeatureUsed(randomFeature))
                    continue;
                else
                {
                    int tempRandomPositionIndex = easyRandom::range(0,newSpin.spinPositions.size()-1);
                    newSpin.spinPositions.at(tempRandomPositionIndex).features.push_back(randomFeature);
                    newSpin.level++;
                    return;
                }
            }
            else if(tempRandomSelect==3) //only occurs in combo spins (see pickRandomAddition sub-function for logic)
            {
                //add intermediate position
                if(newSpin.hasIntermediatePositionFlag)
                    continue;
                else
                {
                    int tempRandomPositionIndex = easyRandom::range(0,newSpin.spinPositions.size()-1);
                    newSpin.spinPositions.insert(newSpin.spinPositions.begin()+tempRandomPositionIndex+1,SpinPosition('i',newSpin.spinPositions.at(tempRandomPositionIndex).footness));
                    newSpin.hasIntermediatePositionFlag = true;
                    newSpin.level++;
                    return;
                }
            }
            else
                throw; //implies that tempRandomSelect is outside of possible spin additions (this would be a bug)
        }
    }
    else if(newSpin.level==4) //level four spins have extra special rules because reasons
    {

    }
}
int SpinSpinner::spin_addLevelRandomly_pickRandomAddition(Spin& newSpin)
{
    //0: difficult variation, 1: spin feature, 2: position feature, 3: intermediate position
    if(newSpin.baseType=='k')
        return easyRandom::pickFromVectorWeighted(std::vector<int>{0,1,2,3},std::vector<double>{ADD_VARIATION_PROB,ADD_SPIN_FEATURE_PROB,ADD_POSITION_FEATURE_PROB,ADD_INTERMEDIATE_POSITION_PROB});
    else
        return easyRandom::pickFromVectorWeighted(std::vector<int>{0,1,2},std::vector<double>{ADD_VARIATION_PROB,ADD_SPIN_FEATURE_PROB,ADD_POSITION_FEATURE_PROB});
}
bool SpinSpinner::spin_checkForDifficultChangeOfPosition(Spin& newSpin)
{
    if(newSpin.spinPositions.size()<2)
        return false;

    char previousPosition;
    char currentPosition;
    for(int i=1;i<newSpin.spinPositions.size();i++)
    {
        previousPosition = newSpin.spinPositions.at(i-1).position;
        currentPosition = newSpin.spinPositions.at(i).position;
        if(previousPosition=='s'||previousPosition=='u')
            if(currentPosition=='c')
            {
                newSpin.spinFeatures.difficultChangeOfPosition = true;
                return true;
            }
    }
    return false;
}

std::string SpinSpinner::spinHistoryToString()
{
    std::string historyString = "";
    for(int i=0;i<spinHistory.size();i++)
    {
        historyString += spinHistory.at(i).toString()+"\n";
    }
    return historyString;
}
