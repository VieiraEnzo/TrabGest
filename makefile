all: DB OlaGuilherme TUNGTUNGTUNG
	
DB:
	g++ GridDBSCAN.cpp -O3 -o "bin/dbscan.out"

OlaGuilherme:
	g++ GridKmeans.cpp -O3 -o "bin/kmeans.out"

TUNGTUNGTUNG:
	g++ GridKohonen.cpp -O3 -o "bin/kohonen.out"
