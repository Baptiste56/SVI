#include "coordinate.h"

Coordinate::Coordinate(){

}

Coordinate::Coordinate(int x, int y){
    this->x = x;
    this->y = y;
}

int Coordinate::getX(){
    return x;
}

int Coordinate::getY(){
    return y;
}

void Coordinate::setCoordinate(int x, int y){
    this->x = x;
    this->y = y;
}

bool Coordinate::operator==(Coordinate& point){
    bool ans = false;
    if(point.getX() == x && point.getY() == y){
        ans = true;
    }
    return ans;
}

