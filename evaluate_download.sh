#!/bin/bash
#!/bin/bash

# 4m
wget https://suitesparse-collection-website.herokuapp.com/MM/SNAP/cit-Patents.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/SNAP/roadNet-CA.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/SNAP/com-Orkut.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/SNAP/com-LiveJournal.tar.gz

tar xvf cit-Patents.tar.gz
tar xvf roadNet-CA.tar.gz
tar xvf com-Orkut.tar.gz
tar xvf com-LiveJournal.tar.gz

mv cit-Patents/cit-Patents.mtx .
mv roadNet-CA/roadNet-CA.mtx .
mv com-Orkut/com-Orkut.mtx .
mv com-LiveJournal/com-LiveJournal.mtx .

######## 8m
wget https://suitesparse-collection-website.herokuapp.com/MM/LAW/indochina-2004.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/DIMACS10/great-britain_osm.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/DIMACS10/italy_osm.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/DIMACS10/asia_osm.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/Gleich/wb-edu.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/DIMACS10/hugetrace-00010.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/DIMACS10/road_central.tar.gz

tar xvf indochina-2004.tar.gz
tar xvf great-britain_osm.tar.gz
tar xvf italy_osm.tar.gz
tar xvf asia_osm.tar.gz
tar xvf wb-edu.tar.gz
tar xvf hugetrace-00010.tar.gz
tar xvf road_central.tar.gz

mv indochina-2004/indochina-2004.mtx .
mv great-britain_osm/great-britain_osm.mtx .
mv italy_osm/italy_osm.mtx .
mv asia_osm/asia_osm.mtx .
mv wb-edu/wb-edu.mtx .
mv hugetrace-00010/hugetrace-00010.mtx .
mv road_central/road_central.mtx .

########  16m
wget https://suitesparse-collection-website.herokuapp.com/MM/LAW/uk-2002.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/DIMACS10/hugetrace-00020.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/DIMACS10/hugebubbles-00000.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/DIMACS10/hugebubbles-00020.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/GAP/GAP-road.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/DIMACS10/road_usa.tar.gz

tar xvf uk-2002.tar.gz
tar xvf hugetrace-00020.tar.gz
tar xvf hugebubbles-00000.tar.gz
tar xvf hugebubbles-00020.tar.gz
tar xvf GAP-road.tar.gz
tar xvf road_usa.tar.gz

mv uk-2002/uk-2002.mtx .
mv hugetrace-00020/hugetrace-00020.mtx .
mv hugebubbles-00000/hugebubbles-00000.mtx .
mv hugebubbles-00020/hugebubbles-00020.mtx .
mv GAP-road/GAP-road.mtx .
mv road_usa/road_usa.mtx .

########  32m
wget https://suitesparse-collection-website.herokuapp.com/MM/DIMACS10/europe_osm.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/GenBank/kmer_V2a.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/GenBank/kmer_U1a.tar.gz
wget https://suitesparse-collection-website.herokuapp.com/MM/MAWI/mawi_201512020000.tar.gz

tar xvf europe_osm.tar.gz
tar xvf kmer_V2a.tar.gz
tar xvf kmer_U1a.tar.gz
tar xvf mawi_201512020000.tar.gz

mv europe_osm/europe_osm.mtx .
mv kmer_V2a/kmer_V2a.mtx .
mv kmer_U1a/kmer_U1a.mtx .
mv mawi_201512020000/mawi_201512020000.mtx .
