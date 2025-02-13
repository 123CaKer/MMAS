#ifndef CITY_H
#define CITY_H
#include<QString>
#include<cmath>
class City
{
public:
    friend double CalCityDistance(const City& a,  const City& b);
    City(double x, double y, const QString& name);
    ~City();
    double X() const{
        return x;
    }
    double Y() const{
        return y;
    }
    QString Name() const{
        return name;
    }
private:
    double x, y;
    QString name;
};

inline double CalCityDistance(const City& a,  const City& b)
{
        double dx = a.x-b.x, dy = a.y-b.y;
        return sqrt(dx*dx+dy*dy);
}
#endif // CITY_H
