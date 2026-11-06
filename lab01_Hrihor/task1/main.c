
#include "project.h"

// Конфігурація для Завдання 1 (Таймер на 250 мкс)
#define TIMER_PERIOD_TICKS   (249u)   /* Clock_1 = 1 МГц (тік = 1 мкс) -> 250 тактів = 250 мкс */
volatile uint32_t isr_count = 0u;

// Обробник переривання для Завдання 1 (Таймер)
CY_ISR(Timer_ISR_Handler)
{
    Pin_ISR_Write((uint8)(1u - Pin_ISR_Read()));   /* інверсія стану піна — toggle */
    isr_count++;
    Timer_1_ClearInterrupt(Timer_1_INTR_MASK_TC);   /* очищення прапора переривання таймера */
}

// Обробник переривання для Завдання 2 (Фізична кнопка P0.7)
CY_ISR(Button_ISR_Handler)
{
    // Асинхронно перемикаємо стан світлодіода по натиску кнопки
    Pin_ISR_Write((uint8)(1u - Pin_ISR_Read()));
    
    // Обов'язкове очищення прапора апаратного переривання на фізичному піні кнопки
    Pin_Button_ClearInterrupt();
}

int main(void)
{
    // Дозволяємо глобальні переривання процесора ARM Cortex-M0
    CyGlobalIntEnable;
    
    // Ініціалізація периферії для Завдання 1 (Таймер)
    Clock_1_Start();
    Timer_1_Start();                                
    Timer_1_WritePeriod(TIMER_PERIOD_TICKS);        
    Timer_1_SetInterruptMode(Timer_1_INTR_MASK_TC); 
    
    isr_Timer_StartEx((cyisraddress)Timer_ISR_Handler); /* активація переривання таймера */

    // Ініціалізація периферії для Завдання 2 (Кнопка)
    isr_Button_StartEx((cyisraddress)Button_ISR_Handler); /* активація переривання кнопки */

    for (;;)
    {
        // Основний цикл програми згідно з Варіантом 4 (затримка 1 мс)
        Pin_Main_Write(1u);
        CyDelay(1u);                                
        Pin_Main_Write(0u);
        CyDelay(1u);
    }
}




