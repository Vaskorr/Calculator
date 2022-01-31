#define M_STACK 100
#define M_STR 100
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/* Реализация стека. Мне кажется, есть смысл хранить элементы обоих
 * стеков как строки */

typedef struct stack{
    int top;
    char element[M_STACK][M_STR];
}stack;

void nullstr(char* str){
    int n = strlen(str);
    for(int i = 0; i < n; ++i){
        str[i] = '\0';
    }
}

int get_priority(char* op){
    /* Возможно для более сложных операций будет смысл сравнивать переданную строку if'ом */
    switch (op[0]) {
        case '+':
        case '-':
            return 1;
        case'*':
        case'/':
            return 2;
        case '^':
            return 3;
    }
}

int get_result(char* expression){
    char str[M_STR] = {0}; // Для считывания операндов из более чем одного символа
    stack stack_num; // Создаем стеки для чисел и операций
    stack stack_op;
    stack_op.top = 0;
    stack_num.top = 0;

    /* Начинаем проход по выражению, разбиваем составляющие на два стека */
    for(int i = 0; expression[i]; ++i){
        switch (expression[i]) {
            case '+':
            case '-':
            case '*':
            case '/':
                if(get_priority(stack_op.element[stack_op.top-1]) > get_priority(expression[i])){
                    int a = atoi(stack_num.element[stack_num.top-1]); // atoi - ф-я для перевода строки в число
                    int b = atoi(stack_num.element[stack_num.top-2]);
                    char op = stack_op.element[stack_op.top-1];
                    stack_num.top--;
                    stack_op.top--;
                    /* Буду благодарен, если придумаешь, как тут без вложенных switch */
                    switch(op){
                        case '+':
                            /* snprintf используется тут для обратного перевода числа в строку */
                            snprintf(stack_num.element[stack_num.top-1],M_STR, "%d", a+b);
                            break;
                        case '-':
                            snprintf(stack_num.element[stack_num.top-1],M_STR, "%d", a-b);
                            break;
                        case '*':
                            snprintf(stack_num.element[stack_num.top-1],M_STR, "%d", a*b);
                            break;
                        case '/':
                            snprintf(stack_num.element[stack_num.top-1],M_STR, "%d", a*b);
                            break;
                    }
                }
            case '(' :
                strcpy(stack_op.element[stack_op.top],expression[i]);
                stack_op.top++; //Добавляем операцию в стек
                if (strlen(str) > 0){
                    strcpy(stack_num.element[stack_num.top], str);
                    stack_num.top++; // Проверяем, был ли до этого операнд, если да, записываем
                    nullstr(str);
                }
                break;
            case ')' :
                /* Считаем последовательно все выражение в скобках */
                stack_num.top--;
                stack_op.top--;
                int a = atoi(stack_num.element[stack_num.top]);
                int b = atoi(stack_num.element[stack_num.top-1]);
                char op = stack_op.element[stack_op.top];
                while(op != '('){
                    /* Буду благодарен, если придумаешь, как тут без вложенных switch */
                    switch(op){
                        case '+':
                            /* snprintf используется тут для обратного перевода числа в строку */
                            snprintf(stack_num.element[stack_num.top-1],M_STR, "%d", a+b);
                            break;
                        case '-':
                            snprintf(stack_num.element[stack_num.top-1],M_STR, "%d", a-b);
                            break;
                        case '*':
                            snprintf(stack_num.element[stack_num.top-1],M_STR, "%d", a*b);
                            break;
                        case '/':
                            snprintf(stack_num.element[stack_num.top-1],M_STR, "%d", a*b);
                            break;
                        case '^':
                            snprintf(stack_num.element[stack_num.top-1],M_STR, "%d", pow(a,b));
                            break;
                    }
                    stack_num.top--;
                    stack_op.top--;
                    a = atoi(stack_num.element[stack_num.top]);
                    b = atoi(stack_num.element[stack_num.top-1]);
                    op = stack_op.element[stack_op.top];
                }
            default:
                str[strlen(str)-1] = expression[i]; // Сохраняем кусок операнда
                break;
        }
    }
    /* Вот тут возможно нужно еще раз пробежаться по стекам и посчитать все, пока
     * длина не станет равной 1 */
    return atoi(stack_num.element[0]);
}

int main() {
    char expression[] = {0};  // тут будет наше математическое выражение
    gets(expression);

    printf("%d", get_result(expression));
    return 0;
}

/*
 * И еще список выражений и констант:
 *
 * Operations list:
 * +
 * -
 * /
 * *
 * ^
 * sqrt
 * cos
 * sin
 * tg
 * log
 * ln
 * pow
 * abs
 * exp
 * real
 * imag
 * mag
 * phase
 *
 * Constants:
 * PI
 * e
 * j
 */

