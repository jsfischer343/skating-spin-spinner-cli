#include "spinposition.hh"

SpinPosition::SpinPosition(char position)
{
    this->position = position;
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
char SpinPosition::pickRandomFeature() const
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
char SpinPosition::pickRandomVariation() const
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
std::string SpinPosition::toCode() const
{
    std::string resultString = "";
    resultString += getPositionString(true);
    resultString += getVariationString(true);
    if(!this->features.empty())
        resultString += "("+getFeatureString(true)+")";
    return resultString;
}
std::string SpinPosition::prettyPrint() const
{
    std::string resultString = "";
    resultString += getPositionString(false)+" ";
    resultString += getVariationString(false);
    if(!this->features.empty())
        resultString += "("+getFeatureString(false)+")";
    return resultString;
}
std::string SpinPosition::getPositionString(bool codeFormat) const
{
    if(codeFormat)
    {
        if(position=='c')
            return "C";
        else if(position=='s')
            return "S";
        else if(position=='u')
            return "U";
        else if(position=='l')
            return "L";
        else if(position=='i')
            return "I";
        return "";
    }
    else
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
}
std::string SpinPosition::getVariationString(bool codeFormat) const
{
    std::string variationString = "";
    if(codeFormat)
    {
        for(int i=0;i<variations.size();i++)
        {
            if(variations.at(i)=='u')
                variationString += "Up";
            else if(variations.at(i)=='s')
                variationString += "Si";
            else if(variations.at(i)=='f')
                variationString += "Fr";
            else if(variations.at(i)=='b')
                variationString += "Be";
        }
    }
    else
    {
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
            if(i!=variations.size()-1)
                variationString += " & ";
        }
    }
    return variationString;
}
std::string SpinPosition::getFeatureString(bool codeFormat) const
{
    std::string featureString = "";
    if(codeFormat)
    {
        for(int i=0;i<features.size();i++)
        {
            if(features.at(i)=='b')
                featureString += "Bl";
            else if(features.at(i)=='c')
                featureString += "Co";
            else if(features.at(i)=='j')
                featureString += "Ju";
            else if(features.at(i)=='8')
                featureString += "8";
            else if(features.at(i)=='s')
                featureString += "Sp";
        }
    }
    else
    {
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
    }
    return featureString;
}
