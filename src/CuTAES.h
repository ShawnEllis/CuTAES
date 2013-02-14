/*
 *  CuTAES
 *
 *  Main class
 *
 *  Singleton which starts application and provides commonly used
 *  info such as screen size, file directory, current user, etc.
 *
 *  TODO: Switch m_workingDirectory before submitting file
 *
 */

#ifndef CUTAES_H
#define CUTAES_H

#define DEBUG

#include <string>

class Student;

class CuTAES {
public:
    static const int DEF_W, DEF_H, KEY_ENT;
    
    void setWorkingDirectory(const std::string &dir) {m_workingDirectory = dir;}
    const std::string& getWorkingDirectory() {return m_workingDirectory;}
    const std::string getDataDirectory() {return m_workingDirectory + "data/";}
    
    Student *getActiveUser();
    
    int main(int argc, const char* argv[]);
    static void onExit();
    
    static CuTAES* instance();

private:
    CuTAES();
    
    std::string m_workingDirectory;
    
    Student *m_pActiveUser;
    
    static CuTAES *m_pInstance;
};

#endif //CUTAES_H

