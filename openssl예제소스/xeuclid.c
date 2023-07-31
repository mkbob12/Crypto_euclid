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
    // 

    BIGNUM *r1, *r2, *s1, *s2, *t1, *t2, *q, *r, *s, *t, *c;
    BIGNUM *gcd = BN_new();
    
    // ====== 초기값 설정 ======== 
    r1 = BN_dup(a); // copy a to r1
    r2 = BN_dup(b); // copy b to r2 

    s1 = BN_new();
    s2 = BN_new();
    t1 = BN_new();
    t2 = BN_new();
    

    q = BN_new();
    r = BN_new();
    s = BN_new();
    t = BN_new();
    c = BN_new(); // 변수 c 초기화


    BN_set_word(s1,1);
    BN_set_word(s2,0);
    BN_set_word(t1,0);
    BN_set_word(t2,1);
    
    while(BN_is_zero(r2) != 1){
        // 임시 buff 
        
        
        BN_div(q, r, r1, r2, BN_CTX_new()); // q = r1 /r2 , r = r1 % r2 
        r1 = BN_dup(r2);
        r2 = BN_dup(r);
   
        //BN_mul(r, q, s2, BN_CTX_new());

        // s = s1 - s2 * q 
        BN_mul(c, q, s2, BN_CTX_new()); // c = s2 * q 
        BN_sub(c,s1,c); // s1 - s2 * q  
    
        s = BN_dup(c);
        
        // dump 하기 
        s1 = BN_dup(s2);
        s2 =  BN_dup(s);

        // t = t1 - t2 * q 
        BN_mul(c,q,t2 ,BN_CTX_new()); // c = t2 * q 
        BN_sub(c,t1,c); // t1 - t2 * q
        t = BN_dup(c); // t = t1 - t2  * q  
        t1 = BN_dup(t2);
        t2 = BN_dup(t);

      
    }
    gcd = BN_dup(r1);
    BN_copy(x, s1); // BN_dup 가 아닌 BN_copy로 해야 오류 해결  ) 이유는 ,, 
    BN_copy(y, t1);

 
    // 할당 해제 
    BN_free(q);
    BN_free(r1);
    BN_free(r2);
    BN_free(r);
    BN_free(s1);
    BN_free(s2);
    BN_free(s);
    BN_free(t1);
    BN_free(t2);
    BN_free(t);
    BN_free(c);

    

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