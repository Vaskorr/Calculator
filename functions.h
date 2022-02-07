#define M_STR 100
#include "complex.h"


char* global; // вспомогательная штука

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


// конвертация в нужную форму записи
char* convert(char* expression) {
    char* ret = malloc(sizeof expression);
    // i-ная форма
    if (!strcmp(expression, "j")) {return "0_1";}
    if (!strcmp(expression, "-j")) {return "0_-1";}
    if (expression[strlen(expression)-1] == 'j') {
        sprintf(ret, "0_%s", expression);
        ret[strlen(ret)-1] = '\0';
        return ret;
    }
    // норм форма
    if (strchr(expression, '_')) {
        sprintf(ret, "%s", expression);
        return ret;
    }
    // и наконец просто дробь
    sprintf(ret, "%s_0", expression);
    return ret;
}

// сложение
char* add(char arg1[M_STR], char arg2[M_STR]) {
    char* ret = malloc(sizeof global);
    memset(ret, 0, sizeof ret);
    strcpy(arg1, convert(arg1));
    strcpy(arg2, convert(arg2));
    double r1, i1, r2, i2;
    sscanf(arg1, "%lf_%lf", &r1, &i1);
    sscanf(arg2, "%lf_%lf", &r2, &i2);
    double complex c1 = r1 + i1*I;
    double complex c2 = r2 + i2*I;
    sprintf(ret, "%lf_%lf", crealf(c1+c2), cimagf(c1+c2));
    return ret;
}

// вычитание
char* sub(char arg1[M_STR], char arg2[M_STR]) {
    char* ret = malloc(sizeof global);
    strcpy(arg1, convert(arg1));
    strcpy(arg2, convert(arg2));
    double r1, i1, r2, i2;
    sscanf(arg1, "%lf_%lf", &r1, &i1);
    sscanf(arg2, "%lf_%lf", &r2, &i2);
    double complex c1 = r1 + i1*I;
    double complex c2 = r2 + i2*I;
    sprintf(ret, "%lf_%lf", crealf(c1-c2), cimagf(c1-c2));
    return ret;
}

// деление
char* divide(char arg1[M_STR], char arg2[M_STR]) {
    char* ret = malloc(sizeof global);
    strcpy(arg1, convert(arg1));
    strcpy(arg2, convert(arg2));
    double r1, i1, r2, i2;
    sscanf(arg1, "%lf_%lf", &r1, &i1);
    sscanf(arg2, "%lf_%lf", &r2, &i2);
    double complex c1 = r1 + i1*I;
    double complex c2 = r2 + i2*I;
    sprintf(ret, "%lf_%lf", crealf(c1/c2), cimagf(c1/c2));
    return ret;
}

// умножение
char* mult(char arg1[M_STR], char arg2[M_STR]) {
    char* ret = malloc(sizeof global);
    strcpy(arg1, convert(arg1));
    strcpy(arg2, convert(arg2));
    double r1, i1, r2, i2;
    sscanf(arg1, "%lf_%lf", &r1, &i1);
    sscanf(arg2, "%lf_%lf", &r2, &i2);
    double complex c1 = r1 + i1*I;
    double complex c2 = r2 + i2*I;
    sprintf(ret, "%lf_%lf", crealf(c1*c2), cimagf(c1*c2));
    return ret;
}

// возведение в степень
char* power(char arg1[M_STR], char arg2[M_STR]) {
    char* ret = malloc(sizeof global);
    strcpy(arg1, convert(arg1));
    strcpy(arg2, convert(arg2));
    double r1, i1, r2, i2;
    sscanf(arg1, "%lf_%lf", &r1, &i1);
    sscanf(arg2, "%lf_%lf", &r2, &i2);
    if (!i1 && !i2) {
        sprintf(ret, "%lf_0", pow(r1, r2));
        return ret;
    }
    double complex c1 = r1 + i1*I;
    double complex c2 = r2 + i2*I;
    sprintf(ret, "%lf_%lf", crealf(cpowf(c1, c2)), cimagf(cpowf(c1, c2)));
    return ret;
}

// унарный минус
char* unary_min(char arg1[M_STR]) {
    char* ret = malloc(sizeof global);
    strcpy(arg1, convert(arg1));
    double r1, i1;
    sscanf(arg1, "%lf_%lf", &r1, &i1);
    double complex c = r1 + i1*I;
    sprintf(ret, "%lf_%lf", crealf(-c), cimagf(-c));
    return ret;
}

// синус
char* cxsin(char arg1[M_STR]) {
    char* ret = malloc(sizeof global);
    strcpy(arg1, convert(arg1));
    double r1, i1;
    sscanf(arg1, "%lf_%lf", &r1, &i1);
    double complex c = csinf(r1 + i1*I);
    sprintf(ret, "%lf_%lf", crealf(c), cimagf(c));
    return ret;
}

// косинус
char* cxcos(char arg1[M_STR]) {
    char* ret = malloc(sizeof global);
    strcpy(arg1, convert(arg1));
    double r1, i1;
    sscanf(arg1, "%lf_%lf", &r1, &i1);
    double complex c = ccosf(r1 + i1*I);
    sprintf(ret, "%lf_%lf", crealf(c), cimagf(c));
    return ret;
}

// тангенс
char* cxtan(char arg1[M_STR]) {
    char* ret = malloc(sizeof global);
    strcpy(arg1, convert(arg1));
    double r1, i1;
    sscanf(arg1, "%lf_%lf", &r1, &i1);
    double complex c = ctanf(r1 + i1*I);
    sprintf(ret, "%lf_%lf", crealf(c), cimagf(c));
    return ret;
}

// квадратный корень
char* cxsqrt(char arg1[M_STR]) {
    char* ret = malloc(sizeof global);
    strcpy(arg1, convert(arg1));
    double r1, i1;
    sscanf(arg1, "%lf_%lf", &r1, &i1);
    if (!i1) {
        if (r1 >= 0) {sprintf(ret, "%lf_0", sqrt(r1));}
        if (r1 < 0) {sprintf(ret, "0_%lf", sqrt(-r1));}
        return ret;
    }
    double complex c = csqrtf(r1 + i1*I);
    sprintf(ret, "%lf_%lf", crealf(c), cimagf(c));
    return ret;
}

// натуральный логарифм
char* cxln(char arg1[M_STR]) {
    char* ret = malloc(sizeof global);
    strcpy(arg1, convert(arg1));
    double r1, i1;
    sscanf(arg1, "%lf_%lf", &r1, &i1);
    double complex c = clogf(r1 + i1*I);
    sprintf(ret, "%lf_%lf", crealf(c), cimagf(c));
    return ret;
}

// е в степени
char* cxexp(char arg1[M_STR]) {
    char* ret = malloc(sizeof global);
    strcpy(arg1, convert(arg1));
    double r1, i1;
    sscanf(arg1, "%lf_%lf", &r1, &i1);
    double complex c = cexpf(r1 + i1*I);
    sprintf(ret, "%lf_%lf", crealf(c), cimagf(c));
    return ret;
}

// модуль
char* cxabs(char arg1[M_STR]) {
    char* ret = malloc(sizeof global);
    strcpy(arg1, convert(arg1));
    double r1, i1;
    sscanf(arg1, "%lf_%lf", &r1, &i1);
    sprintf(ret, "%lf_0", sqrt(r1*r1+i1*i1));
    return ret;
}

// действительная часть
char* cxreal(char arg1[M_STR]) {
    char* ret = malloc(sizeof global);
    strcpy(arg1, convert(arg1));
    double r1, i1;
    sscanf(arg1, "%lf_%lf", &r1, &i1);
    sprintf(ret, "%lf_0", r1);
    return ret;
}

// мнимая часть
char* cximag(char arg1[M_STR]) {
    char* ret = malloc(sizeof global);
    strcpy(arg1, convert(arg1));
    double r1, i1;
    sscanf(arg1, "%lf_%lf", &r1, &i1);
    sprintf(ret, "0_%lf", i1);
    return ret;
}

// десятичный логарифм
char* cxlog10(char arg1[M_STR]) {
    char* ret = malloc(sizeof global);
    strcpy(arg1, convert(arg1));
    double r1, i1;
    sscanf(arg1, "%lf_%lf", &r1, &i1);
    double complex c = clogf(r1 + i1*I);
    c = c / log(10);
    sprintf(ret, "%lf_%lf", crealf(c), cimagf(c));
    return ret;
}

// фаза
char* cxphase(char arg1[M_STR]) {
    char* ret = malloc(sizeof global);
    strcpy(arg1, convert(arg1));
    double r1, i1;
    sscanf(arg1, "%lf_%lf", &r1, &i1);
    double complex c = cargf(r1 + i1*I);
    sprintf(ret, "%lf_%lf", crealf(c), cimagf(c));
    return ret;
}


//поиск сложной функции
char* findFunction(char *expression){
    if (!strcmp(expression, "sqrt(")){ return &cxsqrt;}
    if (!strcmp(expression, "sin(")){return &cxsin;}
    if (!strcmp(expression, "cos(")){return &cxcos;}
    if (!strcmp(expression, "tg(")){return &cxtan;}
    if (!strcmp(expression, "log(")){return &cxlog10;}
    if (!strcmp(expression, "ln(")){return &cxln;}
    if (!strcmp(expression, "abs(")){return &cxabs;}
    if (!strcmp(expression, "mag(")){return &cxabs;}
    if (!strcmp(expression, "exp(")){return &cxexp;}
    if (!strcmp(expression, "real(")){return &cxreal;}
    if (!strcmp(expression, "imag(")){return &cximag;}
    if (!strcmp(expression, "phase(")){return &cxphase;}
    return NULL;
}