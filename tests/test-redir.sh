echo '../mf2t  bintest.mid bintest.test'
../mf2t  bintest.mid bintest.test
echo '../mf2t example1.mid example1.test'
../mf2t example1.mid example1.test
echo '../mf2t example2.mid example2.test'
../mf2t example2.mid example2.test
echo '../mf2t example3.mid example3.test'
../mf2t example3.mid example3.test
echo '../mf2t example4.mid example4.test'
../mf2t example4.mid example4.test
echo '../mf2t example5.mid example5.test'
../mf2t example5.mid example5.test

echo '../t2mf <  bintest.txt > bintest.mmm'
../t2mf <  bintest.txt > bintest.mmm
echo '../t2mf < example1.txt > example1.mmm'
../t2mf < example1.txt > example1.mmm
echo '../t2mf < example2.txt > example2.mmm'
../t2mf < example2.txt > example2.mmm
echo '../t2mf < example3.txt > example3.mmm'
../t2mf < example3.txt > example3.mmm
echo '../t2mf < example4.txt > example4.mmm'
../t2mf < example4.txt > example4.mmm
echo '../t2mf < example5.txt > example5.mmm'
../t2mf < example5.txt > example5.mmm

echo '../mf2t <  bintest.mmm > bintest.ttt'
../mf2t <  bintest.mmm > bintest.ttt
echo '../mf2t < example1.mmm > example1.ttt'
../mf2t < example1.mmm > example1.ttt
echo '../mf2t < example2.mmm > example2.ttt'
../mf2t < example2.mmm > example2.ttt
echo '../mf2t < example3.mmm > example3.ttt'
../mf2t < example3.mmm > example3.ttt
echo '../mf2t < example4.mmm > example4.ttt'
../mf2t < example4.mmm > example4.ttt
echo '../mf2t < example5.mmm > example5.ttt'
../mf2t < example5.mmm > example5.ttt

# Test with
# for f in *.test; do diff --strip-trailing-cr $f `basename $f .test`.txt; done
# for f in *.ttt; do diff --strip-trailing-cr $f `basename $f .ttt`.txt; done
