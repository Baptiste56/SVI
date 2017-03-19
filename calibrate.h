#ifndef CALIBRATE_H
#define CALIBRATE_H

#include <QVector>
#include <QMainWindow>

#include "svimodel.h"
#include <boost/numeric/ublas/matrix.hpp>

using namespace std;
using namespace boost::numeric::ublas;

class Calibrate
{
public:
    Calibrate();
    Calibrate(QMainWindow* main, SVIModel* mod);
    Calibrate(string fileName, SVIModel* mod);

    QVector<double> calibrating();
    QVector<double> calibrating(bool ro_slope);
    template<class T> bool InvertMatrix(const matrix<T>& input, matrix<T>& inverse);

    QVector<double> getDataX();
    QVector<double> getDataY();
    string getFileName();

private:
    int n;
    SVIModel* mod;
    QVector<double> volX;
    QVector<double> volY;
    QVector<double> dataX;
    QVector<double> dataY;
    string fileName;
};

#endif // CALIBRATE_H
