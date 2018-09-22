#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "resultwindow.h"
#include "ui_resultwindow.h"
#include <QtCharts>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_gen_norm_clicked();
    void on_gen_ravn_clicked();
private:
    QVector<double> gen_norm_raspr(int N, double a, double sig);
    QVector<double> gen_ravnom_raspr(int N, double a, double b);

    bool calculate_data(QVector<double> array);
    bool calc_vr(QVector<double> array); //variac ryad
    bool calc_povtor(QVector<double> array);
    bool calc_razmah(QVector<double> array);
    bool calc_mediana(QVector<double> array);
    bool calc_medium_and_disp(QVector<double> array);

    bool calc_dov_disp(QVector<double> array);
    bool calc_dov_nodisp(QVector<double> array);

    bool build_chart(QVector<double> array);
    bool clear_result_lists();

    bool err;
    ResultWindow res_w;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
