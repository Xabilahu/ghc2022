CPPFLAGS = -Wall -Wextra -std=c++17 -pedantic -pipe -lm -O3 -D_LOCAL
EXEC_NAME = solution

.PHONY: all
all:
	$(CXX) $(CPPFLAGS) -o $(EXEC_NAME) $(src)

.PHONY: clean
clean:
	rm $(EXEC_NAME) test
