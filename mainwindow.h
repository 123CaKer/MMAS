#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include "Tsp.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_run_clicked();
    void on_pushButton_initmap_clicked();

    void on_spinBox_it_count_valueChanged(const QString &arg1);

private:
    void initMap();
    void drawPath();
    void initTsp();
    void resetTsp();
    void clearPathLines();
    bool paramtersCheck();
private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsItemGroup *pathGroup;
    CTsp* m_tps;
    bool m_haveRunned = false;
};
#endif // MAINWINDOW_H
