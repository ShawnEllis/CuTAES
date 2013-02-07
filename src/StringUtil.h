#include <string>

class StringUtil {
public:
    static void trimEnd(std::string &str) {
        int pos = str.find_last_not_of(' ');
        str = str.substr(0,pos+1);
    }
    
    static int count(const std::string& str, char c) {
        int count = 0;
        for (int i = 0; i < str.size(); i++) {
            if (str[i] == c) {
                count++;   
            }
        }
        return count;
    }

};
