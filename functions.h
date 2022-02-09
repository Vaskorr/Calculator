#define M_STR 100
#define PRECISION 0.000001 // не трогать бл*н
#include <complex.h>
#include <math.h>
#include <stdlib.h>


// получение приоритета функций
int get_priority(char* op){
    switch (op[0]) {
        case '+':
        case '-':
        case ',':
            return 1;
        case'*':
        case'/':
            return 2;
        case '^':
            return 3;
    }
    return 0;
}

int equals(double complex a, double complex b) {
    return fabs(creal(a)-creal(b)) < PRECISION &&
           fabs(cimag(a)-cimag(b)) < PRECISION;
}


double complex add(double complex a, double complex b) {
    return a+b;
}


double complex sub(double complex a, double complex b) {
    return a-b;
}


double complex divide(double complex a, double complex b) {
    if (equals(b, 0)) {
        printf("Can't divide by zero");
        exit(2);}
    return a/b;
}


double complex mult(double complex a, double complex b) {
    return a*b;
}


double complex power(double complex a, double complex b) {
    return cpow(a, b);
}


double complex cxsin(double complex a) {
    return csin(a);
}


double complex cxcos(double complex a) {
    return ccos(a);
}


double complex cxtan(double complex a) {
    if (equals(ccos(a), 0)) {
        printf("Tangent of %lf + %lfi is undefined", creal(a), cimag(a));
        exit(2);
    }
    return ctan(a);
}


double complex cxsqrt(double complex a) {
    /* if (equals(cimag(a), 0)) {
        if (equals(creal(a), 0) || creal(a) > 0) {
            return sqrt(creal(a));
        } else{return sqrt(creal(a))*I;}
    }
     */
    return csqrt(a);
}


double complex cxabs(double complex a) {
    return cabs(a);
}


double complex cxexp(double complex a) {
    return cexp(a);
}


double complex cxln(double complex a) {
    if (equals(a, 0)) {
        printf("Log with zero argument is undefined");
        exit(2);
    }
    return clog(a);
}


double complex cxlog10(double complex a) {
    if (equals(a, 0)) {
        printf("Log with zero argument is undefined");
        exit(2);
    }
    return clog(a) / clog(10);
}


double complex cxphase(double complex a) {
    return carg(a);
}


//поиск сложной функции
complex double* findFunction(char *expression){
    if (!strcmp(expression, "sqrt(")){ return (complex double *) &cxsqrt;}
    if (!strcmp(expression, "sin(")){return &cxsin;}
    if (!strcmp(expression, "cos(")){return &cxcos;}
    if (!strcmp(expression, "tg(")){return &cxtan;}
    if (!strcmp(expression, "log(")){return &cxlog10;}
    if (!strcmp(expression, "ln(")){return &cxln;}
    if (!strcmp(expression, "abs(")){return &cxabs;}
    if (!strcmp(expression, "mag(")){return &cxabs;}
    if (!strcmp(expression, "exp(")){return &cxexp;}
    if (!strcmp(expression, "real(")){return &creal;}
    if (!strcmp(expression, "imag(")){return &cimag;}
    if (!strcmp(expression, "phase(")){return &cxphase;}
    return NULL;
}


double complex convert(char* str) {
    double i;
    if (!strcmp("j", str)) {return I;}
    if (!strcmp("-j", str)) {return -1*I;}
    if (str[strlen(str)-1] == 'j') {
        sscanf(str, "%lfj", &i);
        return i*I;
    }
    sscanf(str, "%lf", &i);
    return i + 0*I;
}

