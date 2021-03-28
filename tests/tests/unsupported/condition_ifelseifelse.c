int main() {

    int age = 18;
    int categorie;

    if (age < 18) {
        categorie = 1;
    } else if (age > 21) {
        categorie = 3;
    } else {
        categorie = 2;
    }

    return age;
}
