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

#include <xyxlnt/utils/variant.hpp>
#include <helpers/test_suite.hpp>

class variant_test_suite : public test_suite
{
public:
    variant_test_suite()
        : test_suite()
    {
        register_test(test_null);
        register_test(test_int32);
        register_test(test_string);
    }

    void test_null()
    {
        xyxlnt::variant var_null;
        xyxlnt_assert_equals(var_null.value_type(), xyxlnt::variant::type::null);
        xyxlnt_assert(var_null.is(xyxlnt::variant::type::null));
    }

    void test_int32()
    {
        xyxlnt::variant var_int(std::int32_t(10));
        xyxlnt_assert_equals(var_int.value_type(), xyxlnt::variant::type::i4);
        xyxlnt_assert(var_int.is(xyxlnt::variant::type::i4));
        xyxlnt_assert_throws_nothing(var_int.get<std::int32_t>());
        xyxlnt_assert_equals(10, var_int.get<std::int32_t>());
    }

    void test_string()
    {
        xyxlnt::variant var_str1("test1");
        xyxlnt_assert_equals(var_str1.value_type(), xyxlnt::variant::type::lpstr);
        xyxlnt_assert(var_str1.is(xyxlnt::variant::type::lpstr));
        xyxlnt_assert_throws_nothing(var_str1.get<std::string>());
        xyxlnt_assert_equals("test1", var_str1.get<std::string>());

        xyxlnt::variant var_str2(std::string("test2"));
        xyxlnt_assert_equals(var_str2.value_type(), xyxlnt::variant::type::lpstr);
        xyxlnt_assert(var_str2.is(xyxlnt::variant::type::lpstr));
        xyxlnt_assert_throws_nothing(var_str2.get<std::string>());
        xyxlnt_assert_equals("test2", var_str2.get<std::string>());
    }
};
static variant_test_suite x;
