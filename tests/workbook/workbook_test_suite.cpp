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

#include <algorithm>
#include <iostream>

#include <xyxlnt/xyxlnt.hpp>
#include <detail/serialization/open_stream.hpp>
#include <helpers/temporary_file.hpp>
#include <helpers/test_suite.hpp>

#include <xyxlnt/cell/cell.hpp>
#include <xyxlnt/styles/format.hpp>
#include <xyxlnt/styles/style.hpp>
#include <xyxlnt/workbook/workbook.hpp>
#include <xyxlnt/workbook/worksheet_iterator.hpp>
#include <xyxlnt/worksheet/range.hpp>
#include <xyxlnt/worksheet/worksheet.hpp>

class workbook_test_suite : public test_suite
{
public:
    workbook_test_suite()
    {
        register_test(test_active_sheet);
        register_test(test_create_sheet);
        register_test(test_add_correct_sheet);
        register_test(test_add_sheet_from_other_workbook);
        register_test(test_add_sheet_at_index);
        register_test(test_get_sheet_by_title);
        register_test(test_get_sheet_by_title_const);
        register_test(test_get_sheet_by_index);
        register_test(test_get_sheet_by_index_const);
        register_test(test_index_operator);
        register_test(test_contains);
        register_test(test_iter);
        register_test(test_const_iter);
        register_test(test_get_index);
        register_test(test_get_sheet_names);
        register_test(test_add_named_range);
        register_test(test_get_named_range);
        register_test(test_remove_named_range);
        register_test(test_post_increment_iterator);
        register_test(test_copy_iterator);
        register_test(test_manifest);
        register_test(test_memory);
        register_test(test_clear);
        register_test(test_comparison);
        register_test(test_id_gen);
        register_test(test_load_file);
        register_test(test_Issue279);
        register_test(test_Issue353);
        register_test(test_Issue494);
    }

    void test_active_sheet()
    {
        xyxlnt::workbook wb;
        xyxlnt_assert_equals(wb.active_sheet(), wb[0]);
        
        wb.create_sheet();
        wb.create_sheet();
        wb.active_sheet(2);
        xyxlnt_assert_equals(wb.active_sheet(), wb[2]);
    }

    void test_create_sheet()
    {
        xyxlnt::workbook wb;
        auto new_sheet = wb.create_sheet();
        auto last = std::distance(wb.begin(), wb.end()) - 1;
        xyxlnt_assert_equals(new_sheet, wb[last]);
    }

    void test_add_correct_sheet()
    {
        xyxlnt::workbook wb;
        auto new_sheet = wb.create_sheet();
        new_sheet.cell("A6").value(1.498);
        wb.copy_sheet(new_sheet);
        xyxlnt_assert_differs(wb[1].id(), wb[2].id());
        xyxlnt_assert(wb[1].compare(wb[2], false));
        wb.create_sheet().cell("A6").value(1.497);
        xyxlnt_assert(!wb[1].compare(wb[3], false));
    }

    void test_add_sheet_from_other_workbook()
    {
        xyxlnt::workbook wb1, wb2;
        auto new_sheet = wb1.active_sheet();
        xyxlnt_assert_throws(wb2.copy_sheet(new_sheet), xyxlnt::invalid_parameter);
        xyxlnt_assert_throws(wb2.index(new_sheet), std::runtime_error);
    }

    void test_add_sheet_at_index()
    {
        xyxlnt::workbook wb;
        auto ws = wb.active_sheet();
        ws.cell("B3").value(2);
        ws.title("Active");
        wb.copy_sheet(ws, 0);
        xyxlnt_assert_equals(wb.sheet_titles().at(0), "Sheet1");
        xyxlnt_assert_equals(wb.sheet_by_index(0).cell("B3").value<int>(), 2);
        xyxlnt_assert_equals(wb.sheet_titles().at(1), "Active");
        xyxlnt_assert_equals(wb.sheet_by_index(1).cell("B3").value<int>(), 2);
    }

    void test_remove_sheet()
    {
        xyxlnt::workbook wb, wb2;
        auto new_sheet = wb.create_sheet(0);
        new_sheet.title("removed");
        wb.remove_sheet(new_sheet);
        xyxlnt_assert(!wb.contains("removed"));
        xyxlnt_assert_throws(wb.remove_sheet(wb2.active_sheet()), std::runtime_error);
    }

    void test_get_sheet_by_title()
    {
        xyxlnt::workbook wb;
        auto new_sheet = wb.create_sheet();
        std::string title = "my sheet";
        new_sheet.title(title);
        auto found_sheet = wb.sheet_by_title(title);
        xyxlnt_assert_equals(new_sheet, found_sheet);
        xyxlnt_assert_throws(wb.sheet_by_title("error"), xyxlnt::key_not_found);
        const auto &wb_const = wb;
        xyxlnt_assert_throws(wb_const.sheet_by_title("error"), xyxlnt::key_not_found);
    }

    void test_get_sheet_by_title_const()
    {
        xyxlnt::workbook wb;
        auto new_sheet = wb.create_sheet();
        std::string title = "my sheet";
        new_sheet.title(title);
        const xyxlnt::workbook &wbconst = wb;
        auto found_sheet = wbconst.sheet_by_title(title);
        xyxlnt_assert_equals(new_sheet, found_sheet);
    }

    void test_get_sheet_by_index()
    {
        xyxlnt::workbook wb;
        auto new_sheet = wb.create_sheet();
        xyxlnt_assert_equals(new_sheet, wb.sheet_by_index(1)); // in range
        xyxlnt_assert_throws(wb.sheet_by_index(2), xyxlnt::invalid_parameter); // out of range
    }

    void test_get_sheet_by_index_const()
    {
        xyxlnt::workbook wb;
        auto new_sheet = wb.create_sheet();
        const auto &wb_const = wb;
        xyxlnt_assert_equals(new_sheet, wb_const.sheet_by_index(1)); // in range
        xyxlnt_assert_throws(wb_const.sheet_by_index(2), xyxlnt::invalid_parameter); // out of range
    }

    void test_index_operator() // test_getitem
    {
        xyxlnt::workbook wb;
        xyxlnt_assert_throws_nothing(wb["Sheet1"]);
        xyxlnt_assert_throws(wb["NotThere"], xyxlnt::key_not_found);
    }

    void test_contains()
    {
        xyxlnt::workbook wb;
        xyxlnt_assert(wb.contains("Sheet1"));
        xyxlnt_assert(!wb.contains("NotThere"));
    }

    void test_iter()
    {
        xyxlnt::workbook wb;

        for (auto ws : wb)
        {
            xyxlnt_assert_equals(ws.title(), "Sheet1");
        }

        xyxlnt::workbook wb2;
        auto iter = wb.begin();
        auto iter_copy(iter); // copy ctor
        xyxlnt_assert_equals(iter, iter_copy);
        auto begin_2(wb2.begin());
        xyxlnt_assert_differs(begin_2, iter);
        iter = begin_2; // copy assign
        xyxlnt_assert_equals(iter, begin_2);
        iter = wb.begin();
        iter = std::move(begin_2);
        xyxlnt_assert_equals(iter, wb2.begin());

        auto citer = wb.cbegin();
        auto citer_copy(citer); // copy ctor
        xyxlnt_assert_equals(citer, citer_copy);
        auto cbegin_2(wb2.cbegin());
        xyxlnt_assert_differs(cbegin_2, citer);
        citer = cbegin_2; // copy assign
        xyxlnt_assert_equals(citer, cbegin_2);
        citer = wb.cbegin();
        citer = std::move(cbegin_2);
        xyxlnt_assert_equals(citer, wb2.cbegin());

        wb2.create_sheet(); // wb2 iterators assumed invalidated
        iter = wb2.begin();
        xyxlnt_assert_equals((*iter).title(), "Sheet1");
        ++iter;
        xyxlnt_assert_differs(iter, wb2.begin());
        xyxlnt_assert_equals((*iter).title(), "Sheet2");
        auto temp = --iter;
        xyxlnt_assert_equals((*temp).title(), "Sheet1");
        xyxlnt_assert_equals((*iter).title(), "Sheet1");
        iter++;
        xyxlnt_assert_equals((*iter).title(), "Sheet2");
        temp = iter++;
        xyxlnt_assert_equals((*temp).title(), "Sheet2");
        xyxlnt_assert_equals(iter, wb2.end());

        iter = temp--;
        xyxlnt_assert_equals((*iter).title(), "Sheet2");
        xyxlnt_assert_equals((*temp).title(), "Sheet1");

        xyxlnt_assert_equals(xyxlnt::worksheet_iterator{}, xyxlnt::worksheet_iterator{});
    }

    void test_const_iter()
    {
        const xyxlnt::workbook wb;

        for (auto ws : wb)
        {
            xyxlnt_assert_equals(ws.title(), "Sheet1");
        }

        xyxlnt::workbook wb2;
        auto iter = wb.cbegin();
        auto iter_copy(iter); // copy ctor
        xyxlnt_assert_equals(iter, iter_copy);
        auto begin_2(wb2.cbegin());
        xyxlnt_assert_differs(begin_2, iter);
        iter = begin_2; // copy assign
        xyxlnt_assert_equals(iter, begin_2);
        iter = wb.cbegin();
        iter = std::move(begin_2);
        xyxlnt_assert_equals(iter, wb2.cbegin());
        // wb2 iterators assumed invalidated
        wb2.create_sheet();
        iter = wb2.cbegin();
        xyxlnt_assert_equals((*iter).title(), "Sheet1");
        ++iter;
        xyxlnt_assert_differs(iter, wb2.cbegin());
        xyxlnt_assert_equals((*iter).title(), "Sheet2");
        auto temp = --iter;
        xyxlnt_assert_equals((*temp).title(), "Sheet1");
        xyxlnt_assert_equals((*iter).title(), "Sheet1");
        iter++;
        xyxlnt_assert_equals((*iter).title(), "Sheet2");
        temp = iter++;
        xyxlnt_assert_equals((*temp).title(), "Sheet2");
        xyxlnt_assert_equals(iter, wb2.cend());

        iter = temp--;
        xyxlnt_assert_equals((*iter).title(), "Sheet2");
        xyxlnt_assert_equals((*temp).title(), "Sheet1");

        xyxlnt_assert_equals(xyxlnt::const_worksheet_iterator{}, xyxlnt::const_worksheet_iterator{});
    }

    void test_get_index()
    {
        xyxlnt::workbook wb;
        wb.create_sheet().title("1");
        wb.create_sheet().title("2");

        auto sheet_index = wb.index(wb.sheet_by_title("1"));
        xyxlnt_assert_equals(sheet_index, 1);

        sheet_index = wb.index(wb.sheet_by_title("2"));
        xyxlnt_assert_equals(sheet_index, 2);
    }

    void test_get_sheet_names()
    {
        xyxlnt::workbook wb;
        wb.create_sheet().title("test_get_sheet_titles");

        const std::vector<std::string> expected_titles = {"Sheet1", "test_get_sheet_titles"};

        xyxlnt_assert_equals(wb.sheet_titles(), expected_titles);
    }

    void test_add_named_range()
    {
        xyxlnt::workbook wb, wb2;
        auto new_sheet = wb.create_sheet();
        wb.create_named_range("test_nr", new_sheet, "A1");
        xyxlnt_assert(new_sheet.has_named_range("test_nr"));
        xyxlnt_assert(wb.has_named_range("test_nr"));
        xyxlnt_assert_throws(wb2.create_named_range("test_nr", new_sheet, "A1"), std::runtime_error);
    }

    void test_get_named_range()
    {
        xyxlnt::workbook wb;
        auto new_sheet = wb.create_sheet();
        wb.create_named_range("test_nr", new_sheet, "A1");
        auto found_range = wb.named_range("test_nr");
        auto expected_range = new_sheet.range("A1");
        xyxlnt_assert_equals(expected_range, found_range);
        xyxlnt_assert_throws(wb.named_range("test_nr2"), std::runtime_error);
    }

    void test_remove_named_range()
    {
        xyxlnt::workbook wb;
        auto new_sheet = wb.create_sheet();
        wb.create_named_range("test_nr", new_sheet, "A1");
        wb.remove_named_range("test_nr");
        xyxlnt_assert(!new_sheet.has_named_range("test_nr"));
        xyxlnt_assert(!wb.has_named_range("test_nr"));
        xyxlnt_assert_throws(wb.remove_named_range("test_nr2"), std::runtime_error);
    }

    void test_post_increment_iterator()
    {
        xyxlnt::workbook wb;

        wb.create_sheet().title("Sheet2");
        wb.create_sheet().title("Sheet3");

        auto iter = wb.begin();

        xyxlnt_assert_equals((*(iter++)).title(), "Sheet1");
        xyxlnt_assert_equals((*(iter++)).title(), "Sheet2");
        xyxlnt_assert_equals((*(iter++)).title(), "Sheet3");
        xyxlnt_assert_equals(iter, wb.end());

        const auto wb_const = wb;

        auto const_iter = wb_const.begin();

        xyxlnt_assert_equals((*(const_iter++)).title(), "Sheet1");
        xyxlnt_assert_equals((*(const_iter++)).title(), "Sheet2");
        xyxlnt_assert_equals((*(const_iter++)).title(), "Sheet3");
        xyxlnt_assert_equals(const_iter, wb_const.end());
    }

    void test_copy_iterator()
    {
        xyxlnt::workbook wb;

        wb.create_sheet().title("Sheet2");
        wb.create_sheet().title("Sheet3");

        auto iter = wb.begin();
        xyxlnt_assert_equals((*iter).title(), "Sheet1");

        iter++;
        xyxlnt_assert_equals((*iter).title(), "Sheet2");

        auto copy = wb.begin();
        copy = iter;
        xyxlnt_assert_equals((*iter).title(), "Sheet2");
        xyxlnt_assert_equals(iter, copy);

        iter++;
        xyxlnt_assert_equals((*iter).title(), "Sheet3");
        xyxlnt_assert_differs(iter, copy);

        copy++;
        xyxlnt_assert_equals((*iter).title(), "Sheet3");
        xyxlnt_assert_equals(iter, copy);
    }

    void test_manifest()
    {
        xyxlnt::manifest m;
        xyxlnt_assert(!m.has_default_type("xml"));
        xyxlnt_assert_throws(m.default_type("xml"), xyxlnt::key_not_found);
        xyxlnt_assert(!m.has_relationship(xyxlnt::path("/"), xyxlnt::relationship_type::office_document));
        xyxlnt_assert(m.relationships(xyxlnt::path("xl/workbook.xml")).empty());
    }

    void test_memory()
    {
        xyxlnt::workbook wb, wb2;
        wb.active_sheet().title("swap");
        std::swap(wb, wb2);
        xyxlnt_assert_equals(wb.active_sheet().title(), "Sheet1");
        xyxlnt_assert_equals(wb2.active_sheet().title(), "swap");
        wb = wb2;
        xyxlnt_assert_equals(wb.active_sheet().title(), "swap");
    }

    void test_clear()
    {
        xyxlnt::workbook wb;
        xyxlnt::style s = wb.create_style("s");
        wb.active_sheet().cell("B2").value("B2");
        wb.active_sheet().cell("B2").style(s);
        xyxlnt_assert(wb.active_sheet().cell("B2").has_style());
        wb.clear_styles();
        xyxlnt_assert(!wb.active_sheet().cell("B2").has_style());
        xyxlnt::format format = wb.create_format();
        xyxlnt::font font;
        font.size(41);
        format.font(font, true);
        wb.active_sheet().cell("B2").format(format);
        xyxlnt_assert(wb.active_sheet().cell("B2").has_format());
        wb.clear_formats();
        xyxlnt_assert(!wb.active_sheet().cell("B2").has_format());
        wb.clear();
        xyxlnt_assert(wb.sheet_titles().empty());
    }

    void test_comparison()
    {
        xyxlnt::workbook wb, wb2;
        xyxlnt_assert(wb == wb);
        xyxlnt_assert(!(wb != wb));
        xyxlnt_assert(!(wb == wb2));
        xyxlnt_assert(wb != wb2);

        const auto &wb_const = wb;
        //TODO these aren't tests...
        wb_const.manifest();

        xyxlnt_assert(wb.has_theme());

        wb.create_style("style1");
        wb.style("style1");
        wb_const.style("style1");
    }

    void test_id_gen()
    {
        xyxlnt::workbook wb;
        wb.create_sheet();
        wb.create_sheet();
        wb.remove_sheet(wb[1]);
        wb.create_sheet();
        xyxlnt_assert_differs(wb[1].id(), wb[2].id());
    }

    void test_load_file()
    {
        xyxlnt::path file = path_helper::test_file("2_minimal.xlsx");
        xyxlnt::workbook wb_path(file);
        // ctor from ifstream
        std::ifstream file_reader(file.string(), std::ios::binary);
        xyxlnt_assert_equals(wb_path, xyxlnt::workbook(file_reader));
        // load with string
        xyxlnt::workbook wb_load1;
        xyxlnt_assert_differs(wb_path, wb_load1);
        wb_load1.load(file.string());
        xyxlnt_assert_equals(wb_path, wb_load1);
        // load with wstring
        xyxlnt::workbook wb_load2;
        wb_load2.load(file.string());
        xyxlnt_assert_equals(wb_path, wb_load2);
        // load with path
        xyxlnt::workbook wb_load3;
        wb_load3.load(file);
        xyxlnt_assert_equals(wb_path, wb_load3);
        // load with istream
        xyxlnt::workbook wb_load4;
        std::ifstream file_reader2(file.string(), std::ios::binary);
        wb_load4.load(file_reader2);
        xyxlnt_assert_equals(wb_path, wb_load4);
        // load with vector
        std::ifstream file_reader3(file.string(), std::ios::binary);
        file_reader3.unsetf(std::ios::skipws);
        std::vector<uint8_t> data(std::istream_iterator<uint8_t>{file_reader3},
            std::istream_iterator<uint8_t>());
        xyxlnt::workbook wb_load5;
        wb_load5.load(data);
        xyxlnt_assert_equals(wb_path, wb_load5);
    }

    void test_Issue279()
    {
        xyxlnt::workbook wb(path_helper::test_file("Issue279_workbook_delete_rename.xlsx"));
        while (wb.sheet_count() > 1)
        {
            if (wb[1].title() != "BOM")
            {
                wb.remove_sheet(wb[1]);
            }
            else
            {
                wb.remove_sheet(wb[0]);
            }
        }
        // get sheet bom change title
        auto ws1 = wb.sheet_by_index(0);
        ws1.title("checkedBom");
        // report sheet
        auto ws2 = wb.create_sheet(1);
        ws2.title("REPORT");
        //save a copy file
        wb.save("temp.xlsx");
    }

    void test_Issue353()
    {
        xyxlnt::workbook wb1;
        wb1.load(path_helper::test_file("Issue353_first_row_empty_w_properties.xlsx"));
        wb1.save("temp_issue353.xlsx");

        xyxlnt::workbook wb2;
        wb2.load("temp_issue353.xlsx");
        auto ws = wb2.active_sheet();
        xyxlnt_assert_equals(ws.row_properties(1).spans.get(), "1:8");
        xyxlnt_assert_equals(ws.row_properties(17).spans.get(), "2:7");
    }

    void test_Issue494()
    {
        xyxlnt::workbook wb;
        wb.load(path_helper::test_file("Issue494_shared_string.xlsx"));
        auto ws = wb.active_sheet();
        xyxlnt_assert_equals(ws.cell(2, 1).to_string(), "V1.00");
        xyxlnt_assert_equals(ws.cell(2, 2).to_string(), "V1.00");
    }
};
static workbook_test_suite x;
