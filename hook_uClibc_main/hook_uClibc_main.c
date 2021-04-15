#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>

/*trampoline for the real main*/
static int(*main_orig)(int, char **, char **);

/*Fake main -> asusdiscovery*/
int main_hook(int argc, char **argv, char **envp){
	//<arg declarations here> 
	char user_buff[512] = {"\x00"};
	//scanf("%512s", user_buff);
	read(0, user_buff, 512);
	int(*do_thing_ptr)(char *, int, int) = 0x401f30;//might depend on system
	int ret_val = (*do_thing_ptr)(user_buff, 0, 0);
	printf("Ret val %d\n", ret_val);
	return 0;
}

/*uClibc*/
int __libc_start_main(int (*main)(int, char **, char **), int argc, char **argv, int (*init)(int, char **, char **), void (*fini) (void), void(*rtld_fini)(void), void *stack_end){
	//save addres of real main
	main_orig = main;
	//find real main()
	typeof(&__uClibc_main) orig = dlsym(RTLD_NEXT, "__uClibc_main");
	//call custom main()
	return orig(main_hook, argc, argv, init, fini, rtld_fini, stack_end);
}
