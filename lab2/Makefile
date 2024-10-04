TARGET = lab2

CC = g++
CFLAGS = -Wall -O2 -Werror -Wextra

LIBS = -lGL -lGLU -lglut

SRC = lab2.cpp

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)
