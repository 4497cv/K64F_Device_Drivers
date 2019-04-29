

#include "PIT.h"

static uint8_t g_pit_intr_flag = FALSE;

static pit_interrupt_flags_t pit_intr_status_flag = {FALSE};

/*CALLBACKS ptr*/
static void (*PIT_0_callback)(void) = 0;
static void (*PIT_1_callback)(void) = 0;
static void (*PIT_2_callback)(void) = 0;
static void (*PIT_3_callback)(void) = 0;

/*CALLBACK INIT*/
/*FuncrPtr to destFunct*/
void PIT_callback_init(PIT_timer_t pit_timer,void (*handler)(void))
{
	switch(pit_timer)
	{
	case PIT_0:
		PIT_0_callback = handler;
		break;
	case PIT_1:
		PIT_1_callback = handler;
		break;
	case PIT_2:
		PIT_2_callback = handler;
		break;
	case PIT_3:
		PIT_3_callback = handler;
		break;
	default:
		break;
	}
}

void PIT0_IRQHandler()
{
	volatile uint32_t dummy_PIT0;

	PIT->CHANNEL[PIT_0].TFLG |= PIT_TFLG_TIF_MASK;
	dummy_PIT0 = PIT->CHANNEL[PIT_0].TCTRL;	//read control register for clear PIT flag, this is silicon bug
	/*Change status of the flag*/
	pit_intr_status_flag.flag_PIT_0 = TRUE;

	/*CALLBACK*/
	if(PIT_0_callback)
	{
		PIT_0_callback();
	}
	PIT_clear_interrupt_flag(PIT_0);
}
void PIT1_IRQHandler()
{
	volatile uint32_t dummy_PIT1;

	PIT->CHANNEL[PIT_1].TFLG |= PIT_TFLG_TIF_MASK;
	dummy_PIT1 = PIT->CHANNEL[PIT_1].TCTRL;	//read control register for clear PIT flag, this is silicon bug
	/*Change status of the flag*/
	pit_intr_status_flag.flag_PIT_1 = TRUE;

	/*CALLBACK*/
	if(PIT_1_callback)
	{
		PIT_1_callback();
	}
	PIT_clear_interrupt_flag(PIT_1);
}
void PIT2_IRQHandler()
{
	volatile uint32_t dummy_PIT2;

	PIT->CHANNEL[PIT_2].TFLG |= PIT_TFLG_TIF_MASK;
	dummy_PIT2 = PIT->CHANNEL[PIT_2].TCTRL;	//read control register for clear PIT flag, this is silicon bug
	/*Change status of the flag*/
	pit_intr_status_flag.flag_PIT_2 = TRUE;

	/*CALLBACK*/
	if(PIT_2_callback)
	{
		PIT_2_callback();
	}
	PIT_clear_interrupt_flag(PIT_2);
}
void PIT3_IRQHandler()
{
	volatile uint32_t dummy_PIT3;

	PIT->CHANNEL[PIT_3].TFLG |= PIT_TFLG_TIF_MASK;
	dummy_PIT3 = PIT->CHANNEL[PIT_3].TCTRL;	//read control register for clear PIT flag, this is silicon bug
	/*Change status of the flag*/
	pit_intr_status_flag.flag_PIT_3 = TRUE;

	/*CALLBACK*/
	if(PIT_3_callback)
	{
		PIT_3_callback();
	}
	PIT_clear_interrupt_flag(PIT_3);
}

void PIT_delay(PIT_timer_t pit_timer,My_float_pit_t system_clock , My_float_pit_t delay)
{
	uint32_t LDVAL = 0;
	My_float_pit_t clock_period = 0.0F;
	system_clock = system_clock /2;
	clock_period = (1/system_clock);
	LDVAL = (uint32_t)(delay / clock_period);
	LDVAL = LDVAL - 1;


	switch(pit_timer)
	{
	case PIT_0:
		PIT->CHANNEL[PIT_0].LDVAL = LDVAL;
		//PIT_enable_interrupt(PIT_0);
		break;
	case PIT_1:
		PIT->CHANNEL[PIT_1].LDVAL = LDVAL;
		//PIT_enable_interrupt(PIT_1);
		break;
	case PIT_2:
		PIT->CHANNEL[PIT_2].LDVAL = LDVAL;
		//PIT_enable_interrupt(PIT_2);
		break;
	case PIT_3:
		PIT->CHANNEL[PIT_3].LDVAL = LDVAL;
		//PIT_enable_interrupt(PIT_3);
		break;

	default:
		PIT->CHANNEL[0].LDVAL = LDVAL;
		//PIT_enable_interrupt(PIT_0);
		break;
	}


}

void PIT_enable(void)
{
	PIT->MCR  |= PIT_MCR_FRZ_MASK;
	PIT->MCR &= ~PIT_MCR_MDIS_MASK; /* Enable PIT*/
}
void PIT_disable_interrupt(PIT_timer_t pit_timer)
{
	PIT->CHANNEL[pit_timer].TCTRL = MASK_PIT_DISABLE;//disable interuptions
}

void PIT_enable_interrupt(PIT_timer_t pit_timer)
{
	pit_intr_status_flag.flag_PIT_ISR = FALSE;

	if(FALSE == pit_intr_status_flag.flag_PIT_ISR)
	{
		PIT->CHANNEL[pit_timer].TCTRL = (PIT_TCTRL_TIE_MASK | PIT_TCTRL_TEN_MASK);//enables PIT timer interrupt
		/*Change flag*/
		pit_intr_status_flag.flag_PIT_ISR = TRUE;
	}
}

void PIT_clock_gating(void)
{
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
}

uint8_t PIT_get_interrupt_flag_status(PIT_timer_t pit_timer)
{
	switch(pit_timer)
	{
	case PIT_0:
		g_pit_intr_flag = pit_intr_status_flag.flag_PIT_0;
		break;
	case PIT_1:
		g_pit_intr_flag = pit_intr_status_flag.flag_PIT_1;
		break;
	case PIT_2:
		g_pit_intr_flag = pit_intr_status_flag.flag_PIT_2;
		break;
	case PIT_3:
		g_pit_intr_flag = pit_intr_status_flag.flag_PIT_3;
		break;
	default:
		break;
	}
	return (g_pit_intr_flag);
}

void PIT_clear_interrupt_flag(PIT_timer_t pit_timer)
{
	switch(pit_timer)
	{
	case PIT_0:
		pit_intr_status_flag.flag_PIT_0 = FALSE;
		g_pit_intr_flag = pit_intr_status_flag.flag_PIT_0;
		break;
	case PIT_1:
		pit_intr_status_flag.flag_PIT_1 = FALSE;
		g_pit_intr_flag = pit_intr_status_flag.flag_PIT_1;
		break;
	case PIT_2:
		pit_intr_status_flag.flag_PIT_2 = FALSE;
		g_pit_intr_flag = pit_intr_status_flag.flag_PIT_2;
		break;
	case PIT_3:
		pit_intr_status_flag.flag_PIT_3 = FALSE;
		g_pit_intr_flag = pit_intr_status_flag.flag_PIT_3;
		break;
	default:
		break;
	}
}

