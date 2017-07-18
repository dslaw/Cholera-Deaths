CXX = clang++
CXXFLAGS = -std=c++14 -Wall -Wextra -fopenmp -Iinclude
OBJ_FILES = $(patsubst src/%.cpp, build/%.o, $(wildcard src/*.cpp))

.PHONY = clean test


clean:
	rm -f build/* bin/*


# Extract counts, skipping headers.
data/cholera_counts.txt: data/Cholera_Deaths.csv
	tail -n +2 $< | cut -d, -f2 > $@


# Program compilation.
build/%.o: src/%.cpp include/%.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

bin/sampler: sampler.cpp $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^


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
