#ifndef ARBITRAGE_H
#define ARBITRAGE_H

#include "svimodel.h"
#include "coordinate.h"

using namespace std;

class Arbitrage
{
public:
    Arbitrage();
    Arbitrage(SVIModel* mod);

    bool IsArbitrageFree();
    bool IsArbitrageFree(QVector<double> g);

    QVector<double> removeArbitrage();

private:
    /*** Attribute ***/
    SVIModel* mod;
    QVector<double> jwParam;
    QVector<Coordinate> graph;
    QVector<Coordinate> boundary;
    QVector<double> dist;
    double dx;
    double dy;
    int N;

    /*** Method ***/
    QVector<double> jwToRaw(QVector<double> jwParam);
    double distMod(SVIModel* mod, SVIModel* mod2);
    int min(QVector<double> v);
    bool isPositive(QVector<double> g);
    void explore(int direction, int currentPosX, int currentPosY);
    QVector<double> gFct(int nx, int ny);
    bool isMarked(Coordinate point);
};

#endif // ARBITRAGE_H
