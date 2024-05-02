#!/bin/bash

FUNC_SUCCESS=0
FUNC_FAIL=0
COUNTER=0
DIFF=""

declare -a flags=(
"-i"
"-v"
"-c"
"-l"
"-n"
"-r"
)

declare -a files=(
"s21_greptest1.txt"
"s21_greptest1.txt s21_greptest2.txt"
"nofile.txt"
)

for opt1 in "${flags[@]}"
do
    for opt2 in "${flags[@]}"
    do
        for opt3 in "${flags[@]}"
        do
            for opt4 in "${flags[@]}"
            do
                if [ $opt1 != $opt2 ] && [ $opt1 != $opt3 ] \
                && [ $opt1 != $opt4 ] && [ $opt2 != $opt3 ] \
                && [ $opt2 != $opt4 ] && [ $opt3 != $opt4 ]
                then
                    for file in "${files[@]}"
                    do
                        options="-e ut $opt1 $opt2 $opt3 $opt4"
                        #valgrind --log-file="s21_grep_testing.log" --tool=memcheck --leak-check=yes ./s21_grep $options $file > s21_grep_testing1.log
                        leaks -quiet -atExit -- ./s21_grep $options > test_s21_grep.log
                        leak=$(grep -A100000 leaks test_s21_grep.log)
                        (( COUNTER++ ))
                        if [[ $leak == *"0 leaks for 0 total leaked bytes"* ]]
                        then
                            (( FUNC_SUCCESS++ ))
                            echo "grep $str LEAKS SUCCESS $COUNTER"
                        else
                            (( FUNC_FAIL++ ))
                            echo "grep $str LEAKS FAIL $COUNTER"
                            cp test_s21_grep.log ../s21grep.log
                            exit
                        fi
                        rm test_s21_grep.log
                        #rm s21_grep_testing1.log
                    done
                fi
            done
        done
    done
done



echo "FAIL: $FUNC_FAIL"
echo "SUCCESS: $FUNC_SUCCESS"
echo "ALL: $COUNTER"
