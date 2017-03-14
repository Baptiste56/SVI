#include "dialogrange.h"
#include "ui_dialogrange.h"

using namespace std;

DialogRange::DialogRange(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogRange)
{
    ui->setupUi(this);
    range.push_back(-1.5);
    range.push_back(1.5);
    range.push_back(0);
    range.push_back(0.2);
    range2.push_back(-1.5);
    range2.push_back(1.5);
    range2.push_back(-0.2);
    range2.push_back(1.4);
    t = 1.0;
    QObject::connect(ui->doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setRange0(double)));
    QObject::connect(ui->doubleSpinBox_2, SIGNAL(valueChanged(double)), this, SLOT(setRange1(double)));
    QObject::connect(ui->doubleSpinBox_3, SIGNAL(valueChanged(double)), this, SLOT(setRange2(double)));
    QObject::connect(ui->doubleSpinBox_4, SIGNAL(valueChanged(double)), this, SLOT(setRange3(double)));
    QObject::connect(ui->doubleSpinBox2, SIGNAL(valueChanged(double)), this, SLOT(setRange0_2(double)));
    QObject::connect(ui->doubleSpinBox2_2, SIGNAL(valueChanged(double)), this, SLOT(setRange1_2(double)));
    QObject::connect(ui->doubleSpinBox2_3, SIGNAL(valueChanged(double)), this, SLOT(setRange2_2(double)));
    QObject::connect(ui->doubleSpinBox2_4, SIGNAL(valueChanged(double)), this, SLOT(setRange3_2(double)));
    QObject::connect(ui->doubleSpinBox_t, SIGNAL(valueChanged(double)), this, SLOT(setT(double)));
}

QVector<double> DialogRange::getRange(){
    return range;
}

QVector<double> DialogRange::getRange2(){
    return range2;
}

double DialogRange::getT(){
    return t;
}

void DialogRange::setRange(int pos, double val){
    range[pos] = val;
}

void DialogRange::setRange2(int pos, double val){
    range2[pos] = val;
}

void DialogRange::setRange0(double val){
    setRange(0,val);
}

void DialogRange::setRange1(double val){
    setRange(1,val);
}

void DialogRange::setRange2(double val){
    setRange(2,val);
}

void DialogRange::setRange3(double val){
    setRange(3,val);
}

void DialogRange::setRange0_2(double val){
    setRange2(0,val);
}

void DialogRange::setRange1_2(double val){
    setRange2(1,val);
}

void DialogRange::setRange2_2(double val){
    setRange2(2,val);
}

void DialogRange::setRange3_2(double val){
    setRange2(3,val);
}

void DialogRange::setT(double t){
    this->t = t;
}

DialogRange::~DialogRange()
{
    delete ui;
}
