#include <string>

using namespace std;

class StringUtil {
public:
    static void trimEnd(std::string &str) {
        int pos = str.find_last_not_of(' ');
        str = str.substr(0,pos+1);
    }

};
