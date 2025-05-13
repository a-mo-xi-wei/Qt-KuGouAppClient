/*
  RC4 - RC4 加密算法实现
*/

#include "rc4.h"

#include <stdio.h>
#include <string.h>

/** @brief 交换两个字节
 *
 *  交换两个 unsigned char 类型变量的值。
 *
 *  @param a 第一个字节的指针
 *  @param b 第二个字节的指针
 */
static void swap_byte(unsigned char* a, unsigned char* b) {
    unsigned char swapByte;

    swapByte = *a;
    *a = *b;
    *b = swapByte;
}

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
int Rc4Encrypt(const char* szPassWord, unsigned char* szSource, unsigned char * szDec, unsigned short len) {
    int ret_len = 0;

    if (szSource == NULL || szPassWord == NULL || szDec == NULL || len <= 0)
        return ret_len;

    RC4(szSource,
        len,
        (unsigned char*)szPassWord,
        (int)strlen(szPassWord),
        szDec,
        &ret_len);

    return ret_len;
}

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
int Rc4Decrypt(const char* szPassWord, unsigned char* szSource, unsigned char * szDec, unsigned short len) {
    int ret_len = 0;

    if (szSource == NULL || szPassWord == NULL || szDec == NULL || len <= 0)
        return ret_len;

    RC4(szSource,
        len,
        (unsigned char*)szPassWord,
        (int)strlen(szPassWord),
        szDec,
        &ret_len);

    return ret_len;
}

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
int RC4(const unsigned char* data, int data_len, const unsigned char* key, int key_len, unsigned char* out, int* out_len) {
    if (data == NULL || key == NULL || out == NULL)
        return 0;

    unsigned char mBox[BOX_LEN];

    if (GetKey(key, key_len, mBox) == 0)
        return 0;

    int x = 0;
    int y = 0;

    for (int k = 0; k < data_len; k++) {
        x = (x + 1) % BOX_LEN;
        y = (mBox[x] + y) % BOX_LEN;
        swap_byte(&mBox[x], &mBox[y]);
        out[k] = data[k] ^ mBox[(mBox[x] + mBox[y]) % BOX_LEN];
    }

    *out_len = data_len;

    return -1;
}

/** @brief 生成 RC4 密钥状态数组
 *
 *  根据输入的密钥生成 RC4 算法所需的状态数组。
 *
 *  @param pass 密钥数据
 *  @param pass_len 密钥长度
 *  @param out 输出的状态数组
 *  @return 成功返回 -1，失败返回 0
 */
int GetKey(const unsigned char* pass, int pass_len, unsigned char* out) {
    if (pass == NULL || out == NULL)
        return 0;

    int i;

    for (i = 0; i < BOX_LEN; i++)
        out[i] = i;

    int j = 0;
    for (i = 0; i < BOX_LEN; i++) {
        j = (pass[i % pass_len] + out[i] + j) % BOX_LEN;
        swap_byte(&out[i], &out[j]);
    }

    return -1;
}

/** @brief 计算校验和
 *
 *  计算输入数据的校验和。
 *
 *  @param buffer 输入数据
 *  @param size 数据长度
 *  @return 计算得到的校验和
 */
unsigned short checksum(unsigned char *buffer, int size) {
    unsigned int cksum = 0;
    while (size > 1) {
        cksum += *buffer++;
        size -= sizeof(unsigned char);
    }
    if (size) {
        cksum += *buffer;
    }

    cksum = (cksum >> 16) + (cksum & 0xffff);
    cksum += (cksum >> 16);
    return (unsigned short)(~cksum);
}