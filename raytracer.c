#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#define WIDTH 800
#define HEIGHT 600
#define NUM_THREADS 4

typedef struct {
    float x, y, z;
} Vec3;

typedef struct {
    Vec3 center;
    float radius;
    Vec3 color;
} Sphere;

typedef struct {
    Vec3 origin;
    Vec3 direction;
} Ray;

typedef struct {
    int start_row;
    int end_row;
    SDL_Renderer *renderer;
    Sphere *spheres;
    int num_spheres;
} ThreadData;

Vec3 add(Vec3 a, Vec3 b) {
    return (Vec3){a.x + b.x, a.y + b.y, a.z + b.z};
}

Vec3 subtract(Vec3 a, Vec3 b) {
    return (Vec3){a.x - b.x, a.y - b.y, a.z - b.z};
}

Vec3 multiply(Vec3 a, float t) {
    return (Vec3){a.x * t, a.y * t, a.z * t};
}

float dot(Vec3 a, Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 normalize(Vec3 v) {
    float len = sqrt(dot(v, v));
    return (Vec3){v.x / len, v.y / len, v.z / len};
}

int intersect_sphere(Ray ray, Sphere sphere, float *t) {
    Vec3 oc = subtract(ray.origin, sphere.center);
    float a = dot(ray.direction, ray.direction);
    float b = 2.0 * dot(oc, ray.direction);
    float c = dot(oc, oc) - sphere.radius * sphere.radius;
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return 0;
    } else {
        *t = (-b - sqrt(discriminant)) / (2.0 * a);
        return 1;
    }
}

Vec3 trace(Ray ray, Sphere *spheres, int num_spheres) {
    float t_min = INFINITY;
    Sphere *hit_sphere = NULL;

    for (int i = 0; i < num_spheres; i++) {
        float t;
        if (intersect_sphere(ray, spheres[i], &t) && t < t_min) {
            t_min = t;
            hit_sphere = &spheres[i];
        }
    }

    if (hit_sphere) {
        return hit_sphere->color;
    } else {
        return (Vec3){0.0f, 0.0f, 0.0f}; // Background color
    }
}

void *render_thread(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    for (int y = data->start_row; y < data->end_row; y++) {
        for (int x = 0; x < WIDTH; x++) {
            float u = (float)x / (float)WIDTH;
            float v = (float)y / (float)HEIGHT;
            Ray ray = {(Vec3){0.0f, 0.0f, 0.0f}, normalize((Vec3){u - 0.5f, v - 0.5f, 1.0f})};
            Vec3 color = trace(ray, data->spheres, data->num_spheres);

            SDL_SetRenderDrawColor(data->renderer, (int)(color.x * 255), (int)(color.y * 255), (int)(color.z * 255), 255);
            SDL_RenderDrawPoint(data->renderer, x, y);
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Ray Tracer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Sphere spheres[] = {
        {(Vec3){0.0f, 0.0f, -5.0f}, 1.0f, (Vec3){1.0f, 0.0f, 0.0f}},
        {(Vec3){2.0f, 0.0f, -5.0f}, 1.0f, (Vec3){0.0f, 1.0f, 0.0f}},
        {(Vec3){-2.0f, 0.0f, -5.0f}, 1.0f, (Vec3){0.0f, 0.0f, 1.0f}}
    };
    int num_spheres = sizeof(spheres) / sizeof(spheres[0]);

    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
    int rows_per_thread = HEIGHT / NUM_THREADS;

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].start_row = i * rows_per_thread;
        thread_data[i].end_row = (i + 1) * rows_per_thread;
        thread_data[i].renderer = renderer;
        thread_data[i].spheres = spheres;
        thread_data[i].num_spheres = num_spheres;
        pthread_create(&threads[i], NULL, render_thread, &thread_data[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    SDL_RenderPresent(renderer);

    int running = 1;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
