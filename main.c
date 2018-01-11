#include <stdio.h>
#include <stdlib.h>
#include "main.h"

int main() {
    srand(0);
    arbitrary = Rtext();
    PlainInit();
    key = Rtext();
    KeyInit();
    for (long i = 0; i < N; ++i) {
        for (int j = 0; j < 5; ++j) {
            AESENC(plain[i], j);
        }
    }
    printf("%x", plain[45245].bytes[2][3]);
    return 0;
}

struct text Rtext(){
    struct text rtext;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            rtext.bytes[i][j] = (u8) (rand() % 256);
        }
    }
    return rtext;
}

struct text SBox(struct text plain){
    struct text cipher;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            cipher.bytes[i][j] = S[plain.bytes[i][j]];
        }
    }
    return cipher;
}

struct text ShiftRow(struct text plain){
    struct text cipher;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            cipher.bytes[i][j] = S[plain.bytes[i][j]];
        }
    }
    return cipher;
}

struct text MixedColumn(struct text plain){
    struct text cipher;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            cipher.bytes[i][j] = table_2[plain.bytes[i % 4][j]];
            cipher.bytes[i][j] ^= table_3[plain.bytes[(i + 1) % 4][j]];
            cipher.bytes[i][j] ^= plain.bytes[(i + 2) % 4][j];
            cipher.bytes[i][j] ^= plain.bytes[(i + 3) % 4][j];
        }
    }
    return cipher;
}

struct text AddKey(struct text plain, struct text key){
    struct text cipher;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            cipher.bytes[i][j] = plain.bytes[i][j] ^ key.bytes[i][j];
        }
    }
    return cipher;
}

struct text KeySchlude(struct text key, int i){
    struct text nextkey;
    for (int j = 0; j < 4; ++j) {
        nextkey.bytes[j][0] = S[key.bytes[(j + 1) % 4][3]];
    }
    nextkey.bytes[0][0] ^= rcon[i];
    for (int k = 1; k < 4; ++k) {
        for (int j = 0; j < 4; ++j) {
            nextkey.bytes[j][k] = nextkey.bytes[j][k-1] ^ key.bytes[j][k];
        }
    }
    return nextkey;
}

void KeyInit(){
    mainkey[0] = key;
    for (int i = 1; i < R; ++i) {
        mainkey[i] = KeySchlude(mainkey[i-1],i);
    }
}

struct text AESENC(struct text plain, int round){
    struct text cipher;
    cipher = SBox(plain);
    cipher = ShiftRow(cipher);
    cipher = MixedColumn(cipher);
    AddKey(cipher, mainkey[round]);
    return cipher;
}

void PlainInit(){
    u8 p0, p1, p2, p3;
    for (long i = 0; i < N; ++i) {
        p0 = (u8) (i % 256);
        p1 = (u8) (((i - p0) / 256) % 256);
        p2 = (u8) (((i - p1) / D16) % 256);
        p3 = (u8) (((i - p1) / D24) % 256);
        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 4; ++k) {
                plain[i].bytes[j][k] = arbitrary.bytes[j][k];
                if (j == k){
                    switch (j){
                        case 0 :
                            plain[i].bytes[j][k] ^= p0;
                            break;
                        case 1 :
                            plain[i].bytes[j][k] ^= p1;
                            break;
                        case 2 :
                            plain[i].bytes[j][k] ^= p2;
                            break;
                        case 3 :
                            plain[i].bytes[j][k] ^= p3;
                            break;
                    }
                }
            }
        }
    }
}