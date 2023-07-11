#include <stdio.h>

#include "diagnostic.h"

void print_error(Error e, Location* l)
{
    const char* m;
    switch (e) {
    case MORE_THAN_ONE_PERIOD_IN_FLOATING_TOKEN:
        m = "More than one period (`.`) in Floating token";
        break;
    }
    printf("%s: %s:%lu,%lu", m, l->path, l->span.start.row, l->span.start.column);
}
