#ifndef DIALOGRANGE_H
#define DIALOGRANGE_H

#include <QDialog>

namespace Ui {
class DialogRange;
}

class DialogRange : public QDialog
{
    Q_OBJECT

public:
    explicit DialogRange(QWidget *parent = 0);
    QVector<double> getRange();
    QVector<double> getRange2();
    double getT();
    void setRange(int pos, double val);
    void setRange2(int pos, double val);
    ~DialogRange();

public slots:
    void setRange0(double val);
    void setRange1(double val);
    void setRange2(double val);
    void setRange3(double val);
    void setRange0_2(double val);
    void setRange1_2(double val);
    void setRange2_2(double val);
    void setRange3_2(double val);
    void setT(double t);

private:
    Ui::DialogRange *ui;
    QVector<double> range;
    QVector<double> range2;
    double t;
};

#endif // DIALOGRANGE_H
