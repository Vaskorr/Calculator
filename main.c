#define M_STACK 100
#define M_STR 100
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/*
 * Calculator
 * authors (VK ID): maximdood, sveboo, astragor_dragociy, iuliia_kom, vaskorr
 */

/*
 * Sveboo's и совсем чуть-чуть Андреев update:
 * + Добавлен унарный минус
 *
 * - Доделать скобки
 * - сделать сложные операции
 * - стек будет пробовать сложить комплексные числа, например 1+0.3j, надо как-то это обработать
 */

// структура стека
typedef struct stack{
    int top;
    char element[M_STACK][M_STR];
}stack;

struct variable{
    char name[100];
    char expression[100];
};

// хранилище переменных
struct variable vars[10] = {0};

// получение приоритета функций
int get_priority(char* op){
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
    return 0;
}

double get_result(char* expression, int nvars){
    char str[M_STR] = {0}; // Для считывания операндов из более чем одного символа
    stack stack_num = {0}; // Создаем стеки для чисел и операций
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
                // ищем числовое значение операнда в списке переменных
                for (int j = 0; j < nvars; ++j) {
                    if (!strcmp(vars[j].name, str)){
                        f = 0;
                        // тут мы рекурсивно находим значение операнда
                        sprintf(stack_num.element[stack_num.top++], "%f", get_result(vars[j].expression, nvars));
                        break;
                    }
                }
                // а если не нашли такой операнд, значит это число
                if (f){ strcpy(stack_num.element[stack_num.top++], str);}
                f = 1;
                strsize = 1;
                // начинаем выкидывать из стека операции с приоритетом не менее текущего
                for (int j = stack_op.top-1; j >= 0; --j) {
                    if (!stack_op.top){
                        break; // если вдруг стек пуст
                    }
                    if(get_priority(&stack_op.element[j][0]) >= get_priority(&expression[i])){
                        // сразу считаем значение, извлекая операцию из стека
                        switch (stack_op.element[j][0]) {
                            case '+':
                                sprintf(stack_num.element[stack_num.top-2], "%f", atof(stack_num.element[stack_num.top-2])+atof(stack_num.element[stack_num.top-1]));                                break;
                            case '-':
                                sprintf(stack_num.element[stack_num.top-2], "%f", atof(stack_num.element[stack_num.top-2])-atof(stack_num.element[stack_num.top-1]));                                break;
                            case '*':
                                sprintf(stack_num.element[stack_num.top-2], "%f", atof(stack_num.element[stack_num.top-2])*atof(stack_num.element[stack_num.top-1]));                                break;
                            case '/':
                                sprintf(stack_num.element[stack_num.top-2], "%f", atof(stack_num.element[stack_num.top-2])/atof(stack_num.element[stack_num.top-1]));                                break;
                            case '^':
                                sprintf(stack_num.element[stack_num.top-2], "%f", pow(atof(stack_num.element[stack_num.top-2]), atof(stack_num.element[stack_num.top-1])));                    break;
                                break;
                        }
                        stack_num.top--;
                        stack_op.top--;
                    } else{
                        break;
                    }
                }
                // закидываем операцию в стек с операциями
                stack_op.element[stack_op.top++][0] = expression[i];
                break;
            case '(' :
                // остается дописать скобочки
                break;
            case ')' :
                // и эти тоже
                break;
            default:
                // Сохраняем кусок операнда
                str[strsize-1] = expression[i];
                str[strsize++] = '\0';
                break;
        }
    }

    // а теперь добиваем строку и стек
    for (int j = 0; j < nvars; ++j) {
        if (!strcmp(vars[j].name, str)){
            f = 0;
            sprintf(stack_num.element[stack_num.top++], "%f", get_result(vars[j].expression, nvars));
            break;
        }
    }
    if (f){strcpy(stack_num.element[stack_num.top++], str);}
    f = 1;
    if(stack_op.top){
        for (int i = stack_op.top-1; i >= 0; --i) {
            // добавляем операцию в стек
            printf("do %s%s%s\n", stack_num.element[stack_num.top-2],stack_op.element[i], stack_num.element[stack_num.top-1]);
            switch (stack_op.element[i][0]) {
                case '+':
                    sprintf(stack_num.element[stack_num.top-2], "%f", atof(stack_num.element[stack_num.top-2])+atof(stack_num.element[stack_num.top-1]));
                    break;
                case '-':
                    sprintf(stack_num.element[stack_num.top-2], "%f", atof(stack_num.element[stack_num.top-2])-atof(stack_num.element[stack_num.top-1]));
                    break;
                case '*':
                    sprintf(stack_num.element[stack_num.top-2], "%f", atof(stack_num.element[stack_num.top-2])*atof(stack_num.element[stack_num.top-1]));
                    break;
                case '/':
                    sprintf(stack_num.element[stack_num.top-2], "%f", atof(stack_num.element[stack_num.top-2])/atof(stack_num.element[stack_num.top-1]));
                    break;
                case '^':
                    sprintf(stack_num.element[stack_num.top-2], "%f", pow(atof(stack_num.element[stack_num.top-2]), atof(stack_num.element[stack_num.top-1])));                    break;
            }
            stack_num.top--;
            stack_op.top--;
        }
    }
    return atof(stack_num.element[0]);
}

int main() {
    FILE* input;
    input = fopen("input.txt", "r");
    char expression[50] = {0};  // тут будет наше математическое выражение
    fgets(expression, 100, input);
    printf("your expression: %s\nyour variables:\n", expression);
    int i = 0;
    // добавляем переменные
    while (!feof(input)) {
        fscanf(input,"%s = %s", &vars[i].name, &vars[i].expression);
        printf("varname: %s\n value: %s\n", vars[i].name, vars[i].expression);
        i++;
    }
    printf("\n");
    printf("\n\n\nanswer: %f\n\n\n", get_result(&expression, i));
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