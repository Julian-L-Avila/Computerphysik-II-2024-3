#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdlib>

const int GRID_X = 100;
const int GRID_Y = 50;
const float DX = 1.0f / GRID_X;
const float DY = 1.0f / GRID_Y;
const float DT = 0.01f;
const float VISCOSITY = 0.01f;
const float FORCE_X = 0.001f;
const float FORCE_Y = 0.0005f;
const int PARTICLE_LIMIT = 5000;

struct Particle {
    float x, y;
};

std::vector<Particle> particles;
std::vector<std::vector<float>> ux(GRID_X, std::vector<float>(GRID_Y, 0.0f));
std::vector<std::vector<float>> uy(GRID_X, std::vector<float>(GRID_Y, 0.0f));
std::vector<std::vector<float>> pressure(GRID_X, std::vector<float>(GRID_Y, 0.0f));

float pipeWidth(float x) {
    static constexpr float midX = 0.5f;
    return 0.4f - 0.15f * expf(-10 * powf(x - midX, 2));
}

void applyForces() {
    for (int i = 1; i < GRID_X - 1; i++) {
        for (int j = 1; j < GRID_Y - 1; j++) {
            ux[i][j] += FORCE_X * DT;
            uy[i][j] += FORCE_Y * DT;
        }
    }
}

void solvePoissonPressure() {
    for (int iter = 0; iter < 50; iter++) {
        std::vector<std::vector<float>> new_pressure = pressure;
        for (int i = 1; i < GRID_X - 1; i++) {
            for (int j = 1; j < GRID_Y - 1; j++) {
                new_pressure[i][j] = (pressure[i+1][j] + pressure[i-1][j] + 
                                      pressure[i][j+1] + pressure[i][j-1]) / 4.0f;
            }
        }
        pressure = new_pressure;
    }
}

void solveNavierStokes() {
    std::vector<std::vector<float>> new_ux = ux;
    std::vector<std::vector<float>> new_uy = uy;

    for (int i = 1; i < GRID_X - 1; i++) {
        for (int j = 1; j < GRID_Y - 1; j++) {
            float laplacian_ux = (ux[i+1][j] + ux[i-1][j] - 2 * ux[i][j]) / (DX * DX) +
                                  (ux[i][j+1] + ux[i][j-1] - 2 * ux[i][j]) / (DY * DY);
            float laplacian_uy = (uy[i+1][j] + uy[i-1][j] - 2 * uy[i][j]) / (DX * DX) +
                                  (uy[i][j+1] + uy[i][j-1] - 2 * uy[i][j]) / (DY * DY);
            
            new_ux[i][j] = ux[i][j] + DT * (-ux[i][j] * (ux[i+1][j] - ux[i-1][j]) / (2 * DX) 
                                           - uy[i][j] * (ux[i][j+1] - ux[i][j-1]) / (2 * DY)
                                           - (pressure[i+1][j] - pressure[i-1][j]) / (2 * DX)
                                           + VISCOSITY * laplacian_ux);
            
            new_uy[i][j] = uy[i][j] + DT * (-ux[i][j] * (uy[i+1][j] - uy[i-1][j]) / (2 * DX) 
                                           - uy[i][j] * (uy[i][j+1] - uy[i][j-1]) / (2 * DY)
                                           - (pressure[i][j+1] - pressure[i][j-1]) / (2 * DY)
                                           + VISCOSITY * laplacian_uy);
        }
    }
    ux = new_ux;
    uy = new_uy;
}

void generateParticles() {
    if (particles.size() < PARTICLE_LIMIT) {
        float yPos = 0.3f + static_cast<float>(rand()) / RAND_MAX * 0.4f;
        particles.push_back({0.1f, yPos});
    }
}

void updateParticles() {
    for (auto& p : particles) {
        int i = static_cast<int>(p.x * GRID_X);
        int j = static_cast<int>(p.y * GRID_Y);
        if (i >= 0 && i < GRID_X && j >= 0 && j < GRID_Y) {
            p.x += ux[i][j] * DT;
            p.y += uy[i][j] * DT;
            if (p.y < 0.3f || p.y > 0.7f) {
                p.y = 0.3f + static_cast<float>(rand()) / RAND_MAX * 0.4f;
            }
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);
    for (const auto &p : particles) {
        glColor3f(0.0f, 1.0f, 1.0f);
        glVertex2f(p.x * 2.0f - 1.0f, p.y * 2.0f - 1.0f);
    }
    glEnd();
}

int main() {
    if (!glfwInit()) return -1;
    GLFWwindow *window = glfwCreateWindow(800, 400, "Navier-Stokes Simulation", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    glOrtho(-1, 1, -1, 1, -1, 1);

    while (!glfwWindowShouldClose(window)) {
        generateParticles();
        applyForces();
        solvePoissonPressure();
        solveNavierStokes();
        updateParticles();
        display();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}







