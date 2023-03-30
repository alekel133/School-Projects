all: atk133-linecount

atk133-linecout: atk133-linecount.o
	gcc -o atk133-linecount atk133-linecount.o

atk133-linecount.o: atk133-linecount.c
	gcc -c -pthread -o atk133-linecount.o atk133-linecount.c

clean:
	rm -rf *.o
