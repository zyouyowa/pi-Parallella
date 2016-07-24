#include <stdio.h>
#include <math.h>

int isprime(unsigned number){
    unsigned s = sqrt(number);
    for(unsigned i = 3; i <= s; i += 2){
        if(number % i == 0)return 0;
    }
    return 1;
}

int main(int argc, char const *argv[]){
	unsigned n = 1;
	unsigned primes = 0;
	unsigned max_prime;
	scanf("%u", &max_prime);
	while(n < max_prime){
		if(isprime(n)){
			primes++;
		}
		n += 2;
	}
	printf("primes : %u\n", primes);
	return 0;
}