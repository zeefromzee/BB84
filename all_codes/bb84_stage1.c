#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
int main(){
    srand(time(NULL));
// printf("%d",bit_gen(10));

// Alice generates random bits:
    int* Alice = bit_gen(13);
    print_bits(Alice, 13);
    int *aliceAngles = basis_gen(13);
    printf("\n");
    print_bits(aliceAngles, 13);

    // int* Bob = bit_gen(13);
    int *bobAngles = basis_gen(13);
    printf("\n");
    print_bits(bobAngles, 13);
    // free(Bob);
    int *Bob_results = measure(Alice , aliceAngles, bobAngles, 13);
    printf("\n");
    print_bits(Bob_results, 13);

    free(bobAngles);
    free(Alice);
    free(aliceAngles);
    free(Bob_results);

    sifted_key key = sift(Alice, Bob_results, aliceAngles, bobAngles, 13);
    printf("\nsifted length: %d\n", key.length);
    // print_bits(key.aliceBits, key.length);
    // printf("\n");
    // print_bits(key.bobBits, key.length);
    // printf("\n");

    free(key.aliceBits);
    free(key.bobBits);

    return 0;
}
