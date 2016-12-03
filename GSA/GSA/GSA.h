#pragma once
#include<vector>
#include<cstdlib>
#include<math.h>
#include<algorithm>
#include <ctime>

using std::vector;

namespace Prep
{
	double distEuclid(vector<double> a, vector<double> b)
	{
		if (a.size() != b.size()) return 0;
		double r = 0;
		for (size_t i = 0; i < a.size(); i++)
		{
			r += pow((a[i] - b[i]),2);
		}
		return sqrt(r);
	}
	double machineEps()
	{
		double e = 1.0;
		while (1.0 + e / 2.0 > 1.0)
		{
			e /= 2.0;
		}
		return e;
	}
}

/*
Реализация алгоритма гравитационного поиска
*/

//Гравитационный алгоритм поиска (по умолчанию mode = 0 - минимизация функции, 1 - максимизация)
vector<double> GSA(double(*func)(vector<double>), int, bool(*restrict) (vector<double>), double, int, int, bool mode = 0);

//Инициализирует положение агентов в пространстве поиска (случайным образом)
vector<vector<double>> init(int, int, bool (*restrict)(vector<double>));

//Вычисление ускорения агентов в гравитационном поле
vector<vector<double>> calcAcceleration(vector<double>, vector<vector<double>>, double, int);

//Обновление скорости и положения агентов
void updateAgents(vector<vector<double>>&, vector<vector<double>>, vector<vector<double>>&);

//Вычисление массы каждого объекта
vector<double> calcMass(vector<double>, bool mode);

//Вычисление гравитационной константы
inline double calcGconst(int, double);

//Проверяет границы пространства поиска для агентов
vector<vector<double>> spaceBound(vector<vector<double>>, bool (*restrict)(vector<double>));

//Оценка агентов
vector<double> evaluate(vector<vector<double>>, double(*func)(vector<double>));



vector<double> GSA(double(*func)(vector<double>), int dim, bool(*restrict) (vector<double>), double G, int N, int max_it, bool mode)
{
	/*
	Гравитационный алгориртм поиска
	func - указатель на целевую функцию
	restrict - указатель на систему ограничений
	G - гравитационная постоянная
	N - число агентов
	mode - режим задачи поиска экстремума (минимизация = true(0), максимизация false(1))
	*/
	vector<double> result(dim);		//Значения переменных, соответствующие экстремуму по условию задачи
	vector<vector<double>> pos(N);	//Вектор координат позиций агентов
	for (int i = 0; i < N; i++)
	{
		pos[i].resize(dim);
	}
	srand(static_cast<size_t>(time(NULL)));
	pos = init(dim, N, restrict);	//Инициализация агентов случайным образом
	vector<vector<double>> vel(N);			//Вектор скоростей агентов
	for (int i = 0; i < N; i++)
	{
		vel[i].resize(dim);
	}
	vector<double> fitness(N);
	vector<double> mass(N);
	vector<vector<double>> accel(N);
	for (int i = 0; i < N; i++)
	{
		accel[i].resize(dim);
	}
	vector<double> tempBest(dim);	//Хранение текущего результата (позиции)
	double best = 0;		//Лучшее текущее значение для целевой функции
	double tempfunc = 0;			//Значение функции на текущем шаге
	double Gi = 0;
	for (int i = 0; i < max_it; i++)
	{

		fitness = evaluate(pos, func);
		pos = spaceBound(pos, restrict);
		vector<double>::iterator ext = mode ? std::max_element(fitness.begin(), fitness.end()) : std::min_element(fitness.begin(), fitness.end());
		vector<double>::iterator bad = std::max_element(mass.begin(), mass.end());
		tempBest = pos[distance(fitness.begin(), ext)];
		
		tempfunc = func(tempBest);
		if (mode)
		{
			if (best < tempfunc) 
			{ 
				result = tempBest; 
				best = tempfunc; 
			}
		}
		else
		{
			if ((best > tempfunc)||(i == 0)) 
			{ 
				result = tempBest; 
				best = tempfunc; 
			}
		}
		mass = calcMass(fitness,mode);
		Gi = calcGconst(i, G);
		accel = calcAcceleration(mass, pos, Gi, i);
		updateAgents(pos, accel, vel);
		if (N < max_it)
		{
			pos.erase(pos.begin() + distance(fitness.begin(), bad), pos.begin() + distance(fitness.begin(), bad));
		}
	}
	return result;
}


vector<vector<double>> init(int dim, int N, bool(*restrict)(vector<double>))
{
	/*
		Случайная инициализация агентов
		dim - размерность функции (число переменных)
		N - число агентов
		restrict - указатель на функцию с системой ограничений
	*/	
	vector<vector<double>> pos(N);
	for (int i = 0; i < N; i++)
	{
		pos[i].resize(dim);
		do
		{
			for (int j = 0; j < dim; j++)
			{
				pos[i][j] = pow(-1.0, rand()*2/RAND_MAX)*rand()*100/RAND_MAX;
			}
		}while (!restrict(pos[i]));
	}
	return pos;
}


vector<vector<double>> spaceBound(vector<vector<double>> X, bool(*restrict)(vector<double>))
{
	/*
	Проверка границ 
	X - координаты позиций агентов
	restict - система ограничений
	*/
	for (size_t i = 0; i < X.size(); i++)
	{
		while (!restrict(X[i]))
		{
			for (size_t j = 0; j < X[i].size(); j++)
			{
				X[i][j] = pow(-1.0,rand()*2/RAND_MAX)*rand()*100/RAND_MAX;
			}
		}
	}
	return X;
}

vector<vector<double>> calcAcceleration(vector<double> mass, vector<vector<double>> pos, double G, int it)
{
	/*
	Пересчёт ускорений агентов
	mass - массы агентов
	pos - координаты позиций агентов
	it - текущая итерация
	max_it - максимальное число итераций
	G - гравитационная постоянная (для i)
	*/
	vector<vector<double>> ac(pos.size());
	for (size_t i = 0; i < pos.size(); i++)
	{
		ac[i].resize(pos[i].size());
		for (size_t j = 0; j < pos.size(); j++)
		{
			if (j != i)
			{
				//Евклидово расстояние между агентами
				double R = Prep::distEuclid(pos[i], pos[j]);
				for (size_t k = 0; k < pos[j].size(); k++)
				{
					//Ускорение по k-ому измерению
					ac[i][k] = (rand() / double(RAND_MAX)) * G * mass[j] * (pos[j][k] - pos[i][k]) / (R + Prep::machineEps());
				}
			}
		}
	}
	return ac;
}

vector<double> calcMass(vector<double> fitness, bool mode)
{
	/*
	Пересчёт масс
	fitness - фитнесс-функция
	mode - режим
	*/
	vector<double> mass(fitness.size());
	vector<double>::iterator fitMin = min_element(fitness.begin(), fitness.end());
	vector<double>::iterator fitMax = max_element(fitness.begin(), fitness.end());
	double max = mode == 0 ? *fitMin : *fitMax;
	double min = mode == 0 ? *fitMax : *fitMin;
	for (size_t i = 0; i < fitness.size(); i++)
	{
		mass[i] = (fitness[i] - min) / (max - min);
	}
	double sum = 0;
	for (size_t i = 0; i < mass.size(); i++)
	{
		sum += mass[i];
	}
	for (size_t i = 0; i < fitness.size(); i++)
	{
		mass[i] = mass[i] / sum;
	}
	return mass;
}

inline double calcGconst(int it, double G0)
{
	/*
	Рассчёт гравитационной постоянной
	it - текущая итерация
	G0 - начальное значение гравитационной постоянной
	*/
	double d = 0.1; //Свободный параметр алгоритма
	return G0*std::pow((1 / (it + 1)), d);
}

void updateAgents(vector<vector<double>>& pos, vector<vector<double>> acc, vector<vector<double>>& vel)
{
	/*
	Пересчёт скоростей и координат позиций агентов
	pos - вектор координат позиций агентов 
	acc -  вектор ускорений агентов
	vel - вектор скоростей агентов
	*/
	for (size_t i = 0; i < vel.size(); i++)
	{
		for (size_t j = 0; j < pos[i].size(); j++)
		{
			vel[i][j] = rand() / RAND_MAX * vel[i][j] + acc[i][j];
			pos[i][j] = pos[i][j] + vel[i][j];
		}
	}
}

vector<double> evaluate(vector<vector<double>> pos, double(*func)(vector<double>))
{
	/*
	Оценка агентов
	pos - вектор координат прзиций агентов
	func - указатель на целевую функцию
	*/
	vector<double> fitness(pos.size());		//фитнес-функция
	for (size_t i = 0; i < pos.size(); i++)
	{
		vector<double> agent(pos[i].size());
		for (size_t j = 0; j < pos[i].size(); j++)
		{
			agent[j] = pos[i][j];
		}
		fitness[i] = func(agent);
	}
	return fitness;
}