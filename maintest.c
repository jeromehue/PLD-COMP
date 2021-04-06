int f1(int p1, int p2, int p3) {
        int a  =3;
        p1 = p1 + p2 + a + p3;
        return p1;
}

int main() 
{    
        int a  = 42, b =5;
        int c =3, d=4,e=6,f=8;
        a = f1(2, b, 4);
        return a;
}
