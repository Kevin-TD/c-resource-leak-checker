char c[] = "abcdefghi"; // This is a string with 9 bytes

int main() {
        char *a = malloc(8); // Here we allocate memory
        char b[8]; // We then put below it a char array of 8 bytes

        strcpy(b, c); // we attempt to copy into b, c but strcpy not only overflows the last byte of a with 'i', 
		      // but also null terminates the string


        free(a); // Even though accumulation analysis says this is fine, 
		 // this is actually unsound because a no longer points to what a pointed to originally
}
