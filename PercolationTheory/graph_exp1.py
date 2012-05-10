from mpl_toolkits.axes_grid.axislines import SubplotZero
import matplotlib.pyplot as plt
import numpy as np

filename = 'data/plot_exp1_1280.txt'
pair_list = [line.split() for line in open(filename)]
p = [float(pair[0]) for pair in pair_list]
probability = [float(pair[1]) for pair in pair_list]

if 1:
    fig = plt.figure(1)
    ax = SubplotZero(fig, 111)
    fig.add_subplot(ax)

    for direction in ["xzero", "yzero"]:
        ax.axis[direction].set_axisline_style("-|>")
        ax.axis[direction].set_visible(True)

	ax.axis['xzero'].set_label('p')
	ax.axis['yzero'].set_label('probability that the path exists')

    for direction in ["left", "right", "bottom", "top"]:
        ax.axis[direction].set_visible(False)

    ax.plot(p, probability)

    plt.show()
