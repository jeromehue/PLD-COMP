int main()
{
        int a[1] = {1};
        int b[3] = {1, 2, 3};
        int c[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        int index = 4;
        c[index] = 99;
        int element = c[index];
        return element;
}
