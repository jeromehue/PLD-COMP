int main()
{
        int a, b, c, d, e;
        a = 9;
        b = 2;
        c = 4;
        e = 20;
        if (a < 10)
        {
                if (b > 5)
                {
                        if (c == 4)
                        {
                                d = 1;
                        }
                        else
                        {
                                d = 2;
                        }
                        e = 6;
                }
                else
                {
                        d = 3;
                }
                e = 26;
                if (d == 2)
                {
                        d = d + e;
                }
                else
                {
                        d = 10;
                }

                e = 4;
        }
        else
        {
                d = 4;
                if (e > 4)
                {
                        if (b < 23)
                        {
                                d = 13032;
                        }
                        else
                        {
                                d = 456;
                        }
                        e = e + 5;
                }
                else
                {
                        d = d + 2 * e;
                }
        }
        e = e + 6;
        return d;
}
