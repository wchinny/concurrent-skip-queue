# Note: 
# g++-8 is used here to specify the g++/gcc version used in our development computer. 
# change that as necessary.

.PHONY: concurrent stm clean

stm:
	g++-8 ./transactional/main.cpp -fgnu-tm -o klsm

concurrent:
	g++-8 ./concurrent/main.cpp -o klsm

clean:
	rm -f klsm *.txt




