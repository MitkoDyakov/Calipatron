import numpy as np
import matplotlib.pyplot as plt

N = 400
samples_per_half = 50 # 0 to some random 
xs = np.linspace(0, 2 * np.pi, N)
xs_combo = np.linspace(0, N + samples_per_half, N + samples_per_half)
ys = np.sin(xs)
ys_transformed = (ys + 1) / 2

midpoint = len(ys_transformed) // 2

y_top = ys_transformed[:midpoint]

y_plateau = np.full(samples_per_half, 0.5)

y_bottom = ys_transformed[midpoint:]

y_combo = np.concatenate([y_top, y_plateau, y_bottom])

def pdm(real_samples):
    res = []

    error = 0

    for item in real_samples:
        error += item
        res.append(0 if error <= 0 else 1)
        error -= res[-1]

    return np.array(res)

encoded = pdm(y_combo)

plt.figure()

plt.plot(xs_combo, y_combo, label='original')
plt.step(xs_combo, encoded, color='orange', linewidth=0.5, label='PDM')

print(encoded)

plt.legend()
plt.show()
