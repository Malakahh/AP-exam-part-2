rm -f "main.exe"
D:/cygwin64/bin/g++ -I D:/cygwin64/home/Malakahh/boost_1_58_0 Polynomial.cpp -std=c++14 main.cpp -o main -lboost_unit_test_framework
echo "--------------------------------------------------------"
main.exe