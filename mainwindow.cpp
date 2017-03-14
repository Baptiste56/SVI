#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "svimodel.h"
#include "calibrate.h"
#include "dialogrange.h"
#include "arbitrage.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QStringList choices = (QStringList() << "Raw param" << "Natural param" << "SVI-JW param");
    ui->comboBox->addItems(choices);

    diag = new DialogRange(this);

    x.fill(0,101);
    y.fill(0,101);
    g.fill(0,101);
    speedChange = 1000;

    lastRange.push_back(-1.5);
    lastRange.push_back(1.5);
    lastRange.push_back(0);
    lastRange.push_back(0.2);

    mod = new SVIModel(-0.0410,0.1331,0.3060,0.3586,0.4153);
    displayParam();

    x = mod->getX();
    y = mod->getY();
    g = mod->getG();

    /****** PLOT 1 ******/
    // create graph and assign data to it:
    ui->plot->addGraph();
    ui->plot->graph(0)->setData(x, y);
    // give the axes some labels:
    ui->plot->xAxis->setLabel("x");
    ui->plot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    ui->plot->xAxis->setRange(-1.5, 1.5);
    ui->plot->yAxis->setRange(0, 0.2);

    //Add two graph for sample data and arbitrage curve
    ui->plot->addGraph();
    ui->plot->addGraph();

    /****** PLOT 2 ******/
    ui->plot2->addGraph();
    ui->plot2->graph(0)->setData(x, g);
    ui->plot2->xAxis->setLabel("x");
    ui->plot2->yAxis->setLabel("y");
    ui->plot2->xAxis->setRange(-1.5, 1.5);
    ui->plot2->yAxis->setRange(-0.2, 1.4);

    //Add graph for arbitrage curve
    ui->plot2->addGraph();

    ui->plot->replot();

    QObject::connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(changeCurve1(int)));
    QObject::connect(ui->horizontalSlider_2, SIGNAL(valueChanged(int)), this, SLOT(changeCurve2(int)));
    QObject::connect(ui->horizontalSlider_3, SIGNAL(valueChanged(int)), this, SLOT(changeCurve3(int)));
    QObject::connect(ui->horizontalSlider_4, SIGNAL(valueChanged(int)), this, SLOT(changeCurve4(int)));
    QObject::connect(ui->horizontalSlider_5, SIGNAL(valueChanged(int)), this, SLOT(changeCurve5(int)));
    QObject::connect(ui->comboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeParam(QString)));
    QObject::connect(ui->pushButton, SIGNAL(pressed()), this, SLOT(calibrateCurve()));
    QObject::connect(ui->actionAddFile, SIGNAL(triggered()), this, SLOT(showDataSet()));
    QObject::connect(ui->actionset_range, SIGNAL(triggered()), this, SLOT(setRange()));
    QObject::connect(ui->actionset_initial_curve, SIGNAL(triggered()), this, SLOT(setInitialCurve()));
    QObject::connect(ui->pushButton_sliders, SIGNAL(pressed()), this, SLOT(reInitSliders()));
    QObject::connect(ui->spinBox_speedChange, SIGNAL(valueChanged(int)), this, SLOT(setSpeedChange(int)));
    QObject::connect(ui->pushButton_arbitrage, SIGNAL(pressed()), this, SLOT(removeArbitrage()));

}

void MainWindow::showDataSet(){
    Calibrate file(this, mod);
    addSampleData(&file);
}

void MainWindow::setRange(){
    bool res = diag->exec();
    if(res){
        mod->setT(diag->getT());

        QVector<double> range = diag->getRange();
        ui->plot->xAxis->setRange(range[0], range[1]);
        ui->plot->yAxis->setRange(range[2], range[3]);

        QVector<double> range2 = diag->getRange2();
        ui->plot2->xAxis->setRange(range2[0], range2[1]);
        ui->plot2->yAxis->setRange(range2[2], range2[3]);

        y = mod->getY();
        g = mod->getG();
        ui->plot->graph(0)->setData(x, y);
        ui->plot2->graph(0)->setData(x,g);
        ui->plot->replot();
        ui->plot2->replot();
    }
}

void MainWindow::setInitialCurve(){
    QVector<double> range = diag->getRange();
    double u = (lastRange[1]-lastRange[0])/(range[1]-range[0]);
    double v = lastRange[0] - u*(range[0]);
    double u2 = (lastRange[3]-lastRange[2])/(range[3]-range[2]);
    double v2 = lastRange[2] - u2*(range[2]);
    mod->setInitCurve(u,v,u2,v2);

    y = mod->getY();
    g = mod->getG();
    ui->plot->graph(0)->setData(x, y);
    ui->plot2->graph(0)->setData(x,g);
    ui->plot->replot();
    ui->plot2->replot();
    displayParam();

    lastRange = range;
}

void MainWindow::calibrateCurve(){
    Calibrate file(this, mod);
    addSampleData(&file);
    QVector<double> newCoef = file.calibrating();
    mod->setCoef(newCoef[0],newCoef[1],newCoef[2],newCoef[3],newCoef[4]);

    mod->setData();
    y = mod->getY();
    g = mod->getG();
    ui->plot->graph(0)->setData(x, y);
    ui->plot2->graph(0)->setData(x,g);
    ui->plot->replot();
    ui->plot2->replot();
    displayParam();
}

void MainWindow::removeArbitrage(){
    Arbitrage arb(mod);
    if(!arb.IsArbitrageFree()){
        QVector<double> newCoef = arb.removeArbitrage();
        mod->setCoef(newCoef[0],newCoef[1],newCoef[3],newCoef[2],newCoef[4]);

        mod->setData();
        y = mod->getY();
        g = mod->getG();

        QPen blueDotPen;
        blueDotPen.setColor(Qt::magenta);  // normal QColor(30, 40, 255, 150)
        blueDotPen.setStyle(Qt::DotLine);
        ui->plot->graph(2)->setData(x, y);
        ui->plot2->graph(1)->setData(x,g);
        ui->plot->graph(2)->setPen(blueDotPen);
        ui->plot2->graph(1)->setPen(blueDotPen);
        ui->plot->replot();
        ui->plot2->replot();
        displayParam();
    }
}

void MainWindow::displayParam(){
    if(mod->getParam() == "raw"){
        ui->param1->display(mod->getA());
        ui->param2->display(mod->getB());
        ui->param3->display(mod->getM());
        ui->param4->display(mod->getRo());
        ui->param5->display(mod->getSig());
    }
    else if(mod->getParam() == "nat"){
        ui->param1->display(mod->natParam1());
        ui->param2->display(mod->natParam2());
        ui->param3->display(mod->natParam3());
        ui->param4->display(mod->natParam4());
        ui->param5->display(mod->natParam5());
    }
    else if(mod->getParam() == "jw"){
        ui->param1->display(mod->jwParam1());
        ui->param2->display(mod->jwParam2());
        ui->param3->display(mod->jwParam3());
        ui->param4->display(mod->jwParam4());
        ui->param5->display(mod->jwParam5());
    }
}

void MainWindow::addSampleData(Calibrate* cab){
    ui->plot->graph(1)->setData(cab->getDataX(),cab->getDataY());
    ui->plot->graph(1)->setPen(QPen(Qt::red));
    ui->plot->graph(1)->setLineStyle(QCPGraph::lsNone);
    ui->plot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));
    ui->plot->replot();
}

void MainWindow::changeParam(QString name){
    if(name == "Raw param"){
        name = "raw";
    }
    else if(name == "Natural param"){
        name = "nat";
    }
    else if(name == "SVI-JW param"){
        name = "jw";
    }
    else{
        name = "raw";
    }
    mod->setParam(name);
    displayParam();
}

void MainWindow::changeCurve1(int coef){
    double delta = (double)coef/speedChange;
    mod->changeDataA(delta, speedChange);
    if(mod->getParam() == "raw"){
        ui->param1->display(mod->getA());
    }
    else if(mod->getParam() == "nat"){
        ui->param1->display(mod->natParam1());
    }
    y = mod->getY();
    g = mod->getG();

    ui->plot->graph(0)->setData(x, y);
    ui->plot2->graph(0)->setData(x,g);
    ui->plot->replot();
    ui->plot2->replot();
}

void MainWindow::changeCurve2(int coef){
    double delta = (double)coef/speedChange;
    mod->changeDataB(delta, speedChange);
    if(mod->getParam() == "raw"){
        ui->param2->display(mod->getB());
    }
    else if(mod->getParam() == "nat"){
        ui->param1->display(mod->natParam1());
        ui->param4->display(mod->natParam4());
    }
    y = mod->getY();
    g = mod->getG();

    ui->plot->graph(0)->setData(x, y);
    ui->plot2->graph(0)->setData(x,g);
    ui->plot->replot();
    ui->plot2->replot();
}

void MainWindow::changeCurve3(int coef){
    double delta = (double)coef*10/speedChange;
    mod->changeDataM(delta, speedChange);
    if(mod->getParam() == "raw"){
        ui->param3->display(mod->getM());
    }
    else if(mod->getParam() == "nat"){
        ui->param2->display(mod->natParam2());
    }
    y = mod->getY();
    g = mod->getG();

    ui->plot->graph(0)->setData(x, y);
    ui->plot2->graph(0)->setData(x,g);
    ui->plot->replot();
    ui->plot2->replot();
}

void MainWindow::changeCurve4(int coef){
    double delta = (double)coef*5/speedChange;
    mod->changeDataRo(delta, speedChange);
    if(mod->getParam() == "raw"){
        ui->param4->display(mod->getRo());
    }
    else if(mod->getParam() == "nat"){
        ui->param4->display(mod->natParam4());
        ui->param1->display(mod->natParam1());
        ui->param2->display(mod->natParam2());
        ui->param3->display(mod->natParam3());
        ui->param5->display(mod->natParam5());
    }
    y = mod->getY();
    g = mod->getG();

    ui->plot->graph(0)->setData(x, y);
    ui->plot2->graph(0)->setData(x,g);
    ui->plot->replot();
    ui->plot2->replot();
}

void MainWindow::changeCurve5(int coef){
    double delta = (double)coef*5/speedChange;
    mod->changeDataSig(delta, speedChange);
    if(mod->getParam() == "raw"){
        ui->param5->display(mod->getSig());
    }
    else if(mod->getParam() == "nat"){
        ui->param5->display(mod->natParam5());
        ui->param1->display(mod->natParam1());
        ui->param2->display(mod->natParam2());
        ui->param4->display(mod->natParam4());
    }
    y = mod->getY();
    g = mod->getG();

    ui->plot->graph(0)->setData(x, y);
    ui->plot2->graph(0)->setData(x,g);
    ui->plot->replot();
    ui->plot2->replot();
}

void MainWindow::reInitSliders(){
    int temp(0);
    temp = ui->horizontalSlider->value();
    ui->horizontalSlider->setMinimum(temp-50);
    ui->horizontalSlider->setMaximum(temp+50);

    temp = ui->horizontalSlider_2->value();
    ui->horizontalSlider_2->setMinimum(temp-50);
    ui->horizontalSlider_2->setMaximum(temp+50);

    temp = ui->horizontalSlider_3->value();
    ui->horizontalSlider_3->setMinimum(temp-50);
    ui->horizontalSlider_3->setMaximum(temp+50);

    temp = ui->horizontalSlider_4->value();
    ui->horizontalSlider_4->setMinimum(temp-50);
    ui->horizontalSlider_4->setMaximum(temp+50);

    temp = ui->horizontalSlider_5->value();
    ui->horizontalSlider_5->setMinimum(temp-50);
    ui->horizontalSlider_5->setMaximum(temp+50);
}

void MainWindow::setSpeedChange(int val){
    speedChange = pow(10,val);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mod;
    delete diag;
}
