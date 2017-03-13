#ifndef SVIMODEL_H
#define SVIMODEL_H

#include"QVector"

class SVIModel
{
public:
    SVIModel();
    SVIModel(double a, double b, double ro, double m, double sig);
    ~SVIModel();

    /*** Getters ***/
    QVector<double> getX();
    QVector<double> getY();
    QVector<double> getG();
    double getA();
    double getB();
    double getM();
    double getRo();
    double getSig();
    double getT();
    QString getParam();
    double getValue(double x);

    /*** Setters ***/
    void setData();
    void setGFct();
    void setCoef(double a, double b, double ro, double m, double sig);
    void setParam(QString name);
    void setInitCurve(double u, double v, double u2, double v2);

    /*** Param changes ***/
    void changeDataA(double delta, int sp);
    void changeDataB(double delta, int sp);
    void changeDataRo(double delta, int sp);
    void changeDataM(double delta, int sp);
    void changeDataSig(double delta, int sp);

    /*** Natural param ***/
    void changeToNat();
    double natParam1();
    double natParam2();
    double natParam3();
    double natParam4();
    double natParam5();

    /*** SVI-JW param ***/
    void changeToJW();
    double jwParam1();
    double jwParam2();
    double jwParam3();
    double jwParam4();
    double jwParam5();

private:
    double a;
    double b;
    double ro;
    double m;
    double sig;
    double t;
    QVector<double> x;
    QVector<double> y;
    QVector<double> g;
    QVector<double> delta;
    QVector<double> lastSpeed;
    QString param;
};

#endif // SVIMODEL_H
