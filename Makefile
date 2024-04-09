build:
	gcc -Wall -Wextra -g -o tema1 tema1.c
run:
	./tema1
clean:
	rm -f tema1
pack:
	zip -FSr 312CC_Abăhnencei_Alin_Andrei.zip README.md Makefile tema1.c