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

#include <xyxlnt/styles/style.hpp>
#include <xyxlnt/xyxlnt.hpp>
#include <helpers/test_suite.hpp>

class style_test_suite : public test_suite
{
public:
    style_test_suite()
    {
        register_test(test_all);
    }

    void test_all()
    {
        xyxlnt::workbook wb;
        auto test_style = wb.create_style("test_style");
        test_style.number_format(xyxlnt::number_format::date_ddmmyyyy());

        auto copy_style(test_style);
        xyxlnt_assert_equals(test_style, copy_style);

        // number format
        xyxlnt_assert_equals(copy_style.name(), "test_style");
        xyxlnt_assert_equals(copy_style.number_format(), xyxlnt::number_format::date_ddmmyyyy());
        //xyxlnt_assert(!copy_style.number_format_applied()); // this doesn't seem to have sensible behaviour?
        copy_style.number_format(xyxlnt::number_format::date_datetime(), true); // true applied param
        xyxlnt_assert_equals(copy_style.number_format(), xyxlnt::number_format::date_datetime());
        xyxlnt_assert(copy_style.number_format_applied());
        copy_style.number_format(xyxlnt::number_format::date_dmminus(), false); // false applied param
        xyxlnt_assert_equals(copy_style.number_format(), xyxlnt::number_format::date_dmminus());
        xyxlnt_assert(!copy_style.number_format_applied());

        xyxlnt_assert(!copy_style.pivot_button());
        copy_style.pivot_button(true);
        xyxlnt_assert(copy_style.pivot_button());

        xyxlnt_assert(!copy_style.quote_prefix());
        copy_style.quote_prefix(true);
        xyxlnt_assert(copy_style.quote_prefix());
    }
};
static style_test_suite x;
