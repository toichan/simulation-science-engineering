CC = gcc
CFLAGS = -Wall -O2
SRC = src/Q5.c
OUT = Q5
DATA_FILE = data/u_values.dat

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUT) $(SRC)

run: $(OUT)
	./$(OUT) > $(DATA_FILE)

clean:
	rm -f $(OUT) $(DATA_FILE)