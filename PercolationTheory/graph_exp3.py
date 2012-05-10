from mpl_toolkits.axes_grid.axislines import SubplotZero
import matplotlib.pyplot as plt
import numpy as np
import math

filename = 'data/plot_exp3.txt'
pair_list = [line.split() for line in open(filename)]
log_p = [math.log(float(pair[0])) for pair in pair_list]
log_size = [math.log(float(pair[1])) for pair in pair_list]

print np.polyfit(log_p, log_size, 1)

if 1:
    fig = plt.figure(1)
    ax = SubplotZero(fig, 111)
    fig.add_subplot(ax)

    for direction in ["xzero", "yzero"]:
        ax.axis[direction].set_axisline_style("-|>")
        ax.axis[direction].set_visible(True)

	ax.axis['xzero'].set_label('log_n')
	ax.axis['yzero'].set_label('log_s')

    for direction in ["left", "right", "bottom", "top"]:
        ax.axis[direction].set_visible(False)

    ax.plot(log_p, log_size)

    plt.show()
