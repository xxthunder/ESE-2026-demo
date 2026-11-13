/**
 * @file
 */
#include <gtest/gtest.h>
using namespace testing;

extern "C"
{
#include "autoconf.h"
#include "auto_off.h"
#include "rte.h"

    extern void autoOffStep(AutoOffData *const data);
}

// Auto-generated mockups for this component
#include "mockup_components_auto_off.h"

namespace
{
/* Expect the three activity reads of one step, once each. */
void expectActivityReads(class_mockup &mock, bool_t powerKeyEvent, bool_t up, bool_t down)
{
    EXPECT_CALL(mock, RteGetPowerKeyPressedEvent()).Times(1).WillOnce(Return(powerKeyEvent));
    EXPECT_CALL(mock, RteIsKeyPressed(CONTROL_KEY_UP)).Times(1).WillOnce(Return(up));
    EXPECT_CALL(mock, RteIsKeyPressed(CONTROL_KEY_DOWN)).Times(1).WillOnce(Return(down));
}
} // namespace

/**
 * @rst
 * .. test:: auto_off.test_counts_inactive_ticks_while_on
 *    :id: TS_AO-001
 *    :tests: SWDD_AO-002
 * @endrst
 */
TEST(auto_off, test_counts_inactive_ticks_while_on)
{
    /* Arrange */
    CREATE_MOCK(mymock);
    AutoOffData data = {.inactiveTicks = 0, .timeoutTicks = 3};

    {
        InSequence seq;
        EXPECT_CALL(mymock, RteGetPowerKeyPressedEvent()).Times(1).WillOnce(Return(FALSE));
        EXPECT_CALL(mymock, RteIsKeyPressed(CONTROL_KEY_UP)).Times(1).WillOnce(Return(FALSE));
        EXPECT_CALL(mymock, RteIsKeyPressed(CONTROL_KEY_DOWN)).Times(1).WillOnce(Return(FALSE));
        EXPECT_CALL(mymock, RteGetPowerState()).Times(1).WillOnce(Return(POWER_STATE_ON));
    }
    EXPECT_CALL(mymock, RteSetPowerState(_)).Times(0);
    EXPECT_CALL(mymock, RteSetAutoOffState(_)).Times(0);

    /* Act */
    autoOffStep(&data);

    /* Assert */
    EXPECT_EQ(data.inactiveTicks, 1u);
    EXPECT_EQ(data.timeoutTicks, 3u);
}

/**
 * @rst
 * .. test:: auto_off.test_switches_off_exactly_at_timeout
 *    :id: TS_AO-002
 *    :tests: SWDD_AO-003, SWDD_AO-004
 * @endrst
 */
TEST(auto_off, test_switches_off_exactly_at_timeout)
{
    /* The step in which the count reaches timeoutTicks switches off */
    {
        CREATE_MOCK(mymock);
        AutoOffData data = {.inactiveTicks = 2, .timeoutTicks = 3};

        expectActivityReads(mymock, FALSE, FALSE, FALSE);
        EXPECT_CALL(mymock, RteGetPowerState()).Times(1).WillOnce(Return(POWER_STATE_ON));
        EXPECT_CALL(mymock, RteSetPowerState(POWER_STATE_OFF)).Times(1);
        EXPECT_CALL(mymock, RteSetAutoOffState(TRUE)).Times(1);
        EXPECT_CALL(mymock, RteSetAutoOffState(FALSE)).Times(0);

        autoOffStep(&data);

        EXPECT_EQ(data.inactiveTicks, 0u);
    }
    /* One tick earlier it does not */
    {
        CREATE_MOCK(mymock);
        AutoOffData data = {.inactiveTicks = 1, .timeoutTicks = 3};

        expectActivityReads(mymock, FALSE, FALSE, FALSE);
        EXPECT_CALL(mymock, RteGetPowerState()).Times(1).WillOnce(Return(POWER_STATE_ON));
        EXPECT_CALL(mymock, RteSetPowerState(_)).Times(0);
        EXPECT_CALL(mymock, RteSetAutoOffState(_)).Times(0);

        autoOffStep(&data);

        EXPECT_EQ(data.inactiveTicks, 2u);
    }
}

/**
 * @rst
 * .. test:: auto_off.test_activity_resets_timer
 *    :id: TS_AO-003
 *    :tests: SWDD_AO-001, SWDD_AO-005, SWDD_AO-004
 * @endrst
 */
TEST(auto_off, test_activity_resets_timer)
{
    struct
    {
        bool_t powerKeyEvent;
        bool_t up;
        bool_t down;
    } testCases[] = {
        {TRUE, FALSE, FALSE},
        {FALSE, TRUE, FALSE},
        {FALSE, FALSE, TRUE}};

    for (const auto &testCase : testCases)
    {
        /* Arrange */
        CREATE_MOCK(mymock);
        AutoOffData data = {.inactiveTicks = 2, .timeoutTicks = 3};

        {
            InSequence seq;
            EXPECT_CALL(mymock, RteGetPowerKeyPressedEvent()).Times(1).WillOnce(Return(testCase.powerKeyEvent));
            EXPECT_CALL(mymock, RteIsKeyPressed(CONTROL_KEY_UP)).Times(1).WillOnce(Return(testCase.up));
            EXPECT_CALL(mymock, RteIsKeyPressed(CONTROL_KEY_DOWN)).Times(1).WillOnce(Return(testCase.down));
        }
        EXPECT_CALL(mymock, RteSetAutoOffState(FALSE)).Times(1);
        EXPECT_CALL(mymock, RteSetAutoOffState(TRUE)).Times(0);
        EXPECT_CALL(mymock, RteGetPowerState()).Times(0);
        EXPECT_CALL(mymock, RteSetPowerState(_)).Times(0);

        /* Act */
        autoOffStep(&data);

        /* Assert */
        EXPECT_EQ(data.inactiveTicks, 0u);
    }
}

/**
 * @rst
 * .. test:: auto_off.test_activity_at_timeout_keeps_light_on
 *    :id: TS_AO-004
 *    :tests: SWDD_AO-005
 * @endrst
 */
TEST(auto_off, test_activity_at_timeout_keeps_light_on)
{
    /* Arrange: the next inactive tick would switch off */
    CREATE_MOCK(mymock);
    AutoOffData data = {.inactiveTicks = 2, .timeoutTicks = 3};

    expectActivityReads(mymock, FALSE, TRUE, FALSE);
    EXPECT_CALL(mymock, RteSetPowerState(_)).Times(0);
    EXPECT_CALL(mymock, RteSetAutoOffState(TRUE)).Times(0);
    EXPECT_CALL(mymock, RteSetAutoOffState(FALSE)).Times(1);
    EXPECT_CALL(mymock, RteGetPowerState()).Times(0);

    /* Act */
    autoOffStep(&data);

    /* Assert */
    EXPECT_EQ(data.inactiveTicks, 0u);
}

/**
 * @rst
 * .. test:: auto_off.test_count_restarts_after_power_off
 *    :id: TS_AO-005
 *    :tests: SWDD_AO-002
 * @endrst
 */
TEST(auto_off, test_count_restarts_after_power_off)
{
    /* Arrange: light is off, stale count from before */
    CREATE_MOCK(mymock);
    AutoOffData data = {.inactiveTicks = 2, .timeoutTicks = 3};

    expectActivityReads(mymock, FALSE, FALSE, FALSE);
    EXPECT_CALL(mymock, RteGetPowerState()).Times(1).WillOnce(Return(POWER_STATE_OFF));
    EXPECT_CALL(mymock, RteSetPowerState(_)).Times(0);
    EXPECT_CALL(mymock, RteSetAutoOffState(_)).Times(0);

    /* Act */
    autoOffStep(&data);

    /* Assert */
    EXPECT_EQ(data.inactiveTicks, 0u);
}

/**
 * @rst
 * .. test:: auto_off.test_held_key_keeps_light_on
 *    :id: TS_AO-006
 *    :tests: SWDD_AO-001, SWDD_AO-005
 * @endrst
 */
TEST(auto_off, test_held_key_keeps_light_on)
{
    /* Arrange */
    CREATE_MOCK(mymock);
    AutoOffData data = {.inactiveTicks = 0, .timeoutTicks = 3};
    const int steps = 10;

    EXPECT_CALL(mymock, RteGetPowerKeyPressedEvent()).Times(steps).WillRepeatedly(Return(FALSE));
    EXPECT_CALL(mymock, RteIsKeyPressed(CONTROL_KEY_UP)).Times(steps).WillRepeatedly(Return(FALSE));
    EXPECT_CALL(mymock, RteIsKeyPressed(CONTROL_KEY_DOWN)).Times(steps).WillRepeatedly(Return(TRUE));
    EXPECT_CALL(mymock, RteSetAutoOffState(FALSE)).Times(steps);
    EXPECT_CALL(mymock, RteSetAutoOffState(TRUE)).Times(0);
    EXPECT_CALL(mymock, RteGetPowerState()).Times(0);
    EXPECT_CALL(mymock, RteSetPowerState(_)).Times(0);

    /* Act */
    for (int i = 0; i < steps; ++i)
    {
        autoOffStep(&data);
        /* Assert */
        EXPECT_EQ(data.inactiveTicks, 0u);
    }
}

/**
 * @rst
 * .. test:: auto_off.test_runnable_uses_configured_timeout
 *    :id: TS_AO-007
 *    :tests: SWDD_AO-003
 * @endrst
 */
TEST(auto_off, test_runnable_uses_configured_timeout)
{
    const uint32_t ticks = (CONFIG_AUTO_OFF_TIMEOUT * 1000u) / CONFIG_OS_TASK_PERIOD;

    /* Arrange: no switch-off while the count stays below the timeout */
    CREATE_MOCK(mymock);

    EXPECT_CALL(mymock, RteGetPowerKeyPressedEvent()).WillRepeatedly(Return(FALSE));
    EXPECT_CALL(mymock, RteIsKeyPressed(CONTROL_KEY_UP)).WillRepeatedly(Return(FALSE));
    EXPECT_CALL(mymock, RteIsKeyPressed(CONTROL_KEY_DOWN)).WillRepeatedly(Return(FALSE));
    EXPECT_CALL(mymock, RteGetPowerState()).WillRepeatedly(Return(POWER_STATE_ON));
    EXPECT_CALL(mymock, RteSetPowerState(_)).Times(0);
    EXPECT_CALL(mymock, RteSetAutoOffState(_)).Times(0);

    /* Act */
    for (uint32_t i = 0; i < ticks - 1; ++i)
    {
        autoOff();
    }
    Mock::VerifyAndClearExpectations(&mymock);

    /* Arrange: the next tick reaches the timeout */
    EXPECT_CALL(mymock, RteGetPowerKeyPressedEvent()).WillRepeatedly(Return(FALSE));
    EXPECT_CALL(mymock, RteIsKeyPressed(CONTROL_KEY_UP)).WillRepeatedly(Return(FALSE));
    EXPECT_CALL(mymock, RteIsKeyPressed(CONTROL_KEY_DOWN)).WillRepeatedly(Return(FALSE));
    EXPECT_CALL(mymock, RteGetPowerState()).WillRepeatedly(Return(POWER_STATE_ON));
    EXPECT_CALL(mymock, RteSetPowerState(POWER_STATE_OFF)).Times(1);
    EXPECT_CALL(mymock, RteSetAutoOffState(TRUE)).Times(1);

    /* Act */
    autoOff();
}
