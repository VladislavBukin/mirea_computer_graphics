TARGET = lab1

CC = g++
CFLAGS = -Wall -O2

LIBS = -lGL -lGLU -lglut

SRC = lab1.cpp

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)
