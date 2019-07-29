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

void free_lib_names(char** lib_names, int count_libs)
{
	int i;
	for (i = 0; i < count_libs; ++i)
		free(lib_names[i]);
	free(lib_names);
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
	int i, j, k;

	char PATH[] = "/home/users/summer_2019/user21/Complex_numbers_calc/";
	int size_path = strlen(PATH);

	int count_libs = argc - 1;
	int size_buff;

	char* path_buff;
	path_buff = malloc(sizeof(char) * size_path);
	strcpy(path_buff, PATH);
	
	struct Complex compl1;
	struct Complex compl2;
	struct Complex res;

	void** libsd;
	libsd = malloc(sizeof(void*) * count_libs);

	int lib_name_size;

	for (i = 0; i < count_libs; ++i) {
		size_buff = strlen(path_buff);
		lib_name_size = strlen(argv[i + 1]);

		if (size_buff < size_path + lib_name_size + 1)
			path_buff = realloc(path_buff, size_path + lib_name_size + 1);
		
		for (j = size_path, k = 0; j < size_path + lib_name_size; ++j, ++k)
			path_buff[j] = argv[i + 1][k];
		path_buff[j] = '\0';
		
		libsd[i] = dlopen(path_buff, RTLD_NOW);
	
		if (!(libsd[i])) {
			fprintf(stderr, "%s\n", dlerror());
			free(path_buff);
			free(libsd);
			exit(EXIT_FAILURE);
		}

		dlerror();
	}

	free(path_buff);

	#if 1 
	struct Complex (**func)(struct Complex, struct Complex);
	func = malloc(sizeof(*func) * count_libs);

	char** func_names;
	func_names = malloc(sizeof(char*) * count_libs);

	int func_name_size;

	for (i = 0; i < count_libs; ++i) {
		func_name_size = strlen(argv[i + 1]) - 3 - 3; // lib*.so
		func_names[i] = malloc(sizeof(char) * func_name_size + 1);

		for (j = 0; j < func_name_size; ++j)
			func_names[i][j] = argv[i + 1][j + 3];
		func_names[i][j] = '\0';

		func[i] = dlsym(libsd[i], func_names[i]);

		#ifdef DEBUG
		printf("%s\n", func_names[i]);
		#endif

		char* error = dlerror();
		if (error != NULL) {
			fprintf(stderr, "%s\n", error);
			free_libsd(libsd, count_libs);
			free(func);
			free(func_names);
			exit(EXIT_FAILURE);
		}

		dlerror();
	}

	do {
		for (i = 0; i < count_libs; ++i)
			printf("%d) %s\n", i + 1, func_names[i]);
		printf("%d) Exit\n", count_libs + 1);

		printf("Input act: ");
		scanf("%d", &act);
	
		if (act < count_libs + 1) {
			printf("Input first complex number\n");
			read_complex(&compl1);

			printf("Input second comolex number\n");
			read_complex(&compl2);

			res = (func[act - 1])(compl1, compl2);

			printf("Calc result\n");
			complex_print(res);
		} else if (act == count_libs + 1) {
			printf("Exit\n");
		} else {
			printf("Act not found\n");
		}
	} while(act != count_libs + 1);
	#else
	compl1.real = 12;
	compl1.img = 12;

	compl2.real = 12;
	compl2.img = 12;

	struct Complex (*func)(struct Complex, struct Complex);
	func = dlsym(libsd[0], "complex_add");

	char *error = dlerror();
	if (error != NULL) {
		fprintf(stderr, "%s\n", error);
		// free_libsd(libsd, count_libs);
		// free(func);
		// free(func_names);
		exit(EXIT_FAILURE);
	}
	res = (*func)(compl1, compl2);

	complex_print(res);
	#endif

	free_libsd(libsd, count_libs);
	// free(func);
	
	#if 0
	if (func_names != NULL) {
		for (i = 0; i < count_libs; ++i) {
			if (func_names[i] != NULL)
				free(func_names[i]);
		}
		free(func_names);
	}
	#endif

	return 0;
}
