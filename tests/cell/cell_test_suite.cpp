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

#include <sstream>

#include <helpers/assertions.hpp>
#include <helpers/test_suite.hpp>

#include <xyxlnt/cell/cell.hpp>
#include <xyxlnt/cell/comment.hpp>
#include <xyxlnt/cell/hyperlink.hpp>
#include <xyxlnt/styles/alignment.hpp>
#include <xyxlnt/styles/border.hpp>
#include <xyxlnt/styles/fill.hpp>
#include <xyxlnt/styles/format.hpp>
#include <xyxlnt/styles/number_format.hpp>
#include <xyxlnt/styles/protection.hpp>
#include <xyxlnt/styles/style.hpp>
#include <xyxlnt/utils/date.hpp>
#include <xyxlnt/utils/datetime.hpp>
#include <xyxlnt/utils/time.hpp>
#include <xyxlnt/utils/timedelta.hpp>
#include <xyxlnt/workbook/workbook.hpp>
#include <xyxlnt/worksheet/range.hpp>
#include <xyxlnt/worksheet/worksheet.hpp>

class cell_test_suite : public test_suite
{
public:
    cell_test_suite()
    {
        register_test(test_infer_numeric);
        register_test(test_constructor);
        register_test(test_null);
        register_test(test_string);
        register_test(test_formula1);
        register_test(test_formula2);
        register_test(test_formula3);
        register_test(test_not_formula);
        register_test(test_boolean);
        register_test(test_error_codes);
        register_test(test_insert_datetime);
        register_test(test_insert_date);
        register_test(test_insert_time);
        register_test(test_cell_formatted_as_date1);
        register_test(test_cell_formatted_as_date2);
        register_test(test_cell_formatted_as_date3);
        register_test(test_illegal_characters);
        register_test(test_timedelta);
        register_test(test_cell_offset);
        register_test(test_font);
        register_test(test_fill);
        register_test(test_border);
        register_test(test_number_format);
        register_test(test_alignment);
        register_test(test_protection);
        register_test(test_style);
        register_test(test_print);
        register_test(test_values);
        register_test(test_reference);
        register_test(test_anchor);
        register_test(test_hyperlink);
        register_test(test_comment);
        register_test(test_copy_and_compare);
        register_test(test_cell_phonetic_properties);
    }

private:
    void test_infer_numeric()
    {
        xyxlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell("A1");

        cell.value("4.2", true);
        xyxlnt_assert_delta(cell.value<double>(), 4.2, 1E-9);

        cell.value("-42.000", true);
        xyxlnt_assert(cell.value<int>() == -42);

        cell.value("0", true);
        xyxlnt_assert(cell.value<int>() == 0);

        cell.value("0.9999", true);
        xyxlnt_assert(cell.value<double>() == 0.9999);

        cell.value("99E-02", true);
        xyxlnt_assert(cell.value<double>() == 0.99);

        cell.value("4", true);
        xyxlnt_assert(cell.value<int>() == 4);

        cell.value("-1E3", true);
        xyxlnt_assert(cell.value<int>() == -1000);

        cell.value("2e+2", true);
        xyxlnt_assert(cell.value<int>() == 200);

        cell.value("3.1%", true);
        xyxlnt_assert_delta(cell.value<double>(), 0.031, 1E-9);

        cell.value("03:40:16", true);
        xyxlnt_assert(cell.value<xyxlnt::time>() == xyxlnt::time(3, 40, 16));

        cell.value("03:", true);
        xyxlnt_assert_equals(cell.value<std::string>(), "03:");

        cell.value("03:40", true);
        xyxlnt_assert(cell.value<xyxlnt::time>() == xyxlnt::time(3, 40));

        cell.value("30:33.865633336", true);
        xyxlnt_assert(cell.value<xyxlnt::time>() == xyxlnt::time(0, 30, 33, 865633));
    }

    void test_constructor()
    {
        xyxlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell(xyxlnt::cell_reference("A", 1));

        xyxlnt_assert(cell.data_type() == xyxlnt::cell::type::empty);
        xyxlnt_assert(cell.column() == "A");
        xyxlnt_assert(cell.column_index() == 1);
        xyxlnt_assert(cell.row() == 1);
        xyxlnt_assert(cell.reference() == "A1");
        xyxlnt_assert(!cell.has_value());
    }

    void test_null()
    {
        xyxlnt::workbook wb;

        const auto datatypes =
            {
                xyxlnt::cell::type::empty,
                xyxlnt::cell::type::boolean,
                xyxlnt::cell::type::error,
                xyxlnt::cell::type::formula_string,
                xyxlnt::cell::type::number,
                xyxlnt::cell::type::shared_string};

        for (const auto &datatype : datatypes)
        {
            auto ws = wb.active_sheet();
            auto cell = ws.cell(xyxlnt::cell_reference(1, 1));

            cell.data_type(datatype);
            xyxlnt_assert(cell.data_type() == datatype);
            cell.clear_value();
            xyxlnt_assert(cell.data_type() == xyxlnt::cell::type::empty);
        }
    }

    void test_string()
    {
        xyxlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell(xyxlnt::cell_reference(1, 1));

        cell.value("hello");
        xyxlnt_assert(cell.data_type() == xyxlnt::cell::type::shared_string);

        cell.value(".");
        xyxlnt_assert(cell.data_type() == xyxlnt::cell::type::shared_string);

        cell.value("0800");
        xyxlnt_assert(cell.data_type() == xyxlnt::cell::type::shared_string);
    }

    void test_formula1()
    {
        xyxlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell(xyxlnt::cell_reference(1, 1));

        cell.value("=42", true);
        //xyxlnt_assert(cell.data_type() == xyxlnt::cell::type::number);
        xyxlnt_assert(cell.has_formula());
    }

    void test_formula2()
    {
        xyxlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell(xyxlnt::cell_reference(1, 1));

        cell.value("=if(A1<4;-1;1)", true);
        //xyxlnt_assert(cell.data_type() == xyxlnt::cell::type::number);
        xyxlnt_assert(cell.has_formula());
    }

    void test_formula3()
    {
        xyxlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell(xyxlnt::cell_reference(1, 1));

        xyxlnt_assert(!cell.has_formula());
        xyxlnt_assert_throws_nothing(cell.formula(""));
        xyxlnt_assert(!cell.has_formula());
        cell.formula("=42");
        xyxlnt_assert(cell.has_formula());
        xyxlnt_assert_equals(cell.formula(), "42");
        cell.clear_formula();
        xyxlnt_assert(!cell.has_formula());
    }

    void test_not_formula()
    {
        xyxlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell(xyxlnt::cell_reference(1, 1));

        cell.value("=", true);
        xyxlnt_assert(cell.data_type() == xyxlnt::cell::type::shared_string);
        xyxlnt_assert(cell.value<std::string>() == "=");
        xyxlnt_assert(!cell.has_formula());
    }

    void test_boolean()
    {
        xyxlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell(xyxlnt::cell_reference(1, 1));

        for (auto value : {true, false})
        {
            cell.value(value);
            xyxlnt_assert(cell.data_type() == xyxlnt::cell::type::boolean);
        }
    }

    void test_error_codes()
    {
        xyxlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell(xyxlnt::cell_reference(1, 1));
        // error string can't be empty
        xyxlnt_assert_throws(cell.error(""), xyxlnt::exception);
        // error string has to have a leading '#'
        xyxlnt_assert_throws(cell.error("not an error"), xyxlnt::exception);

        for (auto error_code : xyxlnt::cell::error_codes())
        {
            // error type from the string format
            cell.value(error_code.first, true);
            xyxlnt_assert(cell.data_type() == xyxlnt::cell::type::error);
            std::string error;
            xyxlnt_assert_throws_nothing(error = cell.error());
            xyxlnt_assert_equals(error, error_code.first);
            // clearing the value clears the error state
            cell.clear_value();
            xyxlnt_assert_throws(cell.error(), xyxlnt::exception);
            // can explicitly set the error
            xyxlnt_assert_throws_nothing(cell.error(error_code.first));
            std::string error2;
            xyxlnt_assert_throws_nothing(error2 = cell.error());
            xyxlnt_assert_equals(error2, error_code.first);
        }
    }

    void test_insert_datetime()
    {
        xyxlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell(xyxlnt::cell_reference(1, 1));

        cell.value(xyxlnt::datetime(2010, 7, 13, 6, 37, 41));

        xyxlnt_assert(cell.data_type() == xyxlnt::cell::type::number);
        xyxlnt_assert_delta(cell.value<double>(), 40372.27616898148, 1E-9);
        xyxlnt_assert(cell.is_date());
        xyxlnt_assert(cell.number_format().format_string() == "yyyy-mm-dd h:mm:ss");
    }

    void test_insert_date()
    {
        xyxlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell(xyxlnt::cell_reference(1, 1));

        cell.value(xyxlnt::date(2010, 7, 13));
        xyxlnt_assert(cell.data_type() == xyxlnt::cell::type::number);
        xyxlnt_assert(cell.value<double>() == 40372.0);
        xyxlnt_assert(cell.is_date());
        xyxlnt_assert(cell.number_format().format_string() == "yyyy-mm-dd");
    }

    void test_insert_time()
    {
        xyxlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell(xyxlnt::cell_reference(1, 1));

        cell.value(xyxlnt::time(1, 3));
        xyxlnt_assert(cell.data_type() == xyxlnt::cell::type::number);
        xyxlnt_assert_delta(cell.value<double>(), 0.04375, 1E-9);
        xyxlnt_assert(cell.is_date());
        xyxlnt_assert(cell.number_format().format_string() == "h:mm:ss");
    }

    void test_cell_formatted_as_date1()
    {
        xyxlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell(xyxlnt::cell_reference(1, 1));

        cell.value(xyxlnt::datetime::today());
        cell.clear_value();
        xyxlnt_assert(!cell.is_date()); // disagree with openpyxl
        xyxlnt_assert(!cell.has_value());
    }

    void test_cell_formatted_as_date2()
    {
        xyxlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell(xyxlnt::cell_reference(1, 1));

        cell.value(xyxlnt::datetime::today());
        cell.value("testme");
        xyxlnt_assert(!cell.is_date());
        xyxlnt_assert(cell.value<std::string>() == "testme");
    }

    void test_cell_formatted_as_date3()
    {
        xyxlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell(xyxlnt::cell_reference(1, 1));

        cell.value(xyxlnt::datetime::today());
        cell.value(true);
        xyxlnt_assert(!cell.is_date());
        xyxlnt_assert(cell.value<bool>() == true);
    }

    void test_illegal_characters()
    {
        xyxlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell(xyxlnt::cell_reference(1, 1));

        // The bytes 0x00 through 0x1F inclusive must be manually escaped in values.
        auto illegal_chrs = {0, 1, 2, 3, 4, 5, 6, 7, 8, 11, 12, 14, 15, 16, 17, 18,
            19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};

        for (auto i : illegal_chrs)
        {
            std::string str(1, i);
            xyxlnt_assert_throws(cell.value(str), xyxlnt::illegal_character);
        }

        cell.value(std::string(1, 33));
        cell.value(std::string(1, 9)); // Tab
        cell.value(std::string(1, 10)); // Newline
        cell.value(std::string(1, 13)); // Carriage return
        cell.value(" Leading and trailing spaces are legal ");
    }

    // void test_time_regex() {}

    void test_timedelta()
    {
        xyxlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell(xyxlnt::cell_reference(1, 1));

        cell.value(xyxlnt::timedelta(1, 3, 0, 0, 0));

        xyxlnt_assert(cell.value<double>() == 1.125);
        xyxlnt_assert(cell.data_type() == xyxlnt::cell::type::number);
        xyxlnt_assert(!cell.is_date());
        xyxlnt_assert(cell.number_format().format_string() == "[hh]:mm:ss");
    }

    void test_cell_offset()
    {
        xyxlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell(xyxlnt::cell_reference(1, 1));
        xyxlnt_assert(cell.offset(1, 2).reference() == "B3");
    }

    void test_font()
    {
        xyxlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell("A1");

        auto font = xyxlnt::font().bold(true);

        cell.font(font);

        xyxlnt_assert(cell.has_format());
        xyxlnt_assert(cell.format().font_applied());
        xyxlnt_assert_equals(cell.font(), font);
    }

    void test_fill()
    {
        xyxlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell("A1");

        xyxlnt::fill fill(xyxlnt::pattern_fill()
                            .type(xyxlnt::pattern_fill_type::solid)
                            .foreground(xyxlnt::color::red()));
        cell.fill(fill);

        xyxlnt_assert(cell.has_format());
        xyxlnt_assert(cell.format().fill_applied());
        xyxlnt_assert_equals(cell.fill(), fill);
    }

    void test_border()
    {
        xyxlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell("A1");

        xyxlnt::border border;
        cell.border(border);

        xyxlnt_assert(cell.has_format());
        xyxlnt_assert(cell.format().border_applied());
        xyxlnt_assert_equals(cell.border(), border);
    }

    void test_number_format()
    {
        xyxlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell("A1");

        xyxlnt::number_format format("dd--hh--mm");
        cell.number_format(format);

        xyxlnt_assert(cell.has_format());
        xyxlnt_assert(cell.format().number_format_applied());
        xyxlnt_assert_equals(cell.number_format().format_string(), "dd--hh--mm");
    }

    void test_alignment()
    {
        xyxlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell("A1");

        xyxlnt::alignment align;
        align.wrap(true);

        cell.alignment(align);

        xyxlnt_assert(cell.has_format());
        xyxlnt_assert(cell.format().alignment_applied());
        xyxlnt_assert_equals(cell.alignment(), align);
    }

    void test_protection()
    {
        xyxlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell("A1");

        xyxlnt_assert(!cell.has_format());

        auto protection = xyxlnt::protection().locked(false).hidden(true);
        cell.protection(protection);

        xyxlnt_assert(cell.has_format());
        xyxlnt_assert(cell.format().protection_applied());
        xyxlnt_assert_equals(cell.protection(), protection);

        xyxlnt_assert(cell.has_format());
        cell.clear_format();
        xyxlnt_assert(!cell.has_format());
    }

    void test_style()
    {
        xyxlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell("A1");

        xyxlnt_assert(!cell.has_style());

        auto test_style = wb.create_style("test_style");
        test_style.number_format(xyxlnt::number_format::date_ddmmyyyy(), true);

        cell.style(test_style);
        xyxlnt_assert(cell.has_style());
        xyxlnt_assert_equals(cell.style().number_format(), xyxlnt::number_format::date_ddmmyyyy());
        xyxlnt_assert_equals(cell.style(), test_style);

        auto other_style = wb.create_style("other_style");
        other_style.number_format(xyxlnt::number_format::date_time2(), true);

        cell.style("other_style");
        xyxlnt_assert_equals(cell.style().number_format(), xyxlnt::number_format::date_time2());
        xyxlnt_assert_equals(cell.style(), other_style);

        auto last_style = wb.create_style("last_style");
        last_style.number_format(xyxlnt::number_format::percentage(), true);

        cell.style(last_style);
        xyxlnt_assert_equals(cell.style().number_format(), xyxlnt::number_format::percentage());
        xyxlnt_assert_equals(cell.style(), last_style);

        xyxlnt_assert_throws(cell.style("doesn't exist"), xyxlnt::key_not_found);

        cell.clear_style();

        xyxlnt_assert(!cell.has_style());
        xyxlnt_assert_throws(cell.style(), xyxlnt::invalid_attribute);
    }

    void test_print()
    {
        xyxlnt::workbook wb;
        auto ws = wb.active_sheet();

        {
            auto cell = ws.cell("A1");

            std::stringstream ss;
            ss << cell;

            auto stream_string = ss.str();

            xyxlnt_assert_equals(cell.to_string(), stream_string);
            xyxlnt_assert_equals(stream_string, "");
        }

        {
            auto cell = ws.cell("A2");

            cell.value(false);

            std::stringstream ss;
            ss << cell;

            auto stream_string = ss.str();

            xyxlnt_assert_equals(cell.to_string(), stream_string);
            xyxlnt_assert_equals(stream_string, "FALSE");
        }

        {
            auto cell = ws.cell("A3");

            cell.value(true);

            std::stringstream ss;
            ss << cell;

            auto stream_string = ss.str();

            xyxlnt_assert_equals(cell.to_string(), stream_string);
            xyxlnt_assert_equals(stream_string, "TRUE");
        }

        {
            auto cell = ws.cell("A4");

            cell.value(1.234);

            std::stringstream ss;
            ss << cell;

            auto stream_string = ss.str();

            xyxlnt_assert_equals(cell.to_string(), stream_string);
            xyxlnt_assert_equals(stream_string, "1.234");
        }

        {
            auto cell = ws.cell("A5");

            cell.error("#REF");

            std::stringstream ss;
            ss << cell;

            auto stream_string = ss.str();

            xyxlnt_assert_equals(cell.to_string(), stream_string);
            xyxlnt_assert_equals(stream_string, "#REF");
        }

        {
            auto cell = ws.cell("A6");

            cell.value("test");

            std::stringstream ss;
            ss << cell;

            auto stream_string = ss.str();

            xyxlnt_assert_equals(cell.to_string(), stream_string);
            xyxlnt_assert_equals(stream_string, "test");
        }
    }

    void test_values()
    {
        xyxlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell("A1");

        cell.value(static_cast<int>(4));
        xyxlnt_assert_equals(cell.value<int>(), 4);

        cell.value(static_cast<unsigned int>(3));
        xyxlnt_assert_equals(cell.value<unsigned>(), 3);

        cell.value(static_cast<unsigned long long>(4));
        xyxlnt_assert_equals(cell.value<unsigned long long>(), 4);

        cell.value(static_cast<long long int>(3));
        xyxlnt_assert_equals(cell.value<long long int>(), 3);

        cell.value(static_cast<float>(3.14));
        xyxlnt_assert_delta(cell.value<float>(), 3.14, 0.001);

        cell.value(static_cast<double>(3.141592));
        xyxlnt_assert_equals(cell.value<double>(), 3.141592);

        auto cell2 = ws.cell("A2");
        cell2.value(std::string(100000, 'a'));
        cell.value(cell2);
        xyxlnt_assert_equals(cell.value<std::string>(), std::string(32767, 'a'));
    }

    void test_reference()
    {
        xyxlnt::cell_reference_hash hash;
        xyxlnt_assert_differs(hash(xyxlnt::cell_reference("A2")), hash(xyxlnt::cell_reference(1, 1)));
        xyxlnt_assert_equals(hash(xyxlnt::cell_reference("A2")), hash(xyxlnt::cell_reference(1, 2)));

        xyxlnt_assert_equals((xyxlnt::cell_reference("A1"), xyxlnt::cell_reference("B2")), xyxlnt::range_reference("A1:B2"));

        xyxlnt_assert_throws(xyxlnt::cell_reference("A1&"), xyxlnt::invalid_cell_reference);
        xyxlnt_assert_throws(xyxlnt::cell_reference("A"), xyxlnt::invalid_cell_reference);

        auto ref = xyxlnt::cell_reference("$B$7");
        xyxlnt_assert_equals(ref.row_absolute(), true);
        xyxlnt_assert_equals(ref.column_absolute(), true);

        ref = xyxlnt::cell_reference("$B7");
        xyxlnt_assert_equals(ref.row_absolute(), false);
        xyxlnt_assert_equals(ref.column_absolute(), true);

        ref = xyxlnt::cell_reference("B$7");
        xyxlnt_assert_equals(ref.row_absolute(), true);
        xyxlnt_assert_equals(ref.column_absolute(), false);

        xyxlnt_assert(xyxlnt::cell_reference("A1") == "A1");
        xyxlnt_assert(xyxlnt::cell_reference("A1") != "A2");
    }

    void test_anchor()
    {
        xyxlnt::workbook wb;
        auto cell = wb.active_sheet().cell("A1");
        auto anchor = cell.anchor();
        xyxlnt_assert_equals(anchor.first, 0);
        xyxlnt_assert_equals(anchor.second, 0);
    }

    void test_hyperlink()
    {
        xyxlnt::workbook wb;
        auto cell = wb.active_sheet().cell("A1");

        xyxlnt_assert(!cell.has_hyperlink());
        xyxlnt_assert_throws(cell.hyperlink(), xyxlnt::invalid_attribute);
        xyxlnt_assert_throws(cell.hyperlink(""), xyxlnt::invalid_parameter);
        // link without optional display
        const std::string link1("http://example.com");
        cell.hyperlink(link1);
        xyxlnt_assert(cell.has_hyperlink());
        xyxlnt_assert(cell.hyperlink().external());
        xyxlnt_assert_equals(cell.hyperlink().url(), link1);
        xyxlnt_assert_equals(cell.hyperlink().relationship().target().to_string(), link1);
        xyxlnt_assert_equals(cell.hyperlink().display(), link1);
        cell.clear_value();
        // link with display
        const std::string link2("http://example2.com");
        const std::string display_txt("notaurl");
        cell.hyperlink(link2, display_txt);
        xyxlnt_assert(cell.has_hyperlink());
        xyxlnt_assert(cell.hyperlink().external());
        xyxlnt_assert_equals(cell.hyperlink().url(), link2);
        xyxlnt_assert_equals(cell.hyperlink().relationship().target().to_string(), link2);
        xyxlnt_assert_equals(cell.hyperlink().display(), display_txt);
        // relative (local) url
        const std::string local("../test_local");
        cell.hyperlink(local);
        xyxlnt_assert(cell.has_hyperlink());
        xyxlnt_assert(cell.hyperlink().external());
        xyxlnt_assert_equals(cell.hyperlink().url(), local);
        xyxlnt_assert_equals(cell.hyperlink().relationship().target().to_string(), local);
        // value
        int cell_test_val = 123;
        cell.value(cell_test_val);
        std::string cell_value_str = std::to_string(cell_test_val);
        cell.hyperlink(link2, display_txt);
        xyxlnt_assert_equals(cell.value<int>(), 123);
        xyxlnt_assert_equals(cell.hyperlink().display(), cell_value_str); // display text ignored
        cell.clear_value();
        // cell overload without display
        const std::string cell_target_str("A2");
        auto cell_target = wb.active_sheet().cell(cell_target_str);
        std::string link3 = wb.active_sheet().title() + '!' + cell_target_str; // Sheet1!A2
        cell.hyperlink(cell_target);
        xyxlnt_assert(cell.has_hyperlink());
        xyxlnt_assert(!cell.hyperlink().external());
        xyxlnt_assert_equals(cell.hyperlink().target_range(), link3);
        xyxlnt_assert_equals(cell.hyperlink().display(), link3);
        cell.clear_value();
        // cell overload with display
        cell.hyperlink(cell_target, display_txt);
        xyxlnt_assert(cell.has_hyperlink());
        xyxlnt_assert(!cell.hyperlink().external());
        xyxlnt_assert_equals(cell.hyperlink().target_range(), link3);
        xyxlnt_assert_equals(cell.hyperlink().display(), display_txt);
        // value
        cell.value(cell_test_val);
        cell.hyperlink(cell_target, display_txt);
        xyxlnt_assert_equals(cell.value<int>(), 123);
        xyxlnt_assert_equals(cell.hyperlink().display(), cell_value_str); // display text ignored
        cell.clear_value();
        // range overload without display
        const std::string range_target_str("A2:A5");
        xyxlnt::range range_target(wb.active_sheet(), xyxlnt::range_reference(range_target_str));
        std::string link4 = wb.active_sheet().title() + '!' + range_target_str; // Sheet1!A2:A5
        cell.hyperlink(range_target);
        xyxlnt_assert(cell.has_hyperlink());
        xyxlnt_assert(!cell.hyperlink().external());
        xyxlnt_assert_equals(cell.hyperlink().target_range(), link4);
        xyxlnt_assert_equals(cell.hyperlink().display(), link4);
        cell.clear_value();
        // range overload with display
        cell.hyperlink(range_target, display_txt);
        xyxlnt_assert(cell.has_hyperlink());
        xyxlnt_assert(!cell.hyperlink().external());
        xyxlnt_assert_equals(cell.hyperlink().target_range(), link4);
        xyxlnt_assert_equals(cell.hyperlink().display(), display_txt);
        // value
        cell.value(cell_test_val);
        cell.hyperlink(range_target, display_txt);
        xyxlnt_assert_equals(cell.value<int>(), 123);
        xyxlnt_assert_equals(cell.hyperlink().display(), cell_value_str); // display text ignored
        cell.clear_value();
    }

    void test_comment()
    {
        xyxlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell = ws.cell("A1");
        xyxlnt_assert(!cell.has_comment());
        xyxlnt_assert_throws(cell.comment(), xyxlnt::exception);
        cell.comment(xyxlnt::comment("comment", "author"));
        xyxlnt_assert(cell.has_comment());
        xyxlnt_assert_equals(cell.comment(), xyxlnt::comment("comment", "author"));
        cell.clear_comment();
        xyxlnt_assert(!cell.has_comment());
        xyxlnt_assert_throws(cell.comment(), xyxlnt::exception);

        xyxlnt::comment comment_with_size("test comment", "author");
        comment_with_size.size(1000, 30);
        cell.comment(comment_with_size);
        xyxlnt_assert_equals(cell.comment(), comment_with_size);
    }

    void test_copy_and_compare()
    {
        xyxlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell1 = ws.cell("A1");
        auto cell2 = ws.cell("A2");

        xyxlnt_assert_equals(cell1, cell1);
        xyxlnt_assert_equals(cell2, cell2);
        xyxlnt_assert_differs(cell1, cell2);
        xyxlnt_assert_differs(cell2, cell1);
        // nullptr equality
        xyxlnt_assert(nullptr == cell1);
        xyxlnt_assert(cell1 == nullptr);
        cell1.value("test");
        xyxlnt_assert(!(nullptr == cell1));
        xyxlnt_assert(!(cell1 == nullptr));
        // copy
        xyxlnt::cell cell3(cell1);
        xyxlnt_assert_equals(cell1, cell3);
        // assign
        cell3 = cell2;
        xyxlnt_assert_equals(cell2, cell3);
    }

    void test_cell_phonetic_properties()
    {
        xyxlnt::workbook wb;
        auto ws = wb.active_sheet();
        auto cell1 = ws.cell("A1");

        xyxlnt_assert_equals(cell1.phonetics_visible(), false);
        cell1.show_phonetics(true);
        xyxlnt_assert_equals(cell1.phonetics_visible(), true);
        cell1.show_phonetics(false);
        xyxlnt_assert_equals(cell1.phonetics_visible(), false);
    }
};

static cell_test_suite x{};
