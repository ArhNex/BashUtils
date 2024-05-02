GCC = gcc
FLAGS = -Werror -Wextra -Wall -std=c11

all: check_format rebuild

rebuild: clean ARHcat

ARHcat: ARHcat.c ARHcat.h
	$(GCC) $(FLAGS) -o ARHcat ARHcat.c

clean: 
	rm -rf ARHcat

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem *.*

format: 
	clang-format -i -style=google *.c *.h

check_format:
	clang-format -n -style=google *.c *.h
	