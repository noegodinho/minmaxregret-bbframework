c++ -I ~/boost_1_63_0/ -lrt -O2 boost_prim.cpp -o boost_prim -L~/boost/stage/lib/
time ./boost_prim < ../generated/10000_1_50_1000000000.txt
