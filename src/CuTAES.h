#ifndef CUTAES_H
#define CUTAES_H

#define DEBUG

#include <string>

/*
    CuTAES Main Class
*/

class CuTAES {
public:
    static const int DEF_W, DEF_H, KEY_ENT;
    
    void setWorkingDirectory(const std::string &dir);
    const std::string& getWorkingDirectory() {return m_workingDirectory;}
    const std::string getDataDirectory() {return m_workingDirectory + "data/";}
    
    int main(int argc, const char* argv[]);
    static void onExit();
    
    static CuTAES* instance();

private:
    CuTAES();

    std::string m_workingDirectory;
    static CuTAES *m_pInstance;
};

#endif //CUTAES_H

