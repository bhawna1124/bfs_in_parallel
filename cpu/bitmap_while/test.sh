
for i in $(seq 1 20)

do

# test the correctness of bitmap_while

#./bitmap_while ../../test_case/tiny_graph 6


# regular
./bitmap_while ../../test_case/regular/RGL1 $i
#./bitmap_while ../../test_case/regular/RGL2 $i
#./bitmap_while ../../test_case/regular/RGL3 $i
#./bitmap_while ../../test_case/regular/RGL4 $i
#./bitmap_while ../../test_case/regular/RGL5 $i
#./bitmap_while ../../test_case/regular/RGL6 $i
#./bitmap_while ../../test_case/regular/RGL7 $i
#./bitmap_while ../../test_case/regular/RGL8 $i
#./bitmap_while ../../test_case/regular/RGL9 $i
#./bitmap_while ../../test_case/regular/RGL10 $i


# irregular
./bitmap_while ../../test_case/irregular/IRGL1 $i
#./bitmap_while ../../test_case/irregular/IRGL2 $i
#./bitmap_while ../../test_case/irregular/IRGL3 $i
#./bitmap_while ../../test_case/irregular/IRGL4 $i
#./bitmap_while ../../test_case/irregular/IRGL5 $i
#./bitmap_while ../../test_case/irregular/IRGL6 $i
#./bitmap_while ../../test_case/irregular/IRGL7 $i
#./bitmap_while ../../test_case/irregular/IRGL8 $i
#./bitmap_while ../../test_case/irregular/IRGL9 $i
#./bitmap_while ../../test_case/irregular/IRGL10 $i



# test by real graph
./bitmap_while ../../test_case/real_graph/BAY $i
#./bitmap_while ../../test_case/real_graph/NY $i
#./bitmap_while ../../test_case/real_graph/COL $i
#./bitmap_while ../../test_case/real_graph/FLA $i
#./bitmap_while ../../test_case/real_graph/CAL $i
#./bitmap_while ../../test_case/real_graph/NE $i
#./bitmap_while ../../test_case/real_graph/NW $i
#./bitmap_while ../../test_case/real_graph/LKS $i
#./bitmap_while ../../test_case/real_graph/E $i
#./bitmap_while ../../test_case/real_graph/W $i
#./bitmap_while ../../test_case/real_graph/CTR $i
#./bitmap_while ../../test_case/real_graph/USA $i


# small world

./bitmap_while ../../test_case/small_world/SW1 $i
#./bitmap_while ../../test_case/small_world/SW2 $i
#./bitmap_while ../../test_case/small_world/SW3 $i
#./bitmap_while ../../test_case/small_world/SW4 $i
#./bitmap_while ../../test_case/small_world/SW5 $i
#./bitmap_while ../../test_case/small_world/SW6 $i
#./bitmap_while ../../test_case/small_world/SW7 $i
#./bitmap_while ../../test_case/small_world/SW8 $i
#./bitmap_while ../../test_case/small_world/SW9 $i
#./bitmap_while ../../test_case/small_world/SW10 $i


done
