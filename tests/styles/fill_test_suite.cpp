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

#include <xyxlnt/cell/cell.hpp>
#include <xyxlnt/styles/fill.hpp>
#include <xyxlnt/utils/date.hpp>
#include <xyxlnt/worksheet/worksheet.hpp>

class fill_test_suite : public test_suite
{
public:
    fill_test_suite()
    {
        register_test(test_properties);
        register_test(test_comparison);
        register_test(test_two_fills);
    }

    void test_properties()
    {
        xyxlnt::fill fill;
        
        xyxlnt_assert_equals(fill.type(), xyxlnt::fill_type::pattern);
        fill = xyxlnt::fill(xyxlnt::gradient_fill());
        xyxlnt_assert_equals(fill.type(), xyxlnt::fill_type::gradient);
        xyxlnt_assert_equals(fill.gradient_fill().type(), xyxlnt::gradient_fill_type::linear);

        xyxlnt_assert_equals(fill.gradient_fill().left(), 0);
        xyxlnt_assert_equals(fill.gradient_fill().right(), 0);
        xyxlnt_assert_equals(fill.gradient_fill().top(), 0);
        xyxlnt_assert_equals(fill.gradient_fill().bottom(), 0);

        xyxlnt_assert_throws(fill.pattern_fill(), xyxlnt::invalid_attribute);

        xyxlnt_assert_equals(fill.gradient_fill().degree(), 0);
		fill = fill.gradient_fill().degree(1);
        xyxlnt_assert_equals(fill.gradient_fill().degree(), 1);

        fill = xyxlnt::pattern_fill().type(xyxlnt::pattern_fill_type::solid);

        fill = fill.pattern_fill().foreground(xyxlnt::color::black());
        xyxlnt_assert(fill.pattern_fill().foreground().is_set());
        xyxlnt_assert_equals(fill.pattern_fill().foreground().get().rgb().hex_string(),
            xyxlnt::color::black().rgb().hex_string());

        fill = fill.pattern_fill().background(xyxlnt::color::green());
        xyxlnt_assert(fill.pattern_fill().background().is_set());
        xyxlnt_assert_equals(fill.pattern_fill().background().get().rgb().hex_string(),
            xyxlnt::color::green().rgb().hex_string());
        
        const auto &const_fill = fill;
        xyxlnt_assert(const_fill.pattern_fill().foreground().is_set());
        xyxlnt_assert(const_fill.pattern_fill().background().is_set());
    }
    
    void test_comparison()
    {
        xyxlnt::fill pattern_fill = xyxlnt::pattern_fill().type(xyxlnt::pattern_fill_type::solid);
        xyxlnt::fill gradient_fill_linear = xyxlnt::gradient_fill().type(xyxlnt::gradient_fill_type::linear);
        xyxlnt::fill gradient_fill_path = xyxlnt::gradient_fill().type(xyxlnt::gradient_fill_type::path);

        xyxlnt_assert_differs(pattern_fill, gradient_fill_linear);
        xyxlnt_assert_differs(gradient_fill_linear, gradient_fill_path);
        xyxlnt_assert_differs(gradient_fill_path, pattern_fill);
    }

    void test_two_fills()
    {
        xyxlnt::workbook wb;
        auto ws1 = wb.active_sheet();

        auto cell1 = ws1.cell("A10");
        auto cell2 = ws1.cell("A11");

        cell1.fill(xyxlnt::fill::solid(xyxlnt::color::yellow()));
        cell1.value("Fill Yellow");

        cell2.fill(xyxlnt::fill::solid(xyxlnt::color::green()));
        cell2.value(xyxlnt::date(2010, 7, 13));

        xyxlnt_assert_equals(cell1.fill(), xyxlnt::fill::solid(xyxlnt::color::yellow()));
        xyxlnt_assert_equals(cell2.fill(), xyxlnt::fill::solid(xyxlnt::color::green()));
    }
};
static fill_test_suite x;
