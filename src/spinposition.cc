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
bool SpinPosition::pickRandomFeature() const
{
    std::vector<char> validFeatures = {'b','c','j','8','s'};
    std::vector<char> usedFeatures = features;
    std::vector<char> unusedFeatures;
    std::sort(validFeatures.begin(),validFeatures.end());
    std::sort(usedFeatures.begin(),usedFeatures.end());
    std::set_difference(validFeatures.begin(),validFeatures.end(),usedFeatures.begin(),usedFeatures.end(),std::back_inserter(unusedFeatures));
    if(unusedFeatures.empty())
        return -1;
    else
        return easyRandom::pickFromVector(unusedFeatures);
}

bool SpinPosition::pickRandomVariation() const
{
    std::vector<char> validVariations;
    //some variations may only be valid for some spins?
    if(position=='c')
    {
        validVariations = {'u','f','s'};
    }
    else if(position=='s')
    {
        validVariations = {'u','f','b','s'};
    }
    else if(position=='u')
    {
        validVariations = {'u','f','b'}; //does a headless spin count as upright up?
    }
    else if(position=='l')
    {
        validVariations = {'s','b'}; //layback up?
    }
    std::vector<char> usedVariations = variations;
    std::vector<char> unusedVariations;
    std::sort(validVariations.begin(),validVariations.end());
    std::sort(usedVariations.begin(),usedVariations.end());
    std::set_difference(validVariations.begin(),validVariations.end(),usedVariations.begin(),usedVariations.end(),std::back_inserter(unusedVariations));
    if(unusedVariations.empty())
        return -1;
    else
        return easyRandom::pickFromVector(unusedVariations);
}
