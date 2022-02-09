#define M_STACK 100
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "functions.h"


/*
 * Calculator
 * authors (VK ID): maximdood, sveboo, astragor_dragociy, iuliia_kom, vaskorr
 */
                   // ------- новые апдейты сверху не пишем, меняем этот на свой ------- //
/*
 * Андреев & Sveboo's update:
 * + улучшено хранение чисел (сразу в комплексной форме)
 * + наложены ограничения на функции логарифмов, тангенса и деления
 *
 * - ломать и чинить, вдруг ещё не всё работает
 * - сделать адекватный вывод действий с использованием creal, cimag (сейчас он закомментирован)
 * - проверить, не нужны ли ограничения ещё где-то
 * - покомментить
 * - сделать ограничения функций менее неприличными
 */



// структура стека
typedef struct stack{
    int top;
    char element[M_STACK][M_STR];
}stack;


typedef struct cstack {
    int top;
    complex double element[M_STACK];
} cstack;


struct variable{
    char name[100];
    char expression[100];
};

// хранилище переменных
struct variable vars[30] = {0};


// инициализация констант
int init_const(struct variable *vars){
    char buf[M_STR] = {0};
    int count = 0;

    // PI
    strcpy(vars[count].name, "PI");
    sprintf(buf, "%f", M_PI);
    strcpy(vars[count].expression, buf);
    buf[0] = '\0';
    count++;

    // e
    strcpy(vars[count].name, "e");
    sprintf(buf, "%f", M_E);
    strcpy(vars[count].expression, buf);
    buf[0] = '\0';
    count++;

    return count;
}


double complex get_result(char* expression, int nvars){
    char str[M_STR] = {0}; // Для считывания операндов из более чем одного символа
    cstack stack_num = {0}; // Создаем стеки для чисел и операций
    stack stack_op = {0};
    stack_op.top = 0;
    stack_num.top = 0;
    int strsize = 1; // размер прочитанного операнда
    int f = 1; // флажок :)
    /* Начинаем проход по выражению, разбиваем составляющие на два стека */
    for(int i = 0; expression[i]; ++i){
        switch (expression[i]) {
            case '\n':
                // конец строки, выходим
                break;
            case '+':
            case '-':
            case '*':
            case '/':
            case '^':
            case ',':
                // ищем числовое значение операнда в списке переменных
                f = 1;
                if (strsize != 1){
                    for (int j = 0; j < nvars; ++j) {
                        if (!strcmp(vars[j].name, str)){
                            f = 0;
                            // тут мы рекурсивно находим значение операнда
                            stack_num.element[stack_num.top++] = get_result(vars[j].expression, nvars);
                            break;
                        }
                    }
                    // а если не нашли такой операнд, значит это число
                    if (f){
                        stack_num.element[stack_num.top++] = convert(str);
                    }
                    f = 1;
                    strsize = 1;
                }
                // начинаем выкидывать из стека операции с приоритетом не менее текущего
                for (int j = stack_op.top-1; j >= 0; --j) {
                    if (!stack_op.top){
                        break; // если вдруг стек пуст
                    }
                    if(get_priority(&stack_op.element[j][0]) >= get_priority(&expression[i])){
                        // printf("do %s%s%s\n", stack_num.element[stack_num.top-2],stack_op.element[j], stack_num.element[stack_num.top-1]);
                        // сразу считаем значение, извлекая операцию из стека
                        switch (stack_op.element[j][0]) {
                            case '+':
                                stack_num.element[stack_num.top-2] = add(stack_num.element[stack_num.top-2], stack_num.element[stack_num.top-1]);
                                break;
                            case '-':
                                if (stack_num.top == 1){
                                    stack_num.element[stack_num.top-1] = -stack_num.element[stack_num.top-1];
                                    stack_num.top++;
                                }else{
                                    stack_num.element[stack_num.top-2] = sub(stack_num.element[stack_num.top-2], stack_num.element[stack_num.top-1]);
                                }
                                break;
                            case '*':
                                stack_num.element[stack_num.top-2] = mult(stack_num.element[stack_num.top-2], stack_num.element[stack_num.top-1]);
                                break;
                            case '/':
                                stack_num.element[stack_num.top-2] = divide(stack_num.element[stack_num.top-2], stack_num.element[stack_num.top-1]);
                                break;
                            case '^':
                                stack_num.element[stack_num.top-2] = power(stack_num.element[stack_num.top-2], stack_num.element[stack_num.top-1]);
                                break;
                        }
                        stack_num.top--;
                        stack_op.top--;
                    } else{
                        break;
                    }
                }
                // закидываем операцию в стек с операциями
                if (expression[i] != ',') {
                    stack_op.element[stack_op.top][0] = expression[i];
                    stack_op.element[stack_op.top++][1] = '\0';
                }
                break;
            case '(' :
                str[strsize-1] = '(';
                str[strsize++] = '\0';
                strcpy(stack_op.element[stack_op.top++], str);
                strsize = 1;
                break;
            case ')' :
                f = 1;
                for (int j = 0; j < nvars; ++j) {
                    if (!strcmp(vars[j].name, str)){
                        f = 0;
                        // тут мы рекурсивно находим значение операнда
                        stack_num.element[stack_num.top++] = get_result(vars[j].expression, nvars);
                        break;
                    }
                }
                // а если не нашли такой операнд, значит это число
                if (f && strsize != 1){
                    stack_num.element[stack_num.top++] = convert(str);}
                f = 1;
                strsize = 1;
                for (int j = stack_op.top-1; j >= 0; --j) {
                    if (!stack_op.top || stack_op.element[j][strlen(stack_op.element[j])-1] == '('){
                        f = j;
                        break; // если вдруг стек пуст
                    }
                    // printf("do %s%s%s\n", stack_num.element[stack_num.top-2],stack_op.element[j], stack_num.element[stack_num.top-1]);
                    // сразу считаем значение, извлекая операцию из стека
                    switch (stack_op.element[j][0]) {
                        case '+':
                            stack_num.element[stack_num.top-2] = add(stack_num.element[stack_num.top-2], stack_num.element[stack_num.top-1]);
                            break;
                        case '-':
                            if (stack_num.top == 1){
                                stack_num.element[stack_num.top-1] = -stack_num.element[stack_num.top-1];
                                stack_num.top++;
                            }else{
                                stack_num.element[stack_num.top-2] = sub(stack_num.element[stack_num.top-2], stack_num.element[stack_num.top-1]);
                            }
                            break;
                        case '*':
                            stack_num.element[stack_num.top-2] = mult(stack_num.element[stack_num.top-2], stack_num.element[stack_num.top-1]);
                            break;
                        case '/':
                            stack_num.element[stack_num.top-2] = divide(stack_num.element[stack_num.top-2], stack_num.element[stack_num.top-1]);
                            break;
                        case '^':
                            stack_num.element[stack_num.top-2] = power(stack_num.element[stack_num.top-2], stack_num.element[stack_num.top-1]);
                            break;
                    }
                    stack_num.top--;
                    stack_op.top--;
                }
                // степень отдельно, т.к. 2 аргумента
                if (!strcmp(stack_op.element[f], "pow(")){
                    //printf("do pow(%s,%s)\n", stack_num.element[stack_num.top-2], stack_num.element[stack_num.top-1]);
                    stack_num.element[stack_num.top-2] = power(stack_num.element[stack_num.top-2], stack_num.element[stack_num.top-1]);
                    stack_num.top--;
                }

                else {
                    // уКаЗаТеЛь На ФуНкЦиЮ
                    double complex (*operation)(double complex) = findFunction(stack_op.element[f]); //проверяем: является ли элемент сложной ф-цией
                    if (operation) {
                        // printf("%s%s)\n", stack_op.element[f], stack_num.element[stack_num.top - 1]);
                        stack_num.element[stack_num.top - 1] = operation(stack_num.element[stack_num.top - 1]);
                    }
                }
                stack_op.top--;
                break;
            default:
                // Сохраняем кусок операнда
                str[strsize-1] = expression[i];
                str[strsize++] = '\0';
                break;
        }

    }

    // а теперь добиваем строку и стек
    if (strsize!= 1){
        for (int j = 0; j < nvars; ++j) {
            if (!strcmp(vars[j].name, str)){
                f = 0;
                stack_num.element[stack_num.top++] = get_result(vars[j].expression, nvars);
                break;
            }
        }
        if (f){
            stack_num.element[stack_num.top++] = convert(str);
        }
    }

    f = 1;
    if(stack_op.top){
        for (int i = stack_op.top-1; i >= 0; --i) {
            // добавляем операцию в стек
            // printf("do %s%s%s\n", stack_num.element[stack_num.top-2],stack_op.element[i], stack_num.element[stack_num.top-1]);
            switch (stack_op.element[i][0]) {
                case '+':
                    stack_num.element[stack_num.top-2] = add(stack_num.element[stack_num.top-2], stack_num.element[stack_num.top-1]);
                    break;
                case '-':
                    if (stack_num.top == 1){
                        stack_num.element[stack_num.top-1] = -stack_num.element[stack_num.top-1];
                        stack_num.top++;
                    }else{
                        stack_num.element[stack_num.top-2] = sub(stack_num.element[stack_num.top-2], stack_num.element[stack_num.top-1]);
                    }
                    break;
                case '*':
                    stack_num.element[stack_num.top-2] = mult(stack_num.element[stack_num.top-2], stack_num.element[stack_num.top-1]);
                    break;
                case '/':
                    stack_num.element[stack_num.top-2] = divide(stack_num.element[stack_num.top-2], stack_num.element[stack_num.top-1]);
                    break;
                case '^':
                    stack_num.element[stack_num.top-2] = power(stack_num.element[stack_num.top-2], stack_num.element[stack_num.top-1]);
                    break;
            }
            stack_num.top--;
            stack_op.top--;
        }
    }
    return stack_num.element[0];
}

int main() {
    FILE* input;
    input = fopen("input.txt", "r");
    char expression[50] = {0};  // тут будет наше математическое выражение
    fgets(expression, 100, input);
    printf("your expression: %s\nyour variables:\n", expression);
    /* Инициализируем константы */
    int i = init_const(vars);
    // добавляем переменные
    while (!feof(input)) {
        fscanf(input,"%s = %s", &vars[i].name, &vars[i].expression);
        printf("varname: %s\n value:  %s\n", vars[i].name, vars[i].expression);
        i++;
    }
    printf("\n");
    double complex answer = get_result(&expression, i);
    double rans = creal(answer), ians = cimag(answer);
    // форматирование
    if (fabs(ians) < 0.00001) {ians = 0;}
    if (fabs(rans) < 0.00001) {rans = 0;}
    if (ians > 0) { printf("answer: %.6lf + %.6lfi\n", rans, ians);}
    if (ians < 0) { printf("answer: %.6lf - %.6lfi\n", rans, fabs(ians));}
    if (ians== 0) { printf("answer: %.5lf\n", rans);}
    return 0;
}
