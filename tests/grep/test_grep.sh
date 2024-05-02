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

declare -a opt_f=(
"ut ut"
"-e ut -e in"
"ut"
)

declare -a files=(
"s21_greptest1.txt"
"s21_greptest1.txt s21_greptest2.txt"
"nofile.txt"
)

declare -a commline=(
"OPT FILE"
)



function testing()
{
    str=$(echo $@ | sed "s/OPT/$options/")
    str=$(echo $str | sed -e "s/FILE/$file/g")
    ./s21_grep $str > s21_grep_testing.log
    grep $str > system_grep_testing.log
    DIFF="$(diff -s s21_grep_testing.log system_grep_testing.log)"
    (( COUNTER++ ))
    if [ "$DIFF" == "Files s21_grep_testing.log and system_grep_testing.log are identical" ]
    then
        (( FUNC_SUCCESS++ ))
        echo "grep $str FUNCTIONALITY SUCCESS $COUNTER"
    else
        (( FUNC_FAIL++ ))
        echo "grep $str FUNCTIONALITY FAIL $COUNTER"
        cp s21_grep_testing.log ../s21grep.log
        cp system_grep_testing.log ../grep.log
        exit
    fi
    rm s21_grep_testing.log system_grep_testing.log
}

for opt1 in "${flags[@]}"
do
    for fl_op in "${opt_f[@]}"
    do 
        for file in "${files[@]}"
        do
            options="$opt1 $fl_op"
            testing $commline
        done
    done
done


for opt1 in "${flags[@]}"
do
    for fl_op in "${opt_f[@]}"
    do 
        for file in "${files[@]}"
        do
            options="$fl_op $opt1 "
            testing $commline
        done
    done
done

for opt1 in "${flags[@]}"
do
    for opt2 in "${flags[@]}"
    do
        if [ $opt1 != $opt2 ]
        then
            for file in "${files[@]}"
            do
                options="$opt1 -e ut $opt2"
                testing $commline
            done
        fi
    done
done



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
                        options="$opt1 -e ut $opt2 $opt3 $opt4"
                        testing $commline
                    done
                fi
            done
        done
    done
done

echo "FAIL: $FUNC_FAIL"
echo "SUCCESS: $FUNC_SUCCESS"
echo "ALL: $COUNTER"
