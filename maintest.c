int f1(int p1, int p2 ) {
        int a  = 3;
        p1 = p1 + p2 + a;
        return p1;
}

int main() 
{    
        int a  = 42;
        a = f1(2, 5);
        return a;
}
