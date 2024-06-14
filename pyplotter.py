"""
Function to plot the data from file output.dat using matplotlib
data is in the form cluster_id, x, y
each cluster_id is a different color
also plot centroid of each cluster
"""

import matplotlib.pyplot as plt

def plot_data():
    with open('./output.dat') as f:
        data = f.readlines()
    data = [line.split() for line in data]
    if len(data[0])==4:
        data = [[int(line[1]), float(line[2]), float(line[3])] for line in data]
    elif len(data[0])==5:
        data = [[int(line[1]), float(line[2]), float(line[3]), float(line[4])] for line in data]
    data = sorted(data, key=lambda x: x[0])
    clusters = set([line[0] for line in data])
    colors = ['b', 'g', 'r', 'c', 'm', 'y', 'k', 'lime', 'teal', 'purple', 'brown', 'pink', 'gray', 'olive', 'cyan']
    with open('./centroids.dat') as f:
        centroids = f.readlines()
    centroids = [line.split() for line in centroids]
    if len(centroids[0])==2:
        centroids = [[float(line[0]), float(line[1])] for line in centroids]
        for i, cluster in enumerate(clusters):
            cluster_data = [line for line in data if line[0] == cluster]
            x = [line[1] for line in cluster_data]
            y = [line[2] for line in cluster_data]
            plt.scatter(x, y, c=colors[i], label='Cluster ' + str(cluster))
        plt.scatter([line[0] for line in centroids], [line[1] for line in centroids], c='black', marker='x', label='Centroid')
    elif len(centroids[0])==3:
        centroids = [[float(line[0]), float(line[1]), float(line[2])] for line in centroids]
        fig = plt.figure()
        ax = fig.add_subplot(111, projection='3d')
        for i, cluster in enumerate(clusters):
            cluster_data = [line for line in data if line[0] == cluster]
            x = [line[1] for line in cluster_data]
            y = [line[2] for line in cluster_data]
            z = [line[3] for line in cluster_data]
            ax.scatter(x, y, z, c=colors[i], label='Cluster ' + str(cluster))
        ax.scatter([line[0] for line in centroids], [line[1] for line in centroids], [line[2] for line in centroids], c='black', marker='x', label='Centroid')

    
    plt.legend()
    plt.show()

if __name__ == '__main__':
    plot_data()


