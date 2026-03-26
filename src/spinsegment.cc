#include "spinsegment.hh"

SpinSegment::SpinSegment(char footness)
{
    this->footness = footness;
}
SpinSegment::SpinSegment(char footness, std::vector<SpinPosition> spinPositions)
{
    this->footness = footness;
    this->spinPositions = spinPositions;
}
void SpinSegment::swapDirection()
{
    if(direction=='r')
        direction='l';
    else if(direction=='l')
        direction='r';
}
void SpinSegment::swapFootness()
{
    if(footness=='b')
        footness='f';
    else if(footness=='f')
        footness='b';
}
std::vector<char> SpinSegment::getUsedPositions() const
{
    std::vector<char> usedPositions;
    for(int i=0;i<spinPositions.size();i++)
    {
        usedPositions.push_back(spinPositions.at(i).position);
    }
    return usedPositions;
}
int SpinSegment::getBullets() const
{
    int sumOfBullets = 0;

    //count features and variations on spin positions
    for(int i=0;i<spinPositions.size();i++)
    {
        sumOfBullets += spinPositions.at(i).variations.size();
        sumOfBullets += spinPositions.at(i).features.size();
    }

    //count segement specific features
    if(features.difficultChangeOfPosition)
        sumOfBullets++;
    if(features.biellmannAfterLayback)
        sumOfBullets++;

    return sumOfBullets;
}
std::string SpinSegment::getDirectionString() const
{
    if(direction=='r')
        return "ccw";
    else if(direction=='l')
        return "cw";
    return "";
}
std::string SpinSegment::getFootnessString() const
{
    if(footness=='b')
        return "back";
    else if(footness=='f')
        return "forward";
    return "";
}
std::string SpinSegment::prettyPrint() const
{
    std::string resultString = "";

    //direction
    if(this->direction=='r')
        resultString += "(ccw)[ ";
    else if(this->direction=='l')
        resultString += "(cw)[ ";

    //footness
    if(this->footness=='b')
        resultString += "back ";
    else if(this->footness=='f')
        resultString += "forward ";

    for(int i=0;i<spinPositions.size();i++)
    {
        resultString += spinPositions.at(i).prettyPrint();
        if(i!=spinPositions.size()-1)
            resultString += " + ";
    }
    if(this->features.biellmannAfterLayback)
        resultString += " -> biellmann";
    resultString += " ]";

    return resultString;
}
