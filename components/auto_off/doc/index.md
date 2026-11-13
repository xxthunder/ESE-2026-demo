# Software Detailed Design

This module switches the light off by itself after a time without user activity.

## Module Header

```c
#include "auto_off.h"
#include "rte.h"
```

## Function Description

```c
void autoOff(void)
```

```{spec} Detect user activity
:id: SWDD_AO-001
:refines: SWARCH_001, REQ_40
:integrity: QM

Activity in a tick is the power key event, or the Up key pressed, or the Down key pressed.
The function must read all three inputs in every tick.
```

```{spec} Count inactivity only while ON
:id: SWDD_AO-002
:refines: SWARCH_001, REQ_39
:integrity: QM

Without activity, the inactivity count grows by one per tick while the power state is POWER_STATE_ON.
While the power state is POWER_STATE_OFF, the count is zero.
```

```{spec} Switch off at the timeout
:id: SWDD_AO-003
:refines: SWARCH_001, REQ_39
:integrity: QM

When the inactivity count reaches AUTO_OFF_TIMEOUT seconds, expressed in ticks, the function shall set the power state to POWER_STATE_OFF and restart the count.
```

```{spec} Report the auto-off state
:id: SWDD_AO-004
:refines: SWARCH_001, REQ_59
:integrity: QM

The function shall set the auto-off state to TRUE in the tick in which the timer switches the light off, and to FALSE in every tick with activity.
```

```{spec} Activity restarts the timer
:id: SWDD_AO-005
:refines: SWARCH_001, REQ_40
:integrity: QM

Activity in a tick shall restart the inactivity count, also in the tick in which the timeout would have expired.
```

## Function Flow

```{mermaid}
graph TD
   Start[Start]
   Read[Read power key event, Up and Down]
   Activity{Activity?}
   Reset[Count = 0, auto-off state = FALSE]
   IsOn{Power state ON?}
   Count[Count + 1]
   Reached{Count reached timeout?}
   Off[Set power state OFF, auto-off state = TRUE, count = 0]
   Zero[Count = 0]
   End[End]

   Start --> Read --> Activity
   Activity -->|Yes| Reset --> End
   Activity -->|No| IsOn
   IsOn -->|No| Zero --> End
   IsOn -->|Yes| Count --> Reached
   Reached -->|No| End
   Reached -->|Yes| Off --> End
```
