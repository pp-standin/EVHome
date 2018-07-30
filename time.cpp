#include <ctime>
#include <iostream>
#include <fstream>
#include <cstdlib>
int main() {
 	char current[20];
    std::fstream temp;
    float temperature = 0;
	temp.open("/sys/class/thermal/thermal_zone0/temp",std::ios::in);
    temp >> current;
temperature=atoi(current)/1000;
    std::time_t t = std::time(0);   // get time now
    std::tm* now = std::localtime(&t);
    std::cout << (now->tm_year + 1900) << '-' 
         << (now->tm_mon + 1) << '-'
         <<  now->tm_mday<<'='
        <<now->tm_hour<<':'
	<<now->tm_min<<':'
	<<now->tm_sec <<'-'
	<<temperature <<std::endl;
}
