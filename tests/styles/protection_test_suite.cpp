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

#include <xyxlnt/styles/protection.hpp>
#include <helpers/test_suite.hpp>
#include <xyxlnt/xyxlnt.hpp>

class protection_test_suite : public test_suite
{
public:
    protection_test_suite()
    {
        register_test(test_all);
    }

    void test_all()
    {
        auto prot = xyxlnt::protection::unlocked_and_visible();
        xyxlnt_assert(!prot.hidden());
        xyxlnt_assert(!prot.locked());

        prot = xyxlnt::protection::locked_and_visible();
        xyxlnt_assert(!prot.hidden());
        xyxlnt_assert(prot.locked());

        prot = xyxlnt::protection::unlocked_and_hidden();
        xyxlnt_assert(prot.hidden());
        xyxlnt_assert(!prot.locked());

        prot = xyxlnt::protection::locked_and_hidden();
        xyxlnt_assert(prot.hidden());
        xyxlnt_assert(prot.locked());
    }
};
static protection_test_suite x;
