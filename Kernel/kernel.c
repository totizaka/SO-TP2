#include <kernel.h>

#define TOTAL_PROCESSES 3
#define LOWEST 0  // TODO: Change as required
#define MEDIUM 1  // TODO: Change as required
#define HIGHEST 2 // TODO: Change as required
#define WAIT 1000000000 // TODO: Change this value to make the wait long enough to see theese processes beeing run at least twice
	
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
	while(1){
	draw_word ( 0xFFFFFF, "idle\n");
	}
	/*while (1){
		_hlt();
	}*/
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
		draw_word ( 0xFFFFFF, "a");
		for(int i=0; i<10000000;i++){
			i--;
			i++;
			x = i;
		}
}}


void t_b(){
	int x;
		while(1){
		draw_word ( 0xFFFFFF, "b");
		for(int i=0; i<10000000;i++){
			i--;
			i++;
			x = i;
		}
}}

void t_c(){
	int x;
		while(1){
		draw_word (0xFFFFFF, "c");
		for(int i=0; i<10000000;i++){
			i--;
			i++;
			x = i;
		}
}}

// void bussy_wait(uint64_t n) {
//   	uint64_t i;
//   	int x;
// 	for (i = 0; i < n; i++){
// 				i--;
// 				i++;
// 				x = i;
// 	}
// }

// uint64_t itoa(uint64_t number, char* s) {
//     int digits = 0;

//     // Contar los dígitos
//     for (uint64_t n = number; n != 0; digits++, n /= 10);

//     // Manejo del caso donde number es 0
//     if (digits == 0) {
//         s[0] = '0';
//         s[1] = '\0';
//         return 1;
//     }

//     // Terminar la cadena con un carácter nulo
//     s[digits] = '\0';

//     // Convertir los dígitos en orden inverso
//     for (int i = digits - 1; i >= 0; i--) {
//         s[i] = (number % 10) + '0';
//         number /= 10;
//     }

//     return digits;
// }

/*int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};

void endless_loop_print() {
  draw_word(0xFFFFFF, "Endless loop print\n");
  int64_t pid = get_pid();
  char str[4];
    while (1) {
      itoa(pid, str);
      draw_word(0xFFFFFF ,str);
      bussy_wait(10000000);
    }
}



void test_prio() {
  int64_t pids[TOTAL_PROCESSES];
  char *argv[] = {0};
  uint64_t i;

  for (i = 0; i < TOTAL_PROCESSES; i++)
    pids[i] = new_process((void(*))endless_loop_print, LOW_PRIORITY, NULL, 0);

  bussy_wait(WAIT);
  draw_word(0xFFFFFF, "\nCHANGING PRIORITIES...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    nice(pids[i], prio[i]*3);

  bussy_wait(WAIT);
  draw_word(0xFFFFFF, "\nBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    block(pids[i]);

  draw_word(0xFFFFFF, "CHANGING PRIORITIES WHILE BLOCKED...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    nice(pids[i], MEDIUM);

  draw_word(0xFFFFFF, "UNBLOCKING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    ready(pids[i]);

  bussy_wait(WAIT);
  draw_word(0xFFFFFF, "\nKILLING...\n");

  for (i = 0; i < TOTAL_PROCESSES; i++)
    kill_process(pids[i]);
}*/


int main()
{	

	load_idt();

	
	// ncPrintHex(((EntryPoint)sample_code_module_address)());

	draw_word(0xFFFFFF,"hola\n");

	char * argv_idle[] = {"idle"};
	char * argv_shell[] = {"sh"};

	memory_manager = createMemoryManager(memory_address);
	set_idle((void(*))idle_process, LOW_PRIORITY, NULL, 0);
	initialize_scheduler((void(*))sample_code_module_address);
	//new_process(, LOW_PRIORITY, NULL, 0);
	// new_process((void(*))t_a, HIGH_PRIORITY, NULL, 0);
	// new_process((void(*))t_b, MEDIUM_PRIORITY, NULL, 0);
	// new_process((void(*))t_c, LOW_PRIORITY, NULL, 0);

	//new_process((void(*))sample_code_module_address, HIGH_PRIORITY, argv_idle, 1);

	 //new_process((void(*))test_prio, LOW_PRIORITY, NULL, 0);

	 

	

	timer_tick();

	draw_word(0xFFFFFF, "No deberia llegar aca \n");

	return 0;
}
