// This program blinks the red LED on the FRDM-KL25 board.
// The red LED is connected to PORTB pin 18 and is low active.
#include <MKL25Z4.H>

void delay(int);
void delay(int n) {
	int i;
	for(i = 0; i < n; i++) ;
}


int main (void) {
	SIM->SCGC5 |= 0x0400; // enable clock to Port B
	SIM->SCGC5 |= (1<<12); // enable clock to Port D
	
	
	//GREEN PIN
	PORTB->PCR[19] = 0x0100; // make pin PTB18 as GPIO
	FPTB->PDOR |= (1<<19); // switch Red/Green LED off
	FPTB->PDDR |= (1<<19); // enable PTB18/19 as Output
	
	//RED PIN
	PORTB->PCR[18] = 0x0100; // make pin PTB18 as GPIO
	FPTB->PDOR |= (1<<18); // switch Red/Green LED off
	FPTB->PDDR |= (1<<18); // enable PTB18/19 as Output
	
	
	//BLUE PIN
	PORTD->PCR[1] = 0x0100; // make pin PTD1 as GPIO	
	FPTD->PDDR |= (1<<1);
	FPTD->PDOR |= (1<<1);

	/*
	//RED PIN PWM INIT
	PORTB->PCR[18] = PORT_PCR_MUX(3); // make pin PTB18 as TIMER
	
	SIM->SCGC6 |= SIM_SCGC6_TPM2_MASK; //ENABLE CLOCK TO TPM MODULE
	PORTB->PCR[18] = PORT_PCR_MUX(4); //SET PORT MUX TO TIMER TPM2_CH0
	
	FPTB->PDOR |= (1<<18); // switch Red/Green LED off
	FPTB->PDDR |= (1<<18); // enable PTB18/19 as Output
	
	
	//CONFIGURE TPM
	SIM->SOPT2 |= (SIM_SOPT2_TPMSRC(1) | SIM_SOPT2_PLLFLLSEL_MASK); //SELECT 48MHz FOR CLOCK SOURCE
	TPM2->SC |= TPM_SC_PS(6); //6 BIT PRESCALER 750000Hz clock freq
	
	TPM2->SC |= TPM_SC_CMOD(1); //mode 1
	
	TPM2->SC &= ~TPM_SC_CPWMS_MASK; // edge aligned pwm
	
	//SET AS NOT INVERTING
	TPM2_C0SC |= TPM_CnSC_ELSB_MASK;	
	TPM2_C0SC &=	~TPM_CnSC_ELSA_MASK; 
	
	//SET TO COMPARE OUTPUT MODE
	TPM2_C0SC |= TPM_CnSC_MSB_MASK;
	TPM2_C0SC &= ~TPM_CnSC_MSA_MASK;
	
	//CONFIGURE TIMING OF PWM 
	TPM2_MOD = 1875; 
	TPM2_C0V = 9;
	*/
	
	
	
	//prepare loop
	int speed = 10;
	int limit = 4000/speed;

	//increase red
	for (int j =0; j<limit;j++) {
		FPTB->PCOR |= (1<<18); // make the pin output low
		delay(speed*j);
		FPTB->PSOR |= (1<<18); // make the pin output high
		delay(4000-speed*j);
	}
	FPTB->PCOR |= (1<<18); // make the pin output low
	
	//LOOP
	while(1) {		
		
		//increase green 
		for (int j =0; j<limit;j++) {
			FPTB->PCOR |= (1<<19); // make the pin output low
			delay(speed*j);
			FPTB->PSOR |= (1<<19); // make the pin output high
			delay(4000-speed*j);
		}
		FPTB->PCOR |= (1<<19); // make the pin output low
		
		//decrease red
		for (int j =0; j<limit;j++) {
			FPTB->PCOR |= (1<<18); // make the pin output low
			delay(4000-speed*j);
			FPTB->PSOR |= (1<<18); // make the pin output high
			delay(speed*j);
		}
		
		//increase blue 
		for (int j =0; j<limit;j++) {
			FPTD->PCOR |= (1<<1); // make the pin output low
			delay(speed*j);
			FPTD->PSOR |= (1<<1); // make the pin output high
			delay(4000-speed*j);
		}
		FPTD->PCOR |= (1<<1); // make the pin output low
		
		//decrease green
		for (int j =0; j<limit;j++) {
			FPTB->PCOR |= (1<<19); // make the pin output low
			delay(4000-speed*j);
			FPTB->PSOR |= (1<<19); // make the pin output high
			delay(speed*j);
		}
		
		//increase red
		for (int j =0; j<limit;j++) {
			FPTB->PCOR |= (1<<18); // make the pin output low
			delay(speed*j);
			FPTB->PSOR |= (1<<18); // make the pin output high
			delay(4000-speed*j);
		}
		FPTB->PCOR |= (1<<18); // make the pin output low
		
		//decrease blue
		for (int j =0; j<limit;j++) {
			FPTD->PCOR |= (1<<1); // make the pin output low
			delay(4000-speed*j);
			FPTD->PSOR |= (1<<1); // make the pin output high
			delay(speed*j);
		}
	}
}
