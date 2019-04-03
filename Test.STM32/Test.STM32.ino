
#include <RTClock.h>



void setup() {
	Serial.begin(250000);
	delay(1000);
	/*rtc.attachSecondsInterrupt(function);*/

	bkp_init();		// turn on peripheral clocks to PWR and BKP and reset the backup domain via RCC registers.
					// (we reset the backup domain here because we must in order to change the rtc clock source).

	bkp_enable_writes();	// enable writes to the backup registers and the RTC registers via the DBP bit in the PWR control register

	RCC_BASE->BDCR &= ~RCC_BDCR_RTCSEL;
	rcc_start_lse();
	RCC_BASE->BDCR |= RCC_BDCR_RTCSEL_LSE;
	bb_peri_set_bit(&RCC_BASE->BDCR, RCC_BDCR_RTCEN_BIT, 1); // Enable the RTC
	rtc_clear_sync();
	Serial.println("setup");


	while (*bb_perip(&(RTC->regs)->CRL, RTC_CRL_RSF_BIT) == 0);


	Serial.println("setup2");
	rtc_wait_finished();


	rtc_set_prescaler_load(0x7fff);

	Serial.println("setup");


}

void function() {
	Serial.println("RTC");
}

// the loop function runs over and over again until power down or reset
void loop() {
	Serial.println("DD");
	delay(1000);
}
