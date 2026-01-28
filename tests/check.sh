for f in *.test; do echo $f; diff --strip-trailing-cr $f `basename $f .test`.txt; done
for f in *.ttt; do echo $f; diff --strip-trailing-cr $f `basename $f .ttt`.txt; done
