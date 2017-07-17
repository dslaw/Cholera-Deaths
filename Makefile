CXX = clang++
CXXFLAGS = -std=c++14 -Wall -Wextra -Iinclude
OPENMP = -fopenmp

.PHONY = clean test


clean:
	rm -f build/* bin/*


# Extract counts, skipping headers.
data/cholera_counts.txt: data/Cholera_Deaths.csv
	tail -n +2 $< | cut -d, -f2 > $@


# Program compilation.
build/utils.o: include/utils.h src/utils.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ src/utils.cpp

build/distributions.o: include/distributions.h src/distributions.cpp
	$(CXX) $(CXXFLAGS) $(OPENMP) -c -o $@ src/distributions.cpp

build/prefetching.o: include/prefetching.h src/prefetching.cpp
	$(CXX) $(CXXFLAGS) $(OPENMP) -c -o $@ src/prefetching.cpp

build/adapt.o: include/adapt.h src/adapt.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ src/adapt.cpp


# Testing.
test:
	$(CXX) $(CXXFLAGS) -o bin/__test_utils \
		tests/test_utils.cpp \
		src/utils.cpp
	./bin/__test_utils
	$(CXX) $(CXXFLAGS) $(OPENMP) -o bin/__test_prefetching \
		tests/test_prefetching.cpp \
		src/prefetching.cpp
	./bin/__test_prefetching
