flags = -g -Wall
exec = qf.out
sources = $(wildcard src/*.c)
objects = $(sources: .c=.o)


$(exec): $(objects)
	gcc $(objects) $(flags) -o $(exec) -fPIC


%.o: %.c
	gcc -c $(flags) $< -o $@ -fPIC


install:
	make
	mv $(exec) /usr/local/bin/qf

clean:
	-rm *.out
	-rm *.o
	-rm src/*.out
	-rm src/*.o
