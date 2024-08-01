
[![License](http://img.shields.io/badge/license-MIT-blue.svg?style=flat)](http://opensource.org/licenses/MIT)

## Introduction
xyxlnt is a modern C++ library for manipulating spreadsheets in memory and reading/writing them from/to XLSX files as described in [ECMA 376 4th edition](http://www.ecma-international.org/publications/standards/Ecma-376.htm). The first public release of xyxlnt version 1.0 was on May 10th, 2017. Current work is focused on increasing compatibility, improving performance, and brainstorming future development goals. For a high-level summary of what you can do with this library. Contributions are welcome in the form of pull requests or discussions on [the repository's Issues page](https://github.com/xingyun86/xyxyxlnt/issues).

## Example

Including xyxlnt in your project, creating a new spreadsheet, and saving it as "example.xlsx"

```c++
#include <xyxlnt/xyxlnt.hpp>

int main()
{
    xyxlnt::workbook wb;
    xyxlnt::worksheet ws = wb.active_sheet();
    ws.cell("A1").value(5);
    ws.cell("B2").value("string data");
    ws.cell("C3").formula("=RAND()");
    ws.merge_cells("C3:C4");
    ws.freeze_panes("B2");
    wb.save("example.xlsx");
    return 0;
}
// compile with -std=c++14 -Ixyxlnt/include -lxyxlnt
```

## Building xyxlnt - Using vcpkg

You can download and install xyxlnt using the [vcpkg](https://github.com/microsoft/vcpkg) dependency manager:

    git clone https://github.com/microsoft/vcpkg.git
    cd vcpkg
    ./bootstrap-vcpkg.sh
    ./vcpkg integrate install
    ./vcpkg install xyxlnt

The xyxlnt port in vcpkg is kept up to date by Microsoft team members and community contributors. If the version is out of date, please [create an issue or pull request](https://github.com/microsoft/vcpkg) on the vcpkg repository.

## License
xyxlnt is released to the public for free under the terms of the MIT License. See [LICENSE.md](https://github.com/xingyun86/xyxyxlnt/blob/master/LICENSE.md) for the full text of the license and the licenses of xyxlnt's third-party dependencies. [LICENSE.md](https://github.com/xingyun86/xyxyxlnt/blob/master/LICENSE.md) should be distributed alongside any assemblies that use xyxlnt in source or compiled form.
