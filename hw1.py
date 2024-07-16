import math
import sys

EPSILON = 0.001

class Cluster():
    #initiate a cluster
    def __init__(self, centroid):
        self.centroid = centroid
        self.size = 1
        self.cluster = [centroid]
        self.prev = [0 for i in range(len(self.centroid))]#prev_centroid

    #add point to poin list in cluster
    def update_cluster(self, point):
        self.cluster.append(point)
        self.size +=1
    
    #remove all points from point list in cluster
    def clear_cluster(self):
        self.cluster = []
        self.size = 0

    
    def update_centroid(self):
        self.prev = self.centroid[:]
        n = len(self.prev)
        curr = [0 for i in range(n)]
        for i in range(n):
            for point in self.cluster:
                curr[i] += point[i]
            curr[i] /= self.size
        self.centroid = curr
        return self.calc_distance(self.prev)

    def calc_distance(self, point):
        dist = sum((point[i] - self.centroid[i]) ** 2 for i in range(len(point)))
        return math.sqrt(dist)


def verify_data(args):
    if len(args) not in [3, 4] or not path.endswith(".txt"):
        print("An error has occurred!")
        return -1, 0, 0
    str_K = args[1]
    K = int(str_K)
    str_iter = "200" if len(args) == 3 else args[2]
    iterations = int(str_iter)
    path = args[2] if len(args) == 3 else args[3]

    if not path.endswith(".txt"):
        print("An error has occurred!")
        return -1, 0, 0

    with open(path, 'r') as file:
        data = [[float(num) for num in line.split(',')] for line in file]

    if len(data) <= K or K <= 0 or !(str_K.isdigit()):
        print("Invalid number of clusters!")
        return -1, 0, 0
    if iterations >= 1000 or iterations <= 0 !(str_iter.isdigit()):
        print("Invalid maximum iteration!")
        return -1, 0, 0

    return K, iterations, data


def find_closest_clus(point,cluster_list):
    min_dist = float('inf')
    index = 0
    K = len(cluster_list)
    for i in range(K):
        d = cluster_list[i].calc_distance(point)
        if d < min_dist:
            min_dist = d
            index = i
    cluster_list[index].update_cluster(point)


def print_clusters(cluster_list):
    for cluster in cluster_list:
        target = ",".join([str(round(i, 4)) for i in cluster.centroid])

        print(target)


def initialize_clusters(K,data,cluster_list):
    for i, point in enumerate(data):
        if i < K:
            cluster_list.append(Cluster(point))
        else:
            find_closest_clus(point,cluster_list)


def main():
    cluster_list = []
    K, iterations, data = verify_data(sys.argv)
    if K == -1:
        return

    initialize_clusters(K, data, cluster_list)

    while iterations > 0:

        for point in data:
            find_closest_clus(point,cluster_list)

        flag = True
        for cluster in cluster_list:
            if cluster.update_centroid() > EPSILON:
                flag = False
            cluster.clear_cluster()

        if flag:
            break
        iterations -= 1

    print_clusters(cluster_list)


if __name__ == "__main__":
    main()
