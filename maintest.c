int main() {
        int tab[5] = {1,2,3,4,5};
        int index = 2;
        int a;
        int c = tab[index] = 23;
        int d = a  = tab[index];
        return c + d;
}
