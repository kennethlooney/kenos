#include <stdio.h>
#include <stdlib.h>
 

char* image_name = "disk.img";

int main(int argc, char** argv)
{
    FILE* image = fopen(image_name, "wb+");
    if(!image) {
        fprintf(stderr, "Error: could not open file %s\n", image_name);
        return EXIT_FAILURE;
    } 

    

 
    


    return EXIT_SUCCESS;
}