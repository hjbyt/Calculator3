for i in {1..5}
do 
    mkdir -p out/$i
    java SP.SPCalculatorMain -i tests/$i/input$i.in -e out/$i/test$i.err | ./SPCalculator > out/$i/test$i.out
    diff out/$i/test$i.out tests/$i/expected$i.out > out/$i/test$i.out.diff || echo There is a problem with test $i
    diff out/$i/test$i.err tests/$i/expected$i.err > out/$i/test$i.err.diff || echo There is a problem with test $i
done

echo Finished all the tests