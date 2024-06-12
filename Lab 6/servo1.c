#include <MKL25Z4.h>
volatile int sign = 1;

void delay(int);
void delay(int n){
	int i;
	for(i = 0;i < n;i++);
}

void portd_interrupt_init(void);
void portd_interrupt_init() {
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
	PORTD->PCR[7] |= PORT_PCR_PE_MASK | PORT_PCR_MUX(1) | PORT_PCR_IRQC(9);//rising edge triggered gpio interrupt
	PORTD->PCR[7] &= ~PORT_PCR_PS_MASK;//pulldown resistor
	FPTD->PDDR &= ~(1UL << 7);

	NVIC_SetPriority(PORTD_IRQn, 128);//enable interrupts for port d
	NVIC_ClearPendingIRQ(PORTD_IRQn);
	NVIC_EnableIRQ(PORTD_IRQn);
}

static void pwm_init(void);
static void pwm_init(void)
{    
    SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK; // Enable clock to PORTB 
    PORTE_PCR20 |= PORT_PCR_MUX(3);  // PTB18: TPM2_CH0 (ALT3)   
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1); // Select FLL as the Clock to TPM (PWM)   
    SIM->SCGC6 |= SIM_SCGC6_TPM1(1); // Enable clock to TPM2 (PWM) Module    
    TPM1->SC |= (TPM_SC_PS(7) | TPM_SC_CPWMS(0)| TPM_SC_CMOD(1));// Configure as edge aligned PWM. Clock prescaler is 7 (PWM clock devided by 128). This makes PWM clock as 48000000/128 = 375000 Hz (375Khz) 
    TPM1_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_ELSA(0) | TPM_CnSC_MSB(1)  | TPM_CnSC_MSA(0));	// Configured as Edge Aligned PWM								
}

void PORTD_IRQHandler(void);
void PORTD_IRQHandler(void) {
	sign = sign * -1; //change sign
	PORTD->ISFR = 0xffffffff; //clear flags
	FPTB->PTOR |= (1<<18); //debugging purposes
	return;
}

int fit(int angle);
int fit(int angle){
	return( (int)((double)angle * 1.7055) + 82); //formula to find c0v value for angle
}

int main(void);
int main(void){
	SIM->SCGC5 |= (1<<10); //portb is used entirely for debugging purposes
	PORTB_PCR19 |= PORT_PCR_MUX(1);
	PORTB_PCR18 |= PORT_PCR_MUX(1);
	FPTB->PDDR |= (1<<19)|(1<<18);
	FPTB->PDOR |= (1<<19)|(1<<18);
	
	pwm_init();
	portd_interrupt_init();
	
	TPM1_MOD = (uint16_t)3280; 
	
	int angles[] = {90,60,30,0,30,60}; //array of angles to sweep
	
	while(1){
		FPTB->PTOR |= (1<<18);
		for (int j = 0;j<6;j++){		
			FPTB->PTOR |= (1<<19);
			TPM1_C0V = (uint16_t)fit(90-(angles[j]*sign));	//90 +/- angles[j] so it is either 30 or 150 based on the sign
			delay(1500000);	
		}
	}
}

//Here is the shortened version with 31 lines: (i did not try if it works)

#include <MKL25Z4.h>
volatile int sign = 1;
void PORTD_IRQHandler(void);
void PORTD_IRQHandler(void) {
	sign = sign * -1; //change sign
	PORTD->ISFR = 0xffffffff; //clear flags
	return;
}
int main(void);
int main(void){
   	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK | SIM_SCGC5_PORTD_MASK; // Enable clock to PORTB 
   	PORTE->PCR[20] |= PORT_PCR_MUX(3);  // PTE20: TPM1_CH0 (ALT3)   
   	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1); // Select FLL as the Clock to TPM (PWM)   
   	SIM->SCGC6 |= SIM_SCGC6_TPM1(1); // Enable clock to TPM2 (PWM) Module    
    	TPM1->SC |= (TPM_SC_PS(7) | TPM_SC_CPWMS(0)| TPM_SC_CMOD(1));// Configure as edge aligned PWM. Clock prescaler is 7 (PWM clock dİvided by 128). This makes PWM clock as 48000000/128 = 375000 Hz (375Khz) 
    	TPM1_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_ELSA(0) | TPM_CnSC_MSB(1)  | TPM_CnSC_MSA(0));	// Configured as Edge Aligned PWM						
	PORTD->PCR[7] = (PORT_PCR_PE_MASK | PORT_PCR_MUX(1) | PORT_PCR_IRQC(9)) & (~(PORT_PCR_PS_MASK));//rising edge triggered gpio interrupt & pulldown resistor
	FPTD->PDDR &= ~(1UL << 7);
	NVIC_SetPriority(PORTD_IRQn, 128);//enable interrupts for port d
	NVIC_ClearPendingIRQ(PORTD_IRQn);
	NVIC_EnableIRQ(PORTD_IRQn);
	TPM1_MOD = (uint16_t)3280; 	
	int angles[] = {90,60,30,0,30,60}; //array of angles to sweep	
	while(1){
		FPTB->PTOR |= (1<<18);
		for (int j = 0;j<6;j++){		
		TPM1_C0V = (uint16_t)(((int)((double)(90-angles[j]) * 1.7055) + 82)*sign);	//90 +/- angles[j] so it is either 30 or 150 based on the sign
		for(i = 0;i < 1500000;i++){}
		}
	}
}
