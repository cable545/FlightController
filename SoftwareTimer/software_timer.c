/*
 * software_timer.c
 *
 *  Created on: 05.11.2021
 *      Author: burde
 */

#include "software_timer.h"

/* function pointer for a user function which can pe polled at calls of
   STIMER_TestTimer(...) function */
static void (*pPollfunc) (void);

/* function pointers for user callback functions */
static void (*pSTIMER[STIMER_MAX]) (void);
/* sw-PTIM counters */
static volatile uint16_t Ctr[STIMER_MAX];

uint8_t OneSecondExpired;
uint8_t TenMillisecondsExpired;

/**
  * @brief initialise the software STIMER module
  * @par[in] optional pointer to user applic function
  * @retval void None
  * @note Required preconditions: SysTick Timer/Handler initialised @1msec
  */
void STIMER_Init(void (*funcptr) (void))
{
	for(uint8_t i = 0; i < STIMER_MAX; i++)
		Ctr[i] = 0;

	if(funcptr != NULL)
		pPollfunc = funcptr;
}

/**
* @brief start of selected sw-STIMER
* @par[in] SW-Timer ID (swtimer.h)
* @par[in] uint16_t ticks to run in msecs
* @par[in] optional pointer to callback function when timer runs out
* @retval int8_t =0 on success, -1 on error
* @note Required preconditions: SysTick Timer/Handler initialised @1msec
*/
int8_t STIMER_StartTimer(uint8_t soft_id, uint16_t ticks, void (*funcptr) (void))
{
    int8_t retval = 0;

    if (0U == ticks) /* if STIMER set to zero, just ignore */
        return retval;
		else
		{
			pSTIMER[soft_id] = funcptr;
			Ctr[soft_id] = ticks;
		} /* else */

    return retval;
}

/**
* @brief Test of selected sw-STIMER
* @par[in] SW-Timer ID (swtimer.h)
* @retval TRUE if sw-STIMER finished, FALSE if sw-STIMER is running
*/
uint8_t STIMER_TestTimer(uint8_t soft_id)
{
    int16_t retval = FALSE;

    if(pPollfunc != NULL)
			pPollfunc();

    retval = (Ctr[soft_id] == 0U ? TRUE : FALSE);

    return retval;
}

/**
  * @brief Gets remaining ticks of selected sw-STIMER
  * @par SW-Timer ID (swtimer.h)
  * @retval uint16_t number of ticks
  * @par Required preconditions: None
  */
uint16_t STIMER_GetTimer(uint8_t soft_id)
{
	return Ctr[soft_id];
}

/** @brief Kills selected sw-STIMER
  * @par SW-Timer ID (swtimer.h)
  * @retval None
  * @par Required preconditions: None
  */
void STIMER_KillTimer(uint8_t soft_id)
{
	if(Ctr[soft_id] != 0U)
		Ctr[soft_id] = 0;
} /* STIMER_KillTimer */

/** @brief Handles the SW-Timer counters
  * @par None
  * @retval None
  * @par Required preconditions: called every 1msec
  * @caller SysTick Timer ISR
  */
void STIMER_PollHandler(void)
{
	for(uint8_t i = 0; i < STIMER_MAX; i++)
	{
		if(Ctr[i] != 0U)
		{
			--Ctr[i];

			if (0U == Ctr[i])
			{
				if(pSTIMER[i] != NULL)
					pSTIMER[i]();
			}
		}
	}
}

void STIMER_OneSectimerExpired(void)
{
	STIMER_StartTimer(ONE_SEC_TIMER, (uint16_t)10000, STIMER_OneSectimerExpired);
	OneSecondExpired = TRUE;
}

void STIMER_TenMillsectimerExpired(void)
{
	STIMER_StartTimer(TEN_MILLSEC_TIMER, (uint16_t)100, STIMER_TenMillsectimerExpired);
	TenMillisecondsExpired = TRUE;
}
