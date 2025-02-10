#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>
#include <cstdlib>

const int NUM_PARTICLES = 1000;
const float DT = 0.005f;
const float MAX_VELOCITY = 1.0f;
const float PIPE_LENGTH = 1.0f;
const float GRAVITY = -0.1f;

struct Particle {
	float x, y, vx, vy;
};

std::vector<Particle> particles;

// Function to define varying pipe width
float pipeWidth(float x) {
	float midX = 0.5f;
	return 0.4f - 0.15f * exp(-10 * pow(x - midX, 2));
}

// Initialize particles
void initParticles() {
	particles.clear();
	for (int i = 0; i < NUM_PARTICLES; i++) {
		float x = static_cast<float>(rand()) / RAND_MAX * PIPE_LENGTH;
		float y = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * pipeWidth(x);
		particles.push_back({x, y, 0.1f * ((rand() % 2) ? 1 : -1), 0.0f});
	}
}

// Monte Carlo simulation step
void updateParticles() {
	for (auto &p : particles) {
		p.vx += ((rand() % 200 - 100) / 5000.0f);
		p.vy += GRAVITY * DT;

		p.x += p.vx * DT;
		p.y += p.vy * DT;

		if (p.x > PIPE_LENGTH) p.x -= PIPE_LENGTH;
		if (p.x < 0) p.x += PIPE_LENGTH;

		float halfWidth = pipeWidth(p.x) / 2.0f;
		if (fabs(p.y) > halfWidth) {
			p.y = copysign(halfWidth, p.y);
			p.vy *= -0.5f;
		}
	}
}

// Render pipe walls
void renderPipe() {
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_STRIP);
	for (float x = 0; x <= PIPE_LENGTH; x += 0.02f) {
		float w = pipeWidth(x);
		glVertex2f(x, w / 2.0f);
	}
	glEnd();

	glBegin(GL_LINE_STRIP);
	for (float x = 0; x <= PIPE_LENGTH; x += 0.02f) {
		float w = pipeWidth(x);
		glVertex2f(x, -w / 2.0f);
	}
	glEnd();
}

// Render particles
void renderParticles() {
	glColor3f(0.0f, 1.0f, 1.0f);
	glBegin(GL_POINTS);
	for (const auto &p : particles) {
		float colorFactor = std::min(1.0f, fabsf(p.vx) / MAX_VELOCITY);
		glColor3f(1.0f - colorFactor, 1.0f, colorFactor);
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

