#include "Region.h"

Region::Region() {}

Region::Region(int xIn, int yIn, int xIndIn, int yIndIn) {
    x      = xIn;
    y      = yIn;
    xIndex = yIndIn;
    yIndex = yIndIn;
    type   = UNKNOWN;
}

int Region::getMinX() {
    return x;
}

int Region::getMinY() {
    return y;
}

int Region::getIndexX() {
    return xIndex;
}

int Region::getIndexY() {
    return yIndex;
}

RegionType Region::getType() {
    return type;
}

void Region::setX(int xIn) {
    x = xIn;
}

void Region::setY(int yIn) {
    y = yIn;
}

void Region::setType(RegionType t) {
    type = t;
}

std::string Region::toString() {
    return std::to_string(type);
}
