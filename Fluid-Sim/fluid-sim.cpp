#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>

const int NUM_PARTICLES = 5000;
const float DT = 0.005f;
const float PIPE_LENGTH = 1.0f;
const float PRESSURE_FORCE = 0.5f;  // Increased pressure force for noticeable effect
const float MAX_VELOCITY = 5.0f;
const float INJECTION_RATE = 5;
const int N_MC_SAMPLES = 2; // Number of Monte Carlo samples for velocity estimation

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

// Pressure as a function of x (position)
float pressure(float x) {
	float maxWidth = 0.4f;
	float currentWidth = pipeWidth(x);
	float particleDensity = NUM_PARTICLES / currentWidth;  // Particle density in current region
	float temperature = 1.0f;  // Assuming constant temperature
	float pressureValue = (particleDensity * temperature) * (currentWidth / maxWidth);
	return PRESSURE_FORCE * pressureValue;
}

float maxwellBoltzmannVelocity(float temperature) {
	float randVel = rand01(rng);  // Random value between 0 and 1
	return sqrt(-2.0f * log(randVel)) * sqrt(temperature);  // Sample velocity from MB distribution
}

// Initialize particles within the pipe
void initParticles() {
	particles.clear();
	particles.reserve(NUM_PARTICLES);

	float temperature = 1.0f;

	for (int i = 0; i < NUM_PARTICLES; ++i) {
		float y = rand01(rng) * pipeWidth(0) - pipeWidth(0) / 2;
		float vx = maxwellBoltzmannVelocity(temperature);
		float vy = maxwellBoltzmannVelocity(temperature);
		particles.push_back({0.0f, y, vx, vy});
	}
}

// Inject new particles at the left boundary
void injectParticles() {
	for (int i = 0; i < INJECTION_RATE; i++) {
		float y = rand01(rng) * pipeWidth(0) - pipeWidth(0) / 2;
		float vx = 0.5f + 0.2f * rand01(rng);
		particles.push_back({0.0f, y, vx, 0.0f});
	}
}

// Biot-Savart kernel for 2D
std::vector<float> biotSavartKernel(float dx, float dy) {
	std::vector<float> k = {1e-6f, 1e-6f};
	float r2 = dx * dx + dy * dy;
	if (r2 < 1e-6f) return k;  // Avoid division by zero by returning a small value
	k[0] = dx / (2.0f * M_PI * r2);
	k[1] = dy / (2.0f * M_PI * r2);
	return k;
}

// Monte Carlo estimation of velocity using Biot-Savart law
void updateParticles() {
	std::vector<Particle> newParticles;
	newParticles.reserve(particles.size());

	for (auto &p : particles) {
		// Monte Carlo integration to estimate velocity
		float vx_new = p.vx;
		float vy_new = p.vy;

		for (int i = 0; i < N_MC_SAMPLES; ++i) {
			// Random sample point from particles to estimate vorticity
			int randIndex = static_cast<int>(rand01(rng) * (particles.size() - 1));
			auto &sampledParticle = particles[randIndex];

			float dx = p.x - sampledParticle.x;
			float dy = p.y - sampledParticle.y;

			std::vector<float> kernel = biotSavartKernel(dx, dy);
			float vorticity = sampledParticle.vx * dy - sampledParticle.vy * dx; // Simplified 2D vorticity

			float prob = rand01(rng);

			vx_new -= vorticity * kernel[1] / prob;
			vy_new += vorticity * kernel[0] / prob;
		}

		// Average the results from Monte Carlo
		vx_new /= N_MC_SAMPLES;
		vy_new /= N_MC_SAMPLES;

		// Pressure gradient: the force induced by the pressure difference between neighboring particles
		float pressureLeft = pressure(p.x);
		float pressureRight = pressure(p.x + DT);  // Approximate right side of particle
		float pressureGradient = (pressureRight - pressureLeft) / DT;
		float pressureForce = pressureGradient * 0.01f;  // Scale factor for force magnitude

		// Update particle velocity with the pressure gradient force
		vx_new += pressureForce;

		// Update particle velocity with Biot-Savart result (for vortex interaction)
		p.vx = vx_new;
		p.vy = vy_new;

		// Move particle based on updated velocity
		p.x += p.vx * DT;
		p.y += p.vy * DT;

		// Constrain particles within pipe boundaries (reflective walls)
		float width = pipeWidth(p.x);
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
	GLFWwindow *window = glfwCreateWindow(800, 400, "2D Fluid Simulation with Pressure Gradient", NULL, NULL);
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

