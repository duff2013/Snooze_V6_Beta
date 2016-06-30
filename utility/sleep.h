/********************************************************************************
 Low Power Library for Teensy LC/3.x
 * Copyright (c) 2014, Colin Duffy https://github.com/duff2013
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice, development funding notice, and this permission
 * notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 ********************************************************************************
 *  smc.h
 *  Teensy 3.x/LC
 *
 * Purpose: Sleep Routines
 *
 *******************************************************************************/
#ifndef __SMC_H__
#define __SMC_H__
/*******************************************************************************/
#include "util/atomic.h"
/*******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
    static inline void stop( void );
    //static inline void wait( void );
    /*******************************************************************************
     * WAIT mode entry routine. Puts the processor into wait mode.
     * In this mode the core clock is disabled (no code executing), but
     * bus clocks are enabled (peripheral modules are operational).
     *
     * Mode transitions:
     * RUN -> WAIT
     * VLPR -> VLPW
     *
     * This function can be used to enter normal wait mode or VLPW
     * mode. If you are executing in normal run mode when calling this
     * function, then you will enter normal wait mode. If you are in VLPR
     * mode when calling this function, then you will enter VLPW mode instead.
     *
     * WAIT mode is exited using any enabled interrupt or RESET, so no
     * exit_wait routine is needed.
     *
     * If in VLPW mode, the status of the SMC_PMCTRL[LPWUI] bit determines if
     * the processor exits to VLPR (LPWUI cleared) or normal run mode (LPWUI
     * set). The enable_lpwui() and disable_lpwui() functions can be used to
     * set this bit to the desired option prior to calling enter_wait().
     *
     * NOTE:    Some modules include a programmable option to disable them in
     *          wait mode. If those modules are programmed to disable in wait
     *          mode, they will not be able to generate interrupts to wake up
     *          the core.
     *******************************************************************************/
    static inline
    void enter_wait( void )
    __attribute__((always_inline, unused));
    
    static inline
    void enter_wait( void ) {
        //wait( );
    }
    
    /*******************************************************************************
     * STOP mode entry routine. Puts the processor into normal stop mode.
     * In this mode core, bus and peripheral clocks are disabled.
     *
     * Mode transitions:
     * RUN -> STOP
     * VLPR -> VLPS
     *
     * This function can be used to enter normal stop mode or VLPS
     * mode. If you are executing in normal run mode when calling this
     * function, then you will enter normal stop mode. If you are in VLPR
     * mode when calling this function, then you will enter VLPS mode instead.
     *
     * STOP mode is exited using any enabled interrupt or RESET, so no
     * exit_stop routine is needed.
     *******************************************************************************/
    static inline
    void _stop( void )
    __attribute__((always_inline, unused));
    
    static inline
    void _stop( void ) {
        // Set the STOPM field to 0b000 for normal STOP mode - Need to
        // retain state of LPWUI bit 8
        SMC_PMCTRL =  SMC_PMCTRL_STOPM( 0x00 );// set STOPM = 0b000
        ( void ) SMC_PMCTRL;
        stop( );
    }
    
    /*******************************************************************************
     * VLPR mode entry routine. Puts the processor into very low power
     * run mode. In this mode all clocks are enabled, but the core, bus,
     * and peripheral clocks are limited to 2MHz or less. The flash
     * clock is limited to 1MHz or less.
     *
     * Mode transitions:
     * RUN -> VLPR
     *
     * exit_vlpr() function or an interrupt with LPWUI set can be used
     * to switch from VLPR back to RUN. The enable_lpwui() and disable_lpwui()
     * functions can be used to set LPWUI to the desired option prior to
     * calling enter_vlpr().
     *******************************************************************************/
    static inline
    void enter_vlpr( void )
    __attribute__((always_inline, unused));
    
    static inline
    void enter_vlpr( void ) {
        if ( SMC_PMSTAT & SMC_PMSTAT_VLPR ) return;
        
        SMC_PMCTRL = SMC_PMCTRL_RUNM( 0 );
        ( void ) SMC_PMCTRL;
        while( !( SMC_PMSTAT & SMC_PMSTAT_RUN ) );
        
        SMC_PMCTRL = SMC_PMCTRL_RUNM( 2 );
        ( void ) SMC_PMCTRL;
        
        // Wait for VLPS regulator mode to be confirmed
        while( ( PMC_REGSC & PMC_REGSC_REGONS ) ); // 0 Regulator in stop Reg mode
        
        // 1 MCU is in Run regulation mode
        while( !( SMC_PMSTAT & SMC_PMSTAT_VLPR ) );
    }
    
    /*******************************************************************************
     * VLPR mode exit routine. Puts the processor into normal run mode
     * from VLPR mode. You can transition from VLPR to normal run using
     * this function or an interrupt with LPWUI set. The enable_lpwui()
     * and disable_lpwui() functions can be used to set LPWUI to the
     * desired option prior to calling enter_vlpr().
     *
     * Mode transitions:
     * VLPR -> RUN
     *******************************************************************************/
    static inline
    void exit_vlpr( void )
    __attribute__((always_inline, unused));
    
    static inline
    void exit_vlpr( void ) {
        /* check to make sure in VLPR before exiting    */
        if  ( !( SMC_PMSTAT & SMC_PMSTAT_VLPR ) ) return;
        // Clear RUNM
        
#if defined(__MK66FX1M0__)
    #if F_CPU > 120000000
        SMC_PMCTRL = SMC_PMCTRL_RUNM( 0 );
        ( void ) SMC_PMCTRL;
        while( !( SMC_PMSTAT & SMC_PMSTAT_RUN ) );
        
        SMC_PMCTRL = SMC_PMCTRL_RUNM( 0x03 );
        ( void ) SMC_PMCTRL;
    #else
        SMC_PMCTRL = SMC_PMCTRL_RUNM( 0x00 );
        ( void ) SMC_PMCTRL;
    #endif
#else
        SMC_PMCTRL = SMC_PMCTRL_RUNM( 0x00 );
        ( void ) SMC_PMCTRL;
#endif
        
        // Wait for normal RUN regulation mode to be confirmed
        // 0 MCU is not in run regulation mode
        // 1 MCU is in run regulation mode
        while( !( PMC_REGSC & PMC_REGSC_REGONS ) );
#if defined(__MK66FX1M0__)
    #if F_CPU > 120000000
        while( !( SMC_PMSTAT & SMC_PMSTAT_HSRUN ) );
    #else
        while( !( SMC_PMSTAT & SMC_PMSTAT_RUN ) );
    #endif
#else
        while( !( SMC_PMSTAT & SMC_PMSTAT_RUN ) );
#endif
        

    }
    
    /*******************************************************************************
     * VLPS mode entry routine. Puts the processor into VLPS mode directly
     * from normal run mode.
     *
     * Mode transitions:
     * RUN -> VLPS
     *
     * If VLPS mode is entered directly from normal RUN mode, then the
     * LPWUI bit is forced to 1 by hardware. This means that when an
     * interrupt occurs you will exit to normal run mode instead of VLPR.
     *
     * If however VLPS mode is entered from VLPR the state of the LPWUI bit
     * determines the state the MCU will return to upon exit from VLPS. If LPWUI
     * is set and an interrupt occurs you will exit to normal run mode instead
     * of VLPR. If LPWUI is clear and an interrupt occurs you will exit to VLPR.
     *
     * Parameters:  value of LPWUI
     *******************************************************************************/
    static inline
    void vlps( void )
    __attribute__((always_inline, unused));
    
    static inline
    void vlps( void ) {
#if defined(__MK66FX1M0__)
#if F_CPU > 120000000
        SMC_PMCTRL = SMC_PMCTRL_RUNM( 0x03 ) | SMC_PMCTRL_STOPM( 0x03 );
#else
        SMC_PMCTRL = SMC_PMCTRL_STOPM( 0x03 ) ;
        ( void ) SMC_PMCTRL;
#endif
#else
        SMC_PMCTRL = SMC_PMCTRL_STOPM( 0x03 ) ;
        ( void ) SMC_PMCTRL;
#endif
        // Now execute the stop instruction to go into VLPS
        stop( );
    }
    
    /*******************************************************************************
     * LLS mode entry routine. Puts the processor into LLS mode from
     * normal run mode or VLPR.
     *
     * Mode transitions:
     * RUN -> LLS
     * VLPR -> LLS
     *
     * Wakeup from LLS mode is controlled by the LLWU module. Most
     * modules cannot issue a wakeup interrupt in LLS mode, so make
     * sure to setup the desired wakeup sources in the LLWU before
     * calling this function.
     *
     * NOTE:    LLS mode will always exit to RUN mode even if you were
     *          in VLPR mode before entering LLS.
     *******************************************************************************/
    static inline
    void lls( void )
    __attribute__((always_inline, unused));
    
    static inline
    void lls( void ) {
#if defined(__MK66FX1M0__)
#if F_CPU > 120000000
        SMC_PMCTRL = SMC_PMCTRL_RUNM( 0x03 ) | SMC_PMCTRL_STOPM( 0x03 );
#else
        SMC_PMCTRL = SMC_PMCTRL_STOPM( 0x03 ) ;
        ( void ) SMC_PMCTRL;
#endif
#else
        SMC_PMCTRL = SMC_PMCTRL_STOPM( 0x03 ) ;
        ( void ) SMC_PMCTRL;
#endif
        // Now execute the stop instruction to go into LLS
        stop( );
    }
    
    /*******************************************************************************
     * VLLS3 mode entry routine. Puts the processor into VLLS3 mode from
     * normal run mode or VLPR.
     *
     * Mode transitions:
     * RUN -> VLLS3
     * VLPR -> VLLS3
     *
     * Wakeup from VLLSx mode is controlled by the LLWU module. Most
     * modules cannot issue a wakeup interrupt in VLLSx mode, so make
     * sure to setup the desired wakeup sources in the LLWU before
     * calling this function.
     *
     * NOTE:    VLLSx modes will always exit to RUN mode even if you were
     *          in VLPR mode before entering VLLSx.
     *******************************************************************************/
    static inline
    void vlls3( void )
    __attribute__((always_inline, unused));
    
    static inline
    void vlls3( void ) {
#if defined(__MK66FX1M0__)
#if F_CPU > 120000000
        SMC_PMCTRL = SMC_PMCTRL_RUNM( 0x03 ) | SMC_PMCTRL_STOPM( 0x04 );
#else
        SMC_PMCTRL = SMC_PMCTRL_STOPM( 0x04 ) ;
        ( void ) SMC_PMCTRL;
#endif
#else
        SMC_PMCTRL = SMC_PMCTRL_STOPM( 0x04 ) ;
        ( void ) SMC_PMCTRL;
#endif
        SMC_VLLSCTRL =  SMC_VLLSCTRL_VLLSM( 0x03 );// set VLLSM = 0b11
        ( void ) SMC_VLLSCTRL;
        // Now execute the stop instruction to go into VLLS3
        stop( );
    }
    
    /*******************************************************************************
     * VLLS2 mode entry routine. Puts the processor into VLLS2 mode from
     * normal run mode or VLPR.
     *
     * Mode transitions:
     * RUN -> VLLS2
     * VLPR -> VLLS2
     *
     * Wakeup from VLLSx mode is controlled by the LLWU module. Most
     * modules cannot issue a wakeup interrupt in VLLSx mode, so make
     * sure to setup the desired wakeup sources in the LLWU before
     * calling this function.
     *
     * NOTE:    VLLSx modes will always exit to RUN mode even if you were
     *          in VLPR mode before entering VLLSx.
     *******************************************************************************/
    static inline
    void vlls2( void )
    __attribute__((always_inline, unused));
    
    static inline
    void vlls2( void ) {
#if defined(__MK66FX1M0__)
#if F_CPU > 120000000
        SMC_PMCTRL = SMC_PMCTRL_RUNM( 0x03 ) | SMC_PMCTRL_STOPM( 0x04 );
#else
        SMC_PMCTRL = SMC_PMCTRL_STOPM( 0x04 ) ;
        ( void ) SMC_PMCTRL;
#endif
#else
        SMC_PMCTRL = SMC_PMCTRL_STOPM( 0x04 ) ;
        ( void ) SMC_PMCTRL;
#endif
        SMC_VLLSCTRL =  SMC_VLLSCTRL_VLLSM( 0x02 );// set VLLSM = 0b11
        ( void ) SMC_VLLSCTRL;
        // Now execute the stop instruction to go into VLLS2
        stop( );
    }
    
    /*******************************************************************************
     * VLLS1 mode entry routine. Puts the processor into VLLS1 mode from
     * normal run mode or VLPR.
     *
     * Mode transitions:
     * RUN -> VLLS1
     * VLPR -> VLLS1
     *
     * Wakeup from VLLSx mode is controlled by the LLWU module. Most
     * modules cannot issue a wakeup interrupt in VLLSx mode, so make
     * sure to setup the desired wakeup sources in the LLWU before
     * calling this function.
     *
     * NOTE:    VLLSx modes will always exit to RUN mode even if you were
     *          in VLPR mode before entering VLLSx.
     *******************************************************************************/
    static inline
    void vlls1( void )
    __attribute__((always_inline, unused));
    
    static inline
    void vlls1( void ) {
#if defined(__MK66FX1M0__)
#if F_CPU > 120000000
        SMC_PMCTRL = SMC_PMCTRL_RUNM( 0x03 ) | SMC_PMCTRL_STOPM( 0x04 );
#else
        SMC_PMCTRL = SMC_PMCTRL_STOPM( 0x04 ) ;
        ( void ) SMC_PMCTRL;
#endif
#else
        SMC_PMCTRL = SMC_PMCTRL_STOPM( 0x04 ) ;
        ( void ) SMC_PMCTRL;
#endif
        SMC_VLLSCTRL =  SMC_VLLSCTRL_VLLSM( 0x01 );// set VLLSM = 0b11
        ( void ) SMC_VLLSCTRL;
        // Now execute the stop instruction to go into VLLS1
        stop( );
    }
    
    /*******************************************************************************
     * VLLS0 mode entry routine. Puts the processor into VLLS0 mode from
     * normal run mode or VLPR.
     *
     * Mode transitions:
     * RUN -> VLLS0
     * VLPR -> VLLS0
     *
     * Wakeup from VLLSx mode is controlled by the LLWU module. Most
     * modules cannot issue a wakeup interrupt in VLLSx mode, so make
     * sure to setup the desired wakeup sources in the LLWU before
     * calling this function.
     *
     * NOTE:    VLLSx modes will always exit to RUN mode even if you were
     *          in VLPR mode before entering VLLSx.
     *******************************************************************************/
    static inline
    void vlls0( void )
    __attribute__((always_inline, unused));
    
    static inline
    void vlls0( void ) {
#if defined(__MK66FX1M0__)
#if F_CPU > 120000000
        SMC_PMCTRL = SMC_PMCTRL_RUNM( 0x03 ) | SMC_PMCTRL_STOPM( 0x04 );
#else
        SMC_PMCTRL = SMC_PMCTRL_STOPM( 0x04 ) ;
        ( void ) SMC_PMCTRL;
#endif
#else
        SMC_PMCTRL = SMC_PMCTRL_STOPM( 0x04 ) ;
        ( void ) SMC_PMCTRL;
#endif
        SMC_VLLSCTRL =  SMC_VLLSCTRL_VLLSM( 0x00 );// set VLLSM = 0b11
        ( void ) SMC_VLLSCTRL;
        // Now execute the stop instruction to go into VLLS0
        stop( );
    }
    
    /*******************************************************************************
     * VLLS0 mode entry routine. Puts the processor into VLLS0 mode from
     * normal run mode or VLPR with the POR circuit disabled
     *
     * Mode transitions:
     * RUN -> VLLS0
     * VLPR -> VLLS0
     *
     * Wakeup from VLLSx mode is controlled by the LLWU module. Most
     * modules cannot issue a wakeup interrupt in VLLSx mode, so make
     * sure to setup the desired wakeup sources in the LLWU before
     * calling this function.
     *
     * NOTE:    VLLSx modes will always exit to RUN mode even if you were
     *          in VLPR mode before entering VLLSx.
     *******************************************************************************/
    static inline
    void vlls0_nopor( void )
    __attribute__((always_inline, unused));
    
    static inline
    void vlls0_nopor( void ) {
#if defined(__MK66FX1M0__)
#if F_CPU > 120000000
        SMC_PMCTRL = SMC_PMCTRL_RUNM( 0x03 ) | SMC_PMCTRL_STOPM( 0x04 );
#else
        SMC_PMCTRL = SMC_PMCTRL_STOPM( 0x03 ) ;
        ( void ) SMC_PMCTRL;
#endif
#else
        SMC_PMCTRL = SMC_PMCTRL_STOPM( 0x03 ) ;
        ( void ) SMC_PMCTRL;
#endif
        SMC_VLLSCTRL =  SMC_VLLSCTRL_VLLSM( 0x00 );// set VLLSM = 0b11
        ( void ) SMC_VLLSCTRL;
        // Now execute the stop instruction to go into VLLS1
        stop( );
    }
    
    /*******************************************************************************
     * Enable low power wake up on interrupt. This function can be used
     * to set the LPWUI bit. When this bit is set VLPx modes will exit
     * to normal run mode. When this bit is cleared VLPx modes will exit
     * to VLPR mode.
     *
     * The disable_lpwui() function can be used to clear the LPWUI bit.
     *******************************************************************************/
    static inline
    void enable_lpwui( void )
    __attribute__((always_inline, unused));
    
    static inline
    void enable_lpwui( void ) {
        //SMC_PMCTRL |= SMC_PMCTRL_LPWUI;
    }
    
    /*******************************************************************************
     * Disable low power wake up on interrupt. This function can be used
     * to clear the LPWUI bit. When this bit is set VLPx modes will exit
     * to normal run mode. When this bit is cleared VLPx modes will exit
     * to VLPR mode.
     *
     * The enable_lpwui() function can be used to set the LPWUI bit.
     *******************************************************************************/
    static inline
    void disable_lpwui( void )
    __attribute__((always_inline, unused));
    
    static inline
    void disable_lpwui( void ) {
        //#if defined(__MK20DX128__)
        //SMC_PMCTRL &= ~SMC_PMCTRL_LPWUI;
        //#endif
    }
    
    /*******************************************************************************
     * Configures the ARM system control register for STOP (deep sleep) mode
     * and then executes the WFI instruction to enter the mode.
     *
     * Note:    Might want to change this later to allow for passing in a parameter
     *          to optionally set the sleep on exit bit.
     *******************************************************************************/
    static inline
    void stop( void )
    __attribute__((always_inline, unused));
    
    static inline
    void stop( void ) {
        SYST_CSR &= ~SYST_CSR_TICKINT;      // disable systick timer interrupt
        SCB_SCR = SCB_SCR_SLEEPDEEP;  // Set the SLEEPDEEP bit to enable deep sleep mode (STOP)
        ATOMIC_BLOCK( ATOMIC_RESTORESTATE ) { asm volatile( "wfi" ); } // WFI instruction will start entry into STOP mode
        SCB_SCR = 0; // Clear the SLEEPDEEP bit
        SYST_CSR |= SYST_CSR_TICKINT;       // renable systick timer interrupt
    }
    
    /*******************************************************************************
     * Configures the ARM system control register for VLPW (sleep) mode
     * and then executes the WFI instruction to enter the mode.
     *******************************************************************************/
    static inline
    void vlpw( void )
    __attribute__((always_inline, unused));
    
    static inline
    void vlpw( void ) {
        SYST_CSR &= ~SYST_CSR_TICKINT;      // disable systick timer interrupt
        //SCB_SCR = SCB_SCR_SLEEPONEXIT; // Clear the SLEEPDEEP bit to make sure we go into WAIT (sleep) mode instead of deep sleep.
        ATOMIC_BLOCK( ATOMIC_RESTORESTATE ) { asm volatile( "wfi" ); }// WFI instruction will start entry into WAIT mode
        SYST_CSR |= SYST_CSR_TICKINT;       // renable systick timer interrupt
    }
    /*******************************************************************************
     * Configures the ARM system control register for WAIT (sleep) mode
     * and then executes the WFI instruction to enter the mode.
     *******************************************************************************/
    static inline
    void wait( void )
    __attribute__((always_inline, unused));
    
    static inline
    void wait( void ) {
        SCB_SCR = 0; // Clear the SLEEPDEEP bit to make sure we go into WAIT (sleep) mode instead of deep sleep.
        ATOMIC_BLOCK( ATOMIC_RESTORESTATE ) { asm volatile( "wfi" ); }// WFI instruction will start entry into WAIT mode
    }
    
#ifdef __cplusplus
}
#endif
/*******************************************************************************/
#endif /* __SMC_H__ */
