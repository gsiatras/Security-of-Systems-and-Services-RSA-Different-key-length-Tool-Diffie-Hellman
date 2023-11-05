#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <time.h>



void print_help(const char *program_name) {
	printf("RSA Different key length Tool\n");
	printf("Secured Systems TUC 2023-2024\n");
	printf("Created by Georgios Michail Siatras and Andreas Karogiannis\n");
    printf("Usage: %s -i path -o path -k path [-g length] [-d | -e | -a] [-h]\n", program_name);
    printf("-i path   Path to the input file\n");
    printf("-o path   Path to the output file\n");
    printf("-k path   Path to the key file\n");
    printf("-g length Perform RSA key-pair generation with the specified key length\n");
    printf("-d        Decrypt input and store results in output\n");
    printf("-e        Encrypt input and store results in output\n");
    printf("-a        Compare the performance of RSA encryption and decryption with different key lengths\n");
    printf("-h        This help message\n");
}


int read_keys(const char *filename, mpz_t key1, mpz_t key2) {
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        printf("File could not be opened.\n");
        return 1; // Return an error code
    }

    // Read key1 from the first line
    if (mpz_inp_str(key1, file, 10) == 0) {
        printf("Error reading key1 from the file.\n");
        fclose(file);
        return 1; // Return an error code
    }

    // Read key2 from the second line
    if (mpz_inp_str(key2, file, 10) == 0) {
        printf("Error reading key2 from the file.\n");
        fclose(file);
        return 1; // Return an error code
    }

    //gmp_printf("Key 1: %Zd\n", key1);
    //gmp_printf("Key 2: %Zd\n", key2);

    fclose(file);
    return 0; // Success
}


int decrypt_text(const char *inputPath, const char *outputPath, const char *keyPath) {
	mpz_t key_n, key_e;
    mpz_inits(key_n, key_e, NULL);


    // Read the private key
    if (read_keys(keyPath, key_n, key_e) == 1) {
        fprintf(stderr, "Error reading the public keys.\n");
        return 1;
    }

    // Read the text file
    FILE *file;
    FILE *outputFile;
    file = fopen(inputPath, "r");

    // Check if the file was successfully opened
    if (file == NULL) {
        printf("File could not be opened.\n");
        return 1; // Return an error code
    }

    // Open the output file for writing
    outputFile = fopen(outputPath, "w");

    // Read the file character by character
    int character;
    mpz_t dec_char;
    mpz_inits(dec_char, NULL);
    mpz_t ascii_value;
    mpz_init(ascii_value);

    char line[4096];

    while (fgets(line, sizeof(line), file) != NULL) {
        // Read a line from the file
        mpz_set_str(ascii_value, line, 10);
        // Encrypt and store the ASCII value of each character
        // Turn to GMP variable
        
        //mpz_set_ui(ascii_value, (unsigned int)character);
        //gmp_printf("old value: %Zd\n", ascii_value);
        // Encrypt
        mpz_powm(dec_char, ascii_value, key_e, key_n);

        char decrypted_character = (char)(mpz_get_ui(dec_char));

        // Write the modified character to the output file
        //printf("Dec char: %c\n", decrypted_character);
        //gmp_printf("new value: %Zd\n",dec_char);
        fputc(decrypted_character, outputFile);
    }

    // Close the files
    fclose(file);
    fclose(outputFile);

    return 0;
}




int encrypt_text(const char *inputPath, const char *outputPath, const char *keyPath) {
	mpz_t key_n, key_d;
    mpz_inits(key_n, key_d, NULL);


    // Read the public key
    if (read_keys(keyPath, key_n, key_d) == 1) {
        fprintf(stderr, "Error reading the public keys.\n");
        return 1;
    }

    // Read the text file
    FILE *file;
    FILE *outputFile;
    file = fopen(inputPath, "r");

    // Check if the file was successfully opened
    if (file == NULL) {
        printf("File could not be opened.\n");
        return 1; // Return an error code
    }

    // Open the output file for writing
    outputFile = fopen(outputPath, "w");

    // Read the file character by character
    int character;
    mpz_t en_char;
    mpz_inits(en_char, NULL);

    while ((character = fgetc(file)) != EOF) {
        // Encrypt and store the ASCII value of each character
        // Turn to GMP variable
        mpz_t ascii_value;
        mpz_init(ascii_value);
        mpz_set_ui(ascii_value, (unsigned int)character);
        //gmp_printf("old char: %c\n", character);
        //gmp_printf("old value: %Zd\n", ascii_value);
        // Encrypt
        mpz_powm(en_char, ascii_value, key_d, key_n);
        //gmp_printf("new value: %Zd\n", en_char);
        char encrypted_character = (char)(mpz_get_ui(en_char));

        // Write the modified character to the output file
        //printf("En char: %Zd\n", encrypted_character);

        //fputc(encrypted_character, outputFile);
        gmp_fprintf(outputFile,"%Zd\n", en_char);
    }

    // Close the files
    fclose(file);
    fclose(outputFile);

    return 0;
}



void write_to_file(mpz_t n, mpz_t d, const char *outputPath) {
    FILE *file = fopen(outputPath, "w");
    if (file == NULL) {
        printf("Error opening or creating the file.\n");
        return;
    }

    // Write the values to the file
    gmp_fprintf(file, "%Zd\n%Zd", n, d);

    // Close the file
    fclose(file);
}


void generate_random_prime(mpz_t prime, int bit_length) {
    gmp_randstate_t state;
    gmp_randinit_default(state);

    
    // Set a random seed based on the current time
    unsigned long seed = time(NULL);;
    gmp_randseed_ui(state, seed);

    // Generate a random number with the specified bit_length
    mpz_urandomb(prime, state, bit_length);

    // Ensure that the number is odd by setting the least significant bit to 1
    mpz_setbit(prime, 0);

    // Use the Miller-Rabin test to check for primality
    int is_prime = 0;
    while (!is_prime) {
        is_prime = mpz_probab_prime_p(prime, 25); // Adjust the number of Miller-Rabin tests as needed
        if (!is_prime) {
            // If not prime, add 2 to make it odd and test again
            mpz_add_ui(prime, prime, 2);
        }
    }

    gmp_randclear(state);
}


void generate_two_random_primes(mpz_t prime1, mpz_t prime2, int bit_length) {
    gmp_randstate_t state;
    gmp_randinit_default(state);

    int is_prime1 = 0;
    int is_prime2 = 0;

    while (!is_prime1 || !is_prime2) {
        // Generate two random numbers
        mpz_t random1, random2;
        mpz_inits(random1, random2, NULL);

        mpz_urandomb(random1, state, bit_length);
        mpz_urandomb(random2, state, bit_length);

        // Ensure that the numbers are odd by setting the least significant bit to 1
        mpz_setbit(random1, 0);
        mpz_setbit(random2, 0);

        // Use the Miller-Rabin test to check for primality
        is_prime1 = mpz_probab_prime_p(random1, 25);
        is_prime2 = mpz_probab_prime_p(random2, 25);

        if (is_prime1 && is_prime2) {
            mpz_set(prime1, random1);
            mpz_set(prime2, random2);
        }
    }

    gmp_randclear(state);
}





void generateRSAKeyPair(int key_length) {
    // Generate 2 random primes with length = key_length/2
    mpz_t p, q, N, lambda_N, e, d, gcd_result;
	mpz_inits(p, q, N, lambda_N, e, N, d, gcd_result, NULL);

	generate_two_random_primes(p, q, key_length/2);
	


    //gmp_printf("p: %Zd\n", p);
    //gmp_printf("q: %Zd\n", q);


	// Compute N
	mpz_mul(N, p, q);

	//Lambda(N)
	mpz_sub_ui(p, p, 1);  // p - 1
    mpz_sub_ui(q, q, 1);  // q - 1
    mpz_mul(lambda_N, p, q);

    // Iterate until a suitable prime e is found
    while (1) {
        // Generate a random prime number e
        generate_random_prime(e, mpz_sizeinbase(lambda_N, 2));
        mpz_gcd(gcd_result, e, lambda_N);

        // Check if (e % lambda(n) != 0) and (gcd(e, lambda) == 1)
        if (mpz_divisible_p(e, lambda_N) == 0 && mpz_cmp_ui(gcd_result, 1) == 0 && mpz_cmp(e, lambda_N) < 0) {
            break; // Found a suitable e
        }
    }

    // Modular inverse

    if (mpz_invert(d, e, lambda_N) == 0) {
        printf("inv fail");
    }

    //gmp_printf("lamda: %Zd\n", lambda_N);
    char publicfile[100];  
    char privatefile[100];
    snprintf(publicfile, sizeof(publicfile), "public_%d.key", key_length);
    snprintf(privatefile, sizeof(privatefile), "private_%d.key", key_length);


    // Store the pyblic and private keys
    write_to_file(N, d, publicfile);
    write_to_file(N, e, privatefile);
}




int main(int argc, char *argv[]) {
    char *inputPath = NULL;
    char *outputPath = NULL;
    char *keyPath = NULL;
    char *performancePath = NULL;

    int keyLength = 0;
    int decrypt = 0;
    int encrypt = 0;
    int performance = 0;
    int help = 0;

    int option;
    while ((option = getopt(argc, argv, "i:o:k:g:deah")) != -1) {
        switch (option) {
            case 'i':
                inputPath = optarg;
                break;
            case 'o':
                outputPath = optarg;
                break;
            case 'k':
                keyPath = optarg;
                break;
            case 'g':
                keyLength = atoi(optarg);
                break;
            case 'd':
                decrypt = 1;
                break;
            case 'e':
                encrypt = 1;
                break;
            case 'a':
                performance = 1;
                performancePath = optarg;
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

    // Print help
    if (help || argc == 1) {
        print_help(argv[0]);
        return 0;
    }

  
    // Key generator mode
    if (keyLength != 0){
    	generateRSAKeyPair(keyLength);
    	return 0;
    }


    // Encrypt mode
    if (encrypt) {
    	if (!inputPath || !outputPath || !keyPath){
    		fprintf(stderr, "Input file (-i), output file (-o), and key file (-k) are required for encryption or decryption. Use -h for help.\n");
        return 1;
    	}
    	encrypt_text(inputPath, outputPath, keyPath);
    	return 0;
    }

    // Decrypt mode
    if (decrypt) {
    	if (!inputPath || !outputPath || !keyPath){
    		fprintf(stderr, "Input file (-i), output file (-o), and key file (-k) are required for encryption or decryption. Use -h for help.\n");
        return 1;
    	}
    	decrypt_text(inputPath, outputPath, keyPath);
    	return 0;
    }


    // Performance mode
    if (performance) {

        FILE *performanceFile = fopen("performance.txt", "w");
        if (performanceFile == NULL) {
            printf("Error opening performance file.\n");
            return 1;
        }
     	for (int keyLength = 1024; keyLength<= 4096; keyLength *= 2){
     		generateRSAKeyPair(keyLength);
            clock_t start_time, end_time;
            double encrypt_time;
            double decrypt_time;

            char publicfile[100];  
            char privatefile[100];
            snprintf(publicfile, sizeof(publicfile), "public_%d.key", keyLength);
            snprintf(privatefile, sizeof(privatefile), "private_%d.key", keyLength);

            start_time = clock();

            encrypt_text("plaintext.txt", "ciphertext.txt", publicfile);

            end_time = clock();

            encrypt_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

            start_time = clock();

            decrypt_text("ciphertext.txt", "solution.txt", privatefile);

            end_time = clock();

            decrypt_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

            fprintf(performanceFile, "%d: Encrypt time: %f, Decrypt time: %f\n", keyLength, encrypt_time, decrypt_time);

     	}
     	fclose(performanceFile);
        return 0;
    }


    return 0;
}
