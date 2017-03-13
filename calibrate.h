#ifndef CALIBRATE_H
#define CALIBRATE_H

#include <QVector>

#include "svimodel.h"
#include <boost/numeric/ublas/matrix.hpp>

using namespace std;
using namespace boost::numeric::ublas;

class Calibrate
{
public:
    Calibrate();
    Calibrate(string name);

    QVector<double> calibrating(SVIModel* mod);
    template<class T> bool InvertMatrix(const matrix<T>& input, matrix<T>& inverse);

    QVector<double> getDataX();
    QVector<double> getDataY();

private:
    int n;
    QVector<double> dataX;
    QVector<double> dataY;
};

#endif // CALIBRATE_H
