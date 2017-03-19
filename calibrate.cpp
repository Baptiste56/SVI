#include "calibrate.h"
#include "mainwindow.h"
#include <iostream>
#include <fstream>
#include <QFileDialog>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/lu.hpp>

using namespace std;
using namespace boost::numeric::ublas;

Calibrate::Calibrate()
{

}

Calibrate::Calibrate(QMainWindow *main, SVIModel *mod){
    this->mod = mod;
    ifstream file;
    QString fileNameQt = QFileDialog::getOpenFileName(main, "Open a file", QString("../../../../testPlot/data"));
    fileName = fileNameQt.toStdString();
    file.open(fileName);
    if(file.is_open()){
        while(!file.eof()){
            double temp;
            file >> temp;
            volX.push_back(temp);
            file >> temp;
            volY.push_back(temp);
        }
        volX.pop_back();
        volY.pop_back();
    }
    else{
        cout << "Unable to open file" << endl;
    }
    n = volX.size();
    double t = mod->getT();

    /*** Transform Volatility in Total Variance ***/
    for(int i = 0 ; i < n ; i++){
        dataY.push_back(t*pow(volY[i],2));
        dataX.push_back(volX[i]);
    }

    file.close();
}

Calibrate::Calibrate(string fileName2, SVIModel* mod){
    this->mod = mod;
    ifstream file;
    file.open(fileName2);
    if(file.is_open()){
        while(!file.eof()){
            double temp;
            file >> temp;
            volX.push_back(temp);
            file >> temp;
            volY.push_back(temp);
        }
        volX.pop_back();
        volY.pop_back();
    }
    else{
        cout << "Unable to open file" << endl;
    }
    n = volX.size();
    double t = mod->getT();

    /*** Transform Volatility in Total Variance ***/
    for(int i = 0 ; i < n ; i++){
        dataY.push_back(t*pow(volY[i],2));
        dataX.push_back(volX[i]);
    }

    file.close();
}

QVector<double> Calibrate::calibrating(){
    ofstream out;
    out.open("../../../../testPlot/console.out");

    double a = mod->getA();
    double b = mod->getB();
    double ro = mod->getRo();
    double m = mod->getM();
    double sig = mod->getSig();
    matrix<double> J (n,5);
    matrix<double> r (n,1);
    matrix<double> ans (5,1);
    QVector<double> param;

    param.fill(0,5);
    ans(0,0) = a;
    ans(1,0) = b;
    ans(2,0) = ro;
    ans(3,0) = m;
    ans(4,0) = sig;
    int it = 0;
    while(it < 5){
        for(int i = 0 ; i<n ; i++){
            double temp = sqrt(pow(dataX[i]-m,2)+pow(sig,2));
            J(i,0) = 1;    // param a
            J(i,1) = ro*(dataX[i]-m)+temp;   // param b
            J(i,2) = b*(dataX[i] - m); //param ro
            J(i,3) = (-b)*(ro + ((dataX[i]-m)/temp));    //param m
            J(i,4) = b*sig/temp;   // param sig
            double f = a + b*(ro*(dataX[i]-m)+temp);
            r(i,0) = f - dataY[i];
        }
        matrix<double> res(5,5);
        matrix<double> temp2 = prod(trans(J),J);
        bool cond = InvertMatrix(temp2,res);
        if(cond){
            matrix<double> temp3 = prod(trans(J),r);
            matrix<double> temp4 = prod(res,temp3);
            ans = ans - temp4;
            param[0] = ans(0,0);
            param[1] = ans(1,0);
            param[2] = ans(2,0);
            param[3] = ans(3,0);
            param[4] = ans(4,0);
            a = ans(0,0);
            b = ans(1,0);
            ro = ans(2,0);
            m = ans(3,0);
            sig = ans(4,0);

        }
        else{
            out << "problem inverting at step " << it << endl;
        }
        it++;
    }
    for(int i = 0 ; i < 5 ; i++){
        out << param[i] << endl;
    }

    return param;
}

QVector<double> Calibrate::calibrating(bool ro_slope){
    ofstream out;
    out.open("../../../../testPlot/console.out");

    double ro(0);
    if(ro_slope){
        ro = 1;
    }
    else{
        ro = -1;
    }

    double a = mod->getA();
    double b = mod->getB();
    double m = mod->getM();
    double sig = mod->getSig();
    matrix<double> J (n,4);
    matrix<double> r (n,1);
    matrix<double> ans (5,1);
    QVector<double> param;

    param.fill(0,5);
    param[2] = ro;

    ans(0,0) = a;
    ans(1,0) = b;
    ans(2,0) = m;
    ans(3,0) = sig;
    int it = 0;
    while(it < 1){
        for(int i = 0 ; i<n ; i++){
            double temp = sqrt(pow(dataX[i]-m,2)+pow(sig,2));
            J(i,0) = 1;    // param a
            J(i,1) = ro*(dataX[i]-m)+temp;   // param b
            J(i,2) = (-b)*(ro + ((dataX[i]-m)/temp));    //param m
            J(i,3) = b*sig/temp;   // param sig
            double f = a + b*(ro*(dataX[i]-m)+temp);
            r(i,0) = f - dataY[i];
        }
        matrix<double> res(4,4);
        matrix<double> temp2 = prod(trans(J),J);
        bool cond = InvertMatrix(temp2,res);
        if(cond){
            matrix<double> temp3 = prod(trans(J),r);
            matrix<double> temp4 = prod(res,temp3);
            ans = ans - temp4;
            param[0] = ans(0,0);
            param[1] = ans(1,0);
            param[3] = ans(2,0);
            param[4] = ans(3,0);
            a = ans(0,0);
            b = ans(1,0);
            m = ans(3,0);
            sig = ans(4,0);

        }
        else{
            out << "problem inverting at step " << it << endl;
        }
        it++;
    }
    for(int i = 0 ; i < 5 ; i++){
        out << param[i] << endl;
    }

    return param;
}

template<class T>
bool Calibrate::InvertMatrix(const matrix<T>& input, matrix<T>& inverse)
{
    typedef permutation_matrix<std::size_t> pmatrix;
    bool ans = false;

    // create a working copy of the input
    matrix<T> A(input);

    // create a permutation matrix for the LU-factorization
    pmatrix pm(A.size1());

    // perform LU-factorization
    int res = lu_factorize(A, pm);
    if (res != 0){
        ans = false;
    }
    else{
        // create identity matrix of "inverse"
        inverse.assign(identity_matrix<T> (A.size1()));

        // backsubstitute to get the inverse
        lu_substitute(A, pm, inverse);

        ans = true;
    }
    return ans;
}

QVector<double> Calibrate::getDataX(){
    return dataX;
}

QVector<double> Calibrate::getDataY(){
    return dataY;
}

string Calibrate::getFileName(){
    return fileName;
}
