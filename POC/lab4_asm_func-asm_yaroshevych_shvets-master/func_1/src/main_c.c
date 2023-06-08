#include <stdio.h>
#include <stdlib.h>

void func(int32_t *array, size_t size);

int main() {
    size_t size = 15;
    int32_t *input = malloc(size * sizeof(int32_t));

    for (size_t i = 0; i < size; ++i) {
        input[i] = rand() % 100;
        if (rand() % 2 == 0) {
            input[i] *= -1;
        }
    }

    printf("Unsorted array: \n");
    for (size_t i = 0; i < size; ++i) {
        printf("%d ", input[i]);
    }
    printf("\n");

    func(input, size);

    printf("Sorted array: \n");
    for (int i = 0; i < size; ++i) {
        printf("%d ", input[i]);
    }
    printf("\n");

    free(input);
    return 0;
}
