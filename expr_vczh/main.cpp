#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <set>
#include <stack>
#include <string>
#include <algorithm>

using namespace std;

/* 支持的最大操作符数量 */
#define MAX_OP 100

/*
根据条件随机返回一个字符
acceptNumbers == true: 接受数字
acceptOperators == true: 接受操作符
exclude1: 过滤掉的第一个操作符
exclude2: 过滤掉的第二个操作符
*/
char GenerateComponent(bool acceptNumbers, bool acceptOperators, char exclude1, char exclude2)
{
    int count = 0;
    if (acceptNumbers) count += 10;
    if (acceptOperators) count += 4;
    if (exclude1) count -= 1;
    if (exclude2) count -= 1;

    int result = 0;
    int max = RAND_MAX - (RAND_MAX + 1) % count;
    while ((result = rand()) > max);
    result %= count;

    if (acceptNumbers)
    {
        if (result < 10)
        {
            return '0' + result;
        }
        result -= 10;
    }
    if (acceptOperators)
    {
        char ops[5] = { 0 };
        char* write = &ops[0];
        if (exclude1 != '+' && exclude2 != '+') *write++ = '+';
        if (exclude1 != '-' && exclude2 != '-') *write++ = '-';
        if (exclude1 != '*' && exclude2 != '*') *write++ = '*';
        if (exclude1 != '/' && exclude2 != '/') *write++ = '/';
        return ops[result];
    }
    throw "FUCK!";
}

struct ParseRec
{
    string expr;
    char op;
};

/*
后缀转中缀
*/
string Parse(const char* input)
{
    ParseRec recs[MAX_OP + 1];
    int recLen = 0;

    while (char c = *input++)
    {
        if ('0' <= c && c <= '9')
        {
            recs[recLen++] = { string(input - 1, input), c };
        }
        else
        {
            auto right = recs[--recLen];
            auto left = recs[--recLen];

            bool leftBrace = false;
            bool rightBrace = false;

            /* 判断左表达式是否需要加括号 */
            if ((c == '*' || c == '/') && (left.op == '+' || left.op == '-'))
            {
                leftBrace = true;
            }

            /* 判断右表达式是否需要加括号 */
            if (right.op < '0' || '9' < right.op)
            {
                switch (c)
                {
                case '+':
                    rightBrace = (right.op == '-');
                    break;
                case '-':
                    rightBrace = (right.op == '+' || right.op == '-');
                    break;
                case '*':
                    rightBrace = (right.op == '+' || right.op == '-' || right.op == '/');
                    break;
                case '/':
                    rightBrace = (right.op == '+' || right.op == '-' || right.op == '*' || right.op == '/');
                    break;
                }
            }

            /* 组成新的表达式 */
            string newExpr =
                (leftBrace ? "(" + left.expr + ")" : left.expr)
                + " " + string(&c, &c + 1) + " " +
                (rightBrace ? "(" + right.expr + ")" : right.expr);
            recs[recLen++] = { newExpr, c };
        }
    }
    return recs[0].expr;
}

struct TreeRec
{
    int length;
    int value;
};

/*
随机生成一个长度为maxLen（奇数）的后缀表达式。
对于两个不同的表达式，如果经过满足交换率和结合律的变化之后能够变一样，那么这两个表达式就算为相等：
例如：1+2 与 2+1相等，(1+2)+3 与 2+(1+3) 相等
但是：1-(2+3) 与 1-2-3 不想等
这个函数生成的后缀表达式会被归一化，也就是说，如果生成的两个表达式都相等，那么返回的字符串也想等
例如：1+2 与 2+1 都将返回12+，(1+2)+3 与 2+(1+3) 都将返回12+3+
*/
pair<string, int> GenerateExpression(int maxLen)
{
    /*
    数据结构说明:
    swap			：用于交换子表达式的临时缓冲区
    expr, exprLen	：生成过程中的后缀表达式
    tree, treeLen	：保存已经归一化的子表达式片段长度以及它的值

    例如在产生1*(2+3+1)的过程中，将往expr填入7个字符，所有变量的值如下所示：
    expr: 1				tree: {1,1}
    expr: 12			tree: {1,1} {1,2}
    expr: 123			tree: {1,1} {1,2} {1,3}
    expr: 123+			tree: {1,1} {3,5}
    expr: 123+1			tree: {1,1} {3,5}, {1,1}
    expr: 123+1+
    : 112+3+		tree: {1,1} {5,6}
    expr: 112+3+*		tree: {7,6}

    返回：{"112+3+*", 6}
    */
    char expr[MAX_OP * 2 + 2] = { 0 };
    TreeRec tree[MAX_OP * 2 + 1];
    int exprLen = 0;
    int treeLen = 0;

    int numberCount = 0;
    while (exprLen < maxLen)
    {
        if (treeLen < 2)
        {
            char c = GenerateComponent(true, false, 0, 0);
            expr[exprLen++] = c;
            tree[treeLen++] = { 1,c - '0' };
            numberCount++;
        }
        else
        {
            auto right = tree[treeLen - 1];
            auto left = tree[treeLen - 2];

            char* leftExpr = &expr[exprLen - right.length - left.length];
            char* rightExpr = &expr[exprLen - right.length];
            char leftOp = expr[exprLen - right.length - 1];
            char rightOp = expr[exprLen - 1];

            char exclude1 = 0;
            char exclude2 = 0;

            /*
            避免生成类似1+(2+3)这样的表达式，尽量左结合。
            */
            if (rightOp == '+' || rightOp == '*')
            {
                exclude1 = rightOp;
            }

            /* 防止除0 */
            if (right.value == 0)
            {
                exclude2 = '/';
            }

            char c = GenerateComponent(numberCount <= maxLen / 2, true, exclude1, exclude2);
            expr[exprLen++] = c;
            if ('0' <= c && c <= '9')
            {
                numberCount++;
                tree[treeLen++] = { 1,c - '0' };
            }
            else
            {
                /*
                针对交换律和结合律进行归一化。

                也就是说，无论是(1+2)*(3+4)还是(4+3)*(2+1)都将返回12+34+*。
                因为"12+"<"34+"，所以顺序将会被调整，把小的放在左边。
                因此在产生 (4+3)*(2+1) 的过程中
                完成43+就立即调整为34+
                完成21+就立即调整为12+
                完成34+12+*就立即调整为12+34+*

                需要注意的是，在产生+的情况下，如果两边的表达式分别是23+和1，
                那么在调整的时候需要把23+打碎成2和1，重新排序成1、2、3，然后处理成12+3+。
                如果直接比较字符串大小调整为123++，那么这不是一个尽可能左结合的表达式。
                1、2、3可能是子表达式，但是1不能是+表达式，不然就要继续分解下去。

                调整之后tree不用变，因为tree保留的是计算结果，在交换律下不变化
                */
                if (c == '+' || c == '*')
                {
                    /*
                    分割表达式。
                    如果输入的表达式为34*56*+12+，实际上就是3*4 + 5*6 + 1*2，
                    那么这个表达式应该分割为三个部分：3*4、5*6和1*2，
                    他们的字符串形式分别是34*、56*和12*。

                    在这里不需要判断3*4 + (5*6 + 1*2)的情况，因为第182行的处理已经避免了

                    最后对这几个字符串进行排序。这里字符串通过parts数组的成员来表达，引用了expr数组里面的数据。
                    */
                    struct { const char* start; int length; } parts[MAX_OP];
                    int partLen = 0;

                    const char* reading = &expr[exprLen - 1];
                    int remain = 1;
                    while (remain > 0)
                    {
                        if (*reading == c)
                        {
                            remain++;
                            reading--;
                        }
                        else
                        {
                            /*
                            从后往前搜索一个完整的表达式，譬如123*将得到23*。
                            方法很简单，只要找到的数字数量比表达式数量多1，立即停止。
                            */
                            const char* last = reading;
                            int expects = 1;
                            while (expects > 0)
                            {
                                char o = *reading--;
                                if (o == '+' || o == '-' || o == '*' || o == '/')
                                {
                                    expects++;
                                }
                                else
                                {
                                    expects--;
                                }
                            }

                            parts[partLen++] = { reading + 1,last - reading };
                            remain--;
                        }
                    }

                    /*
                    对字符串进行排序。
                    */
                    sort(&parts[0], &parts[partLen], [](auto a, auto b)
                    {
                        int result = strncmp(a.start, b.start, (a.length < b.length ? a.length : b.length));
                        if (result < 0) return true;
                        if (result > 0) return false;
                        return a.length < b.length;
                    });

                    /*
                    重新输出排序后的后缀表达式。
                    这里把结果写进swap是因为，parts引用了expr的数据，如果直接写进expr，那么parts数组的内容就乱掉
                    */
                    char swap[MAX_OP * 2 + 1] = { 0 };
                    char* writing = &swap[0];
                    for (int i = 0; i < partLen; i++)
                    {
                        auto part = parts[i];
                        memcpy(writing, part.start, part.length);
                        writing += part.length;
                        if (i > 0) *writing++ = c;
                    }

                    int total = writing - swap;
                    memcpy(expr + exprLen - total, swap, total);
                }

                /* 缓存表达式结果，用于避免除0 */
                int result = 0;
                switch (c)
                {
                case '+': result = left.value + right.value; break;
                case '-': result = left.value - right.value; break;
                case '*': result = left.value * right.value; break;
                case '/': result = left.value / right.value; break;
                }
                tree[--treeLen - 1] = { left.length + right.length + 1,result };
            }
        }
    }

    return{ string(expr, expr + exprLen),tree[0].value };
}

int main(int argc, char **argv)
{
    srand((unsigned)time(nullptr));
    int count = 0;
    int ops = 0;
    /* 如果要求的表达式的数量超过了操作符数量支持的所有可能将死循环 */
    cin >> count >> ops;

    set<pair<string, int>> exprs;
    for (int i = 0; i < count; i++)
    {
        while (true)
        {
            /*
            GenerateExpression保证了返回的后缀表达式都是归一化的，
            因此只需要比较两个字符串是否相等，就可以判断两个表达式是否相等。
            */
            auto expr = GenerateExpression(ops * 2 + 1);
            if (get<1>(exprs.insert(expr)))
            {
                break;
            }
        }
    }

    for (auto expr : exprs)
    {
        /* 调整输出格式，让人类阅读中缀表达式 */
        cout << get<1>(expr) << " = \t" << Parse(get<0>(expr).c_str()) << endl;
    }
    return 0;
}