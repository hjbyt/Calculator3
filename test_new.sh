
for i in {1..5}
do 
    mkdir -p out_new/$i
    
    
    if [ -f test.txt ]; then
        VAR_FILE_OPTION = -v tests_new/$i/test$i.v
    fi
    
    java SP.SPCalculatorMain -i tests_new/$i/test$i.in -e out_new/$i/test$i.err | ./SPCalculator $VAR_FILE_OPTION -o out_new/$i/test$i.out
    diff out_new/$i/test$i.out tests_new/$i/expected$i.out > out_new/$i/test$i.out.diff || echo There is a problem with test $i
    
done

mkdir -p out_new/median_average_test

java SP.SPCalculatorMain -i tests_new/median_average_test/medianAverage.in -e out_new/median_average_test/medianAverage.err | ./SPCalculator -o out_new/median_average_test/medianAverage.out
    diff out_new/median_average_test/medianAverage.out tests_new/median_average_test/medianAverageExpected.out > out_new/$i/medianAverage_out.diff || echo There is a problem with test medianAverage

echo Finished all new tests
