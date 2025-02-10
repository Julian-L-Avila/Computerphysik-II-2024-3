#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>

const int NUM_PARTICLES = 3000;
const float DT = 0.005f;
const float PIPE_LENGTH = 1.0f;
const float PRESSURE_FORCE = 0.05f;
const float MAX_VELOCITY = 1.0f;
const float INJECTION_RATE = 5;

std::mt19937 rng(std::random_device{}());
std::uniform_real_distribution<float> rand01(0.0f, 1.0f);
std::uniform_real_distribution<float> randSymmetric(-0.01f, 0.01f);

// Particle structure
struct Particle {
	float x, y, vx, vy;
};

std::vector<Particle> particles;

// Define varying pipe width
float pipeWidth(float x) {
	static constexpr float midX = 0.5f;
	return 0.4f - 0.15f * expf(-10 * powf(x - midX, 2));
}

// Initialize particles within the pipe
void initParticles() {
	particles.clear();
	particles.reserve(NUM_PARTICLES);
}

// Inject new particles at the left boundary
void injectParticles() {
	for (int i = 0; i < INJECTION_RATE; i++) {
		float y = (rand01(rng) - 0.5f) * pipeWidth(0);
		float vx = 0.2f + 0.1f * rand01(rng);
		particles.push_back({0.0f, y, vx, 0.0f});
	}
}

// Monte Carlo step with pressure-driven flow
void updateParticles() {
	std::vector<Particle> newParticles;
	newParticles.reserve(particles.size());

	for (auto &p : particles) {
		float width = pipeWidth(p.x);
		float pressureBoost = (PRESSURE_FORCE / width) * DT;
		p.vx = std::min(p.vx + pressureBoost, MAX_VELOCITY);
		p.vx += randSymmetric(rng);
		p.vy += randSymmetric(rng);

		// Move particle
		p.x += p.vx * DT;
		p.y += p.vy * DT;

		// Constrain within pipe
		float halfWidth = width / 2.0f;
		if (std::abs(p.y) > halfWidth) {
			p.y = std::copysign(halfWidth, p.y);
			p.vy *= -0.5f; // Damping
		}

		// Remove particles at the right boundary, keep the rest
		if (p.x < PIPE_LENGTH) {
			newParticles.push_back(p);
		}
	}

	particles = std::move(newParticles);
	injectParticles();
}

// Render pipe walls
void renderPipe() {
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_STRIP);
	for (float x = 0; x <= PIPE_LENGTH; x += 0.02f) {
		glVertex2f(x, pipeWidth(x) / 2.0f);
	}
	glEnd();

	glBegin(GL_LINE_STRIP);
	for (float x = 0; x <= PIPE_LENGTH; x += 0.02f) {
		glVertex2f(x, -pipeWidth(x) / 2.0f);
	}
	glEnd();
}

// Render particles
void renderParticles() {
	glBegin(GL_POINTS);
	for (const auto &p : particles) {
		float speedFactor = std::min(1.0f, std::abs(p.vx) / MAX_VELOCITY);
		glColor3f(1.0f - speedFactor, 1.0f, speedFactor);
		glVertex2f(p.x, p.y);
	}
	glEnd();
}

// OpenGL display function
void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	renderPipe();
	renderParticles();
}

// Main function
int main() {
	if (!glfwInit()) return -1;
	GLFWwindow *window = glfwCreateWindow(800, 400, "2D Fluid Simulation", NULL, NULL);
	if (!window) { glfwTerminate(); return -1; }

	glfwMakeContextCurrent(window);
	glOrtho(0, PIPE_LENGTH, -0.3, 0.3, -1, 1);

	initParticles();

	while (!glfwWindowShouldClose(window)) {
		updateParticles();
		display();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

