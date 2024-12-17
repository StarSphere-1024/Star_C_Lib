#include "quick_io.h"

#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>

const double dten[10] = {0, 1e-1, 1e-2, 1e-3, 1e-4, 1e-5, 1e-6, 1e-7, 1e-8, 1e-9};
/// 一般题目至多要求输出小数点后6位，此数组足矣。

char outputbuf[15]; /// 声明成全局变量可以减小开销

/// @brief 读整数
/// @param x 存放读取的值
/// @note -0的输出为0
/// @return 若成功读到一个整数返回true，若读到EOF返回false
bool QIOGetInt(int *x)
{
    char c;
    while ((c = getchar()) != '-' && !isdigit(c))
        if (c == EOF)
            return false;
    bool neg = false;
    if (c == '-')
        *x = 0, neg = true;
    else
        *x = c & 15;
    while (isdigit(c = getchar()))
        *x = *x * 10 + (c & 15);
    if (neg)
        *x = -*x;
    return true;
}

/// @brief 读正负浮点、零（也可以读整数）并且可以判断".14"这样的输入
/// @param x 存放读取的值
/// @note -0.00这样的输入使得x=-0.000000
void QIOGetDouble(double *x)
{
    char c;
    while ((c = getchar()) != '-' && c != '.' && !isdigit(c))
        ;
    bool neg = false;
    if (c == '-')
    {
        neg = true;
        c = getchar();
    }
    *x = 0;
    if (c != '.')
    {
        // 整数部分
        *x = c & 15;
        while (isdigit(c = getchar()))
            *x = *x * 10 + (c & 15);
    }
    if (c == '.')
    {
        // 小数部分
        double ten = 1.0;
        while (isdigit(c = getchar()))
            *x += (c & 15) * (ten /= 10);
    }
    if (neg)
        *x = -*x;
}

/// @brief 输出整数
/// @param x 要输出的整数
void QIOPutInt(int x)
{
    if (x == 0)
        putchar('0');
    else
    {
        if (x < 0)
        {
            x = -x;
            putchar('-');
        }
        int p = 0;
        while (x)
        {
            outputbuf[p++] = x % 10;
            x /= 10;
        }
        for (int i = p - 1; i >= 0; i--)
            putchar('0' + outputbuf[i]); // 逆序输出
    }
}

static void unsigned_output(int x)
{
    if (x == 0)
        putchar('0');
    else
    {
        int p = 0;
        while (x)
        {
            outputbuf[p++] = x % 10;
            x /= 10;
        }
        for (int i = p - 1; i >= 0; i--)
            putchar('0' + outputbuf[i]); // 逆序输出
    }
}

/// @brief 输出精度为precision的浮点数(四舍五入)
/// @param precision 精度
/// @param x 要输出的浮点数
/// @note 对-0.0输出-0.000000 如果precision为0，则只输出离x最近的整数
void QIOPutDouble(int precision, double x)
{
    if (signbit(x)) // from <math.h>, return true if the sign of x is negative（就相当于返回x的符号位）
    {
        x = -x;
        putchar('-');
    }
    if (precision)
    {
        // 整数部分
        double intpart;
        x = modf(x, &intpart); // from <math.h>
        unsigned_output((int)intpart);
        // 小数部分
        putchar('.');
        for (int i = 1; i < precision && x < dten[i]; ++i)
            putchar('0'); // 输出小数点后有多少0
        int ten = 1;
        while (precision--)
            ten *= 10;
        unsigned_output((int)round(x * ten));
    }
    else
        unsigned_output((int)round(x));
}