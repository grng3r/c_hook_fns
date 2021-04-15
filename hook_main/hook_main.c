/*A hook function for _libc_start_main() that intercepts libc's call to main and replaces it with our own
 * We can replace the stuff in hook_main() with anything needed(This is an example)
 * Compile using 'gcc hook_main.c -o library_name.so -fPIC -ldl'
 * Run target program with LD_PRELOAD*/
#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>

/*Trampoline for the real main*/
static int (*main_orig)(int, char **, char **);
/*Fake main -> Example*/
int main_hook(int argc, char **argv, char **envp){
	
	for(int i=0; i< argc; ++i){
		printf("argv[%d] = %s\n", i, argv[i]);
	}
	
	printf("####Before main####\n");
	int ret = main_orig(argc, argv, envp);
	printf("####After main####\n");
	printf("####main() returned %d\n####", ret);
	return ret;
}
/*Wrapper for __libc_start_main(), replacing the real main with our hooked version*/

int __libc_start_main(int (*main) (int, char**, char **), int argc, char **argv, int (*init)(int, char **, char **), void (*fini)(void), void(*rtld_fini)(void), void *stack_end){
	//save addres of real main()
	main_orig = main;
	//find real __libc_start_main()
	typeof(&__libc_start_main) orig = dlsym(RTLD_NEXT, "__libc_start_main");
	//and call it with our custom main()
	return orig(main_hook, argc, argv, init, fini, rtld_fini, stack_end);
}
