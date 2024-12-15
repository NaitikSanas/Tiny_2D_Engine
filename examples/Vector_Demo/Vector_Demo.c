#include "Vector_Demo.h"

#define GRAVITY -9.81 // Acceleration due to gravity (m/s^2)
#define RESTITUTION 0.8 // Coefficient of restitution (bounciness)
#define TIME_STEP 0.01 // Simulation time step (seconds)
#define EPSILON 0.001 // Threshold to stop simulation when velocity is negligible

static const char *TAG = "BouncingBall";
#include "uCanvas_api.h"
// Structure to represent the ball
typedef struct {
    Vector position;
    Vector velocity;
} Ball;

void update_ball(Ball *ball, float delta_time) {
    // Apply gravity to velocity
    Vector gravity_vector = create_vector(ball->velocity.size);
    if (!gravity_vector.components) return;

    gravity_vector.components[0] = 0.0; // No horizontal gravity
    gravity_vector.components[1] = GRAVITY * delta_time; // Vertical gravity

    Vector new_velocity = add_vectors(&ball->velocity, &gravity_vector);
    free_vector(&gravity_vector);

    // Update position based on velocity
    Vector displacement = scalar_multiply(&new_velocity, delta_time);
    Vector new_position = add_vectors(&ball->position, &displacement);

    // Horizontal boundaries (240x screen width)
    if (new_position.components[0] <= 0.0) {
        new_position.components[0] = 0.0; // Clamp to left boundary
        new_velocity.components[0] *= -RESTITUTION; // Reverse and reduce velocity
    } else if (new_position.components[0] >= 240.0) {
        new_position.components[0] = 240.0; // Clamp to right boundary
        new_velocity.components[0] *= -RESTITUTION; // Reverse and reduce velocity
    }

    // Vertical boundaries (320y height)
    if (new_position.components[1] <= 0.0) {
        new_position.components[1] = 0.0; // Clamp to ground
        new_velocity.components[1] *= -RESTITUTION; // Reverse and reduce velocity
    } else if (new_position.components[1] >= 320.0) {
        new_position.components[1] = 320.0; // Clamp to ceiling
        new_velocity.components[1] *= -RESTITUTION; // Reverse and reduce velocity
    }

    // Update the ball's velocity and position
    free_vector(&ball->velocity);
    free_vector(&ball->position);
    ball->velocity = new_velocity;
    ball->position = new_position;

    free_vector(&displacement);
}


// Example usage of the bouncing ball simulation
void vector_usage_demo() {
    start_uCanvas_engine();
    uCanvas_Scene_t* scene = New_uCanvas_Scene();
    uCanvas_set_active_scene(scene);
    uCanvas_universal_obj_t* ball_obj = New_uCanvas_2DCircle(100,300,20);

    ESP_LOGI(TAG, "Starting Bouncing Ball Simulation");

    // Initialize ball
    Ball ball;
    ball.position = create_vector(2); // 2D position (x, y)
    ball.velocity = create_vector(2); // 2D velocity (vx, vy)

    if (!ball.position.components || !ball.velocity.components) {
        ESP_LOGE(TAG, "Failed to create ball vectors!");
        return;
    }

    // Set initial position and velocity
    ball.position.components[0] = 0.0; // Initial x position
    ball.position.components[1] = 10.0; // Initial y position (10 meters above ground)
    ball.velocity.components[0] = 200.0; // Initial x velocity (2 m/s)
    ball.velocity.components[1] = 1.0; // Initial y velocity

    // Simulation loop
    while (1) {
        ball_obj->properties.position.x = ball.position.components[1];
        ball_obj->properties.position.y = 320 - ball.position.components[0];

        // ESP_LOGI(TAG, "Position: [%.2f, %.2f], Velocity: [%.2f, %.2f]",
        //          ball.position.components[0], ball.position.components[1],
        //          ball.velocity.components[0], ball.velocity.components[1]);

        update_ball(&ball, TIME_STEP);

        // End simulation when ball stops bouncing and is near ground
        if (ball.position.components[1] == 0.0 && fabs(ball.velocity.components[1]) < EPSILON) {
            ESP_LOGI(TAG, "Simulation ended.");
            break;
        }

        // Delay to simulate real-time updates (optional, for visualization)
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    // Clean up
    free_vector(&ball.position);
    free_vector(&ball.velocity);

    ESP_LOGI(TAG, "Bouncing Ball Simulation Complete");
}
