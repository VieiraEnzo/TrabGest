all: DB OlaGuilherme TUNGTUNGTUNG
	
DB:
	g++ GridDBSCAN.cpp -O3 -o "bin/dbscan.out"

OlaGuilherme:
	g++ GridKmeans.cpp -O3 -o "bin/kmeans.out"

TUNGTUNGTUNG:
	g++ GridKohonen.cpp -fopenmp -O3 -o "bin/kohonen.out"
	g++ images/kohonen/kohonen_u_matrix.cpp -I. -O3 -o "bin/kohonen_u_matrix.out"