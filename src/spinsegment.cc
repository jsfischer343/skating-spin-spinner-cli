#include "spinsegment.hh"

SpinSegment::SpinSegment(char direction)
{
    this->direction = direction;
}
void SpinSegment::swapDirection()
{
    if(direction=='r')
        direction='l';
    else if(direction=='l')
        direction='r';
}
int SpinSegment::getBullets(int startPos) const
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
    if(features.laybackTransition)
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
