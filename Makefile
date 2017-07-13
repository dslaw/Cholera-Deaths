CXX = clang++
CXXFLAGS = -std=c++14 -Wall -Wextra -Iinclude

.PHONY = clean test


clean:
	rm -f build/* bin/*


# Extract counts, skipping headers.
data/cholera_counts.txt: data/Cholera_Deaths.csv
	tail -n +2 $< | cut -d, -f2 > $@


# Program compilation.
build/utils.o: include/utils.h src/utils.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ src/utils.cpp


# Testing.
test:
	$(CXX) $(CXXFLAGS) -o bin/__test_utils \
		tests/test_utils.cpp \
		src/utils.cpp
	./bin/__test_utils
