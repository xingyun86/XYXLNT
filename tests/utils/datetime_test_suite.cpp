// Copyright (c) 2014-2021 Thomas Fussell
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE
//
// @license: http://www.opensource.org/licenses/mit-license.php
// @author: see AUTHORS file

#include <iostream>

#include <helpers/test_suite.hpp>
#include <xyxlnt/utils/date.hpp>
#include <xyxlnt/utils/datetime.hpp>
#include <xyxlnt/utils/time.hpp>

class datetime_test_suite : public test_suite
{
public:
    datetime_test_suite()
    {
        register_test(test_from_string);
        register_test(test_to_string);
        register_test(test_carry);
        register_test(test_leap_year_bug);
        register_test(test_early_date);
        register_test(test_mac_calendar);
        register_test(test_operators);
        register_test(test_weekday);
    }

    void test_from_string()
    {
        xyxlnt::time t("10:35:45");
        xyxlnt_assert_equals(t.hour, 10);
        xyxlnt_assert_equals(t.minute, 35);
        xyxlnt_assert_equals(t.second, 45);
    }

    void test_to_string()
    {
        xyxlnt::datetime dt(2016, 7, 16, 9, 11, 32, 999999);
        xyxlnt_assert_equals(dt.to_string(), "2016/7/16 9:11:32:999999");
    }

    void test_carry()
    {
        // We want a time that rolls over to the next second, minute, and hour
        // Start off with a time 1 microsecond before the next hour
        xyxlnt::datetime dt(2016, 7, 9, 10, 59, 59, 999999);
        auto number = dt.to_number(xyxlnt::calendar::windows_1900);

        // Add 600 nanoseconds to the raw number which represents time as a fraction of a day
        // In other words, 6 tenths of a millionth of a sixtieth of a sixtieth of a twenty-fourth of a day
        number += (0.6 / 1000000) / 60 / 60 / 24;
        auto rollover = xyxlnt::datetime::from_number(number, xyxlnt::calendar::windows_1900);

        xyxlnt_assert_equals(rollover.hour, 11);
        xyxlnt_assert_equals(rollover.minute, 00);
        xyxlnt_assert_equals(rollover.second, 00);
        xyxlnt_assert_equals(rollover.microsecond, 00);
    }

    void test_leap_year_bug()
    {
        xyxlnt::datetime dt(1900, 2, 29, 0, 0, 0, 0);
        auto number = dt.to_number(xyxlnt::calendar::windows_1900);
        xyxlnt_assert_equals(static_cast<int>(number), 60);
        auto converted = xyxlnt::datetime::from_number(number, xyxlnt::calendar::windows_1900);
        xyxlnt_assert_equals(dt, converted);
    }

    void test_early_date()
    {
        xyxlnt::date d(1900, 1, 29);
        auto number = d.to_number(xyxlnt::calendar::windows_1900);
        xyxlnt_assert_equals(number, 29);
        auto converted = xyxlnt::date::from_number(number, xyxlnt::calendar::windows_1900);
        xyxlnt_assert_equals(d, converted);
    }

    void test_mac_calendar()
    {
        xyxlnt::date d(2016, 7, 16);
        auto number_1900 = d.to_number(xyxlnt::calendar::windows_1900);
        auto number_1904 = d.to_number(xyxlnt::calendar::mac_1904);
        xyxlnt_assert_equals(number_1900, 42567);
        xyxlnt_assert_equals(number_1904, 41105);
        auto converted_1900 = xyxlnt::date::from_number(number_1900, xyxlnt::calendar::windows_1900);
        auto converted_1904 = xyxlnt::date::from_number(number_1904, xyxlnt::calendar::mac_1904);
        xyxlnt_assert_equals(converted_1900, d);
        xyxlnt_assert_equals(converted_1904, d);
    }

    void test_operators()
    {
        xyxlnt::date d1(2016, 7, 16);
        xyxlnt::date d2(2016, 7, 16);
        xyxlnt::date d3(2016, 7, 15);
        xyxlnt_assert_equals(d1, d2);
        xyxlnt_assert_differs(d1, d3);
    }

    void test_weekday()
    {
        xyxlnt_assert_equals(xyxlnt::date(2000, 1, 1).weekday(), 6); // January 1st 2000 was a Saturday
        xyxlnt_assert_equals(xyxlnt::date(2016, 7, 15).weekday(), 5); // July 15th 2016 was a Friday
        xyxlnt_assert_equals(xyxlnt::date(2018, 10, 29).weekday(), 1); // October 29th 2018 was Monday
        xyxlnt_assert_equals(xyxlnt::date(1970, 1, 1).weekday(), 4); // January 1st 1970 was a Thursday
    }
};

static datetime_test_suite x;
