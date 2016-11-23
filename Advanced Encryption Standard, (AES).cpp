#include <iostream>
using namespace std;
 
/* Define */
#define xtime(x) ((x << 1) ^ (((x >> 7) & 1) * 0x1b))
/* 상태 배열의 각 열을 GF(2^8) 상에서의 다항식들로 고정된 다항식와 곱셈 수행하는 매크로 */
#define Multiply(x,y) (((y & 1) * x) ^ ((y>>1 & 1) * xtime(x)) ^ ((y>>2 & 1) * xtime(xtime(x))) ^ ((y>>3 & 1) * xtime(xtime(xtime(x)))) ^ ((y>>4 & 1) * xtime(xtime(xtime(xtime(x))))))
#define Nb 4 /* Block Length */
#define Nk 4 /* Key Length */
#define Nr 10 /* Round Number */
 
/* 암호화 과정 동안 중간 결과를 저장하는 배열 */
unsigned char state[4][4] = { 0, };
/* Round Key를 저장하는 배열 */
unsigned char RoundKey[240] = { 0, };
/* AES 암/복호화 과정에서 사용하는 Key에 사용될 값 저장하는 배열 */
unsigned char key[32] = { 0x00  ,0x01  ,0x02  ,0x03  ,0x04  ,0x05  ,0x06  ,0x07  ,0x08  ,0x09  ,0x0a  ,0x0b  ,0x0c  ,0x0d  ,0x0e  ,0x0f };
 
/* 암호화시 사용하는 256가지의 모든 가능한 8Bit 값의 순열을 포함하고 있는 16 x 16 크기의 Byte Array이다. */
static const unsigned char Sbox[256] =
{
    0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
    0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
    0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
    0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
    0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
    0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
    0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
    0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
    0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
    0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
    0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
    0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
    0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
    0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
    0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
    0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};
 
/* 복호화시 사용하는 256가지의 모든 가능한 8Bit 값의 순열을 포함하고 있는 16 x 16 크기의 Byte Array이다. */
static const unsigned char In_Sbox[256] =
{
    0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
    0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
    0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
    0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
    0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
    0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
    0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,
    0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
    0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
    0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
    0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
    0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
    0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
    0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
    0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D
};
 
/* 각 라운드 상수에 대한 4Byte에 대한 Array */
static const unsigned char Rcon[256] =
{
    0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a,
    0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39,
    0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a,
    0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8,
    0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef,
    0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc,
    0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b,
    0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3,
    0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94,
    0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20,
    0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35,
    0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f,
    0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04,
    0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63,
    0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd,
    0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d
};
 
/* S-Box를 이용하여 1Byte 단위로 치환하는 함수 */
void SubBytes()
{
    /* 총 16번의 반복을 통하여 상태 배열의 각 Byte 값을 S-Box 표에서 알맞은 값으로 치환한다. */
    for (int i = 0; i<4; i++)
    {
        for (int j = 0; j < Nb; j++)
        { state[i][j] = Sbox[state[i][j]]; }
    }
}
 
/* In_Sbox를 이용하여 1Byte 단위로 치환하는 함수 */
void InvSubBytes()
{
    /* 총 16번의 반복을 통하여 상태 배열의 각 Byte 값을 Inverse S-Box 표에서 알맞은 값으로 치환한다. */
    for (int i = 0; i<4; i++)
    {
        for (int j = 0; j<4; j++)
        { state[i][j] = In_Sbox[state[i][j]]; }
    }
}
 
/* 암호화시 Byte 치환이 이루어진 값을 Bit 단위로 뒤섞는 함수 */
void ShiftRows()
{
    unsigned char temp = 0;
 
    /* 첫 번째 행을 기준으로 왼쪽으로 순환시킨다. */
    temp = state[1][0];
    state[1][0] = state[1][1];
    state[1][1] = state[1][2];
    state[1][2] = state[1][3];
    state[1][3] = temp;
 
    /* 두 번째 행을 기준으로 왼쪽으로 순환시킨다. */
    temp = state[2][0];
    state[2][0] = state[2][2];
    state[2][2] = temp;
 
    temp = state[2][1];
    state[2][1] = state[2][3];
    state[2][3] = temp;
 
    /* 셋 번째 행을 기준으로 왼쪽으로 순환시킨다. */
    temp = state[3][0];
    state[3][0] = state[3][3];
    state[3][3] = state[3][2];
    state[3][2] = state[3][1];
    state[3][1] = temp;
}
 
/* 복호화시 Byte 치환이 이루어진 값을 Bit 단위로 뒤섞는 함수 */
void InvShiftRows()
{
    unsigned char temp;
 
    /* 첫 번째 행을 기준으로 오른쪽으로 순환시킨다. */
    temp = state[1][3];
    state[1][3] = state[1][2];
    state[1][2] = state[1][1];
    state[1][1] = state[1][0];
    state[1][0] = temp;
 
    /* 두 번째 행을 기준으로 오른쪽으로 순환시킨다. */
    temp = state[2][0];
    state[2][0] = state[2][2];
    state[2][2] = temp;
 
    temp = state[2][1];
    state[2][1] = state[2][3];
    state[2][3] = temp;
 
    /* 셋 번째 행을 기준으로 오른쪽으로 순환시킨다. */
    temp = state[3][0];
    state[3][0] = state[3][1];
    state[3][1] = state[3][2];
    state[3][2] = state[3][3];
    state[3][3] = temp;
}
 
/* 암호화시 Bit 단위로 뒤섞인 값을 통해서 4Bit 단위로 Bit 연산 함수 */
void MixColumns()
{
    unsigned char Tmp, Tm, t;
 
    /* 총 4번의 For문을 수행하면서 상태 배열의 각 4열과 고정된 다항식 행렬을 곱한다. */
    for (int i = 0; i<4; i++)
    {
        t = state[0][i];
 
        /* 반복문을 수행하면서 상태 배열의 i(0~3)열과 고정된 다항식 행렬의 곱셈 연산을 수행하여 Tm에 저장한다. */
        /* 열 혼합이 완료된 Temp와 0x000000FF를 Bit 단위 AND 연산하여 얻어진 Byte 값을 다시 상태 배열의 알맞은 위치에 저장한다. */
        t = state[0][i];
        Tmp = state[0][i] ^ state[1][i] ^ state[2][i] ^ state[3][i];
        Tm = state[0][i] ^ state[1][i]; Tm = xtime(Tm); state[0][i] ^= Tm ^ Tmp;
        Tm = state[1][i] ^ state[2][i]; Tm = xtime(Tm); state[1][i] ^= Tm ^ Tmp;
        Tm = state[2][i] ^ state[3][i]; Tm = xtime(Tm); state[2][i] ^= Tm ^ Tmp;
        Tm = state[3][i] ^ t; Tm = xtime(Tm); state[3][i] ^= Tm ^ Tmp;
    }
}
 
/* 복호화시 Bit 단위로 뒤섞인 값을 통해서 4Bit 단위로 Bit 연산 함수 */
void InvMixColumns()
{
    unsigned char a, b, c, d;
    for (int i = 0; i<4; i++)
    {
 
        a = state[0][i];
        b = state[1][i];
        c = state[2][i];
        d = state[3][i];
 
        state[0][i] = Multiply(a, 0x0e) ^ Multiply(b, 0x0b) ^ Multiply(c, 0x0d) ^ Multiply(d, 0x09);
        state[1][i] = Multiply(a, 0x09) ^ Multiply(b, 0x0e) ^ Multiply(c, 0x0b) ^ Multiply(d, 0x0d);
        state[2][i] = Multiply(a, 0x0d) ^ Multiply(b, 0x09) ^ Multiply(c, 0x0e) ^ Multiply(d, 0x0b);
        state[3][i] = Multiply(a, 0x0b) ^ Multiply(b, 0x0d) ^ Multiply(c, 0x09) ^ Multiply(d, 0x0e);
    }
}
 
/* 상태 배열 128 Bit와 라운드 키 128Bit 간의 Bit 단위 XOR 연산 수행 함수 */
void AddRoundKey(int round)
{
    for (int i = 0; i<4; i++)
    {
        /* 상태 배열와 Round Key를 XOR 연산을 통하여 Round Key 값을 추가한다. */
        for (int j = 0; j<4; j++)
        { state[j][i] ^= RoundKey[round * Nb * 4 + i * Nb + j]; }
    }
}
 
/* 각 라운드에서 사용하는 라운드키를 생성하기 위해 키 확장 과정 수행 함수 */
void KeyExpansion()
{
    int i, j;
    unsigned char temp[4], k;
 
    /* 첫 번째 Round Key에 Key 값을 저장한다. */
    for (i = 0; i<Nk; i++)
    {
        RoundKey[i * 4] = key[i * 4];
        RoundKey[i * 4 + 1] = key[i * 4 + 1];
        RoundKey[i * 4 + 2] = key[i * 4 + 2];
        RoundKey[i * 4 + 3] = key[i * 4 + 3];
    }
 
    /* 이전의 Round Key로부터 다른 모든 Round Key를 생성한다. */
    while (i < (Nb * (Nr + 1)))
    {
        for (j = 0; j<4; j++)
        {
            temp[j] = RoundKey[(i - 1) * 4 + j];
        }
        if (i % Nk == 0)
        {
            /* Word 단위로 순환이동 시키는 함수 (RotWord) */
            {
                k = temp[0];
                temp[0] = temp[1];
                temp[1] = temp[2];
                temp[2] = temp[3];
                temp[3] = k;
            } /* B0, B1, B2, B3 순서를 B1, B2, B3, B0 로 만든다. */
 
            /* 네개의 Byte를 S-Box를 통해서 치환 (SubWord) */
            {
                temp[0] = Sbox[temp[0]];
                temp[1] = Sbox[temp[1]];
                temp[2] = Sbox[temp[2]];
                temp[3] = Sbox[temp[3]];
            }
 
            temp[0] = temp[0] ^ Rcon[i / Nk];
        }
        else if (Nk > 6 && i % Nk == 4)
        {
            /* 네개의 Byte를 S-Box를 통해서 치환 (SubWord) */
            {
                temp[0] = Sbox[temp[0]];
                temp[1] = Sbox[temp[1]];
                temp[2] = Sbox[temp[2]];
                temp[3] = Sbox[temp[3]];
            }
        }
        RoundKey[i * 4 + 0] = RoundKey[(i - Nk) * 4 + 0] ^ temp[0];
        RoundKey[i * 4 + 1] = RoundKey[(i - Nk) * 4 + 1] ^ temp[1];
        RoundKey[i * 4 + 2] = RoundKey[(i - Nk) * 4 + 2] ^ temp[2];
        RoundKey[i * 4 + 3] = RoundKey[(i - Nk) * 4 + 3] ^ temp[3];
        i++;
    }
}
 
/* Cipher and Inverse Cipher Implementations 함수 */
int Implement_Cipher(unsigned char * in, unsigned char * out, bool mSwitch)
{
    /* 입력받은 1차원 배열(128 Bit)의 평문을 2차원 상태 배열에 열 단위로 저장한다. */
    for (int i = 0; i<4; i++)
    {
        for (int j = 0; j<Nb; j++)
        {
            state[j][i] = in[i * 4 + j];
        }
    }
 
    /* Switch Cipher Implementations */
    switch (mSwitch)
    {
        /* Cipher Implementations */
    case (true):
    {
        AddRoundKey(0); // 첫 번째 라운드 키 추가
        for (int round = 1; round<Nr; round++) // 9번의 라운드를 돌면서 4단계 연산
        {
            SubBytes(); // 바이트 치환
            ShiftRows(); // 행 이동
            MixColumns(); // 열 혼합
            AddRoundKey(round); // 확장된 키와 현재 블록을 XOR 연산
        }
 
        SubBytes(); // 바이트 치환
        ShiftRows(); // 열 이동
        AddRoundKey(Nr); // 확장된 키 중 마지막 키와 현재 블록을 XOR 연산
        break;
    }
    /* Inverse Cipher Implementations */
    case (false):
    {
        AddRoundKey(Nr);
        for (int round = Nr - 1; round>0; round--)
        {
            InvShiftRows(); // 역 행 이동
            InvSubBytes(); // 역 바이트 치환
            AddRoundKey(round);
            InvMixColumns(); // 역 열 혼합
        }
 
        InvShiftRows(); // 역 행 이동
        InvSubBytes(); // 역 바이트 치환
        AddRoundKey(0); // 확장된 키 중 첫번째 키와 현재 블록을 XOR 연산
        break;
    }
    }
 
    /* 암호화가 완료된 2차원 상태 배열을 1차원 배열인 Out 배열에 저장한다. */
    for (int i = 0; i<4; i++)
    {
        for (int j = 0; j<Nb; j++)
        {
            out[i * 4 + j] = state[j][i];
        }
    }
    return 1;
}
 
int main(void)
{
    /* 암호화할 평문의 값을 저장하는 배열 */
    unsigned char in[128] = { 0, };
    /* 암호화를 위한 Key를 저장하는 배열 */
    unsigned char out[128] = { 0, };
 
    /* Key Expansion */
    KeyExpansion();
 
    /* PlainText Input */
    cout << "● AES 암호화 할 평문 입력 : "; cin >> in;
    /* Cipher Implementations */ Implement_Cipher(in, out, true);
    cout << "● AES 암호화 된 평문 내용 : " << out << endl;
 
    /* Inverse Cipher */
    memset(in, 0, sizeof(in));
    /* Inverse Cipher Implementations */ Implement_Cipher(out, in, false);
    cout<< "● AES 복호화 된 암호 내용 : " << in << endl;
 
    return 0;
}
