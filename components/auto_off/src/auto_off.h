#ifndef AUTO_OFF_H
#define AUTO_OFF_H

#include "rte.h"

typedef struct
{
    uint32_t inactiveTicks;
    const uint32_t timeoutTicks;
} AutoOffData;

void autoOff(void);

#endif /* AUTO_OFF_H */
