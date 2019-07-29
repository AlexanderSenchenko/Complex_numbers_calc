#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>
#include "myComplex.h"

void read_complex(struct Complex *compl)
{
	printf("Input real number: "); 
	scanf("%d", &(compl->real));

	printf("Input img number: ");
	scanf("%d", &(compl->img));
}

void free_name_libs(char** name_libs, int count_libs)
{
	int i;
	for (i = 0; i < count_libs; ++i)
		free(name_libs[i]);
	free(name_libs);
}

void free_libsd(void** libsd, int count_libs)
{
	int i;
	for (i = 0; i < count_libs; ++i)
		dlclose(libsd[i]);
	free(libsd);
}

int main(int argc, char** argv)
{
	int act;

	char PATH[] = "/home/users/summer_2019/user21/Complex_numbers_calc/";
	int size_path = strlen(PATH);

	char** name_libs;
	int count_libs = argc - 1;

	int size_buf;
	char* path_buf;
	path_buf = malloc(sizeof(char) * size_path);
	strcpy(path_buf, PATH);

	struct Complex compl1;
	struct Complex compl2;
	struct Complex res;

	name_libs = malloc(sizeof(char*) * count_libs);

	int i, j;
	size_t name_size;
	for (i = 0; i < count_libs; ++i) {
		name_size = strlen(argv[i + 1]);
		
		#ifdef DEBUG
		printf("%zu\n", name_size);
		#endif
		
		name_libs[i] = malloc(sizeof(char) * name_size);

		// for (j = 0; j < name_size; ++j)
		//	name_libs[i][j] = argv[i + 1][j];
		strcpy(name_libs[i], argv[i + 1]);
	}

	#ifdef DEBUG
	for (i = 0; i < count_libs; ++i)
		printf("%s\n", name_libs[i]);
	#endif

	// void* libd = dlopen("/home/users/summer_2019/user21/compl/libmyComplex.so", RTLD_NOW);	
	void** libsd;

	int k;
	int size_name_lib;
	libsd = malloc(count_libs);
	for (i = 0; i < count_libs; ++i) {
		size_buf = strlen(path_buf);
		size_name_lib = strlen(name_libs[i]);
		
		if (size_buf < size_path + size_name_lib + 1)
			path_buf = realloc(path_buf, size_path + size_name_lib + 1);
		
		for (j = size_path, k = 0; j < size_path + size_name_lib; ++j, ++k)
			path_buf[j] = name_libs[i][k];
		path_buf[j] = '\0';
		
		libsd[i] = dlopen(path_buf, RTLD_NOW);
	
		if (!(libsd[i])) {
			fprintf(stderr, "%s\n", dlerror());
			free_name_libs(name_libs, count_libs);
			free(path_buf);
			free(libsd);
			exit(EXIT_FAILURE);
		}

		dlerror();
	}

	free(path_buf);
	
	struct Complex (**func)(struct Complex, struct Complex);
	func = malloc(sizeof(*func) * count_libs);
	char name_func[15];

	for (i = 0; i < count_libs; ++i) {
		for (j = 0; name_libs[i][j + 3] != '.'; ++j)
			name_func[j] = name_libs[i][j + 3];
		name_func[j] = '\0';

		func[i] = dlsym(libsd[i], name_func);

		#ifdef DEBUG
		printf("%s\n", name_func);
		#endif

		char* error = dlerror();
		if (error != NULL) {
			fprintf(stderr, "%s\n", error);
			free_name_libs(name_libs, count_libs);
			free_libsd(libsd, count_libs);
			free(func);
			exit(EXIT_FAILURE);
		}

		dlerror();
	}

	#if 0
	struct Complex (*func_add)(struct Complex, struct Complex);
	func_add = dlsym(libd, "complex_add");

	char* error = dlerror();
	if (error != NULL) {
		fprintf(stderr, "%s\n", error);
		exit(EXIT_FAILURE);
	}
	#endif

	do {
		printf("1) Add\n");
		printf("2) Sub\n");
		printf("3) Multn\n");
		printf("4) Div\n");
		printf("5) Quit\n");
		printf("Input act: ");

		scanf("%d", &act);

		switch(act) {
			case 1:
				read_complex(&compl1);
				read_complex(&compl2);
				
				// res = (*func_add)(compl1, compl2);
				
				complex_print(res);
				break;
			case 2:
				// complex_sub();
				break;
			case 3:
				// complex_mult();
				break;
			case 4:
				// complex_div();
				break;
			case 5:
				printf("Exit\n");
				break;
			default:
				printf("Act not found\n");
				break;
		}
	} while(act != 5);

	#if 0
	for (i = 0; i < count_libs; ++i)
		free(name_libs[i]);
	free(name_libs);
	#else
	free_name_libs(name_libs, count_libs);
	#endif

	free_libsd(libsd, count_libs);
	free(func);

	return 0;
}
