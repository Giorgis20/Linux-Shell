exc1.o: exc1.c
	gcc -c exc1.c
shell: exc1.o
	gcc -o shell exc1.o
all: shell
clean:
	rm shell
	rm exc1.o
