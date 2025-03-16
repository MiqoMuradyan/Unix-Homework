#include <stdio.h>
#include <stdlib.h>

void triangleArea() {
    float base;
    float height;
    printf("Input the params for a Triangle \n");
    printf("Base: ");
    scanf("%f", &base);

    printf("Height: ");
    scanf("%f", &height);

    float area = 0.5 * base * height;
    printf("triangle Area: %.2f\n", area);
}

int main() {
    triangleArea();
    return 0;
}