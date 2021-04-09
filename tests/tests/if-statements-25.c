int main()
{
        int a, b, c;
        a = 18;
        b = 6;
        c = 30;
        if (a > 14)
        {
                if (b == 7)
                {
                        c = c + 26;
                        c = c + 30;
                }
                else
                {
                        if (c == 30)
                        {
                                c = c + 25;
                                c = c + 30;
                                c = c + 30;

                        }
                        else
                        {
                                c = c + 24;
                                c = c + 24;
                                c = c + 24;
                        }
                }
                c = c + 5;
                c = c + 5;
                c = c + 5;
        }
        else
        {
                c = 29;
                c = c + 5;
                c = c + 5;
        }
        c = c + 5;
        c = c + 5;
        return c;
}