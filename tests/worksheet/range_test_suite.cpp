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
#include <xyxlnt/styles/font.hpp>
#include <xyxlnt/workbook/workbook.hpp>
#include <xyxlnt/worksheet/header_footer.hpp>
#include <xyxlnt/worksheet/range.hpp>
#include <xyxlnt/worksheet/worksheet.hpp>

class range_test_suite : public test_suite
{
public:
    range_test_suite()
    {
        register_test(test_construction);
        register_test(test_batch_formatting);
        register_test(test_clear_cells);
    }

    void test_construction()
    {
        xyxlnt::workbook wb;
        auto ws = wb.active_sheet();

        xyxlnt::range range_1(ws, xyxlnt::range_reference("A1:D10"));
        xyxlnt_assert_equals(range_1.target_worksheet(), ws);
        xyxlnt_assert_equals(1, range_1.front()[0].row()); // NOTE: querying row/column here desperately needs some shortcuts
        xyxlnt_assert_equals(xyxlnt::column_t("D"), range_1.front().back().column());
        xyxlnt_assert_equals(10, range_1.back()[0].row());
        xyxlnt_assert_equals(xyxlnt::column_t("D"), range_1.back().back().column());
        // assert default parameters in ctor
        xyxlnt::range range_2(ws, xyxlnt::range_reference("A1:D10"), xyxlnt::major_order::row, false);
        xyxlnt_assert_equals(range_1, range_2);
        // assert copy
        xyxlnt::range range_3(range_2);
        xyxlnt_assert_equals(range_1, range_3);

        // column order
        xyxlnt::range range_4(ws, xyxlnt::range_reference("A1:D10"), xyxlnt::major_order::column);
        xyxlnt_assert_equals(xyxlnt::column_t("A"), range_4.front()[0].column()); // NOTE: querying row/column here desperately needs some shortcuts
        xyxlnt_assert_equals(10, range_4.front().back().row());
        xyxlnt_assert_equals(xyxlnt::column_t("D"), range_4.back()[0].column());
        xyxlnt_assert_equals(10, range_4.back().back().row());
        // assignment
        range_3 = range_4;
        xyxlnt_assert_equals(range_3, range_4);
    }

    void test_batch_formatting()
    {
        xyxlnt::workbook wb;
        auto ws = wb.active_sheet();

        for (auto row = 1; row <= 10; ++row)
        {
            for (auto column = 1; column <= 10; ++column)
            {
                auto ref = xyxlnt::cell_reference(column, row);
                ws[ref].value(ref.to_string());
            }
        }

        ws.range("A1:A10").font(xyxlnt::font().name("Arial"));
        ws.range("A1:J1").font(xyxlnt::font().bold(true));

        xyxlnt_assert_equals(ws.cell("A1").font().name(), "Calibri");
        xyxlnt_assert(ws.cell("A1").font().bold());

        xyxlnt_assert_equals(ws.cell("A2").font().name(), "Arial");
        xyxlnt_assert(!ws.cell("A2").font().bold());

        xyxlnt_assert_equals(ws.cell("B1").font().name(), "Calibri");
        xyxlnt_assert(ws.cell("B1").font().bold());

        xyxlnt_assert(!ws.cell("B2").has_format());
    }
    
    void test_clear_cells()
    {
        xyxlnt::workbook wb;
        auto ws = wb.active_sheet();
        ws.cell("A1").value("A1");
        ws.cell("A3").value("A3");
        ws.cell("C1").value("C1");
        ws.cell("B2").value("B2");
        ws.cell("C3").value("C3");
        xyxlnt_assert_equals(ws.calculate_dimension(), xyxlnt::range_reference(1, 1, 3, 3));
        auto range = ws.range("B1:C3");
        range.clear_cells();
        xyxlnt_assert_equals(ws.calculate_dimension(), xyxlnt::range_reference(1, 1, 1, 3));
    }
};
static range_test_suite x;
