#include "pwm.h"

//*****************************************************************************
//
// Trecho copiado na íntegra da TivaWare.
// Misc macros for manipulating the encoded generator and output defines used
// by the API.
//
//*****************************************************************************
#define PWM_GEN_BADDR(_mod_, _gen_) \
    ((_mod_) + (_gen_))
#define PWM_GEN_EXT_BADDR(_mod_, _gen_) \
    ((_mod_) + PWM_GEN_EXT_0 +          \
     ((_gen_)-PWM_GEN_0) * 2)
#define PWM_OUT_BADDR(_mod_, _out_) \
    ((_mod_) + ((_out_)&0xFFFFFFC0))
#define PWM_IS_OUTPUT_ODD(_out_) \
    ((_out_)&0x00000001)

// Fim do trecho copiado.

/**********************************************************
 * 
 * Trecho copiado da TivaWare.
 * Gerar endereços acessáveis a partir de
 * variáveis inteiras.
 */ 
//
// Macros for hardware access, both direct and via the bit-band region.
//
//*****************************************************************************
#define HWREG(x) \
    (*((volatile uint32_t *)(x)))
#define HWREGH(x) \
    (*((volatile uint16_t *)(x)))
#define HWREGB(x) \
    (*((volatile uint8_t *)(x)))
#define HWREGBITW(x, b)                             \
    HWREG(((uint32_t)(x)&0xF0000000) | 0x02000000 | \
          (((uint32_t)(x)&0x000FFFFF) << 5) | ((b) << 2))
#define HWREGBITH(x, b)                              \
    HWREGH(((uint32_t)(x)&0xF0000000) | 0x02000000 | \
           (((uint32_t)(x)&0x000FFFFF) << 5) | ((b) << 2))
#define HWREGBITB(x, b)                              \
    HWREGB(((uint32_t)(x)&0xF0000000) | 0x02000000 | \
           (((uint32_t)(x)&0x000FFFFF) << 5) | ((b) << 2))

// Fim do trecho copiado.




void PWMInit(void)
{
    configureGPIOPort();
    configurePWMDependencies();
}

/**************************************************
 * 
 * Funções de inicialização e configuração de HW.
 * 
***************************************************/


void configureGPIOPort(void)
{
    //1a. Ativar o clock para a porta setando o bit correspondente no registrador RCGCGPIO.
    SYSCTL_RCGCGPIO_R |= GPIO_RCGC_PORTF;

    //1b.   ap�s isso verificar no PRGPIO se a porta est� pronta para uso.
    while ((SYSCTL_PRGPIO_R & (GPIO_RCGC_PORTF)) != (GPIO_RCGC_PORTF))
        ;

    // 2. Limpar o AMSEL para desabilitar a anal�gica.
    GPIO_PORTF_AMSEL_R = 0x00;

    // 3. Selecionar módulo 0 do pwm0.
    GPIO_PORTF_PORTCTL_R = GPIO_PORTF_M0PWM0_BIT;

    // 4. Setar bit AFSEL para selecionar modo de função alternativa.
    GPIO_PORTF_ALTFUN_R = GPIO_PORTF0;

    // 5. Setar os bits de DEN para habilitar I/O digital.
    GPIO_PORTF_DIGEN_R = GPIO_PORTF0;
}

void configurePWMDependencies(void)
{
    // Ativando clock do módulo do PWM.
    SYSCTL_RCGCPWM |= 1;
}



/**************************************************
 * 
 * Funções da API do PWM.
 * 
***************************************************/

void PWMClockSet(uint32_t ui32Base, uint32_t ui32Config)
{

}


void PWMGenConfigure(uint32_t ui32Base, uint32_t ui32Gen, uint32_t ui32Config)
{
    uint32_t generatorBaseAddress;
    
    if (!isValidGenerator(ui32Gen))
        return;
    if (!isValidPWMConfiguration(ui32Config))
        return;

    generatorBaseAddress = PWM_GEN_BADDR(ui32Base, ui32Gen);

    // Zerando possíveis configurações feitas e aplicando
    // nova configuração.
    HWREG(ui32Gen + PWM_O_X_CTL) = ((HWREG(ui32Gen + PWM_O_X_CTL) &
                                     ~(PWM_X_CTL_MODE | PWM_X_CTL_DEBUG |
                                       PWM_X_CTL_GENBUPD_M |
                                       PWM_X_CTL_GENAUPD_M |
                                       PWM_X_CTL_LOADUPD | PWM_X_CTL_CMPAUPD |
                                       PWM_X_CTL_CMPBUPD)) |
                                    ui32Config);

    
    if (ui32Config & PWM_X_CTL_MODE)
    {
        // Quando o bit 1 do reg de configuração está setado,
        // o modo de contagem é up/down.
        //
        // In up/down count mode, set the signal high on up count comparison
        // and low on down count comparison (that is, center align the
        // signals).
        //
        HWREG(ui32Gen + PWM_O_X_GENA) = (PWM_X_GENA_ACTCMPAU_ONE |
                                         PWM_X_GENA_ACTCMPAD_ZERO);
        
        // Talvez não seja necessário fazer isso.
        // @TODO Remover se não for necessário
        // HWREG(ui32Gen + PWM_O_X_GENB) = (PWM_X_GENB_ACTCMPBU_ONE |
        //                                  PWM_X_GENB_ACTCMPBD_ZERO);
    }
    else
    {
        // Se o bit 1 está zerado, o modo de contagem é down.
        //
        // In down count mode, set the signal high on load and low on count
        // comparison (that is, left align the signals).
        //
        HWREG(ui32Gen + PWM_O_X_GENA) = (PWM_X_GENA_ACTLOAD_ONE |
                                         PWM_X_GENA_ACTCMPAD_ZERO);
        
        // Talvez não seja necessário fazer isso.
        // @TODO Remover se não for necessário.
        // HWREG(ui32Gen + PWM_O_X_GENB) = (PWM_X_GENB_ACTLOAD_ONE |
        //                                  PWM_X_GENB_ACTCMPBD_ZERO);
    }
}


void PWMGenEnable(uint32_t ui32Base, uint32_t ui32Gen)
{

}


void PWMGenDisable(uint32_t ui32Base, uint32_t ui32Gen)
{

}


/**************************************************
 * 
 * Funções auxiliares
 * 
***************************************************/







