#include "calibrate.h"
#include <iostream>
#include <fstream>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/lu.hpp>

using namespace std;
using namespace boost::numeric::ublas;

Calibrate::Calibrate()
{

}

Calibrate::Calibrate(string fileName){
    ifstream file;
    file.open(fileName);
    if(file.is_open()){
        while(!file.eof()){
            double temp;
            file >> temp;
            dataX.push_back(temp);
            file >> temp;
            dataY.push_back(temp);
        }
        dataX.pop_back();
        dataY.pop_back();
    }
    else{
        cout << "Unable to open file" << endl;
    }
    n = dataX.size();
    file.close();
}

QVector<double> Calibrate::calibrating(SVIModel *mod){
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
        out << temp2 << endl;
        bool cond = InvertMatrix(temp2,res);
        if(cond){
            matrix<double> temp3 = prod(trans(J),r);
            matrix<double> temp4 = prod(res,temp3);
            out << temp4 << endl;
            out << ans << endl;
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
