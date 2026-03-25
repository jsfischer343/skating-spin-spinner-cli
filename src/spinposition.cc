#include "spinposition.hh"

SpinPosition::SpinPosition(char position)
{
    this->position = position;
}
std::string SpinPosition::getPositionString() const
{
    if(position=='c')
        return "camel";
    else if(position=='s')
        return "sit";
    else if(position=='u')
        return "upright";
    else if(position=='l')
        return "layback";
    else if(position=='i')
        return "intermediate";
    return "";
}
std::string SpinPosition::getVariationString() const
{
    std::string variationString = "";
    for(int i=0;i<variations.size();i++)
    {
        if(variations.at(i)=='u')
            variationString += "up";
        else if(variations.at(i)=='s')
            variationString += "side";
        else if(variations.at(i)=='f')
            variationString += "front";
        else if(variations.at(i)=='b')
            variationString += "behind";
    }
    return variationString;
}
std::string SpinPosition::getFeatureString() const
{
    std::string featureString = "";
    for(int i=0;i<features.size();i++)
    {
        if(features.at(i)=='b')
            featureString += "blade";
        else if(features.at(i)=='c')
            featureString += "coe";
        else if(features.at(i)=='j')
            featureString += "jump";
        else if(features.at(i)=='8')
            featureString += "8revs";
        else if(features.at(i)=='s')
            featureString += "speed";
        if(i!=features.size()-1)
            featureString += ", ";
    }
    return featureString;
}
bool SpinPosition::hasVariation() const
{
    if(variations.size()==0)
        return false;
    return true;
}
bool SpinPosition::hasFeature() const
{
    if(features.size()==0)
        return false;
    return true;
}
char SpinPosition::pickRandomFeature()
{
    std::vector<char> validFeatures = {'b','c','j','8','s'};
    return easyRandom::pickFromVector(validFeatures);
}

char SpinPosition::pickRandomVariation(char position)
{
    //some variations may only be valid for some spins?
    if(position=='c')
    {
        return easyRandom::pickFromVector(std::vector<char>{'u','f','s'});
    }
    else if(position=='s')
    {
        return easyRandom::pickFromVector(std::vector<char>{'u','f','b','s'});
    }
    else if(position=='u')
    {
        return easyRandom::pickFromVector(std::vector<char>{'u','f','b'}); //does a headless spin count as upright up?
    }
    else if(position=='l')
    {
        return easyRandom::pickFromVector(std::vector<char>{'s','b'}); //layback up?
    }
    return -1;
}
