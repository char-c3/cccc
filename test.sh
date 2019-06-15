#!/bin/bash
try() {
    expected="$1"
    input="$2"

    ./cccc "$input" > tmp.s
    gcc -o tmp tmp.s
    ./tmp
    actual="$?"

    if [ "$actual" = "$expected" ]; then
        echo "$input => $actual"
    else
        echo "\"$input\": $expected expected, but got $actual"
        exit 1
    fi
}

parse() {
    expected=$1
    input=$2
    ./cccc "$input"
    if [ $? = "0" ]; then
        if [ $expected = "ok" ]; then
            echo "OK, parser can parse $input"
        else
            echo "$expected expected, but result is ok"
            exit 1
        fi
    else
        if [ $expected = "ok" ]; then
            echo "$expected expected, but result is failed"
            exit 1
        else
            echo "OK, parser cannnot parse $input"
        fi
    fi
}

try 0 "0;"
try 42 "42;"
try 41 " 12 + 34 - 5 ;"
try 47 "5+6*7;"
try 15 "5*(9-6);"
try 4 "(3+5)/2;"
try 6 "5 - (-1);"
try 11 "6 + +5;"
try 11 "6 ++5;"
try 30 "6 * +5;"
try 1 "1 < 2;"
try 1 "11 > 4;"
try 0 "123 < 34;"
try 0 "1 > 888;"
try 1 "123 == 123;"
try 0 "12 == 123;"
try 1 "12 != 123;"
try 0 "123 != 123;"
try 1 "1 <= 2;"
try 1 "2 <= 2;"
try 0 "3 <= 2;"
try 1 "2 >= 1;"
try 1 "2 >= 2;"
try 0 "2 >= 3;"
try 0 "+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0;"
try 0 "+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0+0;"
parse "ok" "a;"
parse "ok" "a=1;"
parse "ok" "a + b;"
parse "ok" "a=b=2;"
parse "ok" "1;2;"
parse "ok" "a=1;b=c=222;"
parse "failed" "a=1"
try 1 "z = 1;"
try 2 "a=b=2;"
try 222 "a=1;b=c=222;"
parse "fail" "1=2;"
parse "fail" "a+1=2;"

echo all OK
