CXX = g++
CXXFLAGS = -std=c++17 -O3

TARGET = raytracer
SRC = main.cpp

all:$(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
