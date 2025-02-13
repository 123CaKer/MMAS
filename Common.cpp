#include "common.h"
#include<cstdlib>
double g_Pheromone[N_CITY_COUNT][N_CITY_COUNT]; //两两城市间信息素
double g_Distance[N_CITY_COUNT][N_CITY_COUNT]; //两两城市间距离

double g_Prob[N_CITY_COUNT][N_CITY_COUNT];
double g_DistanceBeta[N_CITY_COUNT][N_CITY_COUNT];



//////////////////////////////////////////////////////////////////////////////
//
//  用 途：返回指定范围内的随机整数
//
//  说 明：无
//
//  接 口：int nLow 随机数的下限
//                     int nUpper 随整数的上限
//
//  返 回：int ,范围是[nLow,nUpper)
//
/////////////
int rnd(int nLow,int nUpper)
{
	
	return nLow+(nUpper-nLow)*rand()/(RAND_MAX+1);

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  用 途：返回指定范围内的随机浮点数
//
//  说 明：无
//
//  接 口：double dbLow 随机数的下限
//                     double dbUpper 随机数的上限
//
//  返 回：double  ,范围是[dbLow,dbUpper)
//
/////////////
double rnd(double dbLow,double dbUpper)
{
	double dbTemp=rand()/((double)RAND_MAX+1.0);
	return dbLow+dbTemp*(dbUpper-dbLow);
}

//返回浮点数四舍五入取整后的浮点数
double ROUND(double dbA)
{
	return (double)((int)(dbA+0.5));
}
