int main() {

    int age = 18;
    int categorie;
    int drink;

    if (age < 18) {
        categorie = 1;
    }

    if (age >= 18) {
        categorie = 2;
    }

    if (categorie == 1) {
        drink = 0;
    }

    if (!drink) {
        return 0;
    }

    return age;
}
