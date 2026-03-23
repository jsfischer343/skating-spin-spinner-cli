#include "spinposition.hh"

SpinPosition::SpinPosition(char position, char footness)
{
    this->position = position;
    this->footness = footness;
}

std::string SpinPosition::getPositionString() const
{
    if(this->position=='c')
        return "camel";
    else if(this->position=='s')
        return "sit";
    else if(this->position=='u')
        return "upright";
    else if(this->position=='l')
        return "layback";
    else if(this->position=='i')
        return "intermediate";
    return "";
}
std::string SpinPosition::getDirectionString() const
{
    if(this->direction=='r')
        return "ccw";
    else if(this->direction=='l')
        return "cw";
    return "";
}
std::string SpinPosition::getFootnessString() const
{
    if(this->footness=='b')
        return "back";
    else if(this->footness=='f')
        return "forward";
    return "";
}
std::string SpinPosition::getVariationString() const
{
    if(this->variation=='u')
        return "up";
    else if(this->variation=='f')
        return "front";
    else if(this->variation=='b')
        return "behind";
    else if(this->variation=='s')
        return "side";
    return "";
}
std::string SpinPosition::getFeatureString() const
{
    std::string featureString = "";
    for(int i=0;i<features.size();i++)
    {
        if(this->features.at(i)=='b')
            featureString += "blade";
        else if(this->features.at(i)=='c')
            featureString += "coe";
        else if(this->features.at(i)=='j')
            featureString += "jump";
        else if(this->features.at(i)=='8')
            featureString += "8revs";
        else if(this->features.at(i)=='s')
            featureString += "speed";
        if(i!=features.size()-1)
            featureString += ", ";
    }
    return featureString;
}
bool SpinPosition::hasVariation() const
{
    if(variation==-1)
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
    if(position=='c')
    {
        return easyRandom::pickFromVector(std::vector<char>{'u','s'});
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
        return easyRandom::pickFromVector(std::vector<char>{'u','s','b'}); //layback up? behind, maybe haircutter?
    }
    return -1;
}
