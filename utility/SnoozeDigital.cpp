/***********************************************************************************
 *  SnoozeDigital.h
 *  Teensy 3.x/LC
 *
 * Purpose: Digital Pin Driver
 *
 ***********************************************************************************/
#include "Arduino.h"
#include "SnoozeDigital.h"
#include "wake.h"

uint64_t SnoozeDigital::isr_pin = 0;

int SnoozeDigital::pinMode( int _pin, int mode, int type ) {
    if ( _pin >= CORE_NUM_DIGITAL ) return -1;
    isUsed = true;
#if defined(KINETISK)
    pin            = pin | ( ( uint64_t )0x01 << _pin );// save pin
    irqType[_pin]  = type;// save type
    irqType[_pin] |= mode << 4;// save mode
#elif defined(KINETISL)
    pin            = pin | ( ( uint32_t )0x01 << _pin );// save pin
    irqType[_pin]  = type;// save type
    irqType[_pin] |= mode << 4;// save mode
#endif
    return _pin;
}

void SnoozeDigital::enableDriver( void ) {
    if ( mode == RUN_LP ) { return; }
#if defined(KINETISK)
    uint64_t _pin = pin;
    isr_pin = pin;
    if ( mode == VLPW || mode == VLPS ) {

        return_isr_a_enabled = NVIC_IS_ENABLED( IRQ_PORTA );
        return_isr_b_enabled = NVIC_IS_ENABLED( IRQ_PORTB );
        return_isr_c_enabled = NVIC_IS_ENABLED( IRQ_PORTC );
        return_isr_d_enabled = NVIC_IS_ENABLED( IRQ_PORTD );
        return_isr_e_enabled = NVIC_IS_ENABLED( IRQ_PORTE );
        NVIC_DISABLE_IRQ(IRQ_PORTA);
        NVIC_DISABLE_IRQ(IRQ_PORTB);
        NVIC_DISABLE_IRQ(IRQ_PORTC);
        NVIC_DISABLE_IRQ(IRQ_PORTD);
        NVIC_DISABLE_IRQ(IRQ_PORTE);
        NVIC_CLEAR_PENDING(IRQ_PORTA);
        NVIC_CLEAR_PENDING(IRQ_PORTB);
        NVIC_CLEAR_PENDING(IRQ_PORTC);
        NVIC_CLEAR_PENDING(IRQ_PORTD);
        NVIC_CLEAR_PENDING(IRQ_PORTE);
        
        int priority = nvic_execution_priority( );// get current priority
        // if running from interrupt set priority higher than current interrupt
        priority = ( priority < 256 ) && ( ( priority - 16 ) > 0 ) ? priority - 16 : 128;
        return_priority_a = NVIC_GET_PRIORITY( IRQ_PORTA );//get current priority
        return_priority_b = NVIC_GET_PRIORITY( IRQ_PORTB );//get current priority
        return_priority_c = NVIC_GET_PRIORITY( IRQ_PORTC );//get current priority
        return_priority_d = NVIC_GET_PRIORITY( IRQ_PORTD );//get current priority
        return_priority_e = NVIC_GET_PRIORITY( IRQ_PORTE );//get current priority
        NVIC_SET_PRIORITY( IRQ_PORTA, priority );//set priority to new level
        NVIC_SET_PRIORITY( IRQ_PORTB, priority );//set priority to new level
        NVIC_SET_PRIORITY( IRQ_PORTC, priority );//set priority to new level
        NVIC_SET_PRIORITY( IRQ_PORTD, priority );//set priority to new level
        NVIC_SET_PRIORITY( IRQ_PORTE, priority );//set priority to new level
        
        __disable_irq( );
        return_porta_irq = _VectorsRam[IRQ_PORTA+16];// save prev isr handler
        return_portb_irq = _VectorsRam[IRQ_PORTB+16];// save prev isr handler
        return_portc_irq = _VectorsRam[IRQ_PORTC+16];// save prev isr handler
        return_portd_irq = _VectorsRam[IRQ_PORTD+16];// save prev isr handler
        return_porte_irq = _VectorsRam[IRQ_PORTE+16];// save prev isr handler
        attachInterruptVector( IRQ_PORTA, isr );// set snooze digA isr
        attachInterruptVector( IRQ_PORTB, isr );// set snooze digB isr
        attachInterruptVector( IRQ_PORTC, isr );// set snooze digC isr
        attachInterruptVector( IRQ_PORTD, isr );// set snooze digD isr
        attachInterruptVector( IRQ_PORTE, isr );// set snooze digE isr
        __enable_irq( );
        
        NVIC_ENABLE_IRQ( IRQ_PORTA );
        NVIC_ENABLE_IRQ( IRQ_PORTB );
        NVIC_ENABLE_IRQ( IRQ_PORTC );
        NVIC_ENABLE_IRQ( IRQ_PORTD );
        NVIC_ENABLE_IRQ( IRQ_PORTE );
    }
    _pin = pin;
    while ( __builtin_popcountll( _pin ) ) {
        uint32_t pinNumber  = 63 - __builtin_clzll( _pin );// get pin
        
        if ( pinNumber > 33 ) break;
        
        uint32_t pin_mode = irqType[pinNumber] >> 4;// get type
        uint32_t pin_type = irqType[pinNumber] & 0x0F;// get mode
        
        volatile uint32_t *config;
        config = portConfigRegister( pinNumber );
        return_core_pin_config[pinNumber] = *config;
        
        if ( pin_mode == INPUT || pin_mode == INPUT_PULLUP ) {// setup pin mode/type/interrupt
            *portModeRegister( pinNumber ) = 0;
            if ( pin_mode == INPUT ) *config = PORT_PCR_MUX( 1 );
            else *config = PORT_PCR_MUX( 1 ) | PORT_PCR_PE | PORT_PCR_PS;// pullup
            if ( mode == VLPW || mode == VLPS ) {
                attachDigitalInterrupt( pinNumber, pin_type );// set pin interrupt
            }
            else {
                llwu_configure_pin_mask( pinNumber, mode );
            }
        } else {
            //pinMode( pinNumber, pin_mode );
            digitalWriteFast( pinNumber, pin_type );
        }
        _pin &= ~( ( uint64_t )1 << pinNumber );// remove pin from list
    }
#elif defined(KINETISL)
    uint32_t _pin = pin;
    if ( mode == VLPW || mode == VLPS ) {// if using sleep must setup pin interrupt to wake
        int priority = nvic_execution_priority( );// get current priority
        // if running from interrupt set priority higher than current interrupt
        priority = ( priority < 256 ) && ( ( priority - 16 ) > 0 ) ? priority - 16 : 128;
        return_priority_a = NVIC_GET_PRIORITY( IRQ_PORTA );//get current priority
        return_priority_b = NVIC_GET_PRIORITY( IRQ_PORTCD );//get current priority
        NVIC_SET_PRIORITY( IRQ_PORTA, priority );//set priority to new level
        NVIC_SET_PRIORITY( IRQ_PORTCD, priority );//set priority to new level
        __disable_irq( );
        return_porta_irq = _VectorsRam[IRQ_PORTA+16];// save prev isr handler
        return_portcd_irq = _VectorsRam[IRQ_PORTCD+16];// save prev isr handler
        attachInterruptVector( IRQ_PORTA, isr );// set snooze isr
        attachInterruptVector( IRQ_PORTCD, isr );// set snooze isr
        __enable_irq( );
    }
    _pin = pin;
    while ( __builtin_popcount( _pin ) ) {
        uint32_t pinNumber  = 31 - __builtin_clz( _pin );// get pin
        uint32_t _mode = irqType[pinNumber] >> 4;// get type
        uint32_t _type = irqType[pinNumber] & 0x0F;// get mode
        
        volatile uint32_t *config;
        config = portConfigRegister( pinNumber );
        return_core_pin_config[pinNumber] = *config;
        
        if ( mode == INPUT || mode == INPUT_PULLUP ) {// setup pin mode/type/interrupt
            *portModeRegister( pinNumber ) &= ~SnoozeDigitalPinToBitMask( pinNumber ); // TODO: atomic
            if ( mode == INPUT ) *config = PORT_PCR_MUX( 1 );
            else *config = PORT_PCR_MUX( 1 ) | PORT_PCR_PE | PORT_PCR_PS;// pullup
            if ( mode == VLPW || mode == VLPS ) {
               attachDigitalInterrupt( pinNumber, _type );// set pin interrupt
            }
            else {
                llwu_configure_pin_mask( pinNumber, mode );
            }
        } else {
            //pinMode( pinNumber, _mode );
            SnoozeDigitalWriteFast( pinNumber, _type );
        }
        _pin &= ~( ( uint32_t )1 << pinNumber );// remove pin from list
    }
#endif
}
/*******************************************************************************************/
void SnoozeDigital::disableDriver( void ) {
    if ( mode == RUN_LP ) { return; }
#if defined(KINETISK)
    uint64_t _pin = pin;
    while ( __builtin_popcountll( _pin ) ) {
        uint32_t pinNumber = 63 - __builtin_clzll( _pin );
        
        if ( pinNumber > 33 ) break;
        
#ifdef KINETISK
        *portModeRegister( pinNumber ) = 0;
#else
        *portModeRegister( pinNumber ) &= ~digitalPinToBitMask( pinNumber );
#endif
        volatile uint32_t *config;
        config = portConfigRegister( pinNumber );
        *config = return_core_pin_config[pinNumber];
        
        _pin &= ~( ( uint64_t )1 << pinNumber );// remove pin from list
    }
#elif defined(KINETISL)
    uint32_t _pin = pin;
    while ( __builtin_popcount( _pin ) ) {
        uint32_t pinNumber = 31 - __builtin_clz( _pin );
        
        if ( pinNumber > 33 ) break;
        
        volatile uint32_t *config;
        config = portConfigRegister( pinNumber );
        *config = return_core_pin_config[pinNumber];
        
        _pin &= ~( ( uint32_t )1 << pinNumber );// remove pin from list
    }
#endif
    if ( mode == VLPW || mode == VLPS ) {
#if defined(KINETISK)
        NVIC_SET_PRIORITY( IRQ_PORTA, return_priority_a );//set priority to new level
        NVIC_SET_PRIORITY( IRQ_PORTB, return_priority_b );//set priority to new level
        NVIC_SET_PRIORITY( IRQ_PORTC, return_priority_c );//set priority to new level
        NVIC_SET_PRIORITY( IRQ_PORTD, return_priority_d );//set priority to new level
        NVIC_SET_PRIORITY( IRQ_PORTE, return_priority_e );//set priority to new level
        __disable_irq( );
        attachInterruptVector( IRQ_PORTA, return_porta_irq );// set snooze isr
        attachInterruptVector( IRQ_PORTB, return_portb_irq );// set snooze isr
        attachInterruptVector( IRQ_PORTC, return_portc_irq );// set snooze isr
        attachInterruptVector( IRQ_PORTD, return_portd_irq );// set snooze isr
        attachInterruptVector( IRQ_PORTE, return_porte_irq );// set snooze isr
        __enable_irq( );
        if ( return_isr_a_enabled == 0 ) NVIC_DISABLE_IRQ( IRQ_PORTA );
        if ( return_isr_b_enabled == 0 ) NVIC_DISABLE_IRQ( IRQ_PORTB );
        if ( return_isr_c_enabled == 0 ) NVIC_DISABLE_IRQ( IRQ_PORTC );
        if ( return_isr_d_enabled == 0 ) NVIC_DISABLE_IRQ( IRQ_PORTD );
        if ( return_isr_e_enabled == 0 ) NVIC_DISABLE_IRQ( IRQ_PORTE );
        /*_pin = pin;
        while ( __builtin_popcountll( _pin ) ) {
            uint32_t pinNumber = 63 - __builtin_clzll( _pin );
            
            if ( pinNumber > 33 ) continue;
            
            if ( mode == VLPW ) {// if using sleep, give back previous isr handler
                if ( pinNumber == 3 || pinNumber == 4 || pinNumber == 24 || pinNumber == 33 ) {
                    NVIC_SET_PRIORITY( IRQ_PORTA, 128 );//return priority to core level
                    __disable_irq( );
                    attachInterruptVector( IRQ_PORTA, return_porta_irq );// return prev interrupt handler
                    __enable_irq( );
                }
                else if ( pinNumber == 0 || pinNumber == 1 || pinNumber == 16 || pinNumber == 17 || pinNumber == 18 || pinNumber == 19 || pinNumber == 25 || pinNumber == 32 ) {
                    NVIC_ENABLE_IRQ(IRQ_PORTB);
                    NVIC_SET_PRIORITY( IRQ_PORTB, 128 );//return priority to core level
                    __disable_irq( );
                    attachInterruptVector( IRQ_PORTB, return_portb_irq );// return prev interrupt handler
                    __enable_irq( );
                }
                else if ( pinNumber == 9 || pinNumber == 10 || pinNumber == 11 || pinNumber == 12 || pinNumber == 13 || pinNumber == 15 || pinNumber == 22 || pinNumber == 23 || pinNumber == 27 || pinNumber == 28 || pinNumber == 29 || pinNumber == 30 ) {
                    NVIC_SET_PRIORITY( IRQ_PORTC, 128 );//return priority to core level
                    __disable_irq( );
                    attachInterruptVector( IRQ_PORTC, return_portc_irq );// return prev interrupt handler
                    __enable_irq( );
                }
                else if ( pinNumber == 2 || pinNumber == 5 || pinNumber == 6 || pinNumber == 7 || pinNumber == 8 || pinNumber == 14 || pinNumber == 20 || pinNumber == 21 ) {
                    NVIC_SET_PRIORITY( IRQ_PORTD, 128 );//return priority to core level
                    __disable_irq( );
                    attachInterruptVector( IRQ_PORTD, return_portd_irq );// return prev interrupt handler
                    __enable_irq( );
                }
                else if ( pinNumber == 26 || pinNumber == 31 ) {
                    NVIC_SET_PRIORITY( IRQ_PORTE, 128 );//return priority to core level
                    __disable_irq( );
                    attachInterruptVector( IRQ_PORTE, return_porte_irq );// return prev interrupt handler
                    __enable_irq( );
                }
            }
            _pin &= ~( ( uint64_t )1 << pinNumber );// remove pin from list
        }*/
#elif defined(KINETISL)
        NVIC_SET_PRIORITY( IRQ_PORTA, return_priority_a );//set priority to new level
        NVIC_SET_PRIORITY( IRQ_PORTCD, return_priority_cd );//set priority to new level
        __disable_irq( );
        attachInterruptVector( IRQ_PORTA, return_porta_irq );// set snooze isr
        attachInterruptVector( IRQ_PORTCD, return_portcd_irq );// set snooze isr
        __enable_irq( );
        if ( return_isr_a_enabled == 0 ) NVIC_DISABLE_IRQ( IRQ_PORTA );
        if ( return_isr_cd_enabled == 0 ) NVIC_DISABLE_IRQ( IRQ_PORTCD );
        /*_pin = pin;
        while ( __builtin_popcount( _pin ) ) {
            uint32_t pinNumber = 31 - __builtin_clz( _pin );
            if ( mode == VLPW ) {// if using sleep give back previous isr handler
                if ( pinNumber == 3 || pinNumber == 4 ) {
                    NVIC_SET_PRIORITY( IRQ_PORTA, 128 );//return priority to core level
                    __disable_irq( );
                    attachInterruptVector( IRQ_PORTA, return_porta_irq );// return prev interrupt handler
                    __enable_irq( );
                }
                else if ( pinNumber == 2 || pinNumber == 5 || pinNumber == 6 || pinNumber == 7 || pinNumber == 8 || pinNumber == 14 || pinNumber == 20 || pinNumber == 21 || pinNumber == 9 || pinNumber == 10 || pinNumber == 11 || pinNumber == 12 || pinNumber == 13 || pinNumber == 15 || pinNumber == 22 || pinNumber == 23 ) {
                    NVIC_SET_PRIORITY( IRQ_PORTCD, 128 );//return priority to core level
                    __disable_irq( );
                    attachInterruptVector( IRQ_PORTCD, return_portcd_irq );// return prev interrupt handler
                    __enable_irq( );
                }
            }
            _pin &= ~( ( uint32_t )1 << pinNumber );// remove pin from list
        }*/
#endif
    }
}
/*******************************************************************************************/
void SnoozeDigital::clearIsrFlags( void ) {
    isr( );
}
/*******************************************************************************************/
void SnoozeDigital::isr( void ) {
    digitalWriteFast(LED_BUILTIN, HIGH);
    uint32_t isfr_a = PORTA_ISFR;
    PORTA_ISFR = isfr_a;
#if defined(KINETISK)
    uint32_t isfr_b = PORTB_ISFR;
    uint32_t isfr_e = PORTE_ISFR;
    PORTB_ISFR = isfr_b;
    PORTE_ISFR = isfr_e;
#endif
    uint32_t isfr_c = PORTC_ISFR;
    uint32_t isfr_d = PORTD_ISFR;
    PORTC_ISFR = isfr_c;
    PORTD_ISFR = isfr_d;

    if ( mode == LLS || mode == VLLS3 ) return;
    
    uint64_t _pin = isr_pin;
    while ( __builtin_popcountll( _pin ) ) {
        uint32_t pinNumber = 63 - __builtin_clzll( _pin );
        
        if ( pinNumber > 33 ) return;
        
        detachDigitalInterrupt( pinNumber );// remove pin interrupt
        
        _pin &= ~( ( uint64_t )1 << pinNumber );// remove pin from list
    }
    
    if ( isfr_a & CORE_PIN3_BITMASK ) source = 3;
    else if ( isfr_a & CORE_PIN4_BITMASK ) source = 4;
#if defined(KINETISK)
    else if ( isfr_a & CORE_PIN24_BITMASK ) source = 24;
    else if ( isfr_a & CORE_PIN33_BITMASK ) source = 33;
    else if ( isfr_b & CORE_PIN0_BITMASK ) source = 0;
    else if ( isfr_b & CORE_PIN1_BITMASK )  source = 1;
    else if ( isfr_b & CORE_PIN16_BITMASK ) source = 16;
    else if ( isfr_b & CORE_PIN17_BITMASK ) source = 17;
    else if ( isfr_b & CORE_PIN18_BITMASK ) source = 18;
    else if ( isfr_b & CORE_PIN19_BITMASK ) source = 19;
    else if ( isfr_b & CORE_PIN25_BITMASK ) source = 25;
    else if ( isfr_b & CORE_PIN32_BITMASK ) source = 32;
#endif
    if ( isfr_c & CORE_PIN9_BITMASK ) source = 9;
    else if ( isfr_c & CORE_PIN10_BITMASK ) source = 10;
    else if ( isfr_c & CORE_PIN11_BITMASK ) source = 11;
    else if ( isfr_c & CORE_PIN12_BITMASK ) source = 12;
    else if ( isfr_c & CORE_PIN13_BITMASK ) source = 13;
    else if ( isfr_c & CORE_PIN15_BITMASK ) source = 15;
    else if ( isfr_c & CORE_PIN22_BITMASK ) source = 22;
    else if ( isfr_c & CORE_PIN23_BITMASK ) source = 23;
#if defined(KINETISK)
    else if ( isfr_c & CORE_PIN27_BITMASK ) source = 27;
    else if ( isfr_c & CORE_PIN28_BITMASK ) source = 28;
    else if ( isfr_c & CORE_PIN29_BITMASK ) source = 29;
    else if ( isfr_c & CORE_PIN30_BITMASK ) source = 30;
#endif
    if ( isfr_d & CORE_PIN2_BITMASK ) source = 2;
    else if ( isfr_d & CORE_PIN5_BITMASK ) source = 5;
    else if ( isfr_d & CORE_PIN6_BITMASK )  source = 6;
    else if ( isfr_d & CORE_PIN7_BITMASK )  source = 7;
    else if ( isfr_d & CORE_PIN8_BITMASK )  source = 8;
    else if ( isfr_d & CORE_PIN14_BITMASK ) source = 14;
    else if ( isfr_d & CORE_PIN20_BITMASK ) source = 20;
    else if ( isfr_d & CORE_PIN21_BITMASK ) source = 21;
#if defined(KINETISK)
    if ( isfr_e & CORE_PIN26_BITMASK ) source = 26;
    else if ( isfr_e & CORE_PIN31_BITMASK ) source = 31;
#endif
}
/*******************************************************************************************/
void SnoozeDigital::attachDigitalInterrupt( uint8_t pin, int mode ) {
    volatile uint32_t *config;
    uint32_t cfg, mask;
    
    if ( pin >= CORE_NUM_DIGITAL ) return;
    switch ( mode ) {
        case CHANGE:	mask = 0x0B; break;
        case RISING:	mask = 0x09; break;
        case FALLING:	mask = 0x0A; break;
        case LOW:	mask = 0x08; break;
        case HIGH:	mask = 0x0C; break;
        default: return;
    }
    __disable_irq( );
    mask = ( mask << 16 ) | 0x01000000;
    config = portConfigRegister( pin );
    cfg = *config;
    cfg &= ~0x000F0000;		// disable any previous interrupt
    *config = cfg;
    cfg |= mask;
    *config = cfg;			// enable the new interrupt
    __enable_irq( );
    
}
/*******************************************************************************************/
void SnoozeDigital::detachDigitalInterrupt( uint8_t pin ) {
    volatile uint32_t *config;
    __disable_irq( );
    config = portConfigRegister( pin );
    *config = ( ( *config & ~0x000F0000 ) | 0x01000000 );
    __enable_irq( );
}