#include <stdio.h>
#include <stdlib.h>

void func(float *a, float *b, float *x, size_t size);

int main() {
    size_t size = 15;
    float *a_coef = malloc(size * sizeof(float));
    float *b_coef = malloc(size * sizeof(float));
    float *result = malloc(size * sizeof(float));

    for (int i = 0; i < size; ++i) {
        a_coef[i] = (float) rand() / (float) (RAND_MAX / 1000);
        b_coef[i] = i + 1;
    }

    func(a_coef, b_coef, result, size);

    for (int i = 0; i < size; ++i) {
        printf("%f*x + %f = 0,  x = %f\n", a_coef[i], b_coef[i], result[i]);
    }

    free(a_coef);
    free(b_coef);
    free(result);
    return 0;
}
