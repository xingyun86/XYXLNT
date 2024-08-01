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

#include <xyxlnt/styles/conditional_format.hpp>
#include <helpers/test_suite.hpp>
#include <xyxlnt/xyxlnt.hpp>

class conditional_format_test_suite : public test_suite
{
public:
    conditional_format_test_suite()
    {
        register_test(test_all);
    }

    void test_all()
    {
        xyxlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto format = ws.conditional_format(xyxlnt::range_reference("A1:A10"), xyxlnt::condition::text_contains("test"));
        xyxlnt_assert(!format.has_border());
        xyxlnt_assert(!format.has_fill());
        xyxlnt_assert(!format.has_font());
        // set border
        auto border = xyxlnt::border().diagonal(xyxlnt::diagonal_direction::both);
        format.border(border);
        xyxlnt_assert(format.has_border());
        xyxlnt_assert_equals(format.border(), border);
        // set fill
        auto fill = xyxlnt::fill(xyxlnt::gradient_fill().type(xyxlnt::gradient_fill_type::path));
        format.fill(fill);
        xyxlnt_assert(format.has_fill());
        xyxlnt_assert_equals(format.fill(), fill);
        // set font
        auto font = xyxlnt::font().color(xyxlnt::color::darkblue());
        format.font(font);
        xyxlnt_assert(format.has_font());
        xyxlnt_assert_equals(format.font(), font);
        // copy ctor
        auto format_copy(format);
        xyxlnt_assert_equals(format, format_copy);
    }
};
static conditional_format_test_suite x;
