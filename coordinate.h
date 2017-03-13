#ifndef COORDINATE_H
#define COORDINATE_H


class Coordinate
{
public:
    Coordinate();
    Coordinate(int x, int y);
    int getX();
    int getY();
    void setCoordinate(int x, int y);
    bool operator==(Coordinate& point);

private:
    int x;
    int y;
};

#endif // COORDINATE_H
