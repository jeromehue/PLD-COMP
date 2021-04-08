int main() {
    int a, c;
    a=2;
    c=7;
    if(a>6)
    {
        c=5;
    }
    else
    {
        c=8;
    }
    a=a+6;
    if(c>6)
    {
        c= c + 24;
    }
    else
    {
        c= c + 1;
    }
    if(a>8)
    {
        c=c+12;
    }
    else
    {
        c=c+1;
    }
    c = c+ 5;
    return c;
}