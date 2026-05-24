#include "heliocelestia/validation/datetime_validator.hpp"

namespace heliocelestia::validation {

//
// Public leap year helper — declared in datetime_validator.hpp.
//

bool isLeapYear(const int year) noexcept
{
    return
        (year % 4 == 0 && year % 100 != 0)
        ||
        (year % 400 == 0);
}

namespace {

// Returns the number of days in a given month, accounting for leap years.
int daysInMonth(const int month, const int year) noexcept
{
    constexpr int kDays[13] = {
        0,  // placeholder (months are 1-indexed)
        31, // January
        28, // February (non-leap)
        31, // March
        30, // April
        31, // May
        30, // June
        31, // July
        31, // August
        30, // September
        31, // October
        30, // November
        31  // December
    };

    if (month == 2) {
        return isLeapYear(year) ? 29 : 28;
    }

    if (month < 1 || month > 12) return 0;

    return kDays[month];
}

} // anonymous namespace

bool isValidDateTime(
    const core::DateTime& dateTime
) noexcept
{
    //
    // Month bounds.
    //

    if (dateTime.month < 1 || dateTime.month > 12) {
        return false;
    }

    //
    // Day bounds — calendar-aware (honours leap years).
    //

    const int maxDay = daysInMonth(dateTime.month, dateTime.year);

    if (dateTime.day < 1 || dateTime.day > maxDay) {
        return false;
    }

    //
    // Hour bounds.
    //

    if (dateTime.hour < 0 || dateTime.hour > 23) {
        return false;
    }

    //
    // Minute bounds.
    //

    if (dateTime.minute < 0 || dateTime.minute > 59) {
        return false;
    }

    //
    // Second bounds.
    //
    // 60.0 is permitted to accommodate UTC leap seconds.
    //

    if (dateTime.second < 0.0 || dateTime.second > 60.0) {
        return false;
    }

    //
    // Timezone offset bounds.
    //
    // UTC−12:00 (Baker Island) to UTC+14:00 (Line Islands).
    //

    if (
        dateTime.timezoneOffsetHours < -12.0
        ||
        dateTime.timezoneOffsetHours > 14.0
    ) {
        return false;
    }

    return true;
}

} // namespace heliocelestia::validation