NVCC = /usr/local/cuda/bin/nvcc
CXX = g++
CXXFLAGS = -std=c++11 -I/usr/local/cuda/include
LDFLAGS = -L/usr/local/cuda/lib64 -lcudart -lnppc -lnppial -lnppicc -lnppidei -lnppif -lnppig -lnppim -lnppist -lnppisu -lnppitc

TARGET = bin/imageRotationNPP
SRC = src/imageRotationNPP.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	mkdir -p bin
	$(NVCC) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	rm -rf bin/*

help:
	@echo "make          - Build the project"
	@echo "make clean    - Clean build"
	@echo "make help     - Show this message"