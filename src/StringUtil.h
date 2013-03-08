#include <sstream>
#include <string>

class StringUtil {
public:
    static void trimEnd(std::string &str) {
        int pos = str.find_last_not_of(' ');
        str = str.substr(0,pos+1);
    }
    
    static void trimStart(std::string &str) {
        int pos = str.find_first_not_of(' ');
        str = str.substr(pos);
    }
    
    static void trim(std::string& str) {
        trimEnd(str);
        trimStart(str);
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
    
    static std::string pad(const std::string& str, int l) {
        if (str.size() >= l) {
            return str;
        }
        std::string str2(l - str.size(), ' ');
        return str + str2;
    }
    
    static std::string itos(int i) {
        std::stringstream ss;
        ss << i;
        return ss.str();
    }
    
    static std::string ftos(float f) {
        std::stringstream ss;
        ss << f;
        return ss.str();
    }

};
