import matplotlib.pyplot as plt
import numpy as np



model_plot=[]
def infection(y0, days, r, N):
    if days == 0:
         return y0
    else:
        changes = infection(y0, days-1, r, N)
        model_plot.append(changes)
        return changes + r*changes*(1-changes/N)

infection(1000, 31, 0.195, 14700)
plt.plot(model_plot)
plt.show()


ax = plt.figure().add_subplot(projection='3d')

x = np.linspace(0, 31, 31)
y = model_plot
ax.plot(x, y, zs = 0, zdir = 'z')

ax.legend()
ax.set_xlim(0, 31)
ax.set_ylim(0, 15000)
ax.set_zlim(0, 1)
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')

plt.show()
