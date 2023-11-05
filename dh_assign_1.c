#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>


void print_help(const char *program_name) {
	printf("Diffie-Hellman Key Exchange Tool\n");
	printf("Secured Systems TUC 2023-2024\n");
	printf("Created by Georgios Michail Siatras and Andreas Karogiannis\n");
    printf("Usage: %s -o path -p number -g number -a number -b number\n", program_name);
    printf("-o path   Path to output file\n");
    printf("-p number Prime number\n");
    printf("-g number Primitive Root for previous prime number\n");
    printf("-a number Private key A\n");
    printf("-b number Private key B\n");
}

void write_to_file(mpz_t publicKeyA, mpz_t publicKeyB, mpz_t secret, const char *outputPath) {
    FILE *file = fopen(outputPath, "w");
    if (file == NULL) {
        printf("Error opening or creating the file.\n");
        return;
    }

    // Write the values to the file
    gmp_fprintf(file, "%Zd,%Zd,%Zd\n", publicKeyA, publicKeyB, secret);

    // Close the file
    fclose(file);
}

int main(int argc, char *argv[]) {
    char *outputPath = NULL;
    mpz_t prime, root, privateKeyA, privateKeyB;
    int help = 0;

    mpz_inits(prime, root, privateKeyA, privateKeyB, NULL);

    int option;
    while ((option = getopt(argc, argv, "o:p:g:a:b:h")) != -1) {
        switch (option) {
            case 'o':
                outputPath = optarg;
                break;
            case 'p':
                mpz_set_str(prime, optarg, 10);
                break;
            case 'g':
                mpz_set_str(root, optarg, 10);
                break;
            case 'a':
                mpz_set_str(privateKeyA, optarg, 10);
                break;
            case 'b':
                mpz_set_str(privateKeyB, optarg, 10);
                break;
            case 'h':
                help = 1;
                break;
            case '?':
                fprintf(stderr, "Unknown option or missing argument: -%c\n", optopt);
                return 1;
            default:
                abort();
        }
    }

    if (help) {
        print_help(argv[0]);
        return 0;
    }

    if (!outputPath || mpz_sgn(prime) == 0 || mpz_sgn(root) == 0 || mpz_sgn(privateKeyA) == 0 || mpz_sgn(privateKeyB) == 0) {
        fprintf(stderr, "Missing required argument(s). Use -h for help.\n");
        mpz_clears(prime, root, privateKeyA, privateKeyB, NULL);
        return 1;
    }

    // Check that private keys are less than prime
    if (mpz_cmp(privateKeyA, prime) > 0 || mpz_cmp(privateKeyB, prime) > 0) {
    	fprintf(stderr, "Private keys must be less than prime. Use -h for help.\n");
        mpz_clears(prime, root, privateKeyA, privateKeyB, NULL);
        return 1;
    }

    // Generate public keys
    mpz_t publicKeyA;
    mpz_t publicKeyB;
    mpz_inits(publicKeyA, publicKeyB, NULL);

    mpz_powm(publicKeyA, root, privateKeyA, prime);
    mpz_powm(publicKeyB, root, privateKeyB, prime);

    // Secret
    mpz_t sA;
    mpz_t sB;
    mpz_inits(sA, sB, NULL);

    mpz_powm(sA, publicKeyB, privateKeyA, prime);
    mpz_powm(sB, publicKeyA, privateKeyB, prime);

    // Check if we are correct and print to file
    if (mpz_cmp(sA, sB) == 0) {
        write_to_file(publicKeyA, publicKeyB, sA, outputPath);
        printf("Operation succesfull");
    } else {
    	fprintf(stderr, "Operation failed. Use -h for help.\n");
    	mpz_clears(prime, root, privateKeyA, privateKeyB, publicKeyB, publicKeyA, sA, sB, NULL);
        return 1;
    }

    // Clear GMP numbers
    mpz_clears(prime, root, privateKeyA, privateKeyB, publicKeyB, publicKeyA, sA, sB, NULL);

    return 0;
}
