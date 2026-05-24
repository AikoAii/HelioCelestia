#include <cassert>

#include "heliocelestia/validation/datetime_validator.hpp"
#include "heliocelestia/core/datetime.hpp"

int main()
{
    using heliocelestia::validation::isValidDateTime;
    using heliocelestia::core::DateTime;

    //
    // Valid baseline date.
    //

    assert(isValidDateTime({ .year=2025, .month=6, .day=15,
                             .hour=12, .minute=30, .second=0.0,
                             .timezoneOffsetHours=7.0 }));

    //
    // Month bounds.
    //

    assert(!isValidDateTime({ .year=2025, .month=0,  .day=1, .hour=0, .minute=0 }));
    assert(!isValidDateTime({ .year=2025, .month=13, .day=1, .hour=0, .minute=0 }));

    //
    // Calendar-aware day bounds.
    //

    // February 28 valid in non-leap year.
    assert( isValidDateTime({ .year=2025, .month=2, .day=28, .hour=0, .minute=0 }));
    // February 29 invalid in non-leap year.
    assert(!isValidDateTime({ .year=2025, .month=2, .day=29, .hour=0, .minute=0 }));
    // February 29 valid in leap year (2024).
    assert( isValidDateTime({ .year=2024, .month=2, .day=29, .hour=0, .minute=0 }));
    // February 30 always invalid.
    assert(!isValidDateTime({ .year=2024, .month=2, .day=30, .hour=0, .minute=0 }));

    // April has 30 days.
    assert( isValidDateTime({ .year=2025, .month=4, .day=30, .hour=0, .minute=0 }));
    assert(!isValidDateTime({ .year=2025, .month=4, .day=31, .hour=0, .minute=0 }));

    //
    // Hour bounds.
    //

    assert(!isValidDateTime({ .year=2025, .month=1, .day=1, .hour=-1, .minute=0 }));
    assert(!isValidDateTime({ .year=2025, .month=1, .day=1, .hour=24, .minute=0 }));

    //
    // Minute bounds.
    //

    assert(!isValidDateTime({ .year=2025, .month=1, .day=1, .hour=0, .minute=60 }));

    //
    // Second bounds (leap second allowed).
    //

    assert( isValidDateTime({ .year=2025, .month=1, .day=1, .hour=0, .minute=0,
                              .second=60.0 }));
    assert(!isValidDateTime({ .year=2025, .month=1, .day=1, .hour=0, .minute=0,
                              .second=61.0 }));
    assert(!isValidDateTime({ .year=2025, .month=1, .day=1, .hour=0, .minute=0,
                              .second=-0.1 }));

    //
    // Timezone offset bounds (UTC-12 to UTC+14).
    //

    assert( isValidDateTime({ .year=2025, .month=1, .day=1, .hour=0, .minute=0,
                              .timezoneOffsetHours=14.0 }));
    assert(!isValidDateTime({ .year=2025, .month=1, .day=1, .hour=0, .minute=0,
                              .timezoneOffsetHours=14.1 }));
    assert( isValidDateTime({ .year=2025, .month=1, .day=1, .hour=0, .minute=0,
                              .timezoneOffsetHours=-12.0 }));
    assert(!isValidDateTime({ .year=2025, .month=1, .day=1, .hour=0, .minute=0,
                              .timezoneOffsetHours=-12.1 }));

    return 0;
}
