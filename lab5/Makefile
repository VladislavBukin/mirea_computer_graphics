TARGET = lab5

CC = g++
CFLAGS = -Wall -O2 -Werror -Wextra

LIBS = -lGL -lGLU -lglut -lGLEW -lglfw 
SRC = lab5.cpp

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)
