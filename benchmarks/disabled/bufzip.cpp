#include <xyxlnt/xyxlnt.hpp>
#include <xyxlnt/serialization/xml_document.hpp>
#include <xyxlnt/serialization/xml_node.hpp>
#include <xyxlnt/serialization/xml_serializer.hpp>

void standard()
{
    xyxlnt::xml_document doc;

    for (int i = 0; i < 1000000; i++) 
    {
	doc.add_child("test");
    }

    xyxlnt::zip_file archive;
    archive.writestr("sheet.xml", doc.to_string());
}

int main()
{
    standard();
    return 0;
}
