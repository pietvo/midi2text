all:
	echo "make clean or make test"

test:
	make clean
	sh test.sh
	sh check.sh
	make clean
	sh test-redir.sh
	sh check.sh

clean:
	rm -f test.out *.mmm *.test *.ttt *~
