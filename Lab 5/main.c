#include <MKL25Z4.h>
#define LED1_POS (9)
#define LED2_POS (19)
#define SW1_POS (5)
#define MASK(x) (1UL << (x))

void delay(int); //polling method delay function
void delay(int n){
	while(n--){}
}


int main(void);
int main(void){
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK; //enable clock to portb for outputs
	 
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK; //enable clock to portd for debugging purposes
	
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK; //enable clock to porte for inputs
	
	// configure portb pins as gpio
	PORTB->PCR[8] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[8] |= PORT_PCR_MUX(1);
	PORTB->PCR[9] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[9] |= PORT_PCR_MUX(1);
	PORTB->PCR[10] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[10] |= PORT_PCR_MUX(1);
	PORTB->PCR[11] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[11] |= PORT_PCR_MUX(1);
	
	//configure porte pins as gpio
	PORTE->PCR[5] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[5] |= PORT_PCR_MUX(1);
	PORTE->PCR[4] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[4] |= PORT_PCR_MUX(1);
	PORTE->PCR[3] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[3] |= PORT_PCR_MUX(1);
	
	//set portd pin as gpio for debugging
	PORTD->PCR[1] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[1] |= PORT_PCR_MUX(1);
	//set portd debug pin as output (blue led)
	FPTD->PDDR |= (1<<1);
	
	//make porte input
	FPTE->PDDR &= ~(1UL<<3);
	FPTE->PDDR &= ~(1UL<<4);
	FPTE->PDDR &= ~(1UL<<5); // clear Switch bit to input
	
	//make portb output
	FPTB->PDDR |= (1<<8) | (1<<9) | (1<<10) | (1<<11); // set LED bits to outputs
	
	
	FPTB->PDOR = 0; // turn off all
	
	uint32_t count = 0; //variable to hold the count
	int delay_var = 500000; //variable to hold the delay corresponding to the delay
	int nothold = 0; //variable to stop the counter if necessary
	while (1) {
		
		FPTD->PTOR = (1<<1);//toggle blue led
		
		FPTB->PDOR = (count<<7); //shift the counter to the corresponding bits (1 bit prescaler)
		
		count = count % 16; //not necessary since the least significant 4 bits will not change 
		
		if (nothold){ //does not increment or decrement if holding
			if (FPTE->PDIR & (1<<3)){ //checks the direction switch increments and decrements accordingly
				count++; 
			}
			else{
				count--;
			}
		}
			
		delay(delay_var); //for delay
		
		
		//checks the state of the remaining two switches and determines holding and delay accordingly
		if (FPTE->PDIR & (1<<4)){
			if (FPTE->PDIR & (1<<5)){
				delay_var = 375000; //11
				nothold = 1;
			}
			else{
				delay_var = 750000; //01
				nothold = 1;
			}
			
		}
		
		else{
			if (FPTE->PDIR & (1<<5)){ //10
				delay_var = 1500000;
				nothold = 1;
			}
			else{
				delay_var = 500000; //00
				nothold = 0;
			}
		}
		
	}
}
