CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

SRC = main.cpp SunPosition.cpp utils.cpp
OUT = app

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

run: all
	./$(OUT)

clean:
	rm -f $(OUT)
