#include <stdio.h>

int main()
{
        int a, b, c, d, e;
        a = 55;
        b = 18;
        c = 18;
        d = 25;
        e = 8;
        if (a < 50)
        {
                if (b > 10)
                {
                        c = 10;
                }
                else
                {
                        if (d == 29)
                        {
                                c = 20;
                        }
                }
        }
        else
        {
                if (b < 15)
                {
                        a = 58;
                        if (d > 14)
                        {
                                if (e > 50)
                                {
                                        c = 40;
                                }
                                if (d > 18)
                                {
                                        c = 50;
                                }
                                c = c + 1;
                                c = c + d;
                        }
                }
        }
        return c;
}