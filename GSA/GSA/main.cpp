#include"test_func.h"
#include"GSA.h"
#include <iostream>
#include <locale>
#include <iomanip>

int main()
{
	bool mode = 0;
	vector<double> max = GSA(func1, 2, restrict1, 1000, 200, 100, mode);
	double best = func1(max);
	setlocale(LC_CTYPE, "");
	std::cout << "Функция f(x,y) = x + y\n";
	std::cout << "Ограничения:\n -4 <= x <=4\n -4 <= y <= 4\n";
	std::cout << "Нахождение максимума\n";
	std::cout << "x = " <<std::fixed<<std::setprecision(2)<< max[0] << std::endl;
	std::cout << "y = " << std::fixed << std::setprecision(2) << max[1] << std::endl;
	std::cout << "Экстремум функции f(x,y) " << std::fixed << std::setprecision(2) << best << std::endl;
	system("pause");
	return 0;
}