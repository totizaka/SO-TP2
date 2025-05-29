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

	ncPrint("[x64BareBones]");
	ncNewline();

	ncPrint("CPU Vendor:");
	ncPrint(cpuVendor(buffer));
	ncNewline();

	ncPrint("[Loading modules]");
	ncNewline();
	void * moduleAddresses[] = {
		sample_code_module_address,
		sample_data_module_address
	};

	loadModules(&end_of_kernel_binary, moduleAddresses);
	ncPrint("[Done]");
	ncNewline();
	ncNewline();

	ncPrint("[Initializing kernel's binary]");
	ncNewline();

	clear_BSS(&bss, &end_of_kernel - &bss);

	ncPrint("  text: 0x");
	ncPrintHex((uint64_t)&text);
	ncNewline();
	ncPrint("  rodata: 0x");
	ncPrintHex((uint64_t)&rodata);
	ncNewline();
	ncPrint("  data: 0x");
	ncPrintHex((uint64_t)&data);
	ncNewline();
	ncPrint("  bss: 0x");
	ncPrintHex((uint64_t)&bss);
	ncNewline();

	ncPrint("[Done]");
	ncNewline();
	ncNewline();
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

void test_process(){
	int x;
		while(1){
		draw_word( 0, "A\n", 1);
		for(int i=0; i<10000000;i++){
			i--;
			i++;
			x = i;
		}
}}

void t_a(){
	int x;
		while(1){
		draw_word ( 0xFFFFFF, "A\n");
		for(int i=0; i<10000000;i++){
			i--;
			i++;
			x = i;
		}
}}


void t_b(){
	int x;
		while(1){
		draw_word ( 0xFFFFFF, "b\n");
		for(int i=0; i<10000000;i++){
			i--;
			i++;
			x = i;
		}
}}

void t_c(){
	int x;
		while(1){
		draw_word (0xFFFFFF, "c\n");
		for(int i=0; i<10000000;i++){
			i--;
			i++;
			x = i;
		}
}}



int main()
{	

	load_idt();

	// ncPrintHex(((EntryPoint)sample_code_module_address)());

	draw_word(0xFFFFFF,"hola\n");

	char * argv_idle[] = {"idle"};
	char * argv_shell[] = {"sh"};

	memory_manager = createMemoryManager(memory_address);
	set_idle((void(*))t_b,5, NULL, 0);
	initialize_scheduler();

	new_process((void(*))t_a, HIGH_PRIORITY, NULL, 0);
	new_process((void(*))t_b, MEDIUM_PRIORITY, NULL, 0);
	timer_tick();
	
	draw_word(0xFFFFFF, "No deberia llegar aca \n");

	/*while (1){
		draw_word(0xFFFFFF, "No deberia llegar aca \n");
	}*/
	

	// new_process((main_function)sample_code_module_address, HIGH_PRIORITY, argv_shell, 1);

	// new_process((main_function) t_b, MEDIUM_PRIORITY, NULL, 0);
	// new_process((main_function) t_c, LOW_PRIORITY, NULL, 0);

	// timer_tick();

	return 0;
}
