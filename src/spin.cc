#include "spin.hh"

Spin::Spin(char baseType)
{
    this->baseType = baseType;
}

bool Spin::positionVariationUsed(char positionChar, char variationChar) const
{
    for(int i=0;i<spinPositions.size();i++)
    {
        if(spinPositions.at(i).position==positionChar && spinPositions.at(i).variation==variationChar)
            return true;
    }
    return false;
}
bool Spin::positionFeatureUsed(char featureChar) const
{
    for(int i=0;i<spinPositions.size();i++)
    {
        for(int j=0;j<spinPositions.at(i).features.size();j++)
        {
            if(spinPositions.at(i).features.at(j)==featureChar)
                return true;
        }
    }
    return false;
}
bool Spin::hasTwoVariations() const
{
    int variationCount = 0;
    for(int i=0;i<spinPositions.size();i++)
    {
        if(spinPositions.at(i).variation!=-1)
            variationCount++;
    }
    if(variationCount>=2)
        return true;
    return false;
}
int Spin::getChangeOfFootIndex() const
{
    for(int i=1;i<spinPositions.size();i++)
    {
        if(spinPositions.at(i-1).footness!=spinPositions.at(i).footness)
        {
            return i;
        }
    }
    return -1;
}
std::string Spin::toString() const
{
    std::string spinString = "";
    if(spinPositions.size()==0) //empty return if spin is "blank"
        return spinString;

    //declare this spin's level
    if(level==0)
        spinString += "Base: ";
    else if(level==1)
        spinString += "Level 1: ";
    else if(level==2)
        spinString += "Level 2: ";
    else if(level==3)
        spinString += "Level 3: ";
    else if(level==4)
        spinString += "Level 4: ";

    //starting direction of spin
    spinString += "("+spinPositions.at(0).getDirectionString()+")[\t";

    //is flying spin?
    if(isFlying)
        spinString += "flying ";

    spinString += spinPositions.at(0).getFootnessString()+" ";

    int endIndex = 0;
    int previousEndIndex = 0;
    std::pair<std::string,int> tempParsingContainer;
    while(endIndex!=spinPositions.size())
    {
        if(spinPositions.at(endIndex).direction!=spinPositions.at(previousEndIndex).direction)
        {
            spinString += "\t] -> ("+spinPositions.at(endIndex).getDirectionString()+")[\t";
        }
        if(spinPositions.at(endIndex).footness!=spinPositions.at(previousEndIndex).footness)
        {
            if(spinFeatures.changeFootByJump)
            {
                spinString.erase(spinString.size()-2);
                spinString += "--jump-- "+spinPositions.at(endIndex).getFootnessString()+" ";
            }
            else
                spinString += spinPositions.at(endIndex).getFootnessString()+" ";
        }
        tempParsingContainer = toString_part(endIndex);
        spinString += tempParsingContainer.first;
        previousEndIndex = endIndex;
        endIndex = tempParsingContainer.second;
    }
    spinString += "\t] ";
    if(spinFeatures.difficultEntrance)
        spinString += "with difficult entrance";
    if(spinFeatures.difficultExit)
        spinString += "with difficult exit";
    return spinString;
}

std::pair<std::string,int> Spin::toString_part(int startIndex) const
{
    int endIndex = startIndex;
    std::string partialSpinString = "";
    for(int i=startIndex;i<spinPositions.size();i++)
    {
        if(spinPositions.at(startIndex).direction!=spinPositions.at(i).direction ||
            spinPositions.at(startIndex).footness!=spinPositions.at(i).footness)
        {
            break;
        }
        partialSpinString += spinPositions.at(i).getPositionString();
        std::string positionVariationString = spinPositions.at(i).getVariationString();
        if(positionVariationString!="")
            partialSpinString += " "+positionVariationString;
        std::string positionFeatureString = spinPositions.at(i).getFeatureString();
        if(positionFeatureString!="")
            partialSpinString += "("+positionFeatureString+")";
        if(i!=spinPositions.size()-1)
            partialSpinString += " + ";
        endIndex = i+1;
    }
    return std::pair<std::string,int>{partialSpinString,endIndex};
}
