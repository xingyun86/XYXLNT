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

#pragma once

#include <xyxlnt/xyxlnt_config.hpp>

// cell
#include <xyxlnt/cell/cell.hpp>
#include <xyxlnt/cell/cell_reference.hpp>
#include <xyxlnt/cell/cell_type.hpp>
#include <xyxlnt/cell/comment.hpp>
#include <xyxlnt/cell/hyperlink.hpp>
#include <xyxlnt/cell/index_types.hpp>
#include <xyxlnt/cell/rich_text.hpp>
#include <xyxlnt/cell/rich_text_run.hpp>

// packaging
#include <xyxlnt/packaging/manifest.hpp>
#include <xyxlnt/packaging/relationship.hpp>
#include <xyxlnt/packaging/uri.hpp>

// styles
#include <xyxlnt/styles/alignment.hpp>
#include <xyxlnt/styles/border.hpp>
#include <xyxlnt/styles/color.hpp>
#include <xyxlnt/styles/fill.hpp>
#include <xyxlnt/styles/font.hpp>
#include <xyxlnt/styles/format.hpp>
#include <xyxlnt/styles/number_format.hpp>
#include <xyxlnt/styles/protection.hpp>
#include <xyxlnt/styles/style.hpp>

// utils
#include <xyxlnt/utils/calendar.hpp>
#include <xyxlnt/utils/date.hpp>
#include <xyxlnt/utils/datetime.hpp>
#include <xyxlnt/utils/exceptions.hpp>
#include <xyxlnt/utils/path.hpp>
#include <xyxlnt/utils/time.hpp>
#include <xyxlnt/utils/timedelta.hpp>
#include <xyxlnt/utils/variant.hpp>

// workbook
#include <xyxlnt/workbook/document_security.hpp>
#include <xyxlnt/workbook/external_book.hpp>
#include <xyxlnt/workbook/metadata_property.hpp>
#include <xyxlnt/workbook/named_range.hpp>
#include <xyxlnt/workbook/streaming_workbook_reader.hpp>
#include <xyxlnt/workbook/streaming_workbook_writer.hpp>
#include <xyxlnt/workbook/theme.hpp>
#include <xyxlnt/workbook/workbook.hpp>
#include <xyxlnt/workbook/worksheet_iterator.hpp>

// worksheet
#include <xyxlnt/worksheet/cell_iterator.hpp>
#include <xyxlnt/worksheet/cell_vector.hpp>
#include <xyxlnt/worksheet/column_properties.hpp>
#include <xyxlnt/worksheet/header_footer.hpp>
#include <xyxlnt/worksheet/major_order.hpp>
#include <xyxlnt/worksheet/page_margins.hpp>
#include <xyxlnt/worksheet/page_setup.hpp>
#include <xyxlnt/worksheet/pane.hpp>
#include <xyxlnt/worksheet/range.hpp>
#include <xyxlnt/worksheet/range_iterator.hpp>
#include <xyxlnt/worksheet/range_reference.hpp>
#include <xyxlnt/worksheet/row_properties.hpp>
#include <xyxlnt/worksheet/selection.hpp>
#include <xyxlnt/worksheet/sheet_format_properties.hpp>
#include <xyxlnt/worksheet/sheet_protection.hpp>
#include <xyxlnt/worksheet/sheet_view.hpp>
#include <xyxlnt/worksheet/worksheet.hpp>
