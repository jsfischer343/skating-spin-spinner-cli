#include "spin.hh"

Spin::Spin(char spinBaseType)
{
    this->spinBaseType = spinBaseType;
}

std::string Spin::toString()
{
    std::string spinString = "";
    if(this->spinPositions.size()==0) //empty return if spin is "blank"
        return spinString;

    //declare this spin's level
    if(this->spinLevel==0)
        spinString += "Base: ";
    else if(this->spinLevel==1)
        spinString += "Level 1: ";
    else if(this->spinLevel==2)
        spinString += "Level 2: ";
    else if(this->spinLevel==3)
        spinString += "Level 3: ";
    else if(this->spinLevel==4)
        spinString += "Level 4: ";

    //starting direction of spin
    spinString += "("+this->spinPositions.at(0).getDirectionString()+")[\t";

    //is flying spin?
    if(this->isFlyingSpin)
        spinString += "flying ";

    spinString += this->spinPositions.at(0).getFootnessString()+" ";

    int endIndex = 0;
    int previousEndIndex = 0;
    std::pair<std::string,int> tempParsingContainer;
    while(endIndex!=this->spinPositions.size())
    {
        if(this->spinPositions.at(endIndex).direction!=this->spinPositions.at(previousEndIndex).direction)
        {
            spinString += "\t] -> ("+this->spinPositions.at(endIndex).getDirectionString()+")[\t";
        }
        if(this->spinPositions.at(endIndex).footness!=this->spinPositions.at(previousEndIndex).footness)
        {
            if(this->spinFeatures.changeFootByJump)
            {
                spinString.erase(spinString.size()-2);
                spinString += "--jump-- "+this->spinPositions.at(endIndex).getFootnessString()+" ";
            }
            else
                spinString += this->spinPositions.at(endIndex).getFootnessString()+" ";
        }
        tempParsingContainer = toString_part(endIndex);
        spinString += tempParsingContainer.first;
        previousEndIndex = endIndex;
        endIndex = tempParsingContainer.second;
    }
    spinString += "\t] ";
    if(this->spinFeatures.difficultEntrance)
        spinString += "with difficult entrance";
    if(this->spinFeatures.difficultExit)
        spinString += "with difficult exit";
    return spinString;
}

bool Spin::positionVariationUsedInSpin(char positionChar, char variationChar)
{
    for(int i=0;i<this->spinPositions.size();i++)
    {
        if(this->spinPositions.at(i).position==positionChar && this->spinPositions.at(i).variation==variationChar)
            return true;
    }
    return false;
}
bool Spin::positionFeatureUsedInSpin(char featureChar)
{
    for(int i=0;i<this->spinPositions.size();i++)
    {
        for(int j=0;j<this->spinPositions.at(i).features.size();j++)
        {
            if(this->spinPositions.at(i).features.at(j)==featureChar)
                return true;
        }
    }
    return false;
}
bool Spin::hasIntermediatePosition()
{
    for(int i=0;i<this->spinPositions.size();i++)
    {
        if(this->spinPositions.at(i).position=='i')
            return true;
    }
    return false;
}
std::pair<std::string,int> Spin::toString_part(int startIndex)
{
    int endIndex = startIndex;
    std::string partialSpinString = "";
    for(int i=startIndex;i<this->spinPositions.size();i++)
    {
        if(this->spinPositions.at(startIndex).direction!=this->spinPositions.at(i).direction ||
            this->spinPositions.at(startIndex).footness!=this->spinPositions.at(i).footness)
        {
            break;
        }
        partialSpinString += this->spinPositions.at(i).getPositionString();
        std::string positionVariationString = this->spinPositions.at(i).getVariationString();
        if(positionVariationString!="")
            partialSpinString += " "+positionVariationString;
        std::string positionFeatureString = this->spinPositions.at(i).getFeatureString();
        if(positionFeatureString!="")
            partialSpinString += "("+positionFeatureString+")";
        if(i!=this->spinPositions.size()-1)
            partialSpinString += " + ";
        endIndex = i+1;
    }
    return std::pair<std::string,int>{partialSpinString,endIndex};
}
