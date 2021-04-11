void display(char c) {
        putchar(c);
}

int main() {
        int  a, b, c;
        int tab[] = {0,1,2,3,4};
        int hello[] = {'H', 'e', 'l', 'l', 'o', ',',
                   ' ','W', 'o', 'r', 'l', 'd','!'};
        int length = 13;
        a = 0;
        b = 1+2+3 + tab[3];
    
        while(a < length) {
                display(hello[a]);
                a = a+1;
        }

        return b;
}
