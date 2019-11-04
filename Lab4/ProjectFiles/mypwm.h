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

// Variáveis com tamanho padrão
typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned short uint16_t;
typedef signed short int16_t;
typedef unsigned int uint32_t;
typedef signed int int32_t;


/****************************************************
 * 
 * System Control Registers Defines
 *
*****************************************************/
// Registrador para ativar o clock na porta GPIO do PWM
#define SYSCTL_PRGPIO_R   (*((volatile uint32_t *) 0x400FEA08))
// Registrador para verificar se a porta já está pronta
#define SYSCTL_RCGCGPIO_R (*((volatile uint32_t *) 0x400FE608))
// Registrador para ativar clock para o módulo do PWM
#define SYSCTL_RCGCPWM_R (*((volatile uint32_t *)0x400FE640))


/****************************************************
 * 
 * GPIO Defines das Portas
 *
*****************************************************/
// Defines GPIO
// Usando pino G1 para PWM
#define GPIO_RCGC_PORTG 64
#define GPIO_PORTG_BITS 2

#define GPIO_PORTG_AHB_AMSEL_R (*((volatile uint32_t *)0x4005E528))
#define GPIO_PORTG_AHB_PCTL_R (*((volatile uint32_t *)0x4005E52C))
#define GPIO_PORTG_AHB_DIR_R (*((volatile uint32_t *)0x4005E400))
#define GPIO_PORTG_AHB_AFSEL_R (*((volatile uint32_t *)0x4005E420))
#define GPIO_PORTG_AHB_DEN_R (*((volatile uint32_t *)0x4005E51C))

/****************************************************
 * 
 * Gerador PWM Defines
 *
*****************************************************/
// Defines de offset dos registradores de configuração do módulo de PWM
#define PWM_O_CTL       0x000 // Offset global do registrador de controle do módulo de PWM da placa
#define PWM_O_SYNC      0x004 // Offset global do registrador de sincronização do módulo de PWM da placa
#define PWM_O_ENABLE    0x008 // Offset global do registrador de enable do módulo de PWM da placa
#define PWM_O_ENUPD     0x028 // Offset global do registrador de enable update do módulo de PWM da placa 
#define PWM_O_CC        0xFC8 // Offset global do registrador de clock configuration do módulo de PWM da placa

#define PWM_BASE 0x40028000 // Pulse Width Modulator (PWM)
#define PWM_CC (*((volatile uint32_t *)(PWM_BASE + PWM_O_CC)))
#define PWM_ENABLE (*((volatile uint32_t*)(PWM_BASE + PWM_O_ENABLE)))


#define PWM_O_X_CTL PWM_O_CTL


// Defines de offsets dentro do Gerador de pwm para tornar mais fácil
// o endereçamento em vez de colocar todos hard coded
#define PWM_O_X_CTL    0x000 // Offset do registrador de controle dentro do Gerador
#define PWM_O_X_LOAD   0x010 // Offset do registrador de load dentro do Gerador
#define PWM_O_X_COUNT  0x014 // Offset do registrador de count dentro do Gerador
#define PWM_O_X_CMPA   0x018 // Offset do registrador de cmpa dentro do Gerador
#define PWM_O_X_CMPB   0x01C // Offset do registrador de cmpb dentro do Gerador
#define PWM_O_X_GENA   0x020 // Offset do registrador de gena dentro do Gerador
#define PWM_O_X_GENB   0x024 // Offset do registrador de genb dentro do Gerador


// PWM Gerador 1 Defines
#define PWM2_BASE        (PWM_BASE + 0x0C0)
#define PWM2_CTL         (*((volatile uint32_t *)(PWM2_BASE + PWM_O_X_CTL))) // Offset do registrador de controle do Gerador 1
#define PWM2_LOAD        (*((volatile uint32_t *)(PWM2_BASE + PWM_O_X_LOAD))) // Offset do registrador de load do Gerador 1
#define PWM2_COUNT       (*((volatile uint32_t *)(PWM2_BASE + PWM_O_X_COUNT))) // Offset do registrador de count do Gerador 1
#define PWM2_CMPA        (*((volatile uint32_t *)(PWM2_BASE + PWM_O_X_CMPA))) // Offset do registrador de cmpa do Gerador 1
#define PWM2_CMPB        (*((volatile uint32_t *)(PWM2_BASE + PWM_O_X_CMPB))) // Offset do registrador de cmpb do Gerador 1
#define PWM2_GENA        (*((volatile uint32_t *)(PWM2_BASE + PWM_O_X_GENA))) // Offset do registrador de gena do Gerador 1
#define PWM2_GENB        (*((volatile uint32_t *)(PWM2_BASE + PWM_O_X_GENB))) // Offset do registrador de genb do Gerador 1

// Endereços do gerador de acordo com API da TivaWare
#define PWM_GEN_2 PWM2_BASE  // Offset do gerador 1



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

//*****************************************************************************
//
// The following are defines for the bit fields in the PWM_O_CC register.
//
//*****************************************************************************
#define PWM_CC_USEPWM 0x00000100   // Use PWM Clock Divisor
#define PWM_CC_PWMDIV_M 0x00000007 // PWM Clock Divider


// Defines das possíveis divisões do clock do sistema
// para setar o período do PWM.
#define PWM_SYSCLK_DIV_1        0x00000000  // Usando clock do sistema
#define PWM_SYSCLK_DIV_2        0x00000100  // Usando clock = system clock / 2
#define PWM_SYSCLK_DIV_4        0x00000101  // Usando clock = system clock / 4
#define PWM_SYSCLK_DIV_8        0x00000102  // Usando clock = system clock / 8
#define PWM_SYSCLK_DIV_16       0x00000103  // Usando clock = system clock / 16
#define PWM_SYSCLK_DIV_32       0x00000104  // Usando clock = system clock / 32
#define PWM_SYSCLK_DIV_64       0x00000105  // Usando clock = system clock / 64

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

// #define PWM_X_GENA_ACTCMPAU_ONE 0x00000030  // Drive pwmA High
// #define PWM_X_GENA_ACTCMPAD_ZERO 0x00000080 // Drive pwmA Low
// #define PWM_X_GENA_ACTLOAD_ONE 0x0000000C   // Drive pwmA High
// #define PWM_X_GENA_ACTCMPBD_ZERO 0x00000800 // Drive pwmA Low
// #define PWM_X_GENA_ACTCMPBU_ONE 0x00000300  // Drive pwmA High

#define PWM_X_GENB_ACTLOAD_ONE   0x0000000C // Drive pwmB High
#define PWM_X_GENB_ACTCMPBD_ZERO 0x00000800 // Drive pwmB Low
#define PWM_X_GENB_ACTCMPBU_ONE 0x00000300  // Drive pwmB High

/****************************************************
 * 
 * Definições dos bit fields para configuração do PWM
 * 
 * Trecho copiado da TivaWare
 * 
 * Como não vamos utilizar os dois sinais de cada
 * gerador, tiramos os defines referentes a
 * DEADBAND
 */
//*****************************************************************************
//
// The following are defines for the bit fields in the PWM_O_X_CTL register.
//
//*****************************************************************************

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

#define M0PWM5 0x20








#define PWM0_CC_R               (*((volatile uint32_t *)0x40028FC8))
#define PWM0_2_CTL_R            (*((volatile uint32_t *)0x400280C0))
#define PWM0_2_GENB_R           (*((volatile uint32_t *)0x400280E4))
#define PWM0_2_LOAD_R           (*((volatile uint32_t *)0x400280D0))
#define PWM0_2_CMPB_R           (*((volatile uint32_t *)0x400280DC))
#define PWM0_ENABLE_R           (*((volatile uint32_t *)0x40028008))








/****************************************************
 * 
 * API Definições de funções.
 * Apesar do mesmo nome das funções da biblioteca,
 * o código aqui será específico para a porta F1.
 *
*****************************************************/

extern uint32_t PWMClockGet();
extern void pwmClockSet(uint32_t clock);
extern void pwmGenConfigure(uint32_t configuration);
extern void pwmGenEnable();
extern void pwmGenDisable();
extern void pwmGenPeriodSet(uint32_t period);
extern void pwmOutputEnable(void);

//--------------------------------------------------
// Função de inicialização
extern void pwmInit(void);

/****************************************************
 * 
 * Funções auxiliares.
 *
*****************************************************/
extern void configureGPIOPort(void);
extern void configurePWMDependencies(void);

#endif
