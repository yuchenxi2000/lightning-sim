import numpy as np
import matplotlib.patches
import matplotlib.pyplot as plt
import os
from matplotlib.animation import FuncAnimation
import matplotlib.animation

fig, ax = plt.subplots(figsize=(8, 8))
fig.set_tight_layout(True)


os.chdir('./simulation-results/300x100,step500,house180-190,0-40/run4')

x = np.loadtxt('outx.txt')

y = np.loadtxt('outy.txt')

# draw the building
ax.add_patch(
    matplotlib.patches.Rectangle(
        (180, 0),   # (x,y)
        9,          # width
        39,         # height
    )
)


def update(i):
    l = min(i, x.size)
    ax.scatter(x[0:l], y[0:l], c='blue')
    ax.axis([100, 200, 0, 100])
    return ax


Writer = matplotlib.animation.writers['ffmpeg']
writer = Writer(fps=25, metadata=dict(artist='Me'), bitrate=1800)

anim = FuncAnimation(fig, update, frames=np.arange(0, 500), interval=200)
anim.save('animate.mp4', writer=writer)
