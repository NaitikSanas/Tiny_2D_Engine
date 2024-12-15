#ifndef VECTOR_MATH_LIBRARY_H
#define VECTOR_MATH_LIBRARY_H

#include <stddef.h> // For size_t

// Define a structure to represent a vector
typedef struct {
    float *components;
    size_t size;
} Vector;

// Function declarations
Vector create_vector(size_t size);
void free_vector(Vector *v);

Vector add_vectors(const Vector *v1, const Vector *v2);
Vector subtract_vectors(const Vector *v1, const Vector *v2);
float dot_product(const Vector *v1, const Vector *v2);
float magnitude(const Vector *v);
Vector scalar_multiply(const Vector *v, float scalar);
Vector normalize(const Vector *v);

void print_vector(const Vector *v);

#endif // VECTOR_MATH_LIBRARY_H
