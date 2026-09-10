
%:
	@if [ -f $@.cpp ]; then \
		g++ -std=c++20 $@.cpp -o test; \
	elif [ -f $@.hpp ]; then \
		g++ -std=c++20 $@.hpp -o test; \
	else \
		echo "Error: Neither $@.cpp nor $@.hpp found."; \
		exit 1; \
	fi