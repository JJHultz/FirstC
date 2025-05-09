#include<stdio.h>

void main() {
	int k; // Initializes an object with address k
	k = 3; // Assigns the value of 3 to object k (this happens at runtime)

	int *ptr; // initializes a pointer variable ptr that will point to the address of an integer
	ptr = &k; // assigns the address of the integer object k (lvalue) to the pointer variable 

	printf("%d\n",*ptr); // does whatever printf does
			     // first part looks to format to a string ending with new-line char
			     // 2nd part looks like it would return the address of what ptr points to
			     // 	which in this case would be the address of k
			     // 
			     // So the address of object k returns the integer 3, formatted to be the string..?

	return;
}
