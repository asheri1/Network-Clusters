FLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors
LIBS = -lm

all: cluster

clean:
	rm -rf *.o cluster

cluster: main.o A.o B_hat.o linked_list_m.o algorithms.o spmat.o checks.o
	gcc main.o A.o B_hat.o linked_list_m.o algorithms.o spmat.o checks.o  -o cluster $(LIBS)

main.o: main.c
	gcc $(FLAGS) -c main.c

A.o: A.c
	gcc $(FLAGS) -c A.c

B_hat.o: B_hat.c
	gcc $(FLAGS) -c B_hat.c

linked_list_m.o: linked_list_m.c
	gcc $(FLAGS) -c linked_list_m.c

algorithms.o: algorithms.c
	gcc $(FLAGS) -c algorithms.c

spmat.o: spmat.c
	gcc $(FLAGS) -c spmat.c

checks.o: checks.c
	gcc $(FLAGS) -c checks.c
