#include "spin.hh"

Spin::Spin(char baseType)
{
    this->baseType = baseType;
}

bool Spin::variationUsed(char positionChar, char variationChar) const
{
    for(int i=0;i<spinSegments.size();i++)
    {
        for(int j=0;j<spinSegments.at(i).spinPositions.size();j++)
        {
            for(int k=0;k<spinSegments.at(i).spinPositions.at(j).positions.size();k++)
            {
                if(spinSegments.at(i).spinPositions.at(j).position==positionChar && spinSegments.at(i).spinPositions.at(j).variations.at(k)==variationChar)
                    return true;
            }
        }
    }
    return false;
}
bool Spin::featureUsed(char featureChar) const
{
    for(int i=0;i<spinSegments.size();i++)
    {
        for(int j=0;j<spinSegments.at(i).spinPositions.size();j++)
        {
            for(int k=0;k<spinSegments.at(i).spinPositions.at(j).positions.size();k++)
            {
                if(spinSegments.at(i).spinPositions.at(j).features.at(k)==featureChar)
                    return true;
            }
        }
    }
    return false;
}
bool Spin::hasTwoVariations() const
{
    int variationCount = 0;
    for(int i=0;i<spinSegments.size();i++)
    {
        for(int j=0;j<spinSegments.at(i).spinPositions.size();j++)
        {
            variationCount += spinSegments.at(i).spinPositions.variations.size();
        }
    }
    if(variationCount>=2)
        return true;
    return false;
}
std::string Spin::prettyPrint() const
{
    return "WIP";
}
