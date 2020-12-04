/*****************************************************
* Project name : Kursach	                        *
* Project type : win32 console application          *
* File name    : Kursach.c                          *
* Programmers  : Frolov E.V.                        *
* Modified     : 15.12.18                           *
* Created      : 08.12.18                           *
* Last Revision: 16.12.18                           *
* Comment      : Вычисление интегралов численными   *
*                методами с двоичного файла         *
*                [ВАРИАНТ 6]                        *
*****************************************************/

#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <float.h>
#include <errno.h>


#define ex 12345678900								//Значения для передачи функции
#define iks 123456789001								//определяющие число/х/е
#define mines_iks 1234567890010
#define mines_ex 123456789002

double f(double x);									//Функция для интегрирования
void equation_assembly(char mass[], int length);	//Получает первоначальные данные о переменных
void to_per();										//Приведение переменных к типу double 
void rectangle_method();								//Метод прямоугольников
void trapezoid_method();								//Метод трапеций 
double rectangleArea(int n);							//Подсчет приближенного значения для прямоугольников
double trapezoidArea(int n);							//Подсчет приближенного значения для трапеций



char chislo1[3],										//переменные для считывания из массива
chislo2[3],
trigonom[6],
arg_f[3],
constanta[3],
ogr1[5],
ogr2[5];
double num1, num2, num3 = 1,
num4, down_border, top_border;
char znak1, znak2, chisl1, chisl2,
flag1 = 0, flag2 = 0, flag3 = 0;					//переменные и знаки приведенные к нормальному виду
int funcCalcs = 0;									//для подсчета итерраций

int main()
{
	 	const char mass[35] = "y=x^-1*arctg(x)+2, xэ(-1,2.5)"; //"y=x^-1*tg(x)+31, xэ(2.5,4)";
		int length = 35;
	const char endOfFile = 3;
	const char filenameIN[] = "Out.bin";				 // исходный файл
	const int  capacity = 70;						 // размер буфера
	char line[70];									 // строка
	//	length;
	int j = 0;                                       // число строк

	/*FILE *in;									     // поток данных
	in = fopen(filenameIN, "rb");					 //открываем файл в двоичном виде

	if (in != NULL)									 // если файл открылся
	{
		for (fread(&length, 1, 1, in); length != endOfFile; j++)   // пока не достигнем конца файла
		{
			fread(line, length, 1, in);
			//printf("Received string: %s\n", line);
			length = 0;								 // обнуляем значения для новой строки
			fread(&length, 1, 1, in);
		}
		fclose(in);									 // закрываем файл
	}
	else
	{
		printf("Error while \"%s\" file opening!\n", filenameIN);  // если что-то пошло не так — выводим ошибку
	}*/

	//if (j == 1)										 // если получена только одна строка
	//{
		equation_assembly(mass, length);			 //функия, выделяющая коэфициенты из нашего уравнения
		to_per();									 //приведение переменных к типу double 
													 //***** Методы трапеций и прямоугольников*****//
		trapezoid_method(num1, num2, num3, num4, down_border, top_border, znak1, znak2, trigonom);
		rectangle_method(num1, num2, num3, num4, down_border, top_border, znak1, znak2, trigonom);
	//}
	system("pause");
}


void equation_assembly(char mass[], int length)
{
	int n; //позиция в массиве
	int pos_step, end_step, pos_skobki, pos_skobki2, end_f, start_ogr, end_ogr;

	//получаем первую цифру или число
	for (int n = 0; n < length; n++)
	{
		if ((mass[n] == 'y') && (mass[n + 1] == '='))
		{
			if (isdigit(mass[n + 2]) || (mass[n + 2] == '-'))
			{
				for (int i = 0; i < 5; i++)// 5 - длина 
				{
					if (mass[n + 2 + i] == '^')
					{
						pos_step = n + 2 + i;
						break;
					}
					chislo1[i] = mass[n + 2 + i];
				}
			}
			if (mass[n + 2] == 'e')
			{
				flag1 = 1;
				pos_step = n + 3;
			}
			if (mass[n + 2] == 'x')
			{
				flag1 = 2;
				pos_step = n + 3;
			}
			if ((mass[n + 2] == '-') && (mass[n + 3] == 'x'))
			{
				flag1 = 3;
				pos_step = n + 4;
			}
			if ((mass[n + 2] == '-') && (mass[n + 3] == 'e'))
			{
				flag1 = 4;
				pos_step = n + 4;
			}

			//получаем степень числа
			if (isdigit(mass[pos_step + 1]) || (mass[pos_step + 1] == '-'))
			{
				for (int i = 0; i < 5; i++)// 5 - длина 
				{
					if (!isdigit(mass[pos_step + 1 + i]) && (mass[pos_step + i] != '^') && (mass[pos_step + i] != '-'))
					{
						end_step = pos_step + 1 + i;
						break;
					}
					chislo2[i] = mass[pos_step + 1 + i]; //степень

				}
			}
			if (mass[pos_step + 1] == 'e')
			{
				flag2 = 1;
				end_step = pos_step + 2;
			}
			if (mass[pos_step + 1] == 'x')
			{
				flag2 = 2;
				end_step = pos_step + 2;
			}
			if ((mass[pos_step + 1] == '-') && (mass[pos_step + 2] == 'x'))
			{
				flag2 = 3;
				end_step = pos_step + 3;
			}
			n = end_step;

			znak1 = mass[n]; //первый знак

							 //получаем тригонометрическую функцию
			for (int i = 0; i < 7; i++)
			{
				if (mass[n + 1 + i] == '(')
				{
					pos_skobki = n + 1 + i;
					break;
				}
				trigonom[i] = mass[n + 1 + i]; //тригонометрич ф-я
			}

			//получаем аргумент
			for (int i = 0; i < 5; i++)// 5 - длина аргумента триг ф.
			{
				if (!isdigit(mass[pos_skobki + 1]))
				{
					pos_skobki2 = pos_skobki + 2;
					flag3 = 1;
					break;
				}
				else
					if (mass[pos_skobki + 1 + i] == '*')
					{
						pos_skobki2 = pos_skobki + i + 3;
						break;
					}
					else arg_f[i] = mass[pos_skobki + 1 + i]; //аргумент функции
			}


			znak2 = mass[pos_skobki2 + 1];		//второй знак

												//получаем константу
			for (int i = 0; i < 5; i++)// 5 - длина 
			{
				if (mass[pos_skobki2 + 2 + i] == ',')
				{
					end_f = pos_skobki2 + 2 + i;
					break;
				}
				constanta[i] = mass[pos_skobki2 + 2 + i]; //запись числа в пер.
			}

			//получаем границы интегрирования
			for (int i = 0; i < length; i++)
			{
				if (mass[end_f + i] == 'x')
				{
					start_ogr = end_f + i + 3; // xэ(_, ) 

					for (int i = 0; i < 5; i++)
					{
						if (mass[start_ogr + i] == ',')
						{
							end_ogr = start_ogr + i;
							break;
						}
						ogr1[i] = mass[start_ogr + i];
					}

					for (int i = 0; i < 5; i++) // xэ( ,_)
					{
						if (mass[end_ogr + 1 + i] == ')')
							break;
						ogr2[i] = mass[end_ogr + 1 + i];
					}
				}
			}
		}
		break;
	}
}

void to_per()
{
	if (flag1 == 0) num1 = atof(chislo1);
	if (flag1 == 1) num1 = ex;
	if (flag1 == 2) num1 = iks;
	if (flag1 == 3) num1 = mines_iks;
	if (flag1 == 4) num1 = mines_ex;

	if (flag2 == 0) num2 = atof(chislo2);
	if (flag2 == 1) num2 = ex;
	if (flag2 == 2) num2 = iks;
	if (flag2 == 3) num2 = mines_iks;

	if (flag3 == 0) num3 = atof(arg_f);

	num4 = atof(constanta);
	down_border = atof(ogr1);
	top_border = atof(ogr2);

	////////////


}

double f(double x)
{
	double a, b, f;
	if (num1 == ex)
		a = exp(num2);
	if (num1 == iks)
		a = pow(x, num2);
	if (num1 == mines_iks)
		a = pow(-x, num2);
	if (num1 == mines_ex)
		a = -exp(num2);

	if ((num1 == 0) && (num2 < 0))
		num1 = DBL_EPSILON;

	if (num2 == ex)
		a = pow(num1, exp(1));
	if (num2 == iks)
		a = pow(num1, x);
	if (num2 == mines_iks)
		a = pow(num1, -x);

	if ((num1 != ex) &&
		(num1 != iks) &&
		(num1 != mines_iks) &&
		(num2 != ex) &&
		(num2 != iks) &&
		(num2 != mines_ex))
		a = pow(num1, num2);

	if ((trigonom[0] == 'c') && (trigonom[2] == 's'))
		b = cos(num3*x);
	if ((trigonom[0] == 's') && (trigonom[2] == 'n'))
		b = sin(num3*x);
	if ((trigonom[0] == 't') && (trigonom[1] == 'g'))
		b = tan(num3*x);
	if ((trigonom[0] == 'c') && (trigonom[2] == 'g'))
		b = (num3*x) / sin(num3*x);//ctg
	if ((trigonom[0] == 'a') && (trigonom[1] == 's') && (trigonom[3] == 'n'))
		b = asin(num3*x);
	if ((trigonom[0] == 'a') && (trigonom[3] == 's'))
		b = acos(num3*x);
	if ((trigonom[0] == 'a') && (trigonom[4] == 'g'))
		b = atan(num3*x);
	if ((trigonom[0] == 's') && (trigonom[1] == 'h'))
		b = sinh(num3*x);
	if ((trigonom[0] == 'c') && (trigonom[1] == 'h'))
		b = cosh(num3*x);
	if ((trigonom[0] == 't') && (trigonom[1] == 'h'))
		b = tanh(num3*x);


	if (znak1 == '+')
		f = a + b;
	if (znak1 == '-')
		f = a - b;
	if (znak1 == '*')
		f = a * b;
	if (znak1 == '/')
	{
		if (fabs(b) < DBL_EPSILON)
			b = DBL_EPSILON;
		f = a / b;;
	}
	if (znak2 == '+')
		f = f + num4;
	if (znak2 == '-')
		f = f - num4;
	if (znak2 == '*')
		f = f * num4;
	if (znak2 == '/')
	{
		if (fabs(num4) < DBL_EPSILON)
			b = DBL_EPSILON;
		f = f / num4;
	}
	funcCalcs++;

	return f;
}

void trapezoid_method()
{
	int mod = 1;
	funcCalcs = 0;
	double eps = 0.0005,       // точность
		s1,                      // результаты
		s;
	int    n = 10,               // начальное число отрезков разбиения
		k = 5,                       // на сколько увеличить число отрезков разбиения
		i = 0;
	s1 = trapezoidArea(n);

	do
	{
		s = s1;
		n = n + k;
		s1 = trapezoidArea(n);
		i++;
	} while (fabs(s1 - s) > eps);
	printf("Trapezoid precise meaning = %f\n", s1);
	printf("Iterations = %d, i= %d\n", funcCalcs, i);  //кол-во вычислений функции
}

void rectangle_method()
{
	funcCalcs = 0;
	double eps = 0.0005,   // точность
		s1,                  // результаты
		s;
	int    n = 10,           // начальное число отрезков разбиения
		k = 5,               // на сколько увеличить число отрезков разбиения
		i = 0;
	s1 = rectangleArea(n);

	do
	{
		s = s1;
		n = n + k;
		s1 = rectangleArea(n);
		i++;
	} while (fabs(s1 - s) > eps);
	printf("Rectangle precise meaning = %f\n", s1);
	printf("Iterations = %d, i= %d\n", funcCalcs, i);
}

double trapezoidArea(int n)
{
	int i;
	double sum = 0.0;
	double h = ((top_border - down_border) / (1.0 * n));
	for (i = 1; i < n; i++)
	{
		errno = f(down_border + h*i) * h;

		if ((errno == EDOM) || (errno == HUGE_VAL))
			sum += (f(down_border + h*i + DBL_EPSILON) * h);
		else
			sum += f(down_border + h*i) * h;
	}
	sum = sum + (((f(down_border) + f(top_border)) / 2.0) * h);
	return sum;
}

double rectangleArea(int n)
{
	double h, sum = 0.0;

	h = ((top_border - down_border) / (1.0 * n));


	for (int i = 1; i < n; i++)
	{
		errno = f(down_border + h*i + h / 2.0);
		if ((errno == EDOM) || (errno == HUGE_VAL))
			sum += f(down_border + h*i + DBL_EPSILON + h / 2.0);
		else
			sum += f(down_border + h*i + h / 2.0);
	}
	return (sum * h);
}
