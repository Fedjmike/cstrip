cstrip: cstrip.cpp
	g++ -std=c++11 $< -o $@

clean:
	rm cstrip

.PHONY: clean
