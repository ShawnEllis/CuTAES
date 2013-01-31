
#ifndef CUTAES_H_
#define CUTAES_H_

/*
    CuTAES Main Class
*/

class CuTAES {
public:
    static const int DEF_W, DEF_H, KEY_ENT;

    static void onExit();

    int main();

    static CuTAES* instance();

private:
    CuTAES();

    static CuTAES *m_pInstance;
};

#endif //CUTAES_H

