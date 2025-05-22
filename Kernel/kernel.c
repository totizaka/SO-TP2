#include <kernel.h>
	
extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t end_of_kernel_binary;
extern uint8_t end_of_kernel;
/*char scan_codes[128] = { 0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ',
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
*/

static void * const sample_code_module_address = (void*)0x400000;
static void * const sample_data_module_address = (void*)0x500000;
static void * const memory_address = (void*)0x600000;

static const uint64_t page_size = 0x1000;

static memory_manager_ADT memory_manager;    // puntero global para accederlo


typedef int (*EntryPoint)();
//unsigned int keyRead();


void clear_BSS(void * bss_address, uint64_t bss_size)
{
	memset(bss_address, 0, bss_size);
}

void * get_stack_base()
{
	return (void*)(
		(uint64_t)&end_of_kernel
		+ page_size * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initialize_kernel_binary()
{
	char buffer[10];

	void * moduleAddresses[] = {
		sample_code_module_address,
		sample_data_module_address
	};

	loadModules(&end_of_kernel_binary, moduleAddresses);
	
	clear_BSS(&bss, &end_of_kernel - &bss);

	return get_stack_base();
}

memory_manager_ADT get_memory_manager(){
	return memory_manager;
}

void idle_process(){
	while (1){
		_hlt();
	}
}

int main()
{	

	load_idt();

	memory_manager = createMemoryManager(memory_address);

	initialize_scheduler(new_process((uint64_t)&idle_process, LOW_PRIORITY, NULL, 0));

	return 0;
}
