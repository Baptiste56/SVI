#include "arbitrage.h"
#include <cmath>
#include <fstream>

using namespace std;

struct pos {
  double dt;
  int nt;
  double init;
  pos(double a, int b, double c){
    dt = a;
    nt = b;
    init = c;
  }

  double cP(){  //current pos
      return init + nt*dt;
  }
};

Arbitrage::Arbitrage(){

}

Arbitrage::Arbitrage(SVIModel *mod){
    this->mod = mod;
    jwParam.push_back(mod->jwParam1());
    jwParam.push_back(mod->jwParam2());
    jwParam.push_back(mod->jwParam3());
    jwParam.push_back(mod->jwParam4());
    jwParam.push_back(mod->jwParam5());
}

bool Arbitrage::IsArbitrageFree(){
    bool ans = true;
    QVector<double> g = mod->getG();
    for(QVector<double>::iterator it = g.begin() ; it != g.end() ; it++){
        if(*it < 0){
            ans = false;
        }
    }
    return ans;
}

bool Arbitrage::IsArbitrageFree(QVector<double> g){
    bool ans = true;
    for(int i = 0 ; i < g.size() ; i++){
        if(g[i] < 0){
            ans = false;
        }
    }
    return ans;
}

QVector<double> Arbitrage::removeArbitrage(){
    QVector<double> ans;
    ans.fill(0,5);
    /*** first guess ***/
    ans[0] = jwParam[0];
    ans[1] = jwParam[1];
    ans[2] = jwParam[2];
    ans[3] = jwParam[2]+2*jwParam[1];
    ans[4] = 4*jwParam[0]*jwParam[2]*ans[3]/pow(jwParam[2]+ans[3],2);

    /*** creation of interval ***/
    N = 10;
    dx = (ans[3] - jwParam[3])/N;
    dy = (ans[4] - jwParam[4])/N;

    int nx(0);
    int ny(0);

    /**** Main loop ****/
    explore(0,0,-1);

    for(QVector<Coordinate>::iterator it = boundary.begin() ; it < boundary.end() ; it++){
        QVector<double> param;
        nx = it->getX();
        ny = it->getY();
        pos currentX(dx, nx, jwParam[3]);
        pos currentY(dy, ny, jwParam[4]);
        param.push_back(jwParam[0]);
        param.push_back(jwParam[1]);
        param.push_back(jwParam[2]);
        param.push_back(currentX.cP());
        param.push_back(currentY.cP());
        param = jwToRaw(param);
        SVIModel cmod(param[0], param[1], param[3], param[2], param[4]);
        dist.push_back(distMod(mod,&cmod));
    }

    int i = min(dist);
    QVector<Coordinate>::iterator it = boundary.begin()+i;
    nx = it->getX();
    ny = it->getY();
    pos currentX(dx, nx, jwParam[3]);
    pos currentY(dy, ny, jwParam[4]);
    ans[3] = currentX.cP();
    ans[4] = currentY.cP();

    ofstream out;
    out.open("../../../../testPlot/boundary.out");
    for(QVector<Coordinate>::iterator it = boundary.begin() ; it != boundary.end() ; it++){
        out << it->getX() << "   " << it->getY() << endl;
    }
    return jwToRaw(ans);
}

QVector<double> Arbitrage::jwToRaw(QVector<double> jwParam){
    QVector<double> ans;
    /*** SVI-JW Param ***/
    double t = mod->getT();
    double v = jwParam[0];
    double psi = jwParam[1];
    double p = jwParam[2];
    double c = jwParam[3];
    double v2 = jwParam[4];
    double w = v*t;
    /**********************/
    ans.fill(0,5);
    ans[1] = (c+p)*(sqrt(w)/2);
    double b = ans[1];
    ans[3] = 1 - (p*sqrt(w)/b);
    double ro = ans[3];
    /*** alpha and beta ***/
    double beta = ro - (2*psi*sqrt(w)/b);
    double alpha(0);
    if(beta >= 0){
        alpha = sqrt((1/pow(beta,2))-1);
    }
    else{
        alpha = -sqrt((1/pow(beta,2))-1);
    }
    /**********************/
    double m(0);
    if(alpha >= 0){
        ans[2] = (v-v2)*t/(b*(-ro+sqrt(1+pow(alpha,2))-alpha*sqrt(1-pow(ro,2))));
        m = ans[2];
    }
    else{
        ans[2] = (v-v2)*t/(b*(-ro-sqrt(1+pow(alpha,2))-alpha*sqrt(1-pow(ro,2))));
        m = ans[2];
    }
    ans[4] = alpha*m;
    double sig = ans[4];
    ans[0] = v2*t - b*sig*sqrt(1-pow(ro,2));

    return ans;
}

double Arbitrage::distMod(SVIModel* mod, SVIModel* mod2){
    QVector<double> y = mod->getY();
    QVector<double> y2 = mod2->getY();
    double ans(0);
    for(int i = 0 ; i < y.size() ; i++){
        ans += pow(y[i]-y2[i],2);
    }
    return ans;
}

int Arbitrage::min(QVector<double> v){
    int ans = 0;
    for(int i = 1 ; i < v.size() ; i++){
        if(v[i] < v[ans] && v[i] >= 0){
            ans = i;
        }
    }
    return ans;
}

bool Arbitrage::isPositive(QVector<double> g){
    bool ans = true;
    for(int i = 0 ; i < g.size() ; i++){
        if(g[i] < 0){
            ans = false;
            i = g.size();
        }
    }
    return ans;
}

void Arbitrage::explore(int direction, int currentPosX, int currentPosY){
    Coordinate point(0,0);
    switch (direction) {
    case 0: //North
        currentPosY += 1;
        break;
    case 1: //East
        currentPosX += 1;
        break;
    case 2: //South
        currentPosX -= 1;
        break;
    case 3: //West
        currentPosX -= 1;
        break;
    default:
        break;
    }
    point.setCoordinate(currentPosX,currentPosY);
    if(IsArbitrageFree(gFct(currentPosX, currentPosY))){
        boundary.push_back(point);
        graph.push_back(point);
    }
    else{
        graph.push_back(point);
        Coordinate nextPoint(0,0);
        int nPX = point.getX();
        int nPY = point.getY();
        for(int i = 0 ; i < 4 ; i++){
            switch (i) {
            case 0: //North
                nPY += 1;
                break;
            case 1: //East
                nPY -= 1;
                nPX += 1;
                break;
            case 2: //South
                nPX -= 1;
                nPY -= 1;
                break;
            case 3: //West
                nPY += 1;
                nPX -= 1;
                break;
            default:
                break;
            }
            nextPoint.setCoordinate(nPX,nPY);
            if(!isMarked(nextPoint)){
                if(nPX >= 0 && nPX <= N && nPY >= 0 && nPY <= N){
                    explore(i,point.getX(),point.getY());
                }
            }
        }
    }
}

QVector<double> Arbitrage::gFct(int nx, int ny){
    QVector<double> param;
    pos currentX(dx, nx, jwParam[3]);
    pos currentY(dy, ny, jwParam[4]);
    param.push_back(jwParam[0]);
    param.push_back(jwParam[1]);
    param.push_back(jwParam[2]);
    param.push_back(currentX.cP());
    param.push_back(currentY.cP());
    param = jwToRaw(param);
    SVIModel cmod(param[0], param[1], param[3], param[2], param[4]);
    return cmod.getG();
}

bool Arbitrage::isMarked(Coordinate point){
    bool ans = false;
    for(QVector<Coordinate>::iterator it = graph.begin() ; it < graph.end() ; it++){
        if(*it == point){
            ans = true;
        }
    }
    return ans;
}
