#include <stdio.h> 
#include <openssl/bn.h>
#include <string.h>
#include <stdlib.h>
#define NBITS 256 

BIGNUM *XEuclid(BIGNUM *x, BIGNUM *y, const BIGNUM *a, const BIGNUM *b); 
void printBN(char *msg, BIGNUM *a);



void printBN(char *msg, BIGNUM * a) 
{ 
	/* Use BN_bn2hex(a) for hex string * Use BN_bn2dec(a) for decimal string */ 
	char * number_str = BN_bn2hex(a);
	printf("%s %s\n", msg, number_str); 
	OPENSSL_free(number_str); 
};


BIGNUM *XEuclid(BIGNUM *x, BIGNUM *y, const BIGNUM *a, const BIGNUM *b)
{
    // ax + by = t 
   // b가 0이면 BN_value_one 을 0으로 반환  
   if (BN_is_zero(b))
   {
    BN_copy(x, BN_value_one()); 
    BN_copy(y, BN_value_one());
    return BN_dup(a);
   }

    // 확장 유클리드 알고리즘을 사용하여 d, x, y를 계산 

    BIGNUM *x1 = BN_new();
    BIGNUM *y1 = BN_new();
    BN_CTX *ctx = BN_CTX_new(); // 구조체 선언 



    BN_mod(x1, a, b, ctx); // 모드 연산  ( 나머지 ) x1 = a mod b  
    BIGNUM *gcd = XEuclid(x1, y1, b, x1);  // x1을 구하는 것 
    
    //printBN("x1 = ", x1);

    BN_div(x, NULL, a, b, ctx);
    BN_mod(y, x, y1, ctx);
    BN_mul(x, x, gcd, ctx);
    BN_sub(x, y1, x);

    BN_free(x1);
    BN_free(y1);
    BN_CTX_free(ctx);

    return gcd;

    
}

int main (int argc, char *argv[])
{
        BIGNUM *a = BN_new();
        BIGNUM *b = BN_new();
        BIGNUM *x = BN_new();
        BIGNUM *y = BN_new();
        BIGNUM *gcd;

        if(argc != 3){
                printf("usage: xeuclid num1 num2");
                return -1;
        }

        BN_dec2bn(&a, argv[1]);
        BN_dec2bn(&b, argv[2]);

        gcd = XEuclid(x,y,a,b);

        printBN("(a,b) = ", gcd);
        printBN("a = ", a);
        printBN("b = ", b);
        printBN("x = ", x);
        printBN("y = ", y);
        printf("%s*(%s) + %s*(%s) = %s\n",BN_bn2dec(a),BN_bn2dec(x),BN_bn2dec(b),BN_bn2dec(y),BN_bn2dec(gcd));

        if(a != NULL) BN_free(a);
        if(b != NULL) BN_free(b);
        if(x != NULL) BN_free(x);
        if(y != NULL) BN_free(y);
        if(gcd != NULL) BN_free(gcd);

        return 0;
}