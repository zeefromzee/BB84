#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define QBER_THRESHOLD 0.11

typedef struct {
    int basis;
    int *bits;
    int key;
    int raw_key;
    int sifting_phase;
    int error_rate;
}qkd;

// function for random bit generation
// dynamically growing array :)
int *bit_gen(int n){
    // seed:
    // unsigned int seed = time(0);
    int *bits = (int*)malloc(n * sizeof(int));

    if(bits == NULL){
        printf("Is empty ˙◠˙");
        return NULL;
    }

    for (int i =0 ; i< n ;i++){
    bits[i] = rand() % 2; // we used mod 2 here so our output would only be 1 and 0
    }
    return bits;
    free(bits);
}

int *basis_gen(int n){
    // function for random basis selection
    // we shall add all the required basis in this array here
    int basis[] = {0, 90, 45, 135};
    int *results = (int*)malloc(n*sizeof(int));

    if(results == NULL){
        printf("Is empty ˙◠˙");
        return NULL;
    }

    int length = sizeof(basis)/ sizeof(basis[0]);
    // srand(time(NULL));
    

    for(int i =0; i< n; i++){
        int random_ind = rand()% length;
        int random_ele = basis[random_ind];
        results[i] = random_ele;
    }
    return results;
}

int print_bits(int *n, int k){
    for(int i= 0;i< k; i++){
        printf("%d",n[i]);
    }
    return 0;
}

int basis_of(int angle) {
    if (angle == 0 || angle == 90) {
        return 0;
    } else {
        return 1;
    }
}

int *measure(int *aliceBits, int *aliceAngles, int *bobAngles, int n) {
    int *result = (int*)malloc(n * sizeof(int));
    if (result == NULL) {
        printf("Is empty ˙◠˙");
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        if (basis_of(aliceAngles[i]) == basis_of(bobAngles[i])) {
            result[i] = aliceBits[i];     
        } else {
            result[i] = rand() % 2;  
        }
    }
    return result;
}

typedef struct {
    int *aliceBits;
    int *bobBits;
    int length;
} sifted_key;

// Public discussion of the selected basis: 
sifted_key sift(int *aliceBits, int *bobBits, int *aliceAngles, int *bobAngles, int n) {
    sifted_key s;
    s.aliceBits = NULL;
    s.bobBits = NULL;
    s.length = 0;

    //ccount the matching positions
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (basis_of(aliceAngles[i]) == basis_of(bobAngles[i])) {
            count++;
        }
    }

    //return the empty struct
    if (count == 0) {
        return s;
    }

    // Allocate both arrays once as now that we know the size
    s.aliceBits = (int*)malloc(count * sizeof(int));
    s.bobBits = (int*)malloc(count * sizeof(int));
    if (s.aliceBits == NULL || s.bobBits == NULL) {
        free(s.aliceBits);  
        free(s.bobBits);
        s.aliceBits = NULL;
        s.bobBits = NULL;
        return s;
    }

    //copyyy the matching positions
    int j = 0;                       // write position in the sifted arrays
    for (int i = 0; i < n; i++) {
        if (basis_of(aliceAngles[i]) == basis_of(bobAngles[i])) {
            s.aliceBits[j] = aliceBits[i];
            s.bobBits[j] = bobBits[i];
            j++;
        }
    }

    s.length = count;
    return s;
}

int *eve(int *aliceBits, int *aliceAngles, int *bobAngles,int n){
    // pick a random basis

    //  phase 1: picking a random basis
    // int *eveBits = bit_gen(n);
    int *eveAngles   = basis_gen(n);

    // phase 2: measure the qubit using random basis
    int *eveMeasured = measure(aliceBits, aliceAngles, eveAngles, n);

    // bob measure eve bits 
    int *Bob_results = measure(eveMeasured, eveAngles, bobAngles, n);    

    free(eveAngles);
    free(eveMeasured);

    return Bob_results;
}

double qber(int *a, int *b, int length) {

     // avoid dividing by zero
    if (length == 0){
        return 0.0; 
    }  

    int errors = 0;
    for (int i = 0; i < length; i++) {
        if (a[i]!= b[i]) {   
            errors++;
        }
    }

    return (double)errors / length; // fraction of errors, as a double
}

// report fn created using ai
void report(const char *label, int *aliceBits, int *bobBits,
            int *aliceAngles, int *bobAngles, int n) {
    sifted_key key = sift(aliceBits, bobBits, aliceAngles, bobAngles, n);
    printf("%s: sifted length = %d, QBER = %.4f\n",
        label, key.length, qber(key.aliceBits, key.bobBits, key.length));

    if (qber(key.aliceBits, key.bobBits, key.length) >= 0.11){
        printf("\nAbort,threshold value of 0.11 crossed!!!");
    }
    free(key.aliceBits);
    free(key.bobBits);
}

// error fn
void add_noise(int *bits, int n, double p) {

    for (int i = 0; i < n; i++) {
        if (((double)rand() / RAND_MAX) < p) {
            bits[i] = bits[i] ^ 1;     // flipin the bit

        }

    }

}


int main(){
    srand(time(NULL));
// printf("%d",bit_gen(10));

    int N = 100;

// Alice generates random bits:
    int* Alice = bit_gen(N);
    print_bits(Alice, N);
    int *aliceAngles = basis_gen(N);
    printf("\n");
    print_bits(aliceAngles, N);

    // int* Bob = bit_gen(N);
    int *bobAngles = basis_gen(N);
    printf("\n");
    print_bits(bobAngles, N);
    // free(Bob);
    int *Bob_results = measure(Alice , aliceAngles, bobAngles, N);
    printf("\n");
    print_bits(Bob_results, N);

    sifted_key key = sift(Alice, Bob_results, aliceAngles, bobAngles, N);
    printf("\nsifted length: %d\n", key.length);
    // print_bits(key.aliceBits, key.length);
    // printf("\n");
    // print_bits(key.bobBits, key.length);
    // printf("\n");

    int *eve_stuf = eve(Alice, aliceAngles,bobAngles, N);

    int n = 100000;
    double p = 0.09;
    int *Alice1 = bit_gen(n);
    int *aliceAngles1 = basis_gen(n);
    int *bobAngles1 = basis_gen(n);

    int *bobClean = measure(Alice1, aliceAngles1, bobAngles1, n);
    int *bobEve = eve(Alice1, aliceAngles1, bobAngles1, n);

    add_noise(bobClean, n, p);
    add_noise(bobEve, n, p);

    report("No Eve ", Alice1, bobClean, aliceAngles1, bobAngles1, n);
    report("With Eve", Alice1, bobEve,  aliceAngles1, bobAngles1, n);

    free(bobAngles);
    free(Alice);
    free(aliceAngles);
    free(Bob_results);
    free(key.aliceBits);
    free(key.bobBits);

    return 0;
}
