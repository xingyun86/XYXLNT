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

#include <xyxlnt/xyxlnt.hpp>
#include <xyxlnt/utils/optional.hpp>
#include <helpers/test_suite.hpp>

// test helpers
namespace {
// increments count when constructed, decrements when destructed
// use to ensure correct ctor/dtor pairing
struct alive_count
{
    alive_count()
    {
        ++count;
    }

    alive_count(const alive_count &)
    {
        ++count;
    }

    alive_count(alive_count &&)
    {
        ++count;
    }

    ~alive_count()
    {
        --count;
    }

    alive_count &operator=(const alive_count &) = default;
    alive_count &operator=(alive_count &&) = default;

    static int count;
};
int alive_count::count = 0;

// implicitly convertible from int
struct convertible
{
    // implicit construction from int
    convertible(int i)
        : val(i)
    {
    }

    int val;
};

// default ctor deleted
struct no_default
{
    no_default() = delete;
    int i;
};
} // namespace

class optional_test_suite : public test_suite
{
public:
    optional_test_suite()
        : test_suite()
    {
        register_test(test_ctor);
        register_test(test_copy_ctor);
        register_test(test_move_ctor);
        register_test(test_copy_assign);
        register_test(test_move_assign);
        register_test(test_set_and_get);
        register_test(test_equality);
        register_test(test_const);
    }

    void test_ctor()
    {
        // default
        xyxlnt::optional<int> opt1;
        xyxlnt_assert(!opt1.is_set());
        // value
        const int test_val = 3;
        xyxlnt::optional<int> opt2(test_val);
        xyxlnt_assert(opt2.is_set());
        xyxlnt_assert_equals(opt2.get(), test_val);
        // converting
        xyxlnt::optional<convertible> opt3(test_val);
        xyxlnt_assert(opt3.is_set());
        xyxlnt_assert_equals(opt3.get().val, test_val);
        // no default ctor
        xyxlnt::optional<no_default> no_def_opt;
    }

    void test_copy_ctor()
    {
        { // copy behaviour
            xyxlnt::optional<int> opt1;
            xyxlnt::optional<int> opt2(opt1);
            xyxlnt_assert_equals(opt1, opt2);

            const int test_val = 123;
            xyxlnt::optional<int> opt3(test_val);
            xyxlnt::optional<int> opt4(opt3);
            xyxlnt_assert_equals(opt3, opt4);
        }
        { // lifetime checks
            xyxlnt::optional<alive_count> opt1(alive_count{});
            xyxlnt_assert_equals(1, alive_count::count);
            {
                xyxlnt::optional<alive_count> opt2(opt1);
                xyxlnt_assert_equals(2, alive_count::count);
            }
            xyxlnt_assert_equals(1, alive_count::count);
        }
        xyxlnt_assert_equals(0, alive_count::count); // dtor test
    }

    void test_move_ctor()
    {
        { // move behaviour
            xyxlnt::optional<int> opt1;
            xyxlnt::optional<int> opt2(std::move(opt1));
            xyxlnt_assert_equals(opt2, xyxlnt::optional<int>{}); // can't test against opt1 so use a temporary

            const int test_val = 123;
            xyxlnt::optional<int> opt3(test_val);
            xyxlnt::optional<int> opt4(std::move(opt3));
            xyxlnt_assert(opt4.is_set()); // moved to optional contains the value
            xyxlnt_assert_equals(opt4.get(), test_val);
        }
        { // lifetime checks
            xyxlnt::optional<alive_count> opt1(alive_count{});
            xyxlnt_assert_equals(1, alive_count::count);
            {
                xyxlnt::optional<alive_count> opt2(std::move(opt1));
                xyxlnt_assert_equals(1, alive_count::count); // opt1 is in a no-value state
            }
            xyxlnt_assert_equals(0, alive_count::count);
        }
        xyxlnt_assert_equals(0, alive_count::count); // dtor test
    }

    void test_copy_assign()
    {
        { // copy assign behaviour
            xyxlnt::optional<int> opt1;
            xyxlnt::optional<int> opt_assign1; // to actually test assignment, the value needs to be already created. using '=' is not enough
            opt_assign1 = opt1;
            xyxlnt_assert_equals(opt1, opt_assign1);

            const int test_val = 123;
            xyxlnt::optional<int> opt2(test_val);
            xyxlnt::optional<int> opt_assign2;
            opt_assign2 = opt2;
            xyxlnt_assert_equals(opt2, opt_assign2);
        }
        { // lifetime checks
            xyxlnt::optional<alive_count> opt1(alive_count{});
            xyxlnt_assert_equals(1, alive_count::count);
            {
                xyxlnt::optional<alive_count> opt_assign1;
                opt_assign1 = opt1;
                xyxlnt_assert_equals(2, alive_count::count);
            }
            xyxlnt_assert_equals(1, alive_count::count);
        }
        xyxlnt_assert_equals(0, alive_count::count); // dtor test
    }

    void test_move_assign()
    {
        { // copy assign behaviour
            xyxlnt::optional<int> opt1;
            xyxlnt::optional<int> opt_assign1; // to actually test assignment, the value needs to be already created. using '=' is not enough
            opt_assign1 = std::move(opt1);
            xyxlnt_assert_equals(opt_assign1, xyxlnt::optional<int>{}); // can't test against opt1 so use a temporary

            const int test_val = 123;
            xyxlnt::optional<int> opt2(test_val);
            xyxlnt::optional<int> opt_assign2;
            opt_assign2 = std::move(opt2);
            xyxlnt_assert(opt_assign2.is_set()); // moved to optional contains the value
            xyxlnt_assert_equals(opt_assign2.get(), test_val);
        }
        { // lifetime checks
            xyxlnt::optional<alive_count> opt1(alive_count{});
            xyxlnt_assert_equals(1, alive_count::count);
            {
                xyxlnt::optional<alive_count> opt_assign1;
                opt_assign1 = std::move(opt1);
                xyxlnt_assert_equals(1, alive_count::count); // opt1 is in a no-value state
            }
            xyxlnt_assert_equals(0, alive_count::count);
        }
        xyxlnt_assert_equals(0, alive_count::count); // dtor test
    }

    void test_set_and_get()
    {
        {
            xyxlnt::optional<int> test_opt;
            xyxlnt_assert(!test_opt.is_set());
            xyxlnt_assert_throws(test_opt.get(), xyxlnt::invalid_attribute);
            // set
            const int test_val1 = 321;
            test_opt.set(test_val1);
            xyxlnt_assert(test_opt.is_set());
            xyxlnt_assert_equals(test_opt.get(), test_val1);
            // set again
            const int test_val2 = 123;
            test_opt.set(test_val2);
            xyxlnt_assert(test_opt.is_set());
            xyxlnt_assert_equals(test_opt.get(), test_val2);
            // clear
            test_opt.clear();
            xyxlnt_assert(!test_opt.is_set());
            xyxlnt_assert_throws(test_opt.get(), xyxlnt::invalid_attribute);
            // set again
            const int test_val3 = 3;
            test_opt.set(test_val3);
            xyxlnt_assert(test_opt.is_set());
            xyxlnt_assert_equals(test_opt.get(), test_val3);
            // operator= set
            xyxlnt::optional<int> test_opt2;
            test_opt2 = test_val1;
            xyxlnt_assert_equals(test_opt2.get(), test_val1);
        }
        { // lifetime checks
            xyxlnt::optional<alive_count> test_opt;
            xyxlnt_assert_equals(0, alive_count::count);
            test_opt.set(alive_count());
            xyxlnt_assert_equals(1, alive_count::count);
            test_opt.set(alive_count()); // reassignment doesn't somehow skip the dtor
            xyxlnt_assert_equals(1, alive_count::count);
            test_opt.clear();
            xyxlnt_assert_equals(0, alive_count::count);
        }
    }

    void test_equality()
    {
        xyxlnt::optional<int> test_opt1;
        xyxlnt::optional<int> test_opt2;
        // no value opts compare equal
        xyxlnt_assert(test_opt1 == test_opt2);
        xyxlnt_assert(!(test_opt1 != test_opt2));
        xyxlnt_assert(test_opt2 == test_opt1);
        xyxlnt_assert(!(test_opt2 != test_opt1));
        // value compares false with no value
        const int test_val = 1;
        test_opt1.set(test_val);
        xyxlnt_assert(test_opt1 != test_opt2);
        xyxlnt_assert(!(test_opt1 == test_opt2));
        xyxlnt_assert(test_opt2 != test_opt1);
        xyxlnt_assert(!(test_opt2 == test_opt1));
        // value compares false with a different value
        const int test_val2 = 2;
        test_opt2.set(test_val2);
        xyxlnt_assert(test_opt1 != test_opt2);
        xyxlnt_assert(!(test_opt1 == test_opt2));
        xyxlnt_assert(test_opt2 != test_opt1);
        xyxlnt_assert(!(test_opt2 == test_opt1));
        // value compares equal with same value
        test_opt2.set(test_val);
        xyxlnt_assert(test_opt1 == test_opt2);
        xyxlnt_assert(!(test_opt1 != test_opt2));
        xyxlnt_assert(test_opt2 == test_opt1);
        xyxlnt_assert(!(test_opt2 != test_opt1));
    }

    void test_const()
    {
        // functions on a const optional
        const int test_val = 1;
        const xyxlnt::optional<int> opt(test_val);
        xyxlnt_assert(opt.is_set());
        xyxlnt_assert(opt.get() == test_val);

        xyxlnt::optional<int> opt2(test_val);
        xyxlnt_assert(opt == opt2);
        xyxlnt_assert(opt2 == opt);
        xyxlnt_assert(!(opt != opt2));
        xyxlnt_assert(!(opt2 != opt));
    }
};
static optional_test_suite x;
