#ifndef FUNCTIONS_H
#define FUNCTIONS_H


// вывод числа
void print_n(double complex number);
// получение приоритета функций
int get_priority(const char* op);
// сравнение с точностью
int equals(double complex a, double complex b);
// вычислительные функции
double complex add(double complex a, double complex b);
double complex sub(double complex a, double complex b);
double complex divide(double complex a, double complex b);
double complex mult(double complex a, double complex b);
double complex power(double complex a, double complex b);
double complex cxsin(double complex a);
double complex cxcos(double complex a);
double complex cxtan(double complex a);
double complex cxsqrt(double complex a);
double complex cxabs(double complex a);
double complex cxexp(double complex a);
double complex cxln(double complex a);
double complex cxlog10(double complex a);
double complex cxphase(double complex a);
// поиск сложной функции
complex double* findFunction(char *expression);
// приведение строки к числу
double complex convert(char* str);
#endif