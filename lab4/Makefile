TARGET = lab4

CC = g++
CFLAGS = -Wall -O2 -Werror -Wextra

LIBS = -lGL -lGLU -lglut

SRC = lab4.cpp

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)
