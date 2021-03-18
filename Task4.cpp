#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
using namespace std;
template <typename T>

struct list
{
    T id;
    list* next;
    int val = 1;
};
template <typename T>
void push(list<T>*& ptr, T x)
{
    list<T>* tmp = new list<T>;
    (*tmp).id = x;
    tmp->next = ptr;
    ptr = tmp;
}
template <typename T>
void print_list(list<T>* ptr)
{
    if (ptr->val != 0)
    {
        cout << ptr->id << " -> ";
        print_list(ptr->next);
    }
}
template <typename T>
T pop(list<T>*& ptr)
{
    list<T>* tmp = ptr;
    T x = tmp->id;
    ptr = ptr->next;
    delete(tmp);
    return(x);
}
int priority(char x) {
    if (x == '=') return 1;
    if (x == '(') return 2;
    if (x == '+' || x == '-') return 3;
    if (x == '*' || x == '/') return 4;
    return -1;
}
int operate(list<char>*& lst1, list<double>*& lst, string& s) {
    char op = pop(lst1);
    s += op;
    s += " ";
    switch (op) {
    case('+'):
        push(lst, pop(lst) + pop(lst));
        break;
    case('-'):
        push(lst, -(pop(lst) - pop(lst)));
        break;
    case('*'):
        push(lst, pop(lst) * pop(lst));
        break;
    case('/'):
        double x = pop(lst);
        double y = pop(lst);
        if (x != 0) push(lst, y / x);
        else {
            std::cout << "Division by 0\n-----\n";
            return 0;

        }
        break;
    }
    return -1;
}
int main()
{
    ifstream f("input.txt");
    if (!f) {
        std::cout << "File not open";
        exit(1);
    }
    list<double>* stackX = NULL;
    list<char>* stackY = NULL;

    string str;
    std::getline(f, str);
    std::cout << str << "\n";
    size_t i = 0;         
    double x;
    int sgn = 1;            
    string s;             
    while (i < str.size()) {
        if (str[i] == ' ' || str[i] == '\t') {
            i++;
            continue;
        }
        if (isdigit(str[i])) {
            size_t d = 0;
            const char* pBeg = str.c_str() + i;            
            char* pEnd;
            x = strtod(pBeg, &pEnd);
            d = pEnd - pBeg;
            push(stackX, sgn * x);
            if (sgn == -1) s += '-';
            s += str.substr(i, d) + ' ';
            i += d;
            sgn = 1;

        }
        else {
            if (str[i] == ')') {
                while (stackY != NULL && stackY->id != '(') {
                    if (operate(stackY, stackX, s) == 0)
                        return 0;
                }
                pop(stackY);
                i++;
            }
            else {
                if (str[i] == '=') {
                    while (stackY != NULL) {
                        if (operate(stackY, stackX, s) == 0)
                            return 0;
                    }
                    i++;
                }
                else {
                    int t = priority(str[i]);
                    if (t == 2) {
                        push(stackY, str[i]);
                        i++;
                    }
                    else {
                        if ((str[i] == '-' && i == 0) || (str[i] == '-' && str[i - 1] == '(')) sgn = -1;
                        else {
                            while (stackY != NULL && t <= priority(stackY->id)) {
                                operate(stackY, stackX, s);
                            }
                            push(stackY, str[i]);

                        }
                        i++;
                    }
                }
            }
        }
    }
    std::cout << "\n" << s << "=";
    std::cout <<  stackX->id << "\n";
    return 0;
}
