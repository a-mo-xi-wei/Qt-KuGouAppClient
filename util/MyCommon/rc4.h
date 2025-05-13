/*
RC4 - RC4 加密算法实现
*/

#ifndef RC4_H
#define RC4_H

#if defined(__cplusplus)
extern "C" {
#endif

    /** @def RC4_KEY
     *  @brief 默认 RC4 加密密钥
     */
#define RC4_KEY "ergh5683^#*@reyv"

    /** @def BOX_LEN
     *  @brief RC4 算法的状态数组长度
     */
#define BOX_LEN 256

    /** @brief 生成 RC4 密钥状态数组
     *
     *  根据输入的密钥生成 RC4 算法所需的状态数组。
     *
     *  @param pass 密钥数据
     *  @param pass_len 密钥长度
     *  @param out 输出的状态数组
     *  @return 成功返回 -1，失败返回 0
     */
    int GetKey(const unsigned char* pass, int pass_len, unsigned char* out);

    /** @brief 执行 RC4 加密/解密
     *
     *  使用 RC4 算法对输入数据进行加密或解密，输出结果到指定缓冲区。
     *
     *  @param data 输入数据
     *  @param data_len 输入数据长度
     *  @param key 密钥
     *  @param key_len 密钥长度
     *  @param out 输出缓冲区
     *  @param out_len 输出数据的长度
     *  @return 成功返回 -1，失败返回 0
     */
    int RC4(const unsigned char* data, int data_len, const unsigned char* key, int key_len, unsigned char* out, int* out_len);

    /** @brief 计算校验和
     *
     *  计算输入数据的校验和。
     *
     *  @param buffer 输入数据
     *  @param size 数据长度
     *  @return 计算得到的校验和
     */
    unsigned short checksum(unsigned char *buffer, int size);

    /** @brief RC4 加密
     *
     *  使用指定的密码对输入数据进行 RC4 加密。
     *
     *  @param szPassWord 加密密码
     *  @param szSource 输入数据
     *  @param szDec 输出加密结果
     *  @param len 输入数据长度
     *  @return 加密结果的长度，失败返回 0
     */
    int Rc4Encrypt(const char* szPassWord, unsigned char* szSource, unsigned char * szDec, unsigned short len);

    /** @brief RC4 解密
     *
     *  使用指定的密码对输入数据进行 RC4 解密。
     *
     *  @param szPassWord 解密密码
     *  @param szSource 输入加密数据
     *  @param szDec 输出解密结果
     *  @param len 输入数据长度
     *  @return 解密结果的长度，失败返回 0
     */
    int Rc4Decrypt(const char* szPassWord, unsigned char* szSource, unsigned char * szDec, unsigned short len);

#if defined(__cplusplus)
}
#endif

#endif // RC4_H