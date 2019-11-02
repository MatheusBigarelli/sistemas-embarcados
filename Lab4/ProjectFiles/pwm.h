#ifndef __PWM_H__
#define __PWM_H__

/****************************************************
 * 
 * O código possui marcadores para indicar possíveis
 * zona de erro, dúvida, tarefa pendente, etc.
 * 
 * Esses marcadores são:
 * - @WARNING
 * - @TODO
 *
*****************************************************/

// Valores booleanos
#define true 1;
#define false 0;

// Variáveis com tamanho padrão
typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned short uint16_t;
typedef signed short int16_t;
typedef unsigned int uint32_t;
typedef signed int int32_t;
typedef uint8_t bool;


/****************************************************
 * 
 * System Control Registers Defines
 *
*****************************************************/
#define SYSCTL_RCGCGPIO_R       0x400FE608  // Registrador para ativar o clock na porta GPIO do PWM
#define SYSCTL_PRGPIO_R         0x400FEA08  // Registrador para verificar se a porta já está pronta
#define SYSCTL_RCGCPWM          0x400F0640  // Registrador para ativar clock para o módulo do PWM



/****************************************************
 * 
 * GPIO Defines das Portas
 *
*****************************************************/

#define GPIO_DATA_REG_OFF       0x000 // Offset do registrador de dados das portas GPIO
#define GPIO_DIR_REG_OFF        0x400 // Offset do registrador de direção (entrada ou saída) das portas GPIO
#define GPIO_ALTFUN_REG_OFF     0x420 // Offset do registrador de função alternativa das portas GPIO
#define GPIO_DIGEN_REG_OFF      0x51C // Offset do registrador de digital enable das portas GPIO
#define GPIO_AMSEL_REG_OFF      0x528 // Offset do registrador de modo analógico das portas GPIO
#define GPIO_PORTCTL_REG_OFF    0x52C // Offset do registrador de configuração das portas GPIO

#define GPIO_PORTF_BASE         0x4005D000 // Endereço da porta F do GPIO no barramento de alta velocidade
#define GPIO_PORTF_DATA_R       (GPIO_PORTF_BASE + GPIO_DATA_REG_OFF)
#define GPIO_PORTF_DIR_R        (GPIO_PORTF_BASE + GPIO_DIR_REG_OFF)
#define GPIO_PORTF_ALTFUN_R     (GPIO_PORTF_BASE + GPIO_ALTFUN_REG_OFF)
#define GPIO_PORTF_DIGEN_R      (GPIO_PORTF_BASE + GPIO_DIGEN_REG_OFF)
#define GPIO_PORTF_AMSEL_R      (GPIO_PORTF_BASE + GPIO_AMSEL_REG_OFF)
#define GPIO_PORTF_PORTCTL_R    (GPIO_PORTF_BASE + GPIO_PORTCTL_REG_OFF)

#define GPIO_RCGC_PORTF         0b00000000100000 // Ativando clock na porta F no RCGC_GPIO
#define GPIO_PORTF_M0PWM0_BIT   0b100000 // Bit 6 do PCTL para ativar o M0_PWM0
#define GPIO_PORTF0             0x01 // Ativar função alternativa no pino 0 da porta F



/****************************************************
 * 
 * Gerador PWM Defines
 *
*****************************************************/
// Defines de offset dos registradores de configuração do módulo de PWM
#define PWMCTLOFF       0x000 // Offset global do registrador de controle do módulo de PWM da placa
#define PWMSYNCOFF      0x004 // Offset global do registrador de sincronização do módulo de PWM da placa
#define PWMENABLEOFF    0x008 // Offset global do registrador de enable do módulo de PWM da placa
#define PWMENUPD        0x028 // Offset global do registrador de enable update do módulo de PWM da placa 
#define PWMCC           0xFC8 // Offset global do registrador de clock configuration do módulo de PWM da placa

// Defines de offsets dentro do Gerador de pwm para tornar mais fácil
// o endereçamento em vez de colocar todos hard coded
#define PWMm_CTL_REG_OFF    0x000 // Offset do registrador de controle dentro do Gerador
#define PWMm_LOAD_REG_OFF   0x010 // Offset do registrador de load dentro do Gerador
#define PWMm_COUNT_REG_OFF  0x014 // Offset do registrador de count dentro do Gerador
#define PWMm_CMPA_REG_OFF   0x018 // Offset do registrador de cmpa dentro do Gerador
#define PWMm_CMPB_REG_OFF   0x01C // Offset do registrador de cmpb dentro do Gerador
#define PWMm_GENA_REG_OFF   0x020 // Offset do registrador de gena dentro do Gerador
#define PWMm_GENB_REG_OFF   0x024 // Offset do registrador de genb dentro do Gerador

// PWM Gerador 0 Defines
#define PWM0BASE        0x040   // Offset do Gerador 0 do PWM
#define PWM0CTL         (PWM0BASE + PWMm_CTL_REG_OFF) // Offset do registrador de controle do Gerador 0
#define PWM0LOAD        (PWM0BASE + PWMm_LOAD_REG_OFF) // Offset do registrador de load do Gerador 0
#define PWM0COUNT       (PWM0BASE + PWMm_COUNT_REG_OFF) // Offset do registrador de count do Gerador 0
#define PWM0CMPA        (PWM0BASE + PWMm_CMPA_REG_OFF) // Offset do registrador de cmpa do Gerador 0
#define PWM0CMPB        (PWM0BASE + PWMm_CMPB_REG_OFF) // Offset do registrador de cmpb do Gerador 0
#define PWM0GENA        (PWM0BASE + PWMm_GENA_REG_OFF) // Offset do registrador de gena do Gerador 0
#define PWM0GENB        (PWM0BASE + PWMm_GENB_REG_OFF) // Offset do registrador de genb do Gerador 0

// PWM Gerador 1 Defines
#define PWM1BASE        0x080
#define PWM1CTL         (PWM1BASE + PWMm_CTL_REG_OFF) // Offset do registrador de controle do Gerador 1
#define PWM1LOAD        (PWM1BASE + PWMm_LOAD_REG_OFF) // Offset do registrador de load do Gerador 1
#define PWM1COUNT       (PWM1BASE + PWMm_COUNT_REG_OFF) // Offset do registrador de count do Gerador 1
#define PWM1CMPA        (PWM1BASE + PWMm_CMPA_REG_OFF) // Offset do registrador de cmpa do Gerador 1
#define PWM1CMPB        (PWM1BASE + PWMm_CMPB_REG_OFF) // Offset do registrador de cmpb do Gerador 1
#define PWM1GENA        (PWM1BASE + PWMm_GENA_REG_OFF) // Offset do registrador de gena do Gerador 1
#define PWM1GENB        (PWM1BASE + PWMm_GENB_REG_OFF) // Offset do registrador de genb do Gerador 1

// PWM Gerador 2 Defines
#define PWM2BASE        0x0C0
#define PWM2CTL         (PWM2BASE + PWMm_CTL_REG_OFF) // Offset do registrador de controle do Gerador 2
#define PWM2LOAD        (PWM2BASE + PWMm_LOAD_REG_OFF) // Offset do registrador de load do Gerador 2
#define PWM2COUNT       (PWM2BASE + PWMm_COUNT_REG_OFF) // Offset do registrador de count do Gerador 2
#define PWM2CMPA        (PWM2BASE + PWMm_CMPA_REG_OFF) // Offset do registrador de cmpa do Gerador 2
#define PWM2CMPB        (PWM2BASE + PWMm_CMPB_REG_OFF) // Offset do registrador de cmpb do Gerador 2
#define PWM2GENA        (PWM2BASE + PWMm_GENA_REG_OFF) // Offset do registrador de gena do Gerador 2
#define PWM2GENB        (PWM2BASE + PWMm_GENB_REG_OFF) // Offset do registrador de genb do Gerador 2

// PWM Gerador 2 Defines
#define PWM3BASE        0x100
#define PWM3CTL         (PWM3BASE + PWMm_CTL_REG_OFF) // Offset do registrador de controle do Gerador 3
#define PWM3LOAD        (PWM3BASE + PWMm_LOAD_REG_OFF) // Offset do registrador de load do Gerador 3
#define PWM3COUNT       (PWM3BASE + PWMm_COUNT_REG_OFF) // Offset do registrador de count do Gerador 3
#define PWM3CMPA        (PWM3BASE + PWMm_CMPA_REG_OFF) // Offset do registrador de cmpa do Gerador 3
#define PWM3CMPB        (PWM3BASE + PWMm_CMPB_REG_OFF) // Offset do registrador de cmpb do Gerador 3
#define PWM3GENA        (PWM3BASE + PWMm_GENA_REG_OFF) // Offset do registrador de gena do Gerador 3
#define PWM3GENB        (PWM3BASE + PWMm_GENB_REG_OFF) // Offset do registrador de genb do Gerador 3


// Endereços dos geradores de acordo com API da TivaWare
#define PWM_GEN_0 PWM0BASE  // Offset do gerador 0
#define PWM_GEN_1 PWM1BASE  // Offset do gerador 1
#define PWM_GEN_2 PWM2BASE  // Offset do gerador 2
#define PWM_GEN_3 PWM3BASE  // Offset do gerador 3



// Defines de configuração dos geradores (registrador pwm ctl de cada gerador)
// Uma escolha de disjunção (OU lógico) desses parâmetros nos dá UMA configuração
// do gerador do PWM.
// Pode-se escolher uma dentre as opções separadas por grupos como abaixo 
// Registradores são de 32 bits, com os bits 31-19 (inclusive) reservados
// Operações devem ser feitas com read-modify-write
// Os valores dos defines são bits a serem setados no registrador PWMnCTL
#define PWM_GEN_DISABLE               0x00000000 // Desativando PWM no gerador n - bit field 0
#define PWM_GEN_ENABLE                0x00000001 // Ativando gerador PWM no gerador n - bit field 0

#define PWM_GEN_MODE_DOWN             0x00000000 // Ativando modo de contagem down - bit field 1
#define PWM_GEN_MODE_UP_DOWN          0x00000002 // Ativando modo de contagem up/down - bit field 1

#define PWM_GEN_MODE_SYNC             0x00000038 // Setando modo de atualização síncrona - mais de um bit setado
#define PWM_GEN_MODE_NO_SYNC          0x00000000 // Setando modo de atualização assíncrona - reset de vários bits

#define PWM_GEN_MODE_DBG_RUN          0x00000004 // Executar PWM no modo debug
#define PWM_GEN_MODE_DBG_STOP         0x00000000 // Desativando modo debug

#define PWM_GEN_MODE_GEN_NO_SYNC      0x00000000 // Atualizações ocorrem imediatamente
#define PWM_GEN_MODE_GEN_SYNC_LOCAL   0x00000280 // Atualizações são síncronas localmente
#define PWM_GEN_MODE_GEN_SYNC_GLOBAL  0x000003C0 // Atualizações são síncronas globalmente




// Define de configuração de clock do PWM
#define PWM_CC_SYS_CLK      0x00000000 // Usar clock do sistema
#define PWM_CC_DIV_CLK      0x00000100 // Usar clock do divisor de clock do sistema

// Defines das possíveis divisões do clock do sistema
// para setar o período do PWM.
#define PWM_SYSCLK_DIV_1        0x00000000  // Usando clock do sistema
#define PWM_SYSCLK_DIV_2        0x00000100  // Usando clock = system clock / 2
#define PWM_SYSCLK_DIV_4        0x00000101  // Usando clock = system clock / 4
#define PWM_SYSCLK_DIV_8        0x00000102  // Usando clock = system clock / 8
#define PWM_SYSCLK_DIV_16       0x00000103  // Usando clock = system clock / 16
#define PWM_SYSCLK_DIV_32       0x00000104  // Usando clock = system clock / 32
#define PWM_SYSCLK_DIV_64       0x00000105  // Usando clock = system clock / 64

#define PWM0_BASE       0x40028000 // Pulse Width Modulator (PWM)
#define M0_PWM0         GPIO_PORTF_PIN0

/****************************************************
 * 
 * Definições dos bit fields para configuração do PWM
 * 
 * Trecho copiado da TivaWare
 * 
 * Como não vamos utilizar os dois sinais de cada
 * gerador, tiramos os defines referentes a
 * DEADBAND
 *
//*****************************************************************************
//
// The following are defines for the bit fields in the PWM_O_X_CTL register.
//
//*****************************************************************************/

#define PWM_X_CTL_GENBUPD_M 0x00000300    // PWMnGENB Update Mode
#define PWM_X_CTL_GENBUPD_I 0x00000000    // Immediate
#define PWM_X_CTL_GENBUPD_LS 0x00000200   // Locally Synchronized
#define PWM_X_CTL_GENBUPD_GS 0x00000300   // Globally Synchronized
#define PWM_X_CTL_GENAUPD_M 0x000000C0    // PWMnGENA Update Mode
#define PWM_X_CTL_GENAUPD_I 0x00000000    // Immediate
#define PWM_X_CTL_GENAUPD_LS 0x00000080   // Locally Synchronized
#define PWM_X_CTL_GENAUPD_GS 0x000000C0   // Globally Synchronized
#define PWM_X_CTL_CMPBUPD 0x00000020      // Comparator B Update Mode
#define PWM_X_CTL_CMPAUPD 0x00000010      // Comparator A Update Mode
#define PWM_X_CTL_LOADUPD 0x00000008      // Load Register Update Mode
#define PWM_X_CTL_DEBUG 0x00000004        // Debug Mode
#define PWM_X_CTL_MODE 0x00000002         // Counter Mode
#define PWM_X_CTL_ENABLE 0x00000001       // PWM Block Enable

// Fim do trecho copiado


/****************************************************
 * 
 * Defines das ações a serem tomadas ao receber
 * os pulsos do comparador do PWM.
 * 
 * Como não utilizaremos mais de 1 pino, outras ações
 * são desnecessárias (eu espero)
 * 
 * @WARNING
 * 
*****************************************************/

#define PWM_X_GENA_ACTCMPAU_ONE 0x00000030 // Drive pwmA High
#define PWM_X_GENA_ACTCMPAD_ZERO \
    0x00000080 // Drive pwmA Low
#define PWM_X_GENB_ACTCMPBU_ONE 0x00000300 // Drive pwmB High
#define PWM_X_GENA_ACTLOAD_ONE 0x0000000C  // Drive pwmA High

/****************************************************
 * 
 * Definições auxiliares.
 * 
 * Estas definições são usadas para casar código
 * feito com constantes utilizadas no código que
 * copiado da TivaWare
 *
*****************************************************/
#define PWM_O_CTL               PWMm_CTL_REG_OFF
#define PWM_O_SYNC              0x00000004
#define PWM_O_ENABLE            0x00000008
#define PWM_O_ENUPD             0x00000028





/****************************************************
 * 
 * API Definições de funções.
 *
*****************************************************/

uint32_t PWMClockGet (uint32_t ui32Base);
void PWMClockSet (uint32_t ui32Base, uint32_t ui32Config);
void PWMGenConfigure(uint32_t ui32Base, uint32_t ui32Gen, uint32_t ui32Config);
void PWMGenEnable(uint32_t ui32Base, uint32_t ui32Gen);
void PWMGenDisable(uint32_t ui32Base, uint32_t ui32Gen);

/****************************************************
 * 
 * Funções auxiliares.
 *
*****************************************************/

void configureGPIOPort(void);
void configurePWMDependencies(void);
bool isValidGenerator(ui32Gen);
bool isValidPWMConfiguration(ui32Config);

#endif
