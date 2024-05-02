GCC = gcc
FLAGS = -Werror -Wextra -Wall -std=c11

all: check_format rebuild

rebuild: clean ARHgrep

ARHgrep: ARHgrep.c ARHgrep.h
	$(GCC) $(FLAGS) -o ARHgrep ARHgrep.c

clean: 
	rm -rf ARHgrep

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem *.*

format: 
	clang-format -i -style=google *.c *.h

check_format:
	clang-format -n -style=google *.c *.h
	