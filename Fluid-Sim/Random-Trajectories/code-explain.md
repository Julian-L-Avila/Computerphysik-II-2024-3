###### **Physics Behind the Code**
This simulation models **pressure-driven flow** of particles through a 2D pipe with varying width. The key physical principles involved include:

1. **Continuity Equation**
   The pipe has a constriction, meaning that fluid velocity must increase in narrower regions to conserve mass flow (assuming an incompressible fluid). This follows from:
   \[
   A_1 v_1 = A_2 v_2
   \]
   where \( A \) is the cross-sectional area and \( v \) is velocity.

2. **Pressure-Driven Flow**
   A small external force (`PRESSURE_FORCE`) is applied to accelerate particles, mimicking pressure gradients in a real fluid. This is an approximation of:
   \[
   F = -\frac{dP}{dx} A
   \]
   where \( dP/dx \) represents the pressure gradient.

3. **Brownian Motion Approximation**
   Small random perturbations (`randSymmetric`) are applied to particle velocities, representing thermal fluctuations or turbulence.

4. **Particle-Wall Interaction**
   If a particle reaches the pipe boundary, it is reflected with damping (\( v_y \) is inverted and multiplied by -0.5), simulating an imperfect collision with energy loss.

5. **Particle Injection & Removal**  
   New particles are introduced at the left end at a rate of `INJECTION_RATE`, and particles are removed once they reach the right boundary.

---

## **Implementation Details**
### 1. **Data Structures & Initial Setup**
- `Particle` struct stores position and velocity components.
- `std::vector<Particle>` holds all active particles.

### 2. **Pipe Geometry**
- The function `pipeWidth(x)` defines a constriction around \( x = 0.5 \) using a Gaussian-like shape:
  \[
  W(x) = 0.4 - 0.15 e^{-10 (x - 0.5)^2}
  \]
  This models a Venturi-like narrowing.

### 3. **Particle Motion**
- `updateParticles()` applies:
  - Pressure force: \( v_x \) increases with a factor inversely proportional to pipe width.
  - Random perturbations in both directions.
  - Movement using \( x' = x + v_x \cdot DT \), \( y' = y + v_y \cdot DT \).
  - Boundary conditions to constrain \( y \) within the pipe.
  - Removal of particles that exit the pipe.

### 4. **Rendering**
- `renderPipe()` draws the pipe shape.
- `renderParticles()` visualizes particles as color-coded points:
  - **Red**: Low velocity.
  - **Blue**: High velocity.

---

## **Advantages**
1. **Efficient Particle-Based Flow Approximation**  
   - No need for solving Navier-Stokes equations.
   - Uses simple physics approximations to achieve visually correct behavior.

2. **Adaptability to Complex Geometries**  
   - The `pipeWidth(x)` function can be modified to simulate different pipe shapes.

3. **Computationally Lightweight**  
   - Uses an explicit time-stepping approach.
   - Avoids expensive matrix operations or finite difference methods.

4. **Smooth Visualization**  
   - OpenGL rendering is simple yet effective for visualizing flow behavior.

---

## **Disadvantages**
1. **Lacks Real Fluid Interactions**  
   - No particle-particle interactions (e.g., viscosity, collisions).
   - Ignores Reynolds number effects and turbulence modeling.

2. **Oversimplified Pressure Modeling**  
   - Pressure force is artificially applied rather than derived from Navier-Stokes.

3. **No Outflow Dynamics**  
   - Particles disappear when they exit, instead of smoothly transitioning.

4. **Limited Resolution**  
   - The number of particles (`NUM_PARTICLES = 3000`) may not be enough to fully resolve flow details.

---

## **Possible Improvements**
1. **Add Viscosity Effects**  
   - Implement velocity smoothing using neighbor interactions (e.g., **SPH method**).
   - Introduce a velocity-dependent damping force.

2. **More Realistic Boundary Conditions**  
   - Instead of bouncing particles, apply a no-slip condition where velocities approach wall velocity.

3. **Better Pressure Model**  
   - Compute pressure field based on a local density estimation.

4. **Outflow Velocity Continuation**  
   - Maintain velocity distribution at the right boundary instead of removing particles abruptly.

5. **GPU Acceleration**  
   - Use CUDA/OpenCL to handle larger particle counts efficiently.

---

## **Potential Errors and Issues**
1. **Particles Can Accumulate at Boundaries**  
   - If a particle’s \( v_x \) is too low, it may linger near the inlet.

2. **Random Perturbations May Cause Instability**  
   - Large values in `randSymmetric` can lead to excessive diffusion.

3. **Rendering Performance Issues**  
   - OpenGL immediate mode (`glBegin`/`glEnd`) is outdated and slow for large numbers of particles.

---
