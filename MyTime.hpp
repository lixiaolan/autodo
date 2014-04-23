#ifndef MY_TIME_LJJ
#define MY_TIME_LJJ

#include <ctime>
#include <sstream>
#include <string>

using namespace std;

class MyTime {
private:
  unsigned int year;
  unsigned int month;
  unsigned int day;
  unsigned int hour;
  unsigned int min;
  unsigned int sec;
  void get(unsigned int i);
public:
  MyTime();
  MyTime(unsigned int i);
  string date();
  void setDate(string);

  bool operator<(MyTime &);
  bool operator>(MyTime &);
  bool operator<=(MyTime &);
  bool operator>=(MyTime &);
  bool operator==(MyTime &);
  bool operator!=(MyTime &);
}; 

#endif
