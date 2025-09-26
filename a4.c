/* a4.c
 * CSC Fall 2022
 * 
 * Student name: Blake Stewart
 * Student UVic ID: v00966622
 * Date of completed work:
 *
 *
 * Code provided for Assignment #4
 *
 * Author: Mike Zastre (2022-Nov-22)
 *
 * This skeleton of a C language program is provided to help you
 * begin the programming tasks for A#4. As with the previous
 * assignments, there are "DO NOT TOUCH" sections. You are *not* to
 * modify the lines within these section.
 *
 * You are also NOT to introduce any new program-or file-scope
 * variables (i.e., ALL of your variables must be local variables).
 * YOU MAY, however, read from and write to the existing program- and
 * file-scope variables. Note: "global" variables are program-
 * and file-scope variables.
 *
 * UNAPPROVED CHANGES to "DO NOT TOUCH" sections could result in
 * either incorrect code execution during assignment evaluation, or
 * perhaps even code that cannot be compiled.  The resulting mark may
 * be zero.
 */


/* =============================================
 * ==== BEGINNING OF "DO NOT TOUCH" SECTION ====
 * =============================================
 */

#define __DELAY_BACKWARD_COMPATIBLE__ 1
#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define DELAY1 0.000001
#define DELAY3 0.01

#define PRESCALE_DIV1 8
#define PRESCALE_DIV3 64
#define TOP1 ((int)(0.5 + (F_CPU/PRESCALE_DIV1*DELAY1))) 
#define TOP3 ((int)(0.5 + (F_CPU/PRESCALE_DIV3*DELAY3)))

#define PWM_PERIOD ((long int)500)

volatile long int count = 0;
volatile long int slow_count = 0;


ISR(TIMER1_COMPA_vect) {
	count++;
}


ISR(TIMER3_COMPA_vect) {
	slow_count += 5;
}

/* =======================================
 * ==== END OF "DO NOT TOUCH" SECTION ====
 * =======================================
 */


/* *********************************************
 * **** BEGINNING OF "STUDENT CODE" SECTION ****
 * *********************************************
 */

void led_state(uint8_t LED, uint8_t state) {
	// Mask for the LED bit, initialized to 0
	uint8_t led_mask = 0;

	// Determine which LED to control based on the led_number
	switch (LED) {
		case 0: led_mask = 0b10000000; break; // LED #0
		case 1: led_mask = 0b00100000; break; // LED #1
		case 2: led_mask = 0b00001000; break; // LED #2
		case 3: led_mask = 0b00000010; break; // LED #3
		default: return; // If invalid LED number, do nothing
	}

	// Turn the specified LED on or off based on the state parameter
	if (state == 0) {
		// Turn off the LED
		PORTL &= ~led_mask;
	} else {
		// Turn on the LED
		PORTL |= led_mask;
	}
}



void SOS() {
    uint8_t light[] = {
        0x1, 0, 0x1, 0, 0x1, 0,
        0xf, 0, 0xf, 0, 0xf, 0,
        0x1, 0, 0x1, 0, 0x1, 0,
        0x0
    };

    int duration[] = {
        100, 250, 100, 250, 100, 500,
        250, 250, 250, 250, 250, 500,
        100, 250, 100, 250, 100, 250,
        250
    };

	int length = 19;
	
	for(int i = 0; i < length; i++) {
		// Check each bit in the pattern and control the LEDs accordingly
		for (uint8_t j = 0; j < 4; j++) { // Assuming 4 LEDs controlled by the lower 4 bits
			// Calculate the mask for the current LED
			uint8_t led_mask = 1 << j;
			// Determine the state of the current LED from the pattern. Using a ternary operator.
			uint8_t state = (light[i] & led_mask) ? 1 : 0;
			// Initialize led_number to 0. Used to hold the desired led pattern for display.
			uint8_t led_number = 0;
			switch(j) {
				case 0: led_number = 0; break; // Mapping bit 7 to LED #3
				case 1: led_number = 1; break; // Mapping bit 5 to LED #2
				case 2: led_number = 2; break; // Mapping bit 3 to LED #1
				case 3: led_number = 3; break; // Mapping bit 1 to LED #0
			}
		// Call `led_state()` for the current LED
		led_state(led_number, state);
		}
		
	// Delay for the duration of the current pattern
	_delay_ms(duration[i]);
	}
		
}


void glow(uint8_t LED, float brightness) {
	long int threshold = PWM_PERIOD * brightness;
	
	while(1) {
		if (count < threshold) { // If the current count is less than the threshold, turn the LED on.
			led_state(LED, 1);
		} else if (count < PWM_PERIOD) { // If the count is within the PWM period but above the threshold, turn the LED off.
			led_state(LED, 0);
		} else { // If the count exceeds the PWM period, reset it to 0 and turn the LED on.
			count = 0;
			led_state(LED, 1);
		}
	}
}



void pulse_glow(uint8_t LED) {
	// Start of solution
	while(1) { // Infinite loop to continuously cycle the glow effect.
		slow_count = 0; // Initialize slow_count, intended to control glow rate.
		count = 0; // Reset count at the beginning of each pulse cycle.

		for(int threshold = 0; threshold <= PWM_PERIOD; threshold = slow_count * 0.1) {
			if(count < threshold) {
				led_state(LED, 1); // Turn LED on (increasing brightness phase).
				} else if(count < PWM_PERIOD) {
				led_state(LED, 0); // Turn LED off if count exceeds threshold but is less than PWM_PERIOD.
				} else {
				count = 0; // Reset count when it matches PWM_PERIOD to start a new cycle.
				led_state(LED, 1); // Ensure LED is on at the start of the cycle.
			}
		}
		slow_count = 0;
		count = 0;
		
		for(int threshold = 0; threshold <= PWM_PERIOD; threshold = slow_count * 0.1) {
			if(count < threshold) {
				led_state(LED, 0); // Turn LED off (decreasing brightness phase).
				} else if(count < PWM_PERIOD) {
				led_state(LED, 1); // Turn LED on if count exceeds threshold but is less than PWM_PERIOD.
				} else {
				count = 0; // Reset count to start a new cycle.
				led_state(LED, 1); // Ensure LED is on at the start of the decreasing cycle.
			}
		}
	}
}


void light_show() {
    uint8_t light[] = {
	    0x0f, 0, 0x0f, 0, 0x0f, 0, 0x06, 0, 0x09, 0, 0x0f, 0, 0x0f, 0, 0x0f, 0, 0x09, 0, 0x06, 0, 0x08, 0x0c, 0x06, 
		0x03, 0x01, 0x03, 0x06, 0x0c, 0x08, 0x0c, 0x06, 0x03, 0x01, 0x03, 0x06, 0x0f, 0, 0x0f, 0, 0x06, 0, 0x06, 0
    };

    int duration[] = {
	    250, 250, 250, 250, 250, 250, 100, 100, 100, 100, 250, 250, 250, 250, 250,
		250, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
		100, 100, 100, 100, 250, 250, 250, 250, 250, 375, 375, 375, 375
    };

    int length = 43;
    
    for(int i = 0; i < length; i++) {
	    // Check each bit in the pattern and control the LEDs accordingly
	    for (uint8_t j = 0; j < 4; j++) { // Assuming 4 LEDs controlled by the lower 4 bits
		    // Calculate the mask for the current LED
		    uint8_t led_mask = 1 << j;
		    // Determine the state of the current LED from the pattern. Using a ternary operator.
		    uint8_t state = (light[i] & led_mask) ? 1 : 0;
		    // Initialize led_number to 0. Used to hold the desired led pattern for display.
		    uint8_t led_number = 0;
		    switch(j) {
			    case 0: led_number = 0; break; // Mapping bit 7 to LED #3
			    case 1: led_number = 1; break; // Mapping bit 5 to LED #2
			    case 2: led_number = 2; break; // Mapping bit 3 to LED #1
			    case 3: led_number = 3; break; // Mapping bit 1 to LED #0
		    }
		    // Call `led_state()` for the current LED
		    led_state(led_number, state);
	    }
	    
	    // Delay for the duration of the current pattern
	    _delay_ms(duration[i]);
    }
}


/* ***************************************************
 * **** END OF FIRST "STUDENT CODE" SECTION **********
 * ***************************************************
 */


/* =============================================
 * ==== BEGINNING OF "DO NOT TOUCH" SECTION ====
 * =============================================
 */

int main() {
    /* Turn off global interrupts while setting up timers. */

	cli();

	/* Set up timer 1, i.e., an interrupt every 1 microsecond. */
	OCR1A = TOP1;
	TCCR1A = 0;
	TCCR1B = 0;
	TCCR1B |= (1 << WGM12);
    /* Next two lines provide a prescaler value of 8. */
	TCCR1B |= (1 << CS11);
	TCCR1B |= (1 << CS10);
	TIMSK1 |= (1 << OCIE1A);

	/* Set up timer 3, i.e., an interrupt every 10 milliseconds. */
	OCR3A = TOP3;
	TCCR3A = 0;
	TCCR3B = 0;
	TCCR3B |= (1 << WGM32);
    /* Next line provides a prescaler value of 64. */
	TCCR3B |= (1 << CS31);
	TIMSK3 |= (1 << OCIE3A);


	/* Turn on global interrupts */
	sei();

/* =======================================
 * ==== END OF "DO NOT TOUCH" SECTION ====
 * =======================================
 */


/* *********************************************
 * **** BEGINNING OF "STUDENT CODE" SECTION ****
 * *********************************************
 */

 /*This code could be used to test your work for part A.

	led_state(0, 1);
	_delay_ms(1000);
	led_state(2, 1);
	_delay_ms(1000);
	led_state(1, 1);
	_delay_ms(1000);
	led_state(2, 0);
	_delay_ms(1000);
	led_state(0, 0);
	_delay_ms(1000);
	led_state(1, 0);
	_delay_ms(1000);
 */

/* This code could be used to test your work for part B. 

	SOS();
 */

/* This code could be used to test your work for part C. 

	glow(2, 0.1);
 */


/* This code could be used to test your work for part D. 

	pulse_glow(3);
 */


/* This code could be used to test your work for the bonus part. */

	light_show();
 

/* ****************************************************
 * **** END OF SECOND "STUDENT CODE" SECTION **********
 * ****************************************************
 */
}
