int main() {
        int a = 54 | 87;
        int b = a & 63;
        int c = a ^ b + 1;
        c = a ^ b & c | a;
        return c;
}