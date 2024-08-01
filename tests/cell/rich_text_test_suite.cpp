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

#include <ctime>
#include <iostream>
#include <sstream>

#include <helpers/test_suite.hpp>

#include <xyxlnt/cell/rich_text.hpp>

class rich_text_test_suite : public test_suite
{
public:
    rich_text_test_suite()
    {
        register_test(test_operators);
        register_test(test_runs);
        register_test(test_phonetic_runs);
        register_test(test_phonetic_properties);
    }

    void test_operators()
    {
        xyxlnt::rich_text text1;
        xyxlnt::rich_text text2;
        xyxlnt_assert_equals(text1, text2);
        xyxlnt::rich_text_run run_default;
        text1.add_run(run_default);
        xyxlnt_assert_differs(text1, text2);
        text2.add_run(run_default);
        xyxlnt_assert_equals(text1, text2);

        xyxlnt::rich_text_run run_formatted;
        xyxlnt::font run_font;
        run_font.color(xyxlnt::color::green());
        run_font.name("Cambria");
        run_font.scheme("ascheme");
        run_font.size(40);
        run_font.family(17);
        run_formatted.second = run_font;

        xyxlnt::rich_text text_formatted;
        text_formatted.add_run(run_formatted);

        xyxlnt::rich_text_run run_color_differs = run_formatted;
        run_font = xyxlnt::font();
        run_font.color(xyxlnt::color::red());
        run_color_differs.second = run_font;
        xyxlnt::rich_text text_color_differs;
        text_color_differs.add_run(run_color_differs);
        xyxlnt_assert_differs(text_formatted, text_color_differs);

        xyxlnt::rich_text_run run_font_differs = run_formatted;
        run_font = xyxlnt::font();
        run_font.name("Calibri");
        run_font_differs.second = run_font;
        xyxlnt::rich_text text_font_differs;
        text_font_differs.add_run(run_font_differs);
        xyxlnt_assert_differs(text_formatted, text_font_differs);

        xyxlnt::rich_text_run run_scheme_differs = run_formatted;
        run_font = xyxlnt::font();
        run_font.scheme("bscheme");
        run_scheme_differs.second = run_font;
        xyxlnt::rich_text text_scheme_differs;
        text_scheme_differs.add_run(run_scheme_differs);
        xyxlnt_assert_differs(text_formatted, text_scheme_differs);

        xyxlnt::rich_text_run run_size_differs = run_formatted;
        run_font = xyxlnt::font();
        run_font.size(41);
        run_size_differs.second = run_font;
        xyxlnt::rich_text text_size_differs;
        text_size_differs.add_run(run_size_differs);
        xyxlnt_assert_differs(text_formatted, text_size_differs);

        xyxlnt::rich_text_run run_family_differs = run_formatted;
        run_font = xyxlnt::font();
        run_font.family(18);
        run_family_differs.second = run_font;
        xyxlnt::rich_text text_family_differs;
        text_family_differs.add_run(run_family_differs);
        xyxlnt_assert_differs(text_formatted, text_family_differs);
    }

    void test_runs()
    {
        xyxlnt::rich_text rt;
        xyxlnt_assert(rt.runs().empty());
        std::vector<xyxlnt::rich_text_run> test_runs{xyxlnt::rich_text_run{"1_abc_test_123"}, xyxlnt::rich_text_run{"2_abc_test_123"}, xyxlnt::rich_text_run{"3_abc_test_123"}};
        // just one
        rt.add_run(test_runs[0]);
        xyxlnt_assert_equals(1, rt.runs().size());
        xyxlnt_assert_equals(test_runs[0], rt.runs()[0]);
        // whole set
        rt.runs(test_runs);
        xyxlnt_assert_equals(test_runs, rt.runs());
    }

    void test_phonetic_runs()
    {
        xyxlnt::rich_text rt;
        rt.plain_text("取引", true);
        rt.add_phonetic_run({"トリヒキ", 0, 2});

        xyxlnt_assert_equals(rt.phonetic_runs().size(), 1);
        xyxlnt_assert_equals(rt.phonetic_runs()[0].text, "トリヒキ");
        xyxlnt_assert_equals(rt.phonetic_runs()[0].start, 0);
        xyxlnt_assert_equals(rt.phonetic_runs()[0].end, 2);
    }

    void test_phonetic_properties()
    {
        xyxlnt::rich_text rt;
        xyxlnt::phonetic_pr ph(1);
        ph.type(xyxlnt::phonetic_pr::type_from_string("fullwidthKatakana"));
        ph.alignment(xyxlnt::phonetic_pr::alignment_from_string("Center"));
        rt.phonetic_properties(ph);

        xyxlnt_assert_equals(rt.has_phonetic_properties(), true);
        xyxlnt_assert_equals(rt.phonetic_properties().has_type(), true);
        xyxlnt_assert_equals(rt.phonetic_properties().has_alignment(), true);
    }
};
static rich_text_test_suite x{};
