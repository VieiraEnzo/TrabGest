all: DB OlaGuilherme TUNGTUNGTUNG BMU_PROJECTION
	
DB:
	g++ GridDBSCAN.cpp -O3 -o "bin/dbscan.out"

OlaGuilherme:
	g++ GridKmeans.cpp -O3 -o "bin/kmeans.out"

TUNGTUNGTUNG:
	g++ GridKohonen.cpp -fopenmp -O3 -o "bin/kohonen.out"
	g++ images/kohonen/kohonen_grid_u_matrix.cpp -I. -O3 -o "bin/kohonen_grid_u_matrix.out"

BMU_PROJECTION:
	g++ images/kohonen/kohonen_clusters.cpp -I. -fopenmp -O3 -o "bin/kohonen_clusters.out"