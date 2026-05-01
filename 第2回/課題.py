import numpy as np
import matplotlib.pyplot as plt

GM = 4 * np.pi**2

dt = 0.01
t_max = 100
n_steps = int(t_max / dt)

x = 1.0
y = 0.0
vx = 0.0
vy = 2 * np.pi

x_list = []
y_list = []

for i in range(n_steps):
    r = np.sqrt(x**2 + y**2)

    ax = -GM * x / r**3
    ay = -GM * y / r**3

    vx = vx + ax * dt
    vy = vy + ay * dt

    x = x + vx * dt
    y = y + vy * dt

    x_list.append(x)
    y_list.append(y)

plt.figure(figsize=(6, 6))
plt.plot(x_list, y_list)
plt.scatter(0, 0)
plt.axis("equal")
plt.show()