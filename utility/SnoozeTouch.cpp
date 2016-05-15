/***********************************************************************************
 *  SnoozeTimer.h
 *  Teensy 3.x/LC
 *
 * Purpose: Touch (TSI) Driver
 *
 ***********************************************************************************/
#include "Arduino.h"
#include "SnoozeTouch.h"
#include "wake.h"

#if defined(KINETISK)

#define TSI_PEN_LPSP_MASK    0xF0000u
#define TSI_PEN_LPSP_SHIFT   16
#define TSI_PEN_LPSP(x)      (((uint32_t)(((uint32_t)(x))<<TSI_PEN_LPSP_SHIFT))&TSI_PEN_LPSP_MASK)

static const uint8_t tsi_pins[] = {
    //0    1    2    3    4    5    6    7    8    9
    9,  10, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255,  13,   0,   6,   8,   7,
    255, 255,  14,  15, 255,  12, 255, 255, 255, 255,
    255, 255,  11,   5
};
#elif defined(KINETISL)
static const uint8_t tsi_pins[] = {
    //0    1    2    3    4    5    6    7    8    9
    9,  10, 255,   2,   3, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255,  13,   0,   6,   8,   7,
    255, 255,  14,  15, 255, 255, 255
};
#endif

void SnoozeTouch::pinMode ( int _pin, int thresh ) {
    isUsed = true;
    pin = _pin;
    threshold = thresh;
}

void SnoozeTouch::disableDriver( void ) {
    if ( mode == RUN_LP || mode == VLPW || mode == RUN_LP ) return;
    uint8_t _pin = pin;
    TSI0_GENCS     &= ~TSI_GENCS_TSIEN;
    TSI0_GENCS      = GENCS;
    TSI0_THRESHOLD  = THRESHOLD;
    TSI0_SCANC      = SCANC;
    TSI0_PEN        = PEN;
    
    volatile uint32_t *config;
    config = portConfigRegister( _pin );
    *config = return_core_pin_config;
    
    if ( !SIM_SCGC5_clock_active ) SIM_SCGC5 &= ~SIM_SCGC5_TSI;
}

void SnoozeTouch::enableDriver( void ) {
    if ( mode == RUN_LP || mode == VLPW ) return;
    uint8_t _pin = pin;
    if ( _pin >= NUM_DIGITAL_PINS ) return;
    
    llwu_configure_modules_mask( LLWU_TSI_MOD );
    
    uint16_t _threshold = threshold;
    
    GENCS     = TSI0_GENCS;
    THRESHOLD = TSI0_THRESHOLD;
    SCANC     = TSI0_SCANC;
    PEN       = TSI0_PEN;
    
    uint32_t ch;
    ch = tsi_pins[pin];
    if ( ch == 255 ) return;
    
    volatile uint32_t *config;
    config = portConfigRegister( _pin );
    return_core_pin_config = *config;
    
    *portConfigRegister( _pin ) = PORT_PCR_MUX( 0 );
    
    if ( SIM_SCGC5  & SIM_SCGC5_TSI ) SIM_SCGC5_clock_active = true;
    else SIM_SCGC5 |= SIM_SCGC5_TSI;
    
    TSI0_GENCS = 0;
#if defined(KINETISK)
    TSI0_THRESHOLD = _threshold;
    TSI0_SCANC =  (
                   ( TSI_SCANC_EXTCHRG( 2 ) ) |
                   ( TSI_SCANC_REFCHRG( 3 ) ) |
                   ( TSI_SCANC_AMPSC( 1 )   ) |
                   ( TSI_SCANC_AMCLKS( 0 )  )
                   );
    TSI0_PEN = TSI_PEN_LPSP( ch );
    TSI0_GENCS = (
                  ( TSI_GENCS_NSCN( 9 )    ) |
                  ( TSI_GENCS_PS( 2 )       ) |
                  ( TSI_GENCS_LPSCNITV( 7 ) ) |
                  ( TSI_GENCS_STPE          ) |
                  ( TSI_GENCS_TSIIE         ) |
                  ( TSI_GENCS_OUTRGF        ) |
                  ( TSI_GENCS_EOSF          ) |
                  ( TSI_GENCS_TSIEN         )
                  );
#elif defined(KINETISL)
    TSI0_TSHD = ( _threshold << 16) & 0xFFFF0000;
    TSI0_DATA = TSI_DATA_TSICH( ch );
    TSI0_GENCS = (
                  ( TSI_GENCS_NSCN( 9 )    ) |
                  ( TSI_GENCS_PS( 2 )      ) |
                  ( TSI_GENCS_STPE         ) |
                  ( TSI_GENCS_REFCHRG( 4 ) ) |
                  ( TSI_GENCS_EXTCHRG( 3 ) ) |
                  ( TSI_GENCS_TSIIEN       ) |
                  ( TSI_GENCS_STM          ) |
                  ( TSI_GENCS_OUTRGF       ) |
                  ( TSI_GENCS_EOSF         ) |
                  ( TSI_GENCS_TSIEN        )
                  );
    
    SIM_SOPT1 |= SIM_SOPT1_OSC32KSEL( 3 );
    SIM_SCGC5 |= SIM_SCGC5_LPTIMER;
    LPTMR0_PSR = LPTMR_PSR_PBYP | LPTMR_PSR_PCS( 1 );// LPO Clock
    LPTMR0_CMR = 1;
    LPTMR0_CSR = LPTMR_CSR_TEN | LPTMR_CSR_TCF;
#endif
}

void SnoozeTouch::clearIsrFlags( void ) {
    isr( );
}

void SnoozeTouch::isr( void ) {
    if ( !( SIM_SCGC5 & SIM_SCGC5_TSI ) ) return;
    TSI0_GENCS = TSI_GENCS_OUTRGF | TSI_GENCS_EOSF;
#if defined(KINETISL)
    LPTMR0_CSR = LPTMR_CSR_TCF;
    SIM_SCGC5 &= ~SIM_SCGC5_LPTIMER;
#endif
    if ( mode == VLPW || mode == VLPS ) source = 37;
}