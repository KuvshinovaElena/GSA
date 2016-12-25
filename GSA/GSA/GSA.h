#pragma once
#include<vector>
#include<cstdlib>
#include<math.h>
#include<algorithm>
#include <ctime>

using std::vector;

namespace Prep
{
	//Нахождение евклидова расстояния
	double distEuclid(vector<double> a, vector<double> b);

	//Нахождение машинного эпсилон
	double machineEps();
}

/*
Реализация алгоритма гравитационного поиска
*/
namespace GSA {
	//Гравитационный алгоритм поиска (mode = 0 - минимизация функции, 1 - максимизация)
	vector<double> GSA(double(*func)(vector<double>), int, bool(*restrict) (vector<double>), double, int, int, bool mode);

	//Инициализирует положение агентов в пространстве поиска (случайным образом)
	vector<vector<double>> init(int, int, bool(*restrict)(vector<double>));

	//Вычисление ускорения агентов в гравитационном поле
	vector<vector<double>> calcAcceleration(vector<double>, vector<vector<double>>, double, int);

	//Обновление скорости и положения агентов
	void updateAgents(vector<vector<double>>&, vector<vector<double>>, vector<vector<double>>&);

	//Вычисление массы каждого объекта
	vector<double> calcMass(vector<double>, bool mode);

	//Вычисление гравитационной константы
	inline double calcGconst(int, double);

	//Проверяет границы пространства поиска для агентов
	vector<vector<double>> spaceBound(vector<vector<double>>, bool(*restrict)(vector<double>));

	//Оценка агентов
	vector<double> evaluate(vector<vector<double>>, double(*func)(vector<double>));
}