performance_test: *.c *.h
	gcc performance_test.c -DHASH=STR hashmap.c list.c murmur.c -g -o performance_test

basic_test: *.c *.h
	gcc basic_test.c hashmap.c list.c murmur.c -o basic_test
