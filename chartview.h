#ifndef CHARTVIEW_H
#define CHARTVIEW_H
#include <QChartView>
#include<QtCharts>
#include<QList>
#include<QPair>
class ChartView: public QtCharts::QChartView
{
public:
    ChartView(QWidget *parent = nullptr);
};
QtCharts::QChart* createLineChart(const QList<QPair<int, double>>& data);
#endif // CHARTVIEW_H
