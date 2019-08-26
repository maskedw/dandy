/**
 *       @file  DDateTime.cpp
 *      @brief
 *
 *    @details
 *
 *     @author  MaskedW
 *
 *   @internal
 *     Created  2019/07/25
 * ===================================================================
 */

/*
 * License: MIT license
 * Copyright (c) <2019> <MaskedW [maskedw00@gmail.com]>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#include <dandy/chrono/DDateTime.hpp>
#include <dandy/core/utils/DStringUtils.hpp>


static const uint8_t D__LENGTH_OF_MONTH_TABLE[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

DDate::DDate()
    : m_year(-1)
    , m_month(-1)
    , m_day(-1)
{
}

DDate::DDate(int year, int month, int day)
    : m_year(year)
    , m_month(month)
    , m_day(day)
{
}

int DDate::getYear() const
{
    return m_year;
}

int DDate::getMonth() const
{
    return m_month;
}

int DDate::getDay() const
{
    return m_day;
}

int DDate::lengthOfMonth() const
{
    return DDateTimeUtils::lengthOfMonth(m_year, m_month);
}

int64_t DDate::totalDays() const
{
    if (!this->isValid())
        return -1;

    const int year = m_year - 1;
    int64_t days = (year * 365LL) + DDateTimeUtils::countLeapYears(year);
    for (int m = 1; m < m_month; m++)
    {
        days += DDateTimeUtils::lengthOfMonth(m_year, m);
    }
    days += (m_day - 1);

    return days;
}

int64_t DDate::totalSeconds() const
{
    if (!this->isValid())
        return 0;

    return this->totalDays() * 24LL * 60 * 60;
}

bool DDate::isValid() const
{
    if (m_year < 1900)
        return false;

    if (m_month <= 0)
        return false;

    if (m_month > 12)
        return false;

    if (m_day <= 0)
        return false;

    if (this->lengthOfMonth() < 0)
        return false;
    return true;
}

std::string DDate::format(const char* fmt) const
{
    struct tm tm;
    ::memset(&tm, 0, sizeof(tm));
    tm.tm_year = m_year - 1900;
    tm.tm_mon = m_month - 1;
    tm.tm_mday = m_day;
    return d_strftime(fmt, &tm);
}

std::string DDate::toString() const
{
    return this->format();
}

DDate DDate::now()
{
    return DDateTime::now().date();
}

DDate DDate::fromCString(const char* str, const char* fmt)
{
    struct tm tm;
    ::memset(&tm, 0, sizeof(tm));
    x_strptime(str, fmt, &tm);
    const DDate result(tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

    return result;
}

DTime::DTime()
    : m_hour(0)
    , m_minute(0)
    , m_second(0)
{
}

DTime::DTime(int hour, int minute, int second)
    : m_hour(hour)
    , m_minute(minute)
    , m_second(second)
{
}

int DTime::getHour() const
{
    return m_hour;
}

int DTime::getMinute() const
{
    return m_minute;
}

int DTime::getSecond() const
{
    return m_second;
}

bool DTime::isValid() const
{
    if (m_second < 0)
        return false;
    if (m_second >= 60)
        return false;
    if (m_minute < 0)
        return false;
    if (m_minute >= 60)
        return false;
    if (m_hour < 0)
        return false;
    if (m_hour >= 24)
        return false;
    return true;
}

int64_t DTime::toSeconds() const
{
    return (m_hour * 60LL * 60LL) +
           (m_minute * 60LL) +
           (m_second);
}

std::string DTime::format(const char* fmt) const
{
    struct tm tm;
    ::memset(&tm, 0, sizeof(tm));
    tm.tm_hour = m_hour;
    tm.tm_min = m_minute;
    tm.tm_sec = m_second;
    return d_strftime(fmt, &tm);
}

std::string DTime::toString() const
{
    return this->format();
}

DTime DTime::now()
{
    return DDateTime::now().time();
}

DTime DTime::fromCString(const char* str, const char* fmt)
{
    struct tm tm;
    ::memset(&tm, 0, sizeof(tm));
    x_strptime(str, fmt, &tm);
    const DTime result(tm.tm_hour, tm.tm_min, tm.tm_sec);

    return result;
}

DTimeDelta::DTimeDelta()
    : days(0)
    , hours(0)
    , minutes(0)
    , seconds(0)
{
}


int64_t DTimeDelta::totalSeconds() const
{
    const int64_t total =
        days * 24 * 60 * 60 +
        hours * 60 * 60 +
        minutes * 60 +
        seconds;

    return total;
}

std::string DTimeDelta::toString() const
{
    const std::string result(
            DStringUtils::format("d:%d H:%d M:%d S:%d",
                days, hours, minutes, seconds));
    return result;
}

DTimeDelta DTimeDelta::ofDays(int days)
{
    DTimeDelta obj;
    obj.days = days;
    return obj;
}

DTimeDelta DTimeDelta::ofHours(int hours)
{
    DTimeDelta obj;
    obj.hours = hours;
    return obj;
}

DTimeDelta DTimeDelta::ofMinutes(int minutes)
{
    DTimeDelta obj;
    obj.minutes = minutes;
    return obj;
}

DTimeDelta DTimeDelta::ofSeconds(int seconds)
{
    DTimeDelta obj;
    obj.seconds = seconds;
    return obj;
}

DTimeDelta DTimeDelta::ofTime(const DTime& time)
{
    DTimeDelta obj;
    obj.days = time.getHour();
    obj.hours = time.getMinute();
    obj.seconds = time.getSecond();
    return obj;
}

DTimeDelta& DTimeDelta::operator+=(const DTimeDelta& other)
{
    days += other.days;
    hours += other.hours;
    minutes += other.minutes;
    seconds += other.seconds;

    return *this;
}

DTimeDelta& DTimeDelta::operator-=(const DTimeDelta& other)
{
    days -= other.days;
    hours -= other.hours;
    minutes -= other.minutes;
    seconds -= other.seconds;

    return *this;
}

const DTimeDelta operator+(const DTimeDelta& a, const DTimeDelta& b)
{
    DTimeDelta result = a;
    result += b;
    return result;
}

const DTimeDelta operator-(const DTimeDelta& a, const DTimeDelta& b)
{
    DTimeDelta result = a;
    result -= b;
    return result;
}

DTimeRange::DTimeRange()
{
}

DTimeRange::DTimeRange(const DTime& begin, const DTime& end)
    : m_begin(begin)
    , m_end(end)
{
}

DTime DTimeRange::getBegin() const
{
    return m_begin;
}

DTime DTimeRange::getEnd() const
{
    return m_end;
}

bool DTimeRange::contains(const DTime& time) const
{
    const uint32_t begin = m_begin.toSeconds();
    const uint32_t end = m_end.toSeconds();
    const uint32_t now = time.toSeconds();

    if (begin == end)
    {
        return false;
    }
    else if (begin > end) /* ex. 23:00-05:00 */
    {
        if ((end <= now) && (now < begin))
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else  /* ex. 10:00 ~ 20:00 */
    {
        if ((begin <= now) && (now < end))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

DDateTime DTimeRange::after(const DDateTime& dateTime) const
{
    const uint32_t begin = m_begin.toSeconds();
    const uint32_t end = m_end.toSeconds();

    if (begin == end)
        return dateTime;

    if (dateTime.time().toSeconds() > end)
    {   /* ex.
         * 08/23 23:50 range=23:00~05:00 after=08/24 05:00
         * 08/23 23:59 range=23:59~00:00 after=08/24 00:00
         */
        return DDateTime(dateTime.addDays(1).date(), m_end);
    }

    /* ex.
     * 08/23 03:00 range=23:00~05:00 after=08/23 05:00
     * 08/23 03:00 range=23:00~03:00 after=08/23 03:00
     */
    return DDateTime(dateTime.date(), m_end);
}

bool DTimeRange::isValid() const
{
    return m_begin.isValid() && m_end.isValid();
}

bool DTimeRange::isEmpty() const
{
    return m_begin.toSeconds() == m_end.toSeconds();
}

std::string DTimeRange::toString() const
{
    const std::string result(DStringUtils::format(
            "%s-%s",
            m_begin.toString().c_str(),
            m_end.toString().c_str()));

    return result;
}

DDateTime::DDateTime()
    : m_timestamp(0)
{
}

DDateTime::DDateTime(const DTimestamp& timestamp)
    : m_timestamp(timestamp)
{
}

DDateTime::DDateTime(int year, int month, int day, int hour, int minute, int second)
{
    struct tm tm;
    ::memset(&tm, 0, sizeof(tm));

    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;
    tm.tm_hour = hour;
    tm.tm_min = minute;
    tm.tm_sec = second;
    m_timestamp = ::mktime(&tm);
}

DDateTime::DDateTime(const DDate& date, const DTime& time)
{
    struct tm tm;
    ::memset(&tm, 0, sizeof(tm));

    tm.tm_year = date.getYear() - 1900;
    tm.tm_mon = date.getMonth() - 1;
    tm.tm_mday = date.getDay();
    tm.tm_hour = time.getHour();
    tm.tm_min = time.getMinute();
    tm.tm_sec = time.getSecond();
    m_timestamp = ::mktime(&tm);
}

DDateTime::DDateTime(const DDate& date)
{
    struct tm tm;
    ::memset(&tm, 0, sizeof(tm));

    tm.tm_year = date.getYear() - 1900;
    tm.tm_mon = date.getMonth() - 1;
    tm.tm_mday = date.getDay();
    m_timestamp = ::mktime(&tm);
}

DDateTime::DDateTime(const DDateTime& other)
    : m_timestamp(other.m_timestamp)
{
}

DDateTime& DDateTime::operator=(const DDateTime& other)
{
    if (this != &other)
    {
        m_timestamp = other.m_timestamp;
    }

    return *this;
}

DDate DDateTime::date() const
{
    struct tm* tm = ::localtime(&m_timestamp);
    const DDate result(tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday);
    return result;
}

DTime DDateTime::time() const
{
    struct tm* tm = ::localtime(&m_timestamp);
    const DTime result(tm->tm_hour, tm->tm_min, tm->tm_sec);
    return result;
}

time_t DDateTime::timestamp() const
{
    return m_timestamp;
}

DDateTime DDateTime::addDelta(const DTimeDelta& timeDelta) const
{
    struct tm tm;

    memcpy(&tm, ::localtime(&m_timestamp), sizeof(tm));

    /* 値がマイナスになったり、秒が60を超えたりしてもうまいこと丸めて計算してく
     * れる
     */
    tm.tm_sec += timeDelta.seconds;
    tm.tm_min += timeDelta.minutes;
    tm.tm_hour += timeDelta.hours;
    tm.tm_mday += timeDelta.days;

    return DDateTime(::mktime(&tm));
}

DDateTime DDateTime::addDays(int days) const
{
    DTimeDelta delta;
    delta.days = days;

    const DDateTime result = this->addDelta(delta);
    return result;
}

DDateTime DDateTime::addHours(int hours) const
{
    DTimeDelta delta;
    delta.hours = hours;

    const DDateTime result = this->addDelta(delta);
    return result;
}

DDateTime DDateTime::addMinutes(int minutes) const
{
    DTimeDelta delta;
    delta.minutes = minutes;

    const DDateTime result = this->addDelta(delta);
    return result;
}

DDateTime DDateTime::addSeconds(int seconds) const
{
    DTimeDelta delta;
    delta.seconds = seconds;

    const DDateTime result = this->addDelta(delta);
    return result;
}

std::string DDateTime::format(const char* fmt) const
{
    return d_strftime(fmt, m_timestamp);
}

std::string DDateTime::toString() const
{
    return this->format();
}

DDateTime DDateTime::now()
{
    return DDateTime(::time(NULL));
}

DDateTime DDateTime::min()
{
    return DDateTime();
}

DDateTime DDateTime::max()
{
    // https://en.wikipedia.org/wiki/Year_2038_problem
    return DDateTime(INT32_MAX);
}

DDateTime DDateTime::fromCString(const char* str, const char* fmt)
{
    struct tm tm;
    ::memset(&tm, 0, sizeof(tm));
    x_strptime(str, fmt, &tm);

    return DDateTime(::mktime(&tm));
}

bool DDateTimeUtils::isLeapYear(int year)
{
    if (year % 4 == 0)
    {
        if(year % 100 == 0)
        {
            if (year % 400 == 0)
                return true;
            else
                return false;
        }
        else
            return true;
    }
    return false;
}

int DDateTimeUtils::countLeapYears(int year)
{
    // https://stackoverflow.com/a/4587611/3270390
    if (year <= 0)
        return 0;

    return (year / 4) - (year / 100) + (year / 400);
}

int DDateTimeUtils::lengthOfMonth(int year, int month)
{
    if (year < 0)
        return -1;

    if (month <= 0)
        return -1;

    if (month > 12)
        return -1;

    const int length = D__LENGTH_OF_MONTH_TABLE[month - 1];
    if ((month == 2) && DDateTimeUtils::isLeapYear(year))
        return length + 1;
    return length;
}

std::string d_strftime(const char* fmt, DTimestamp timestamp)
{
    const struct tm* tmPtr = ::localtime(&timestamp);
    return d_strftime(fmt, tmPtr);
}

std::string d_strftime(const char* fmt, const struct tm* tmPtr)
{
    char buffer[128];
    ::strftime(buffer, sizeof(buffer), fmt, tmPtr);
    return buffer;
}
