#include "svimodel.h"
#include <QVector>
#include <cmath>

SVIModel::SVIModel()
{
    this->setCoef(0,0,0,0,0);
    t = 1.0;
    x.fill(0,101);
    y.fill(0,101);
    g.fill(0,101);
    delta.fill(0,5);
    lastSpeed.fill(1000,5);
    param = "raw";
}

SVIModel::SVIModel(double a, double b, double ro, double m, double sig){
    this->setCoef(a,b,ro,m,sig);
    t = 1.0;
    x.fill(0,101);
    y.fill(0,101);
    g.fill(0,101);
    delta.fill(0,5);
    lastSpeed.fill(1000,5);
    param = "raw";
    for (int i=0; i<101; ++i)
    {
      x[i] = i/25.0 - 2; // x goes from -2 to 2
    }
    setData();
}

void SVIModel::setData(){
    for (int i=0; i<101; ++i)
    {
      y[i] = a + b*(ro*(x[i]-m)+sqrt(pow(x[i]-m,2)+pow(sig,2))); // formula of the raw SVI parametrization
    }
    setGFct();
}

void SVIModel::setGFct(){
    QVector<double> dy;
    QVector<double> d2y;
    for (int i=0; i<101; ++i)
    {
        double temp = sqrt(pow(x[i]-m,2)+pow(sig,2));
        dy.push_back(b*(ro+(x[i]-m)/temp));
        d2y.push_back(b*(1/temp-pow(x[i]-m,2)/pow(temp,3)));
        g[i] = pow(1-x[i]*dy[i]/(2*y[i]),2)-pow(dy[i],2)*(1/y[i]+1/4)/4+d2y[i]/2;
    }
}

void SVIModel::setCoef(double a, double b, double ro, double m, double sig){
    this->a = a;
    this->b = b;
    this->m = m;
    this->ro = ro;
    this->sig = sig;
}

void SVIModel::setInitCurve(double u, double v, double u2, double v2){
    a = (a-v2)/u2;
    b = b*u/u2;
    m = (m-v)/u;
    sig = sig/u;
    setData();
}

/*** Param changes ***/
void SVIModel::changeDataA(double d, int sp){
    a += d;
    a -= delta[0]*(lastSpeed[0]/sp);
    delta[0] = d;
    lastSpeed[0] = sp;
    setData();
}

void SVIModel::changeDataB(double d, int sp){
    b += d;
    b -= delta[1]*(lastSpeed[1]/sp);
    delta[1] = d;
    lastSpeed[1] = sp;
    setData();
}

void SVIModel::changeDataM(double d, int sp){
    m += d;
    m -= delta[2]*(lastSpeed[2]/sp);
    delta[2] = d;
    lastSpeed[2] = sp;
    setData();
}

void SVIModel::changeDataRo(double d, int sp){
    ro += d;
    ro -= delta[3]*(lastSpeed[3]/sp);
    delta[3] = d;
    lastSpeed[3] = sp;
    setData();
}

void SVIModel::changeDataSig(double d, int sp){
    sig += d;
    sig -= delta[4]*(lastSpeed[4]/sp);
    delta[4] = d;
    lastSpeed[4] = sp;
    setData();
}

double SVIModel::getA(){
    return a;
}

double SVIModel::getB(){
    return b;
}

double SVIModel::getM(){
    return m;
}

double SVIModel::getRo(){
    return ro;
}

double SVIModel::getSig(){
    return sig;
}

double SVIModel::getT(){
    return t;
}

QString SVIModel::getParam(){
    return param;
}

double SVIModel::getValue(double x){
    double temp = a + b*(ro*(x-m)+sqrt(pow(x-m,2)+pow(sig,2)));
    return temp;
}

void SVIModel::setParam(QString name){
    this->param = name;
}

/*** Natural Param ***/
void SVIModel::changeToNat(){
    param = "nat";
}

double SVIModel::natParam1(){
    double temp = a - (natParam4()/2)*(1-pow(ro,2));
    return temp;
}

double SVIModel::natParam2(){
    double temp = m + (ro*sig/sqrt(1-pow(ro,2)));
    return temp;
}

double SVIModel::natParam3(){
    return ro;
}

double SVIModel::natParam4(){
    double temp = 2*b*sig/sqrt(1-pow(ro,2));
    return temp;
}

double SVIModel::natParam5(){
    double temp = sqrt(1-pow(ro,2))/sig;
    return temp;
}

/*** SVI-JW Param ***/
void SVIModel::changeToJW(){
    param = "jw";
}

double SVIModel::jwParam1(){
    double temp = (1/t)*(a+b*(sqrt(pow(m,2)+pow(sig,2))-ro*m));
    return temp;
}

double SVIModel::jwParam2(){
    double temp = (b/(2*sqrt(jwParam1()*t)))*(ro-(m/sqrt(pow(m,2)+pow(sig,2))));
    return temp;
}

double SVIModel::jwParam3(){
    double temp = b*(1-ro)/(sqrt(jwParam1()*t));
    return temp;
}

double SVIModel::jwParam4(){
    double temp = b*(1+ro)/(sqrt(jwParam1()*t));
    return temp;
}

double SVIModel::jwParam5(){
    double temp = (1/t)*(a+b*sig*sqrt(1-pow(ro,2)));
    return temp;
}

QVector<double> SVIModel::getX(){
    return x;
}

QVector<double> SVIModel::getY(){
    return y;
}

QVector<double> SVIModel::getG(){
    return g;
}

SVIModel::~SVIModel(){

}
