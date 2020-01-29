//dekompilace funkcí
struct handler {
	void *ptr;
	char xor_byte;
	char index;
	char padding[6];
};

handler **handler_array;    
/*globální pole ukazatelů na handlery. Nikdy není uvolněno, vždycky bude memory leak*/

#define PARAMETER_LENGTH 0x17

void allocate_handler_array(char * newMem) {
	handler_array = malloc((PARAMETER_LENGTH+1)*sizeof(handler*));  //0x18 * sizeof(handler*) == 0xc0

	//každý znak má vlastní handler. Kód pro každý znak má 17 bajtů (které se potom za běhu vždy xorují
	//xorovací hodnota je 
	for (int i = 0;i<PARAMETER_LENGTH;i++) {
		handler *handler = malloc(sizeof(handler)); //sizeof(handler) == 0x10
		handler.ptr = newMem + 17 * i;			
		handler.xor_byte = (i * 17) & 0xff;
		handler.index = i;
		handler_array[i]= handler;
	}
	handler_array[PARAMETER_LENGTH] = NULL;
}

void shuffle_callback_pointers() {
	srand(time(NULL));
	for (int i = 0; i < 0x100;i++) {
		int a = rand() % 0x16 + 1; //promíchá ukazatele na handlery; ty tak nejdou popořadě
		int b = rand() % 0x16 + 1; //ale to nic neovlivní
		handler *ptr = handler_array[a];	
		handler_array[a] = handler_array[b];
		handler_array[b] = ptr;
	}
}

void decode(char *character, char * encrypted_mem, int xor_byte) {
	if (*character != 0x33)  //pro ostatní znaky se bude lišit kód k porovnání
		exit(1);

	for (int i = 0;i<17;i++, encrypted_mem++)
		*encrypted_mem ^= xor_byte; //xor decrypt nové instrukce pro další handler

}


int main(int agrc, char **argv) {
	void *newMem = mmap(NULL, 4096, 7, 0x22, -1, 0); //třetí parametr memory protection: zde všechno (rwx)
	allocate_handler_array(newMem);
	memcpy(newMem, someSource, 0x2f5);
	if (argc != 2 || strlen(argv[1]) != PARAMETER_LENGTH)
		exit(1);

	shuffle_callback_pointers();

	for (int i = 0; i < PARAMETER_LENGTH; i++) {
		handler *current = handler_array[i];
		handler *next = handler_array[i+1];
		if (next) //execute checks for all characters
			current->ptr(argv[1] + current->index, next->ptr, next->xor_byte);
		else 
			current->ptr(argv[1] + current->index, newMem, 0);
	}
	//if code passess all checks without _exit, success
	puts("What are you waiting for, go submit that flag!");
	return 0;
}

