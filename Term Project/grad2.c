#include <MKL25Z4.h>
volatile int red = 26;
volatile int green = 237;
volatile int blue = 202;
volatile int alpha = 255;

void delay(int);
void delay(int n)
{
	int i;
	for(i = 0;i < n;i++);
}
void gpio_init(void); //initialise the ports that will be used as gpio
void gpio_init(void)
{
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTD_MASK; 
	PORTB->PCR[0] |= PORT_PCR_MUX(1); //S2 TCS3200
	PORTB->PCR[1] |= PORT_PCR_MUX(1); //S3 TCS3200
	PORTB->PCR[2] |= PORT_PCR_MUX(1); //BUZZER
	
	PORTC->PCR[1] |= PORT_PCR_MUX(1); //button
	PORTC->PCR[2] |= PORT_PCR_MUX(1); //OUT TCS3200
	

	FPTB->PDDR |= (1<<2)|(1<<1)|(1<<0);
	FPTB->PDOR = 0;
	FPTC->PDDR &= ~((1<<2)|(1<<1));
}


static void pwm_init(void); //initialise pwm for red and green led
static void pwm_init(void)
{
    // Enable clock to PORTB
    SIM_SCGC5 |= (uint32_t)(1 << 10); 
     
    // PTB18: TPM2_CH0 (ALT3)
    PORTB_PCR18 |= PORT_PCR_MUX(3);
		PORTB_PCR19 |= PORT_PCR_MUX(3);
     
    // Select FLL as the Clock to TPM (PWM)
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
     
    // Enable clock to TPM2 (PWM) Module
    SIM->SCGC6 |= SIM_SCGC6_TPM2(1);
     
    // Configure as edge aligned PWM
    TPM2->SC |= (TPM_SC_CPWMS(0)| TPM_SC_CMOD(1));
     
    // Clock prescaler is 7 (PWM clock devided by 128)
    // This makes PWM clock as 48000000/128 = 375000 Hz (375Khz)
    TPM2->SC |= TPM_SC_PS(6);
     
    // Configured as Edge Aligned PWM
    TPM2_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_ELSA(0) |
                  TPM_CnSC_MSB(1)  | TPM_CnSC_MSA(0));
									
		TPM2_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_ELSA(0) |
                  TPM_CnSC_MSB(1)  | TPM_CnSC_MSA(0));
									
		TPM2_MOD = (uint16_t)1875; 
		TPM2_C0V = (uint16_t)900;
}

static void pwm_init2(void); //initalise pwm  for blue led
static void pwm_init2(void)
{
    // Enable clock to PORTD
    SIM_SCGC5 |= (uint32_t)(1 << 12); 
     
    // PTB18: TPM2_CH0 (ALT3)
    PORTD_PCR1 |= PORT_PCR_MUX(4);
		
     
    // Select FLL as the Clock to TPM (PWM)
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
     
    // Enable clock to TPM2 (PWM) Module
    SIM->SCGC6 |= SIM_SCGC6_TPM0(1);
     
    // Configure as edge aligned PWM
    TPM0->SC |= (TPM_SC_CPWMS(0)| TPM_SC_CMOD(1));
     
    // Clock prescaler is 7 (PWM clock devided by 128)
    // This makes PWM clock as 48000000/128 = 375000 Hz (375Khz)
    TPM0->SC |= TPM_SC_PS(6);
     
    // Configured as Edge Aligned PWM
    				
		TPM0_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_ELSA(0) |
                  TPM_CnSC_MSB(1)  | TPM_CnSC_MSA(0));
									
		
		TPM0_MOD = (uint16_t)1875; 
		TPM0_C1V = (uint16_t)900;
}

static void pwm_init3(void); //initalise pwm for porte20 servo 1
static void pwm_init3(void)
{
    // Enable clock to PORTB
    SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK; 
     
    // PTB18: TPM2_CH0 (ALT3)
    PORTE_PCR20 |= PORT_PCR_MUX(3);

    // Select FLL as the Clock to TPM (PWM)
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
     
    // Enable clock to TPM2 (PWM) Module
    SIM->SCGC6 |= SIM_SCGC6_TPM1(1);
     
    // Configure as edge aligned PWM
    TPM1->SC |= (TPM_SC_PS(7) | TPM_SC_CPWMS(0)| TPM_SC_CMOD(1));
     
    // Clock prescaler is 7 (PWM clock devided by 128)
    // This makes PWM clock as 48000000/128 = 375000 Hz (375Khz)
   
    // Configured as Edge Aligned PWM
    TPM1_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_ELSA(0) | TPM_CnSC_MSB(1)  | TPM_CnSC_MSA(0));		 

		TPM1_MOD = (uint16_t)3280; 
}

static void pwm_init4(void);
static void pwm_init4(void)
{
    // Enable clock to PORTB
    SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK; 
     
    // PTB18: TPM2_CH0 (ALT3)
    PORTE_PCR21 |= PORT_PCR_MUX(3);

    // Select FLL as the Clock to TPM (PWM)
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
     
    // Enable clock to TPM2 (PWM) Module
    SIM->SCGC6 |= SIM_SCGC6_TPM1(1);
     
    // Configure as edge aligned PWM
    TPM1->SC |= (TPM_SC_PS(7) | TPM_SC_CPWMS(0)| TPM_SC_CMOD(1));
     
    // Clock prescaler is 7 (PWM clock devided by 128)
    // This makes PWM clock as 48000000/128 = 375000 Hz (375Khz)
   
    // Configured as Edge Aligned PWM
    TPM1_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_ELSA(0) | TPM_CnSC_MSB(1)  | TPM_CnSC_MSA(0));									
									
		TPM1_MOD = (uint16_t)3280; 
}

int get_freq(void); //gets a number that is correlated to the freq of portc2
int get_freq(void)
{
	uint16_t count = 0;
	int freq;
	while(1){
		if(!(FPTC->PDIR & (1<<2))){
			break;
		}
	}
	while(1){
		if(FPTC->PDIR & (1<<2)){
			break;
		}
	}
	while(FPTC->PDIR & (1<<2)){
		count++;
	}
	freq = (int)(52480/(double)count) * 50;
	return(count);
}
	

void normalise(double r1,double r2,double r3); //adjusts the volatile color variables to hex standart
void normalise(double r1,double r2,double r3)
{
		double max = r2;
		if (max < r1){
			max = r1;
		}
		if (max < r3){
			max = r3;
		}
		double nf = 255.0 / max;
		red = (int)(nf * r1);
		green = (int)(nf * r2);
		blue = (int)(nf * r3);
}


void update_color(void); //updates the color variables from the sensor
void update_color(void)
{
	double red_freq;
	double green_freq;
	double blue_freq;
	double alpha_freq;
	FPTB->PDOR &= ~((1UL<<1) | (1UL<<0));
	FPTB->PDOR |= 00; //red
	red_freq = get_freq();
	delay(1000);
	red_freq += get_freq();
	delay(1000);
	red_freq += get_freq();
	red_freq = red_freq / 3;
	delay(1000);
	FPTB->PDOR &= ~((1UL<<1) | (1UL<<0));
	FPTB->PDOR |= (1<<0); //alpha
	alpha_freq = get_freq();
	delay(1000);
	alpha_freq += get_freq();
	delay(1000);
	alpha_freq += get_freq();
	alpha_freq = alpha_freq / 3;
	delay(1000);
	FPTB->PDOR &= ~((1UL<<1) | (1UL<<0));
	FPTB->PDOR |= (1<<1); //blue
	blue_freq = get_freq();
	delay(1000);
	blue_freq += get_freq();
	delay(1000);
	blue_freq += get_freq();
	blue_freq = blue_freq / 3;
	delay(1000);
	FPTB->PDOR &= ~((1UL<<1) | (1UL<<0));
	FPTB->PDOR |= (1<<1)|(1<<0); //green
	green_freq = get_freq();
	delay(1000);
	green_freq += get_freq();
	delay(1000);
	green_freq += get_freq();
	green_freq = green_freq / 3;
	delay(1000);
	
	double red_ratio = (double)((red_freq/alpha_freq)*(1.2));
	double green_ratio = (double)((green_freq/alpha_freq)*(1.1));
	double blue_ratio = (double)((blue_freq/alpha_freq)*(0.9));
	normalise(red_ratio,green_ratio,blue_ratio);
}

int fit(double angle);
int fit(double angle)
{
	return( (int)((angle * 1.7055) + 82)); //formula to find c0v value for angle
}


int main(void); //main
int main(void)
 {
	gpio_init();
	pwm_init();
	pwm_init2();
	pwm_init3();
	pwm_init4();
	 
	TPM1_MOD = (uint16_t)3280;
	
	update_color();
	int state = 0;
	while(1){
		if (state==0){
			
			FPTB->PCOR |= (1<<2); //buzzer off
			
			update_color();

			TPM1_C0V = (uint16_t)fit(90); //put on fire
			
			TPM1_C1V = (uint16_t)fit(92.7); //slow turn

			if (!(red>green)){
				delay(100);
				if (!(red>green)){
					state = 1;
				}
			}
			else{
				TPM1_C0V= (uint16_t)fit(0); 
			}
			if (FPTC->PDIR & (1<<1)){
				state = 1;
				TPM1_C0V= (uint16_t)fit(90); 
				delay(500000);
			}
			TPM2_C0V = 900UL+(uint16_t)((double)red / (0.272));
			TPM2_C1V = 900UL+(uint16_t)((double)green / (0.272));
			TPM0_C1V = 900UL+(uint16_t)((double)blue / (0.272));
		}//end of state 0;
		else{
			FPTB->PSOR |= (1<<2);
			
			TPM1_C1V = (uint16_t)fit(95); 
			
			TPM1_C0V= (uint16_t)fit(0); 
			if (FPTC->PDIR & (1<<1)){
				state = 0;
				delay(1000000);
			}
		}//end of state 1;
	}
}
