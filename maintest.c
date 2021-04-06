int f1(int p1, int p2, int p3) {
        p1 = p1+p2+p3;
        return p1;
}

int f2(int a, int b) {
        a = a*b;
        return a;
}

int main() 
{    
        int a, b; 
        a = f1(2, 4, 4);
        b = f2(3);
        a = a+b;
        return a;
}
