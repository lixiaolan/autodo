#include "MyTime.hpp"

void MyTime::get(unsigned int shift) {
  time_t t = time(0);   // get time now
  t = t + (shift*60*60*24);
  struct tm * now = localtime( & t );
  stringstream ss;
  ss << (1900+now->tm_year) << " " 
     << (now->tm_mon + 1) << " "
     << now->tm_mday;
  ss >> year;
  ss >> month;
  ss >> day;
};

MyTime::MyTime() { get(0); };

MyTime::MyTime(unsigned int i) {get(i); };

string MyTime::date() {
  stringstream ss;
  string zm, zd;
  zm = (month < 10) ? "0" : "";
  zd = (day < 10) ? "0" : "";
  ss << year << zm << month << zd << day;
  return ss.str();
};

void MyTime::setDate(string in) {
  if (in.size() == 8) {
    stringstream ss;
    ss << in.substr(0,4) << " " 
       << in.substr(4,2) << " "
       << in.substr(6,2);
    ss >> year >> month >> day;
  }
  else {
    get(0);
  }
};

bool MyTime::operator<(MyTime & RhsT) {
  stringstream sst,ssr;
  unsigned int it, ir;
  sst << date();
  sst >> it;
  ssr << RhsT.date();
  ssr >> ir;
  return (it < ir);
};
bool MyTime::operator>(MyTime & RhsT) {
  stringstream sst,ssr;
  unsigned int it, ir;
  sst << date();
  sst >> it;
  ssr << RhsT.date();
  ssr >> ir;
  return (it > ir);
};
bool MyTime::operator<=(MyTime & RhsT) {
  stringstream sst,ssr;
  unsigned int it, ir;
  sst << date();
  sst >> it;
  ssr << RhsT.date();
  ssr >> ir;
  return (it <= ir);
};
bool MyTime::operator>=(MyTime & RhsT) {
  stringstream sst,ssr;
  unsigned int it, ir;
  sst << date();
  sst >> it;
  ssr << RhsT.date();
  ssr >> ir;
  return (it >= ir);
};
bool MyTime::operator==(MyTime & RhsT) {
  stringstream sst,ssr;
  unsigned int it, ir;
  sst << date();
  sst >> it;
  ssr << RhsT.date();
  ssr >> ir;
  return (it == ir);
};
bool MyTime::operator!=(MyTime & RhsT) {
  stringstream sst,ssr;
  unsigned int it, ir;
  sst << date();
  sst >> it;
  ssr << RhsT.date();
  ssr >> ir;
  return (it != ir);
};
