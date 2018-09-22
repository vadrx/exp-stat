#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <algorithm>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , err(false)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}
MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::calculate_data(QVector<double> array)
{
    err = clear_result_lists();
    err = calc_vr(array);
    err = calc_razmah(array);
    err = calc_povtor(array);
    err = calc_mediana(array);
    err = calc_medium_and_disp(array);
    err = calc_dov_disp(array);
    err = calc_dov_nodisp(array);
    err = build_chart(array);
    return true;
}

bool MainWindow::clear_result_lists()
{
    res_w.get_ui()->listWidget_S->clear();
    res_w.get_ui()->list_Moda->clear();
    return true;
}

bool MainWindow::calc_vr(QVector<double> array) //var ryad
{
    std::sort(array.begin(), array.end());
    for (int i = 0; i < array.size(); i++)
        res_w.get_ui()->listWidget_S->addItem(QString::number(array.value(i)));
    return true;
}

bool MainWindow::calc_povtor(QVector<double> array)
{
    int count = 0;
    int free = 0;
    int max = -1;
    int N = array.size();
    QVector<double>c (N);
    QVector<double>v (N);

    for (int i = 0; i < N; i++)
    {
        count = 0;
        for (int j = 0; j < N; j++)
        {
            if (array[i] == v[j])
            {
                c[j]++;
                max = (max < c[j]) ? c[j] : max;
                break;
            }
            else
                count++;
        }
        if (count == N)
        {
            v[free] = array[i];
            c[free] = 1;
            free++;
        }
    }
    std::sort(v.begin(), v.end());
    for (int i = 0; i < N; i++)
        if (c[i] == max)
                res_w.get_ui()->list_Moda->addItem(QString::number(v[i]));
    return true;
}

bool MainWindow::build_chart(QVector<double> array)
{
    double N = array.size();
    double razm = QString(res_w.get_ui()->lineEdit_Razmah->text()).toDouble();
    double m = 1 + 3.2218 * log(N);
    double len_p_inters = razm / N;
    res_w.get_ui()->lineEdit_col_inters->setText(QString::number(m));
    res_w.get_ui()->lineEdit_len_p_inters->setText(QString::number(len_p_inters));

    QBarSet *set = new QBarSet("Интервалы");
    for(int i=0; i< static_cast<int>(m); i++)
        *set << array.value(i) / (N * len_p_inters);

    QBarSeries *series = new QBarSeries();
    series->append(set);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Гистограмма");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axis = new QBarCategoryAxis();
    chart->createDefaultAxes();
    chart->setAxisX(axis, series);
    chart->axisY(series)->setTitleText("Частота");

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    res_w.get_ui()->widget->setChart(chart);
    return true;
}

bool MainWindow::calc_razmah(QVector<double> array)
{
    double max_elem = *std::max_element(array.constBegin(), array.constEnd());
    double min_elem = *std::min_element(array.constBegin(), array.constEnd());
    double razm = max_elem - min_elem;
    res_w.get_ui()->lineEdit_Razmah->setText(QString::number(razm));
    return true;
}

bool MainWindow::calc_mediana(QVector<double> array)
{
    int N = array.size();
    double me = 0.0;

    if (N % 2 != 0.0)
        me = array.value((N+1)/2 - 1);
    else
        me = (array.value(N/2 - 1) + array.value(N/2)) / 2;
    res_w.get_ui()->lineEdit_Med->setText(QString::number(me));
    return true;
}

bool MainWindow::calc_medium_and_disp(QVector<double> array)
{
    int N = array.size();
    double sred_sum = 0.0;
    double sred = 0.0;
    double disp_sum = 0.0;
    double disp = 0.0;

    for (int i = 0; i < N; i++)
    {
        sred_sum += array.value(i);
        disp_sum += pow(array.value(i) - sred, 2);
    }
    sred = sred_sum / N;
    disp = pow(disp_sum / N , 2);
    res_w.get_ui()->lineEdit_Isp_Disp->setText(QString::number(disp));
    res_w.get_ui()->lineEdit_Sred->setText(QString::number(sred));
    return true;
}

bool MainWindow::calc_dov_disp(QVector<double> array)
{
    double N  = array.size();
    double vib_sr = QString(res_w.get_ui()->lineEdit_Sred->text()).toDouble();
    double sig  = QString(ui->lineEdit_nsig->text()).toInt();
    double kvantil = QString(ui->comboBox_kvantil->currentText()).toDouble();

    double l_part = vib_sr - kvantil * sig / sqrt(N);
    double r_part = vib_sr + kvantil * sig / sqrt(N);
    res_w.get_ui()->lineEdit_l_disp->setText(QString::number(l_part));
    res_w.get_ui()->lineEdit_r_disp->setText(QString::number(r_part));
    return true;
}
bool MainWindow::calc_dov_nodisp(QVector<double>array)
{
    double N  = array.size();
    double vib_sr = QString(res_w.get_ui()->lineEdit_Sred->text()).toDouble();
    double ispr_disp = QString(res_w.get_ui()->lineEdit_Isp_Disp->text()).toDouble();
    double kvantil = QString(ui->comboBox_kvantil->currentText()).toDouble();

    double l_part = vib_sr - kvantil * sqrt(ispr_disp) / sqrt(N);
    double r_part = vib_sr + kvantil * sqrt(ispr_disp) / sqrt(N);
    res_w.get_ui()->lineEdit_l_nodisp->setText(QString::number(l_part));
    res_w.get_ui()->lineEdit_r_nodisp->setText(QString::number(r_part));
    return true;
}

QVector<double> MainWindow::gen_norm_raspr(int N, double a, double sig)
{
    double s = 0.0;
    QVector<double> arr(N);
    QVector<double> buff(N);

    for (int i=0; i < N; i++)
    {
        for (int j=0; j < 12; j++)
        {
            arr[i] = rand() % 5000 / 5000.0;
            s += arr[i];
        }
        buff[i] = s - 6;
        s = 0.0;
        arr[i] = static_cast<double>(sig * buff[i] + a);
    }
    buff.clear();
    return arr;
}

QVector<double> MainWindow::gen_ravnom_raspr(int N, double a, double b)
{
    QVector<double> arr(N);
    for (int i=0; i < N; i++)
        arr[i] = a + rand() % 1000 * (b-a) / 1000.0;
    return arr;
}
//  normalnoe raspr
void MainWindow::on_gen_norm_clicked()
{
    res_w.show();
    //  read_lines
    double n  = QString(ui->lineEdit_n->text()).toInt();
    double sig  = QString(ui->lineEdit_nsig->text()).toInt();
    double a  = QString(ui->lineEdit_ra->text()).toDouble();

    srand(static_cast<unsigned int>(time(nullptr)));
    //  generate
    QVector<double> arr = gen_norm_raspr(n, a, sig);
    err = calculate_data(arr);
    arr.clear();
}
//  ravnomernoe raspr
void MainWindow::on_gen_ravn_clicked()
{
    res_w.show();
    //  read_lines
    double n  = QString(ui->lineEdit_n->text()).toInt();
    double a  = QString(ui->lineEdit_ra->text()).toDouble();
    double b  = QString(ui->lineEdit_rb->text()).toDouble();

    srand(static_cast<unsigned int>(time(nullptr)));
    QVector<double> arr = gen_ravnom_raspr(n, a, b);
    //  generate
    err = calculate_data(arr);
    arr.clear();
}
