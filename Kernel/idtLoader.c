#include <stdint.h>
#include <idtLoader.h>
#include <defs.h>
#include <interrupts.h>

#pragma pack(push)		/* Push de la alineación actual */
#pragma pack (1) 		/* Alinear las siguiente estructuras a 1 byte */

/* Descriptor de interrupcion */
typedef struct {
  uint16_t offset_l, selector;
  uint8_t cero, access;
  uint16_t offset_m;
  uint32_t offset_h, other_cero;
} DESCR_INT;

#pragma pack(pop)		/* Reestablece la alinceación actual */



DESCR_INT * idt = (DESCR_INT *) 0;	// IDT de 255 entradas


//El PIC (Controladora de Interrupciones Programable) recibe una señal y genera una interrupción.
//Asigna un número de interrupción específico para cada evento, IRQ0, IRQ1, etc
//En el caso del teclado la interrupción es IRQ1 que corresponde a la interrupción 0x21 en la IDT.
//Cuando el PIC genera la interrupción, la CPU detiene lo que estaba haciendo y consulta la entrada correspondiente en la IDT.

void load_idt() {   
                                            //Setea las entradas para las interrupciones, cada interrupicon tiene una funcion asociada.

  setup_IDT_entry (0x20, (uint64_t)&_irq00Handler);           //TimerTick
  setup_IDT_entry (0x21, (uint64_t)&_irq01Handler);           //Keyboard,    la entrada 0x21 apunta a la función _irq01Handler, que es el handler de la interrupción del teclado:

  setup_IDT_entry (0x00, (uint64_t)&_exception0Handler);      // Division by zero
  setup_IDT_entry (0x06, (uint64_t)&_exception06Handler);      // invalid opcode

  setup_IDT_entry (0x80, (uint64_t)&_int80Handler);

 

	//Solo interrupcion timer tick y keyboard habilitadas
	picMasterMask(0xFC);
	picSlaveMask(0xFF);
        
	_sti();
}



void setup_IDT_entry (int index, uint64_t offset) {
  idt[index].selector = 0x08;
  idt[index].offset_l = offset & 0xFFFF;
  idt[index].offset_m = (offset >> 16) & 0xFFFF;
  idt[index].offset_h = (offset >> 32) & 0xFFFFFFFF;
  idt[index].access = ACS_INT;
  idt[index].cero = 0;
  idt[index].other_cero = (uint64_t) 0;
}