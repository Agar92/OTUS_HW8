#include <iostream>
#include <string>
#include <boost/regex.hpp>

using namespace std;
//g++ -I /usr/include/boost/ -L /lib/x86_64-linux-gnu/libboost_regex.so a.cpp -o a -lboost_regex

int main()
{
	string s="1239 rty";
  	boost::regex expr{"(\\d{4})\\s(\\w+)"};
  	boost::smatch what;
  	std::cout << std::boolalpha << boost::regex_match(s, what, expr) << '\n';
  	cout<<what[0]<<" tokens: "<<what[1]<<" "<<what[2]<<endl;
}