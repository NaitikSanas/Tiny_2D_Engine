#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "esp_log.h" // For ESP-IDF logging

// Define a structure to represent a vector
typedef struct {
    float *components;
    size_t size;
} Vector;

static const char *TAG = "VectorMath";

// Function to create a vector
Vector create_vector(size_t size) {
    Vector v;
    v.size = size;
    v.components = (float *)malloc(size * sizeof(float));
    if (!v.components) {
        ESP_LOGE(TAG, "Memory allocation failed!");
        v.size = 0; // Set size to 0 to indicate failure
    }
    return v;
}

// Function to free a vector
void free_vector(Vector *v) {
    if (v->components) {
        free(v->components);
        v->components = NULL;
        v->size = 0;
    }
}

// Function to add two vectors
Vector add_vectors(const Vector *v1, const Vector *v2) {
    if (v1->size != v2->size) {
        ESP_LOGE(TAG, "Vector sizes do not match for addition!");
        return (Vector){NULL, 0};
    }
    Vector result = create_vector(v1->size);
    if (!result.components) return result;
    for (size_t i = 0; i < v1->size; ++i) {
        result.components[i] = v1->components[i] + v2->components[i];
    }
    return result;
}

// Function to subtract two vectors
Vector subtract_vectors(const Vector *v1, const Vector *v2) {
    if (v1->size != v2->size) {
        ESP_LOGE(TAG, "Vector sizes do not match for subtraction!");
        return (Vector){NULL, 0};
    }
    Vector result = create_vector(v1->size);
    if (!result.components) return result;
    for (size_t i = 0; i < v1->size; ++i) {
        result.components[i] = v1->components[i] - v2->components[i];
    }
    return result;
}

// Function to compute the dot product of two vectors
float dot_product(const Vector *v1, const Vector *v2) {
    if (v1->size != v2->size) {
        ESP_LOGE(TAG, "Vector sizes do not match for dot product!");
        return NAN;
    }
    float result = 0.0;
    for (size_t i = 0; i < v1->size; ++i) {
        result += v1->components[i] * v2->components[i];
    }
    return result;
}

// Function to compute the magnitude of a vector
float magnitude(const Vector *v) {
    if (!v->components || v->size == 0) {
        ESP_LOGE(TAG, "Invalid vector for magnitude calculation!");
        return NAN;
    }
    float sum = 0.0;
    for (size_t i = 0; i < v->size; ++i) {
        sum += v->components[i] * v->components[i];
    }
    return sqrt(sum);
}

// Function to multiply a vector by a scalar
Vector scalar_multiply(const Vector *v, float scalar) {
    if (!v->components || v->size == 0) {
        ESP_LOGE(TAG, "Invalid vector for scalar multiplication!");
        return (Vector){NULL, 0};
    }
    Vector result = create_vector(v->size);
    if (!result.components) return result;
    for (size_t i = 0; i < v->size; ++i) {
        result.components[i] = v->components[i] * scalar;
    }
    return result;
}

// Function to normalize a vector
Vector normalize(const Vector *v) {
    float mag = magnitude(v);
    if (mag == 0.0 || isnan(mag)) {
        ESP_LOGE(TAG, "Cannot normalize a zero or invalid vector!");
        return (Vector){NULL, 0};
    }
    return scalar_multiply(v, 1.0 / mag);
}

// Function to print a vector
void print_vector(const Vector *v) {
    if (!v->components || v->size == 0) {
        ESP_LOGE(TAG, "Invalid vector for printing!");
        return;
    }
    printf("[ ");
    for (size_t i = 0; i < v->size; ++i) {
        printf("%.2f ", v->components[i]);
    }
    printf("]\n");
}
