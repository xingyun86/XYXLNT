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

#include <xyxlnt/styles/color.hpp>

class color_test_suite : public test_suite
{
public:
    color_test_suite()
    {
        register_test(test_known_colors);
        register_test(test_non_rgb_colors);
    }

    void test_known_colors()
    {
        const std::vector<std::pair<xyxlnt::color, std::string>> known_colors{
            {xyxlnt::color::black(), "FF000000"},
            {xyxlnt::color::white(), "FFFFFFFF"},
            {xyxlnt::color::red(), "FFFF0000"},
            {xyxlnt::color::darkred(), "FF8B0000"},
            {xyxlnt::color::blue(), "FF0000FF"},
            {xyxlnt::color::darkblue(), "FF00008B"},
            {xyxlnt::color::green(), "FF00FF00"},
            {xyxlnt::color::darkgreen(), "FF008B00"},
            {xyxlnt::color::yellow(), "FFFFFF00"},
            {xyxlnt::color::darkyellow(), "FFCCCC00"}};

        for (auto pair : known_colors)
        {
            xyxlnt_assert_equals(pair.first.rgb().hex_string(), pair.second);
        }
    }

    void test_non_rgb_colors()
    {
        xyxlnt::color indexed = xyxlnt::indexed_color(1);
        xyxlnt_assert(!indexed.auto_());
        xyxlnt_assert_equals(indexed.indexed().index(), 1);
        indexed.indexed().index(2);
        xyxlnt_assert_equals(indexed.indexed().index(), 2);
        xyxlnt_assert_throws(indexed.theme(), xyxlnt::invalid_attribute);
        xyxlnt_assert_throws(indexed.rgb(), xyxlnt::invalid_attribute);

        xyxlnt::color theme = xyxlnt::theme_color(3);
        xyxlnt_assert(!theme.auto_());
        xyxlnt_assert_equals(theme.theme().index(), 3);
        theme.theme().index(4);
        xyxlnt_assert_equals(theme.theme().index(), 4);
        xyxlnt_assert_throws(theme.indexed(), xyxlnt::invalid_attribute);
        xyxlnt_assert_throws(theme.rgb(), xyxlnt::invalid_attribute);
    }
};
static color_test_suite x;
