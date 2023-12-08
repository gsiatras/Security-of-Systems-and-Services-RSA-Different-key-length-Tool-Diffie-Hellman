# Diffie-Hellman Key Exchange Tool and RSA Different key length Tool      

Developed by Georgios Michail Siatras and Andreas Karogiannis			    

TUC: Security of Systems and Services      

Proffessor: Sotirios Ioannidis      


**Diffie-Hellman Key Exchange Tool**	
The tool was created following the given instructions.
We create A, B then we create the secret for both users, we check if the secret is correct and we print it in output.txt file.

**RSA Different key length Tool**
The tool was created following the given instructions.
**Methods:**
generateRSAKeyPair:
Inputs: keyLength (int)

We create 2 random prime numbers (p,q) of length = key_length/2, using generate_two_random_primes method. 
Then we compute N, lambda_N. 
To compute e we generate random prime numbers and we test the given conditions.
Then we do the modular inverse to compute d.
Then we save the public and private keys.

This method works fine on lengths 1024 and 2048. On length = 4096 the method takes some minutes to run probably because of the random methods.

encrypt_text:
Inputs: 
input path (const char)
output path (const char)
keyPath (const char)

Outputs:
0 normal finish
1 error

We open the files.
Read the input character character by character encypt the character and store the encypted value to output file line by line.


decrypt_text
encrypt_text:
Inputs: 
input path (const char)
output path (const char)
keyPath (const char)

Outputs:
0 normal finish
1 error

We open the files.
Read the input file line by line to take the encrypted charactes(numerical values). Decrypt and store to output file.

Performance:
We create 3 sets of keys (takes some minutes as mentioned above)
Perform encrypt and decrypt and take the time.
Store to performance.txt file.
No inputs needed. Just -a command.
