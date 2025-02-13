#include "chartview.h"
#include<QDebug>
ChartView::ChartView(QWidget *parent)
    :QtCharts::QChartView(parent)
{

}

QChart* createLineChart(const QList<QPair<int, double>>& data)
{
    QChart *chart = new QChart();
    chart->setTitle(QString::fromLocal8Bit("最短距离"));
    chart->setBackgroundBrush(Qt::white);
    //![1]

//    //![2]
    QString name(QString::fromLocal8Bit("每次迭代最短距离"));
    QLineSeries *series = new QLineSeries(chart);
    series->setName(name);
    series->setColor(Qt::blue);
    int xMax = 0;
    double yMax = 0.0;
    for(const QPair<int, double> p :data)
    {
        if (xMax<p.first) xMax = p.first;
        if(yMax<p.second) yMax = p.second;
        series->append(p.first, p.second);
    }

    chart->addSeries(series);

//    for (const DataList &list : m_dataTable) {
//        QLineSeries *series = new QLineSeries(chart);
//        for (const Data &data : list)
//            series->append(data.first);
//        series->setName(name + QString::number(nameIndex));
//        nameIndex++;
//        chart->addSeries(series);
//    }
//    //![2]

//    //![3]
    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setRange(1, xMax);
    chart->axes(Qt::Vertical).first()->setRange(0, yMax);

//    //![3]
//    //![4]
//    // Add space to label to add space between labels and axis
   QValueAxis *axisX = qobject_cast<QValueAxis*>(chart->axes(Qt::Horizontal).first());
   Q_ASSERT(axisX);
   axisX->setLabelFormat("%d  ");
   axisX->setTitleText(QString::fromLocal8Bit("迭代次数"));
   axisX->setTickCount(11);

   QValueAxis *axisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).first());
   Q_ASSERT(axisY);
   axisY->setTitleText(QString::fromLocal8Bit("最短路线长度"));
   axisY->setTickCount(11);
    //![4]

    return chart;
}
