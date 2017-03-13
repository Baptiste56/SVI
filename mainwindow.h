#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "svimodel.h"
#include "calibrate.h"
#include "dialogrange.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    public slots:
    void calibrateCurve();
    void removeArbitrage();
    void showDataSet();
    void setRange();
    void setInitialCurve();
    void changeParam(QString name);
    void changeCurve1(int coef);
    void changeCurve2(int coef);
    void changeCurve3(int coef);
    void changeCurve4(int coef);
    void changeCurve5(int coef);
    void reInitSliders();
    void setSpeedChange(int val);

    private:
    Ui::MainWindow *ui;
    DialogRange* diag;
    SVIModel *mod;
    void displayParam();
    void addSampleData(Calibrate* cab);
    QVector<double> x;
    QVector<double> y;
    QVector<double> g;
    QVector<double> lastRange;
    int speedChange;
};

#endif // MAINWINDOW_H
