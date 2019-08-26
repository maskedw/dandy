/**
 *       @file  DDateTime.hpp
 *      @brief
 *
 *    @details
 *
 *    この辺のClass構成を参考に必要に応じて機能を追加する感じで実装してます。
 *    https://docs.oracle.com/javase/jp/8/docs/api/java/time/LocalDateTime.html
 *    https://docs.oracle.com/javase/jp/8/docs/api/java/time/LocalDate.html
 *    https://docs.oracle.com/javase/jp/8/docs/api/java/time/LocalTime.html
 *    https://docs.python.jp/3/library/datetime.html
 *    https://doc.qt.io/qt-5/qdatetime.html
 *    https://doc.qt.io/qt-5/qdate.html
 *    https://doc.qt.io/qt-5/qtime.html
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


#ifndef dandy_chrono_DDateTime_hpp_
#define dandy_chrono_DDateTime_hpp_


#include <dandy/core/DCore.hpp>


#ifndef D_DATE_DEFAULT_FORMAT
    #define D_DATE_DEFAULT_FORMAT "%Y/%m/%d"
#endif

#ifndef D_TIME_DEFAULT_FORMAT
    #define D_TIME_DEFAULT_FORMAT "%H:%M:%S"
#endif

#ifndef D_DATE_TIME_DEFAULT_FORMAT
    #define D_DATE_TIME_DEFAULT_FORMAT "%Y/%m/%d %H:%M:%S"
#endif

typedef time_t DTimestamp;
std::string d_strftime(const char* fmt, DTimestamp timestamp);
std::string d_strftime(const char* fmt, const struct tm* tmPtr);


class DDate
{
public:
    DDate();
    DDate(int year, int month, int day);

    int getYear() const;
    int getMonth() const;
    int getDay() const;
    bool isValid() const;
    int lengthOfMonth() const;

    /** Returns the number of days since A.D.1 January 1
     */
    int64_t totalDays() const;

    /** Returns the number of seconds since A.D.1 January 1
     */
    int64_t totalSeconds() const;

    std::string format(const char* fmt=D_DATE_DEFAULT_FORMAT) const;
    std::string toString() const;
    static DDate now();
    static DDate fromCString(const char* str, const char* fmt=D_DATE_DEFAULT_FORMAT);

private:
    int m_year;
    int m_month;
    int m_day;
};


class DTime
{
public:
    DTime();
    DTime(int hour, int minute, int second);
    int getHour() const;
    int getMinute() const;
    int getSecond() const;
    bool isValid() const;
    int64_t toSeconds() const;
    std::string format(const char* fmt=D_TIME_DEFAULT_FORMAT) const;
    std::string toString() const;
    static DTime now();
    static DTime fromCString(const char* str, const char* fmt=D_DATE_DEFAULT_FORMAT);

    int operator==(const DTime& other) const { return (this->toSeconds() == other.toSeconds()); }
    int operator!=(const DTime& other) const { return (this->toSeconds() != other.toSeconds()); }
    int operator<=(const DTime& other) const { return (this->toSeconds() <= other.toSeconds()); }
    int operator>=(const DTime& other) const { return (this->toSeconds() >= other.toSeconds()); }
    int operator< (const DTime& other) const { return (this->toSeconds() <  other.toSeconds()); }
    int operator> (const DTime& other) const { return (this->toSeconds() >  other.toSeconds()); }

private:
    int m_hour;
    int m_minute;
    int m_second;
};


class DTimeDelta
{
public:
    DTimeDelta();
    int days;
    int hours;
    int minutes;
    int seconds;
    int64_t totalSeconds() const;
    std::string toString() const;

    static DTimeDelta ofDays(int days);
    static DTimeDelta ofHours(int hours);
    static DTimeDelta ofMinutes(int minutes);
    static DTimeDelta ofSeconds(int seconds);
    static DTimeDelta ofTime(const DTime& time);

    DTimeDelta& operator+=(const DTimeDelta& p);
    DTimeDelta& operator-=(const DTimeDelta& p);
    friend const DTimeDelta operator+(const DTimeDelta&, const DTimeDelta&);
    friend const DTimeDelta operator-(const DTimeDelta&, const DTimeDelta&);
};


class DDateTime
{
public:
    DDateTime();
    DDateTime(const DTimestamp& timestamp);
    DDateTime(int year, int month, int day, int hour, int minute, int second);
    DDateTime(const DDate& date, const DTime& time);
    DDateTime(const DDate& date);
    DDateTime(const DDateTime& other);
    DDateTime& operator=(const DDateTime& other);

    DDate date() const;
    DTime time() const;
    DTimestamp timestamp() const;
    DDateTime addDelta(const DTimeDelta& timeDelta) const;
    DDateTime addDays(int days) const;
    DDateTime addHours(int hours) const;
    DDateTime addMinutes(int minutes) const;
    DDateTime addSeconds(int seconds) const;

    std::string format(const char* fmt=D_DATE_TIME_DEFAULT_FORMAT) const;
    std::string toString() const;

    static DDateTime now();
    static DDateTime min();
    static DDateTime max();
    static DDateTime fromCString(const char* str, const char* fmt=D_DATE_DEFAULT_FORMAT);

    int operator==(const DDateTime& other) const { return (m_timestamp == other.m_timestamp); }
    int operator!=(const DDateTime& other) const { return (m_timestamp != other.m_timestamp); }
    int operator<=(const DDateTime& other) const { return (m_timestamp <= other.m_timestamp); }
    int operator>=(const DDateTime& other) const { return (m_timestamp >= other.m_timestamp); }
    int operator< (const DDateTime& other) const { return (m_timestamp <  other.m_timestamp); }
    int operator> (const DDateTime& other) const { return (m_timestamp >  other.m_timestamp); }

private:
    DTimestamp m_timestamp;
};


class DTimeRange
{
public:
    DTimeRange();
    DTimeRange(const DTime& begin, const DTime& end);
    DTime getBegin() const;
    DTime getEnd() const;
    bool contains(const DTime& time) const;
    DDateTime after(const DDateTime& dateTime) const;
    bool isValid() const;
    bool isEmpty() const;
    std::string toString() const;

private:
    DTime m_begin;
    DTime m_end;
};


class DDateTimeUtils
{
public:
    /** Returns whether it is a leap year
     */
    static bool isLeapYear(int year);

    /** Returns the number of leap years
     */
    static int countLeapYears(int year);

    /** Returns the number of days of the month
     */
    static int lengthOfMonth(int year, int mouth);
};


#endif /* end of include guard: dandy_chrono_DDateTime_hpp_ */
