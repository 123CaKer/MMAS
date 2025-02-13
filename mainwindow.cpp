#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Tsp.h"
#include<QGraphicsTextItem>
#include<QGraphicsLineItem>
#include "Common.h"
#include <QDebug>
#include "chartview.h"
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , scene(new QGraphicsScene(this))
    , pathGroup(nullptr)
    , m_tps(nullptr)
{
    ui->setupUi(this);
    ui->graphicsView_map->setScene(scene);

}

MainWindow::~MainWindow()
{
    delete ui;
    if(m_tps){
        delete m_tps;
    }
}


void MainWindow::on_pushButton_run_clicked()
{
    if(m_tps){
        if(m_haveRunned){
            if(!paramtersCheck()) return;
            resetTsp();
        }
        if(!pathGroup){
            pathGroup = new QGraphicsItemGroup;
            scene->addItem(pathGroup);
        }
        ui->pushButton_run->setDisabled(true);
        ui->pushButton_initmap->setDisabled(true);
        m_tps->setSence(scene, pathGroup);
        m_tps->Search();
        drawPath();
        auto ant = m_tps->m_cGlobalBestAnt;
        ui->doubleSpinBox_result->setValue(ant.WalkLength());
        auto chart = createLineChart(m_tps->StepResult());
        ui->graphicsView_result->setChart(chart); 
        ui->pushButton_run->setEnabled(true);
//        ui->pushButton_initmap->setEnabled(true);
        m_haveRunned = true;
    }
}

void MainWindow::initMap()
{
    auto cities = m_tps->Cities();
    QPen pen(Qt::blue);
    QBrush brush(Qt::red);
    scene->clear();
    foreach(auto& city, cities)
    {
        double x = city.X(), y = city.Y();
        scene->addEllipse(QRectF(x-5, y-5, 10, 10),pen, brush);
        auto item = scene->addText(QString("%0").arg(city.Name()));
        item->setPos(x-5, y+5);
    }
}

void MainWindow::drawPath()
{
    auto best   = m_tps->BestAnt();
    auto path   = best.Path();
    auto cities = m_tps->Cities();
    draw_path(pathGroup, cities,  path);
}

void MainWindow::initTsp()
{
    if(m_tps){
        delete m_tps;
        m_tps = nullptr;
    }

    int ant_num  = ui->spinBox_ant_number->value();
    int it_count = ui->spinBox_it_count->value();
    double alpha = ui->doubleSpinBox_alpha->value();
    double beta  = ui->doubleSpinBox_beta->value();
    double rou   = ui->doubleSpinBox_rho->value();
    double tmax  = ui->doubleSpinBox_tmax->value();
    double tmin  = ui->doubleSpinBox_tmin->value();

    m_tps = new CTsp(it_count, ant_num, alpha, beta, rou, tmin, tmax);
    m_tps->InitData();
    m_haveRunned = false;
}

void MainWindow::resetTsp()
{
    int ant_num  = ui->spinBox_ant_number->value();
    int it_count = ui->spinBox_it_count->value();
    double alpha = ui->doubleSpinBox_alpha->value();
    double beta  = ui->doubleSpinBox_beta->value();
    double rou   = ui->doubleSpinBox_rho->value();
    double tmax  = ui->doubleSpinBox_tmax->value();
    double tmin  = ui->doubleSpinBox_tmin->value();
    m_tps->ResetData(it_count,ant_num, alpha, beta, rou, tmin, tmax);
    m_haveRunned = false;
}

void MainWindow::clearPathLines()
{
    if(pathGroup){
        foreach(auto line, pathGroup->childItems())
        {
            pathGroup->removeFromGroup(line);
            delete line;
        }
    }
}

bool MainWindow::paramtersCheck()
{
    double tmax  = ui->doubleSpinBox_tmax->value();
    double tmin  = ui->doubleSpinBox_tmin->value();
    if(tmax - tmin < 0)
    {
        QMessageBox::warning(this, STR_CONV("¾¯¸æ"), STR_CONV("tmax ±ØÐë >= tmin"));
        ui->doubleSpinBox_tmax->setValue(0);
        ui->doubleSpinBox_tmin->setValue(0);
        return false;
    }
    return true;
}



void MainWindow::on_pushButton_initmap_clicked()
{
    if(!paramtersCheck()) return;
    initTsp();
    initMap();
}

void MainWindow::on_spinBox_it_count_valueChanged(const QString &arg1)
{

}
