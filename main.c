#define M_STACK 100
#define M_STR 100
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


/*
 * Calculator
 * authors (VK ID): maximdood, sveboo, astragor_dragociy, iuliia_kom, vaskorr
 */
                   // ------- новые апдейты сверху не пишем, меняем этот на свой по правилам ниже ------- //
/*
 * Vaskorr's update:                                 <<- а теперь о том, как писать апдейты.
 * + пофикшен унарный минус                                  меняем эту строчку на (к примеру) Sveboo's update
 * + почищены комментарии и вывод
 * + пофикшены проблемы со скобками                  <<- тут указываем, какой вклад внесен этим апдейтом
 * + чувствую себя фиксиком
 *
 * - добавить остальные функции (готовые отмечены знаком "+" в самом конце файла)    <<- тут убираем только те строки, которые удалось решить апдейтом
 * - вопросы насчет pow. Нам ещё и запятую обрабатывать?...
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
struct variable vars[30] = {0};

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

int init_const(struct variable *vars){
    // Сюда константы
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
                f = 1;
                if (strsize != 1){
                    for (int j = 0; j < nvars; ++j) {
                        if (!strcmp(vars[j].name, str)){
                            f = 0;
                            // тут мы рекурсивно находим значение операнда
                            sprintf(stack_num.element[stack_num.top++], "%f", get_result(vars[j].expression, nvars));
                            break;
                        }
                    }
                    // а если не нашли такой операнд, значит это число
                    if (f){
                        strcpy(stack_num.element[stack_num.top++], str);
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
                        printf("do %s%s%s\n", stack_num.element[stack_num.top-2],stack_op.element[j], stack_num.element[stack_num.top-1]);
                        // сразу считаем значение, извлекая операцию из стека
                        switch (stack_op.element[j][0]) {
                            case '+':
                                sprintf(stack_num.element[stack_num.top-2], "%f", atof(stack_num.element[stack_num.top-2])+atof(stack_num.element[stack_num.top-1]));break;
                            case '-':
                                if (stack_num.top == 1){
                                    sprintf(stack_num.element[stack_num.top-1], "%f", 0-atof(stack_num.element[stack_num.top-1]));
                                    stack_num.top++;
                                }else{sprintf(stack_num.element[stack_num.top-2], "%f", atof(stack_num.element[stack_num.top-2])-atof(stack_num.element[stack_num.top-1]));}
                                break;
                            case '*':
                                sprintf(stack_num.element[stack_num.top-2], "%f", atof(stack_num.element[stack_num.top-2])*atof(stack_num.element[stack_num.top-1]));break;
                            case '/':
                                sprintf(stack_num.element[stack_num.top-2], "%f", atof(stack_num.element[stack_num.top-2])/atof(stack_num.element[stack_num.top-1]));break;
                            case '^':
                                sprintf(stack_num.element[stack_num.top-2], "%f", pow(atof(stack_num.element[stack_num.top-2]), atof(stack_num.element[stack_num.top-1])));
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
            case ',':
                // ищем числовое значение операнда в списке переменных
                f = 1;
                if (strsize != 1){
                    for (int j = 0; j < nvars; ++j) {
                        if (!strcmp(vars[j].name, str)){
                            f = 0;
                            // тут мы рекурсивно находим значение операнда
                            sprintf(stack_num.element[stack_num.top++], "%f", get_result(vars[j].expression, nvars));
                            break;
                        }
                    }
                    // а если не нашли такой операнд, значит это число
                    if (f){
                        strcpy(stack_num.element[stack_num.top++], str);
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
                        printf("do %s%s%s\n", stack_num.element[stack_num.top-2],stack_op.element[j], stack_num.element[stack_num.top-1]);
                        // сразу считаем значение, извлекая операцию из стека
                        switch (stack_op.element[j][0]) {
                            case '+':
                                sprintf(stack_num.element[stack_num.top-2], "%f", atof(stack_num.element[stack_num.top-2])+atof(stack_num.element[stack_num.top-1])); break;
                            case '-':
                                if (stack_num.top == 1){
                                    sprintf(stack_num.element[stack_num.top-1], "%f", 0-atof(stack_num.element[stack_num.top-1]));
                                    stack_num.top++;
                                }else{sprintf(stack_num.element[stack_num.top-2], "%f", atof(stack_num.element[stack_num.top-2])-atof(stack_num.element[stack_num.top-1]));}
                                break;
                            case '*':
                                sprintf(stack_num.element[stack_num.top-2], "%f", atof(stack_num.element[stack_num.top-2])*atof(stack_num.element[stack_num.top-1])); break;
                            case '/':
                                sprintf(stack_num.element[stack_num.top-2], "%f", atof(stack_num.element[stack_num.top-2])/atof(stack_num.element[stack_num.top-1])); break;
                            case '^':
                                sprintf(stack_num.element[stack_num.top-2], "%f", pow(atof(stack_num.element[stack_num.top-2]), atof(stack_num.element[stack_num.top-1])));
                                break;
                        }
                        stack_num.top--;
                        stack_op.top--;
                    } else{
                        break;
                    }
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
                        sprintf(stack_num.element[stack_num.top++], "%f", get_result(vars[j].expression, nvars));
                        break;
                    }
                }
                // а если не нашли такой операнд, значит это число
                if (f && strsize != 1){ strcpy(stack_num.element[stack_num.top++], str);}
                f = 1;
                strsize = 1;
                for (int j = stack_op.top-1; j >= 0; --j) {
                    if (!stack_op.top || stack_op.element[j][strlen(stack_op.element[j])-1] == '('){
                        f = j;
                        break; // если вдруг стек пуст
                    }
                    printf("do %s%s%s\n", stack_num.element[stack_num.top-2],stack_op.element[j], stack_num.element[stack_num.top-1]);
                    // сразу считаем значение, извлекая операцию из стека
                    switch (stack_op.element[j][0]) {
                        case '+':
                            sprintf(stack_num.element[stack_num.top-2], "%f", atof(stack_num.element[stack_num.top-2])+atof(stack_num.element[stack_num.top-1]));break;
                        case '-':
                            if (stack_num.top == 1){
                                sprintf(stack_num.element[stack_num.top-1], "%f", 0-atof(stack_num.element[stack_num.top-1]));
                                stack_num.top++;
                            }else{sprintf(stack_num.element[stack_num.top-2], "%f", atof(stack_num.element[stack_num.top-2])-atof(stack_num.element[stack_num.top-1]));}
                            break;
                        case '*':
                            sprintf(stack_num.element[stack_num.top-2], "%f", atof(stack_num.element[stack_num.top-2])*atof(stack_num.element[stack_num.top-1]));break;
                        case '/':
                            sprintf(stack_num.element[stack_num.top-2], "%f", atof(stack_num.element[stack_num.top-2])/atof(stack_num.element[stack_num.top-1]));break;
                        case '^':
                            sprintf(stack_num.element[stack_num.top-2], "%f", pow(atof(stack_num.element[stack_num.top-2]), atof(stack_num.element[stack_num.top-1])));
                            break;
                    }
                    stack_num.top--;
                    stack_op.top--;
                }
                // и вот сюда вот дописываем сложные функции
                if (!strcmp(stack_op.element[f], "sqrt(")){
                    printf("do sqrt(%s)\n", stack_num.element[stack_num.top-1]);
                    sprintf(stack_num.element[stack_num.top-1], "%f", sqrt(atof(stack_num.element[stack_num.top-1])));
                }
                if (!strcmp(stack_op.element[f], "sin(")){
                    printf("do sin(%s)\n", stack_num.element[stack_num.top-1]);
                    sprintf(stack_num.element[stack_num.top-1], "%f", sin(atof(stack_num.element[stack_num.top-1])));
                }
                if (!strcmp(stack_op.element[f], "cos(")){
                    printf("do cos(%s)\n", stack_num.element[stack_num.top-1]);
                    sprintf(stack_num.element[stack_num.top-1], "%f", cos(atof(stack_num.element[stack_num.top-1])));
                }
                if (!strcmp(stack_op.element[f], "tg(")){
                    printf("do tg(%s)\n", stack_num.element[stack_num.top-1]);
                    sprintf(stack_num.element[stack_num.top-1], "%f", tan(atof(stack_num.element[stack_num.top-1])));
                }
                if (!strcmp(stack_op.element[f], "log(")){
                    printf("do log(%s)\n", stack_num.element[stack_num.top-1]);
                    sprintf(stack_num.element[stack_num.top-1], "%f", log10(atof(stack_num.element[stack_num.top-1])));
                }
                if (!strcmp(stack_op.element[f], "ln(")){
                    printf("do ln(%s)\n", stack_num.element[stack_num.top-1]);
                    sprintf(stack_num.element[stack_num.top-1], "%f", log(atof(stack_num.element[stack_num.top-1])));
                }
                if (!strcmp(stack_op.element[f], "pow(")){
                    printf("do pow(%s,%s)\n", stack_num.element[stack_num.top-2], stack_num.element[stack_num.top-1]);
                    sprintf(stack_num.element[stack_num.top-1], "%f", pow(atof(stack_num.element[stack_num.top-2]), atof(stack_num.element[stack_num.top-1])));
                }
                if (!strcmp(stack_op.element[f], "abs(")){
                    printf("do abs(%s)\n", stack_num.element[stack_num.top-1]);
                    sprintf(stack_num.element[stack_num.top-1], "%f", fabs(atof(stack_num.element[stack_num.top-1])));
                }
                if (!strcmp(stack_op.element[f], "exp(")){
                    printf("do exp(%s)\n", stack_num.element[stack_num.top-1]);
                    sprintf(stack_num.element[stack_num.top-1], "%f", exp(atof(stack_num.element[stack_num.top-1])));
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
                sprintf(stack_num.element[stack_num.top++], "%f", get_result(vars[j].expression, nvars));
                break;
            }
        }
        if (f){strcpy(stack_num.element[stack_num.top++], str);}
    }

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
                    if (stack_num.top == 1){
                        sprintf(stack_num.element[stack_num.top-1], "%f", 0-atof(stack_num.element[stack_num.top-1]));
                        stack_num.top++;
                    }else{sprintf(stack_num.element[stack_num.top-2], "%f", atof(stack_num.element[stack_num.top-2])-atof(stack_num.element[stack_num.top-1]));}
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
    /* Инициируем константы */
    int i = init_const(vars);
    // добавляем переменные
    while (!feof(input)) {
        fscanf(input,"%s = %s", &vars[i].name, &vars[i].expression);
        printf("varname: %s\n value:  %s\n", vars[i].name, vars[i].expression);
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
 * +       +
 * -       +
 * /       +
 * *       +
 * ^       +
 * sqrt    +
 * cos     +
 * sin     +
 * tg      +
 * log     +
 * ln      +
 * pow     +
 * abs     +
 * exp     +
 * real
 * imag
 * mag
 * phase
 */