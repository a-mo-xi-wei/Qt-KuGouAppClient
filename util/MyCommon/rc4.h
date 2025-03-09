#ifndef RC4_H
#define RC4_H

#if defined(__cplusplus)
extern "C" {
#endif

    #define RC4_KEY "ergh5683^#*@reyv"

    #define BOX_LEN 256

    int GetKey(const unsigned char* pass, int pass_len, unsigned char *out);
    int RC4(const unsigned char* data, int data_len, const unsigned char* key, int key_len, unsigned char* out, int* out_len);
    unsigned short checksum(unsigned char *buffer, int size);

    int Rc4Encrypt(const char* szPassWord,unsigned char* szSource,unsigned char * szDec,unsigned short len); // 加密，返回加密结果
    int Rc4Decrypt(const char* szPassWord,unsigned char* szSource,unsigned char * szDec,unsigned short len); // 解密，返回解密结果

#if defined(__cplusplus)
}
#endif

#endif // RC4_H
