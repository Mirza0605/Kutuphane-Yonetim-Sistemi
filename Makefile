# Makefile

# Derleyici
CXX = g++
# Derleyici bayrakları
CXXFLAGS = -Wall -std=c++17
# Hedef dosya adı
TARGET = program

# Varsayılan hedef
all: $(TARGET)

# Derleme komutu
$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.cpp

# Temizleme işlemi
clean:
	rm -f $(TARGET)
