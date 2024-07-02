import math
import sys

EPSILON = 0.001

class Cluster():
    def __init__(self, centroid):
        self.centroid = centroid
        self.size = 1
        self.cluster = [centroid]


    def update_cluster(self, x):
        self.cluster.append(x)
        dist = self.update_centroid(x)
        self.update_size()
        return dist

    def update_centroid(self, x):
        prev = self.centroid
        curr = []
        size = self.size
        for i in range(len(prev)):
            curr_i = (prev[i] * size + x[i])/(size + 1)
            curr.append(round(curr_i,4))
        self.centroid = curr
        return self.calc_distance(prev)

    def update_size(self):
        self.size += 1

    def calc_distance(self, x):
        dist = 0
        for i in range(len(x)):
            dist += (x[i]-self.centroid[i])**2
        return math.sqrt(dist)

def data_verify(lst):
    K = int(lst[1])

    if len(lst) == 3:
        iter = 200
        path = lst[2]
    elif len(lst) == 4:
        iter = int(lst[2])
        path = lst[3]
    else:
        print("An error has occurred!")
        return -1, 0, 0

    if path[-4:] != ".txt":
        return -1, 0, 0

    file = open(path, 'r')
    data = file.readlines()

    if len(data) <= K or K <= 0:
        print("Invalid number of clusters!")
        return -1, 0, 0
    if iter >= 1000 or iter <= 0:
        print("Invalid maximum iteration!")
        return -1, 0, 0

    iter = min(len(data)-1, iter)

    return K, iter, data

def closest_clus(x,cluster_list):
    min_dist = sys.maxsize
    index = 0
    K = len(cluster_list)
    for i in range(K):
        d = cluster_list[i].calc_distance(x)
        if d < min_dist:
            min_dist = d
            index = i
    delta = cluster_list[index].update_cluster(x)

    return delta
def print_clusters(cluster_list):
    for cluster in cluster_list:
        target = ",".join([str(i) for i in cluster.centroid])
        print(target)
    return


def main():
    cluster_list = []
    K, iter, data = data_verify(sys.argv)
    if K == -1:
        return

    for i in range(iter):
        x = data[i].split(',')
        for j in range(len(x)):
            x[j] = round(float(x[j]),4)
        if i < K:
            cluster_list.append(Cluster(x))
        else:
            delta = closest_clus(x,cluster_list)
            if delta <= EPSILON:
                print_clusters(cluster_list)
                return
    print_clusters(cluster_list)
    return

if __name__ == "__main__":
    main()
