#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>

void hex2bytes(const char *hex, unsigned char *bytes) {
    for(size_t i = 0; i < strlen(hex); i += 2) {
        sscanf(hex + i, "%2hhx", &bytes[i/2]);
    }
}

int main() {
    const char* hashString = "6b58db9ddb38fa5f385c178ac7d25212a4d2f8f8d43ff5e8d916c9d517540251";  // 将此处替换为你的hash字符串

    if (strlen(hashString) != 2*SHA256_DIGEST_LENGTH) {
        printf("Error: Invalid SHA256 hash string length.\n");
        return -1;
    }

    unsigned char hashValue[SHA256_DIGEST_LENGTH];
    hex2bytes(hashString, hashValue);

    // 打印转换后的结果，以验证
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        printf("%02x", hashValue[i]);
    }
    printf("\n");

    return 0;
}