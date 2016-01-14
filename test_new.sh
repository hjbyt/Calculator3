
for i in {1..5}
do 
    mkdir -p out_new/$i
    
    if [ -f tests_new/$i/test$i.v ]; then
        java SP.SPCalculatorMain -i tests_new/$i/test$i.in -e out_new/$i/test$i.err | ./SPCalculator -v tests_new/$i/test$i.v -o out_new/$i/test$i.out
    else
        java SP.SPCalculatorMain -i tests_new/$i/test$i.in -e out_new/$i/test$i.err | ./SPCalculator -o out_new/$i/test$i.out
    fi
    
    diff out_new/$i/test$i.out tests_new/$i/expected$i.out > out_new/$i/test$i.out.diff || echo There is a problem with test $i
    
done

mkdir -p out_new/median_average_test

java SP.SPCalculatorMain -i tests_new/median_average_test/medianAverage.in -e out_new/median_average_test/medianAverage.err | ./SPCalculator -o out_new/median_average_test/medianAverage.out
    diff out_new/median_average_test/medianAverage.out tests_new/median_average_test/medianAverageExpected.out > out_new/$i/medianAverage_out.diff || echo There is a problem with test medianAverage

echo Finished all new tests
