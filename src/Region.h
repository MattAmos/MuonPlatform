#ifndef REGION_H
#define REGION_H

#include <string>

#include "RegionType.h"

class Region {
private:
    int x, y;
    int xIndex, yIndex;
    RegionType type;

public:
    Region();
    Region(int xIn, int yIn, int xIndIn, int yIndIn);
    int getMinX();
    int getMinY();
    int getIndexX();
    int getIndexY();
    RegionType getType();

    void setX(int xIn);
    void setY(int yIn);
    void setType(RegionType t);

    std::string toString();
};

#endif
