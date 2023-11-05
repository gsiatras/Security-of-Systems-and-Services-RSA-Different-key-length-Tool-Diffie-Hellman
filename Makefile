CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lgmp

# Target for dh_assign_1
dh_assign_1: dh_assign_1.c
	$(CC) $(CFLAGS) -o dh_assign_1 dh_assign_1.c $(LDFLAGS)

# Target for rsa_assign_1
rsa_assign_1: rsa_assign_1.c
	$(CC) $(CFLAGS) -o rsa_assign_1 rsa_assign_1.c $(LDFLAGS)

.PHONY: clean

clean:
	rm -f dh_assign_1 rsa_assign_1
