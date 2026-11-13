/**
 * @file auto_off.c
 * @brief Module to switch the light off after a time without activity.
 */

#include "auto_off.h"

/** @brief Calculate the number of ticks of the auto-off timeout. */
#define AUTO_OFF_TIMEOUT_TICKS ((uint32_t)((CONFIG_AUTO_OFF_TIMEOUT * 1000) / CONFIG_OS_TASK_PERIOD))

/**
 * @rst
 * .. impl:: Auto off step
 *    :id: SWIMPL_AO-001
 *    :implements: SWDD_AO-001, SWDD_AO-002, SWDD_AO-003, SWDD_AO-004, SWDD_AO-005
 *    :fulfills: REQ_39, REQ_40, REQ_59
 * @endrst
 */
SPLE_TESTABLE_STATIC void autoOffStep(AutoOffData *const data)
{
    /* All three inputs are read in every tick */
    const bool_t powerKeyEvent = RteGetPowerKeyPressedEvent();
    const bool_t upPressed = RteIsKeyPressed(CONTROL_KEY_UP);
    const bool_t downPressed = RteIsKeyPressed(CONTROL_KEY_DOWN);

    if ((powerKeyEvent == TRUE) || (upPressed == TRUE) || (downPressed == TRUE))
    {
        data->inactiveTicks = 0;
        RteSetAutoOffState(FALSE);
    }
    else if (RteGetPowerState() == POWER_STATE_ON)
    {
        data->inactiveTicks++;
        if (data->inactiveTicks >= data->timeoutTicks)
        {
            RteSetPowerState(POWER_STATE_OFF);
            RteSetAutoOffState(TRUE);
            data->inactiveTicks = 0;
        }
    }
    else
    {
        data->inactiveTicks = 0;
    }
}

/**
 * @rst
 * .. impl:: Auto off runnable
 *    :id: SWIMPL_AO-002
 *    :implements: SWDD_AO-003
 *    :fulfills: REQ_39
 * @endrst
 */
void autoOff(void)
{
    static AutoOffData data = {
        .inactiveTicks = 0,
        .timeoutTicks = AUTO_OFF_TIMEOUT_TICKS};

    autoOffStep(&data);
}
