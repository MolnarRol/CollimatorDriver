/**
 * @file pwm.c
 * @brief Pulse width modulation functions
 * @details Details
 *
 * =================================================================
 * @author Bc. Roland Molnar
 *
 * =================================================================
 * KEM, FEI, TUKE
 * @date 17.03.2024
 * @defgroup PWM Pulse width modulator
 * @{
 */
#include <PWM_core.h>
#include <TRAN.h>

const static boolean s_PWM_Initialized_b = False_b;

/**
 * @brief PWM initialization function
 * @details Three PWM phases use ePWM3 and ePWM6
 */
void PWM_Init(void)
{
    if(s_PWM_Initialized_b != False_b)                          /* Lock running initialization more than once. */
    {
        return;
    }
    *((boolean*)&s_PWM_Initialized_b) = True_b;

    PWM_InitGpioPins();
    EALLOW;                                                     /* Enable writing protected registers. */
    /* Enable clocks for timers. */
    CpuSysRegs.PCLKCR2.bit.EPWM3        = (U16)1;               /* Enable clocking of EPWM3 module. */
    CpuSysRegs.PCLKCR2.bit.EPWM6        = (U16)1;               /* Enable clocking of EPWM6 module. */
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC    = (U16)0;               /* Stop clock synch. */

    /* Timer 6 setup */
    EPwm6Regs.TBCTL.bit.FREE_SOFT       = (U16)3;               /* Free running in debug. */
    EPwm6Regs.TBCTL.bit.CLKDIV          = (U16)0;               /* No division. */
    EPwm6Regs.TBCTL.bit.HSPCLKDIV       = (U16)0;               /* No division. */
    EPwm6Regs.TBCTL.bit.PHSDIR          = (U16)1;               /* Start with counting up. */
    EPwm6Regs.TBCTL.bit.CTRMODE         = (U16)2;               /* Up-down counter. (Triangle) */
    EPwm6Regs.TBPRD                     = PWM_PERIOD_dU16;      /* PWM period setup. */
    EPwm6Regs.AQCTLA.bit.CAU            = (U16)2;               /* Set pin high on A compare in up counting. */
    EPwm6Regs.AQCTLA.bit.CAD            = (U16)1;               /* Set pin low on A compare in down counting. */
    EPwm6Regs.AQCTLB.bit.CBU            = (U16)2;               /* Set pin high on B compare in up counting. */
    EPwm6Regs.AQCTLB.bit.CBD            = (U16)1;               /* Set pin low on B compare in down counting. */

    EPwm6Regs.CMPA.bit.CMPA             = PWM_DEFAULT_CMP_dU16; /* Setting compare > period - pin is low after initialization. */
    EPwm6Regs.CMPB.bit.CMPB             = PWM_DEFAULT_CMP_dU16; /* Setting compare > period - pin is low after initialization. */
    EPwm3Regs.TBCTL.bit.SWFSYNC         = (U16)1;               /* Sync the timer clock. */

    /* Timer 3 setup */
    EPwm3Regs.TBCTL.bit.FREE_SOFT       = (U16)3;               /* Free running in debug. */
    EPwm3Regs.TBCTL.bit.CLKDIV          = (U16)0;               /* No division. */
    EPwm3Regs.TBCTL.bit.HSPCLKDIV       = (U16)0;               /* No division. */
    EPwm3Regs.TBCTL.bit.PHSDIR          = (U16)1;               /* Start with counting up. */
    EPwm3Regs.TBCTL.bit.CTRMODE         = (U16)2;               /* Up-down counter. (Triangle) */
    EPwm3Regs.TBPRD                     = PWM_PERIOD_dU16;      /* PWM period setup. */
    EPwm3Regs.AQCTLA.bit.CAU            = (U16)2;               /* Set pin high on A compare in up counting. */
    EPwm3Regs.AQCTLA.bit.CAD            = (U16)1;               /* Set pin low on A compare in down counting. */
    EPwm3Regs.TBCTL.bit.SWFSYNC         = (U16)1;               /* Sync the timer clock. */

    EPwm3Regs.CMPA.bit.CMPA             = PWM_DEFAULT_CMP_dU16; /* Setting compare > period - pin is low after initialization. */

    /* ADC SOC trigger setup. */
    EPwm3Regs.ETSEL.bit.SOCAEN          = (U16)1;               /* Enable start of conversion pulse. */
    EPwm3Regs.ETSEL.bit.SOCASEL         = (U16)2;               /* Start of conversion pulse on counter max value. */

    EPwm3Regs.ETSEL.bit.SOCBEN          = (U16)1;
    EPwm3Regs.ETSEL.bit.SOCBSEL         = (U16)2;
    EPwm3Regs.ETPS.bit.SOCPSSEL         = (U16)1;
    EPwm3Regs.ETSOCPS.bit.SOCAPRD2      = (U16)1;
    EPwm3Regs.ETSOCPS.bit.SOCBPRD2      = (U16)10;

    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC    = (U16)1;               /* Sync PWM clocks */
    EDIS;                                                       /* Disable writing to protected registers. */
}

/**
 * @brief               Set compare values for phases
 * @param comp_U_U16    is a compare value for phase U.
 * @param comp_V_U16    is a compare value for phase V.
 * @param comp_W_U16    is a compare value for phase W.
 */
void PWM_SetCompareValues(const U16 comp_U_U16, const U16 comp_V_U16, const U16 comp_W_U16)
{
    EPwm3Regs.CMPA.bit.CMPA = comp_U_U16;
    EPwm6Regs.CMPB.bit.CMPB = comp_V_U16;
    EPwm6Regs.CMPA.bit.CMPA = comp_W_U16;
}

/**
 *  @brief Pwm output pin configuration
 */
static inline void PWM_InitGpioPins(void)
{
    EALLOW;
    /************************
     *      Phase pins      *
     ************************/
    /* Phase U - GPIO 4 - EPWM3A */
    GpioCtrlRegs.GPAMUX1.bit.GPIO4      = (U16)1;           /* Set pin to PWM function. */
    GpioCtrlRegs.GPAGMUX1.bit.GPIO4     = (U16)0;

    /* Phase V - GPIO11 - EPWM6B */
    GpioCtrlRegs.GPAMUX1.bit.GPIO11     = (U16)1;           /* Set pin to PWM function. */
    GpioCtrlRegs.GPAGMUX1.bit.GPIO11    = (U16)0;

    /* Phase W - GPIO10 - EPWM6A */
    GpioCtrlRegs.GPAMUX1.bit.GPIO10     = (U16)1;           /* Set pin to PWM function. */
    GpioCtrlRegs.GPAGMUX1.bit.GPIO10    = (U16)0;

    PWM_SetOutputEnable(False_b);                           /* Set outputs to predefined state. */

    /************************
     * Phase shutdown pins  *
     ************************/
    /* Phase U shutdown - GPIO 2 */
    GpioCtrlRegs.GPADIR.bit.GPIO2       = (U16)1;
    GpioCtrlRegs.GPAPUD.bit.GPIO2       = (U16)1;

    /* Phase V shutdown - GPIO 12 */
    GpioCtrlRegs.GPADIR.bit.GPIO12      = (U16)1;
    GpioCtrlRegs.GPAPUD.bit.GPIO12      = (U16)1;

    /* Phase W shutdown - GPIO 3 */
    GpioCtrlRegs.GPADIR.bit.GPIO3       = (U16)1;
    GpioCtrlRegs.GPAPUD.bit.GPIO3       = (U16)1;

    EDIS;
}

/**
 * @brief           Enable phase driver outputs
 * @param enable_b  sets driver enable status
 * @info            Without setting output enable the PWM will not propagate to phase output.
 */
void PWM_SetOutputEnable(const boolean enable_b)
{
    /* Disable phase outputs - set pins to low. */
    if(enable_b == False_b)
    {
        GpioDataRegs.GPACLEAR.all |= ( (1 << 2) | (1 << 3) | (1 << 12) );
    }
    /* Enable phase outputs - set pins to high. */
    else
    {
        GpioDataRegs.GPASET.all |= ( (1 << 2) | (1 << 3) | (1 << 12) );
    }
}

void PWM_ForceAngle(const F32 forced_angle__rad__F32, const F32 voltage_amplitude__V__F32, const F32 dc_link__V__F32)
{
    TRAN_struct local_transf_s =
    {
     .dq_s = {
              .d_F32 = voltage_amplitude__V__F32,
              .q_F32 = (F32)0.0
     },
     .angle__rad__F32 = forced_angle__rad__F32
    };

    TRAN_DqToAbc(&local_transf_s);
    PWM_SetCompareValues(PWM_DUTY_TO_CMP_dMU16( (local_transf_s.abc_s.a_F32 / dc_link__V__F32) + (F32)0.5 ),
                         PWM_DUTY_TO_CMP_dMU16( (local_transf_s.abc_s.b_F32 / dc_link__V__F32) + (F32)0.5 ),
                         PWM_DUTY_TO_CMP_dMU16( (local_transf_s.abc_s.c_F32 / dc_link__V__F32) + (F32)0.5 ));
}

/*
 * @}
 */
