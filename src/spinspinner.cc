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
            newSpin.spinSegments.push_back(SpinSegment('f',std::vector<SpinPosition>{SpinPosition(type)}));
            newSpin.spinSegments.push_back(SpinSegment('b',std::vector<SpinPosition>{SpinPosition(type)}));
        }
        else
        {
            newSpin.spinSegments.push_back(SpinSegment('b',std::vector<SpinPosition>{SpinPosition(type)}));
            newSpin.spinSegments.push_back(SpinSegment('f',std::vector<SpinPosition>{SpinPosition(type)}));
        }
    }
    else
    {
        if(easyRandom::range(0,1))
        {
            newSpin.spinSegments.push_back(SpinSegment('f',std::vector<SpinPosition>{SpinPosition(type)}));
        }
        else
        {
            newSpin.spinSegments.push_back(SpinSegment('b',std::vector<SpinPosition>{SpinPosition(type)}));
        }
    }
    //add levels
    for(int i=0;i<level;i++)
        spin_addLevelRandomly(newSpin);

    //record spin
    spinHistory.push_back(newSpin);
    //get string representation of spin
    return newSpin.prettyPrint();
}
std::string SpinSpinner::spin_combo(int level)
{
    Spin newSpin = Spin('k');
    spin_decideRandomBaseQualities(newSpin);

    //pick first position
    char randomStartPosition;
    int randomSelect;
    if(level<1)
        randomSelect = 2;
    else
        randomSelect = easyRandom::pickFromVectorWeighted(std::vector<int>{0,1,2},std::vector<double>{COMBO_START_UPRIGHT_PROB,COMBO_START_SIT_PROB,COMBO_START_CAMEL_PROB});
    if(randomSelect==0) //upright start (unlikely)
        randomStartPosition = 'u';
    else if(randomSelect==1) //sit start
        randomStartPosition = 's';
    else if(randomSelect==2) //camel start
        randomStartPosition = 'c';

    //pick backwards or forward start
    char randomFootness;
    if(easyRandom::range(0,1))
        randomFootness = 'b';
    else
        randomFootness = 'f';

    //push first segment and position
    newSpin.spinSegments.push_back(SpinSegment(randomFootness,std::vector<SpinPosition>{SpinPosition(randomStartPosition)}));

    if(newSpin.isChangeFoot)
    {
        bool endSpinBool = false; //randomly turned true to end the spin early (i.e. less than 6 positions)
        bool swappedFeet = false;
        char secondFootFootness;
        if(randomFootness=='b')
            secondFootFootness = 'f';
        if(randomFootness=='f')
            secondFootFootness = 'b';

        while(newSpin.getTotalPositions()<6 && !endSpinBool)
        {
            if(!swappedFeet)
            {
                //after each position, randomly need to decide if the change of foot will take place
                if(easyRandom::weightedTruth(0.4) || newSpin.spinSegments.at(0).spinPositions.size()==3) //if spin already has 3 positions on one foot the next position must be on the other foot
                {
                    newSpin.spinSegments.push_back(SpinSegment(secondFootFootness));
                    swappedFeet = true;
                    spin_combo_addComboPosition(newSpin,level);
                }
                else
                {
                    spin_combo_addComboPosition(newSpin,level);
                }
            }
            else
            {
                spin_combo_addComboPosition(newSpin,level);
                if(newSpin.hasAllPrimaryPositions())
                {
                    if(newSpin.spinSegments.at(1).spinPositions.size()==3) //must end if on second foot and done three positions
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
        spin_combo_addComboPosition(newSpin,level); //add two last positions on same foot
        spin_combo_addComboPosition(newSpin,level);
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
    return newSpin.prettyPrint();
}
void SpinSpinner::spin_combo_addComboPosition(Spin& newSpin, int level)
{
    SpinSegment* currentSegment = &newSpin.spinSegments.at(newSpin.spinSegments.size()-1);

    if(currentSegment->spinPositions.size()==3 || newSpin.spinSegments.size()>2) //If this is true then it would imply that we are trying to add a 4th basic position to the same foot OR add a third segment, which wouldn't make sense
    {
        throw;
    }

    if(level>=1) //combo spins level 1 or higher can have a difficult change of position so the logic is more lax (i.e. there is more rolling of random numbers)
    {
        std::vector<char> validPositions = {'c','s','u'};
        std::vector<char> usedPositions = currentSegment->getUsedPositions();
        std::vector<char> unusedPositions;

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

        char nextPosition = easyRandom::pickFromVectorWeighted(unusedPositions,positionWeightsForUnusedPositions);
        currentSegment->spinPositions.push_back(SpinPosition(nextPosition));
    }
    else //there is a small amount of base combos that can be rolled so the logic is a bit simpler. Note: intermediate positions aren't valid for a base spin
    {
        char nextPosition;
        if(currentSegment->spinPositions.size()==0) //Is change of foot spin and this is the first position
        {
            SpinSegment* previousSegment = &newSpin.spinSegments.at(newSpin.spinSegments.size()-2);
            int lastIndex = previousSegment->spinPositions.size()-1;
            if(easyRandom::range(0,1)) //50/50 to repeat position on previous foot or change
            {
                if(previousSegment->spinPositions.at(lastIndex).position=='c')
                    nextPosition = 'c';
                else if(previousSegment->spinPositions.at(lastIndex).position=='s')
                    nextPosition = 's';
                else if(previousSegment->spinPositions.at(lastIndex).position=='u')
                    nextPosition = 'u';
            }
            else //
            {
                if(previousSegment->spinPositions.at(lastIndex).position=='c')
                    nextPosition = 's';
                else if(previousSegment->spinPositions.at(lastIndex).position=='s')
                    nextPosition = 'u';
            }
        }
        else //implies that this position is on the same foot as the previous position
        {
            int previousIndex = currentSegment->spinPositions.size()-1;
            if(currentSegment->spinPositions.at(previousIndex).position=='c')
                nextPosition = 's';
            else if(currentSegment->spinPositions.at(previousIndex).position=='s')
                nextPosition = 'u';
        }

        currentSegment->spinPositions.push_back(SpinPosition(nextPosition));
    }
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
    if(newSpin.level==0)
    {
        int randomSelect = spin_addLevelRandomly_pickRandomAddition(newSpin);
        if(randomSelect==0) //add variation
        {
            SpinPosition* randomPosition = spin_addLevelRandomly_pickRandomPosition(newSpin);
            char randomVariation = randomPosition->pickRandomVariation();
            randomPosition->variations.push_back(randomVariation);
        }
        else if(randomSelect==1) //add spin/segment feature
        {
            //add spin feature
            int randomSelect2;
            int tempLowerRandomRange = 1;
            int tempUpperRandomRange = 2;
            if(newSpin.isChangeFoot)
                tempLowerRandomRange--;
            if(newSpin.baseType=='l')
                tempUpperRandomRange = 3;

            randomSelect2 = easyRandom::range(tempLowerRandomRange,tempUpperRandomRange);
            if(randomSelect2==0) //changeFootByJump
            {
                newSpin.features.changeFootByJump = true;
            }
            else if(randomSelect2==1) //difficultEntrance
            {
                newSpin.features.difficultEntrance = true;
            }
            else if(randomSelect2==2) //difficultExit
            {
                newSpin.features.difficultExit = true;
            }
            else if(randomSelect2==3) //biellmann after layback
            {
                SpinSegment* randomSegment = spin_addLevelRandomly_pickRandomSegment(newSpin);
                randomSegment->features.biellmannAfterLayback = true;
            }
        }
        else if(randomSelect==2) //add position feature
        {
            SpinPosition* randomPosition = spin_addLevelRandomly_pickRandomPosition(newSpin);
            char randomFeature = randomPosition->pickRandomFeature();
            randomPosition->features.push_back(randomFeature);
        }
        else if(randomSelect==3) //add intermediate position (only for combos)
        {
            SpinSegment* randomSegment = spin_addLevelRandomly_pickRandomSegment(newSpin);
            int randomSpinPositionIndex = easyRandom::range(0,randomSegment->spinPositions.size()-1);
            randomSegment->spinPositions.insert(randomSegment->spinPositions.begin()+randomSpinPositionIndex+1,SpinPosition('i'));
            newSpin.intermediatePositionFlag = true;
        }
        else if(randomSelect==4) //change of direction (only for change foot spins)
        {
            if(easyRandom::range(0,1))
                newSpin.spinSegments.at(0).swapDirection();
            else
                newSpin.spinSegments.at(1).swapDirection();
            newSpin.changeDirectionFlag = true;
        }
        else
            throw; //implies that randomSelect is outside of possible spin additions (this would be a bug)
        newSpin.level++;
        return;
    }
    else if(newSpin.level==1 || newSpin.level==2) //increasing to level 2 and 3 is relatively similar but requires checks for conflicts (current implementation of "checks" just means re-roll until it doesn't conflict)
    {
        while(true) //keep looping until the rolled "spin addition" (variation/features) doesn't conflict with any other additions (lazy implementation)
        {
            int randomSelect = spin_addLevelRandomly_pickRandomAddition(newSpin);
            if(randomSelect==0)
            {
                //add variation
                if(newSpin.twoVariationsFlag)
                    continue;
                else
                {
                    SpinPosition* randomPosition = spin_addLevelRandomly_pickRandomPosition(newSpin);
                    char randomVariation = randomPosition->pickRandomVariation();
                    if(newSpin.variationUsed(randomPosition->position,randomVariation)) //if it is already used then re-roll selection of type of "spin addition"
                        continue;
                    else
                    {
                        randomPosition->variations.push_back(randomVariation);
                        if(newSpin.hasTwoVariations())
                        {
                            newSpin.twoVariationsFlag = true;
                            break;
                        }
                    }
                }
            }
            else if(randomSelect==1)
            {
                //add spin feature
                int randomSelect2;
                int tempLowerRandomRange = 1;
                int tempUpperRandomRange = 2;
                if(newSpin.isChangeFoot)
                    tempLowerRandomRange--;
                if(newSpin.baseType=='l')
                    tempUpperRandomRange = 3;

                randomSelect2 = easyRandom::range(tempLowerRandomRange,tempUpperRandomRange);
                if(randomSelect2==0 && !newSpin.features.changeFootByJump) //changeFootByJump
                {
                    newSpin.features.changeFootByJump = true;
                    break;
                }
                else if(randomSelect2==1 &&
                !newSpin.features.difficultExit &&
                !newSpin.features.difficultEntrance) //difficultEntrance
                {
                    newSpin.features.difficultEntrance = true;
                    break;
                }
                else if(randomSelect2==2 &&
                !newSpin.features.difficultExit &&
                !newSpin.features.difficultEntrance) //difficultExit
                {
                    newSpin.features.difficultExit = true;
                    break;
                }
                else if(randomSelect2==3) //biellmann after layback
                {
                    SpinSegment* randomSegment = spin_addLevelRandomly_pickRandomSegment(newSpin);
                    if(randomSegment->features.biellmannAfterLayback)
                        continue;
                    randomSegment->features.biellmannAfterLayback = true;
                    break;
                }
                else
                    continue;
            }
            else if(randomSelect==2)
            {
                //add position feature
                SpinPosition* randomPosition = spin_addLevelRandomly_pickRandomPosition(newSpin);
                char randomFeature = randomPosition->pickRandomFeature();
                if(newSpin.featureUsed(randomFeature))
                    continue;
                else
                {
                    randomPosition->features.push_back(randomFeature);
                    break;
                }
            }
            else if(randomSelect==3) //only occurs in combo spins (see pickRandomAddition sub-function for logic)
            {
                //add intermediate position
                if(newSpin.intermediatePositionFlag)
                    continue;
                else
                {
                    SpinSegment* randomSegment = spin_addLevelRandomly_pickRandomSegment(newSpin);
                    int randomSpinPositionIndex = easyRandom::range(0,randomSegment->spinPositions.size()-1);
                    randomSegment->spinPositions.insert(randomSegment->spinPositions.begin()+randomSpinPositionIndex+1,SpinPosition('i'));
                    newSpin.intermediatePositionFlag = true;
                    break;
                }
            }
            else if(randomSelect==4) //change of direction (only for change foot spins)
            {
                if(newSpin.changeDirectionFlag)
                    continue;
                else
                {
                    if(easyRandom::range(0,1))
                        newSpin.spinSegments.at(0).swapDirection();
                    else
                        newSpin.spinSegments.at(1).swapDirection();
                    if(easyRandom::range(0,1))
                        newSpin.spinSegments.at(1).swapFootness();
                    newSpin.changeDirectionFlag = true;
                    break;
                }
            }
            else
                throw; //implies that randomSelect is outside of possible spin additions (this would be a bug)
        }
        newSpin.level++;
    }
    else if(newSpin.level==4) //level four spins have extra special rules because reasons
    {

    }
}
int SpinSpinner::spin_addLevelRandomly_pickRandomAddition(Spin& newSpin)
{
    //0: difficult variation, 1: spin feature, 2: position feature, 3: intermediate position
    if(newSpin.baseType=='k')
    {
        if(newSpin.isChangeFoot)
            return easyRandom::pickFromVectorWeighted(std::vector<int>{0,1,2,3,4},std::vector<double>{ADD_VARIATION_PROB,ADD_SPIN_FEATURE_PROB,ADD_POSITION_FEATURE_PROB,ADD_INTERMEDIATE_POSITION_PROB,ADD_CHANGE_OF_DIRECTION_PROB});
        else
            return easyRandom::pickFromVectorWeighted(std::vector<int>{0,1,2,3},std::vector<double>{ADD_VARIATION_PROB,ADD_SPIN_FEATURE_PROB,ADD_POSITION_FEATURE_PROB,ADD_INTERMEDIATE_POSITION_PROB});
    }
    else
    {
        if(newSpin.isChangeFoot)
            return easyRandom::pickFromVectorWeighted(std::vector<int>{0,1,2,4},std::vector<double>{ADD_VARIATION_PROB,ADD_SPIN_FEATURE_PROB,ADD_POSITION_FEATURE_PROB,ADD_CHANGE_OF_DIRECTION_PROB});
        else
            return easyRandom::pickFromVectorWeighted(std::vector<int>{0,1,2},std::vector<double>{ADD_VARIATION_PROB,ADD_SPIN_FEATURE_PROB,ADD_POSITION_FEATURE_PROB});
    }
}
SpinSegment* SpinSpinner::spin_addLevelRandomly_pickRandomSegment(Spin& newSpin)
{
    int randomSegmentIndex = easyRandom::range(0,newSpin.spinSegments.size()-1);
    return &newSpin.spinSegments.at(randomSegmentIndex);
}
SpinPosition* SpinSpinner::spin_addLevelRandomly_pickRandomPosition(Spin& newSpin)
{
    if(!newSpin.isChangeFoot) //not change foot spin
    {
        int randomIndex = easyRandom::range(0,newSpin.spinSegments.at(0).spinPositions.size()-1);
        return &newSpin.spinSegments.at(0).spinPositions.at(randomIndex);
    }
    else //is change foot spin (need to check for only 2 bullets on each foot)
    {
        int bulletsOnFirstSegment = newSpin.spinSegments.at(0).getBullets();
        int bulletsOnSecondSegment = newSpin.spinSegments.at(1).getBullets();
        if(bulletsOnFirstSegment==2 && bulletsOnSecondSegment<2) //1. 2 bullets on first side
        {
            int randomIndex = easyRandom::range(0,newSpin.spinSegments.at(1).spinPositions.size()-1);
            return &newSpin.spinSegments.at(1).spinPositions.at(randomIndex);
        }
        else if(bulletsOnFirstSegment<2 && bulletsOnSecondSegment==2) //2. 2 bullets on second side
        {
            int randomIndex = easyRandom::range(0,newSpin.spinSegments.at(0).spinPositions.size()-1);
            return &newSpin.spinSegments.at(0).spinPositions.at(randomIndex);
        }
        else if(bulletsOnFirstSegment<2 && bulletsOnSecondSegment<2) //3. less than 2 bullets on both sides
        {
            int randomSegmentIndex = easyRandom::range(0,1);
            int randomIndex = easyRandom::range(0,newSpin.spinSegments.at(randomSegmentIndex).spinPositions.size()-1);
            return &newSpin.spinSegments.at(randomSegmentIndex).spinPositions.at(randomIndex);
        }
        else
            throw; //when adding levels properly there should never be more than 2 bullets on each foot and 4 bullets total
    }
}
bool SpinSpinner::spin_checkForDifficultChangeOfPosition(Spin& newSpin)
{
    for(int i=0;i<newSpin.spinSegments.size();i++)
    {
        SpinSegment* currentSegment = &newSpin.spinSegments.at(i);
        for(int j=1;j<currentSegment->spinPositions.size();j++)
        {
            char previousPosition = currentSegment->spinPositions.at(j-1).position;
            char currentPosition = currentSegment->spinPositions.at(j).position;
            if(previousPosition=='s'||previousPosition=='u')
            {
                if(currentPosition=='c')
                {
                    currentSegment->features.difficultChangeOfPosition = true;
                    return true;
                }
            }
        }
    }
    return false;
}
std::string SpinSpinner::spinHistoryToCode()
{
    std::string historyString = "";
    for(int i=0;i<spinHistory.size();i++)
    {
        historyString += spinHistory.at(i).toCode()+"\n";
    }
    return historyString;
}
std::string SpinSpinner::spinHistoryPrettyPrint()
{
    std::string historyString = "";
    for(int i=0;i<spinHistory.size();i++)
    {
        historyString += spinHistory.at(i).prettyPrint()+"\n";
    }
    return historyString;
}
