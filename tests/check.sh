(for f in *.test; do echo $f; diff --strip-trailing-cr $f `basename $f .test`.txt; done) > test.out
(for f in *.ttt; do echo $f; diff --strip-trailing-cr $f `basename $f .ttt`.txt; done) >> test.out
if cmp test.out check.out
    then echo "TEST OK"
    else (echo "TEST FAILED"; cat test.out)
fi
