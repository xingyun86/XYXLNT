#include <xyxlnt/xyxlnt.hpp>
#include <chrono>
#include <helpers/path_helper.hpp>

namespace {
using milliseconds_d = std::chrono::duration<double, std::milli>;

void run_load_test(const xyxlnt::path &file, int runs = 10)
{
    std::cout << file.string() << "\n\n";

    xyxlnt::workbook wb;
    std::vector<std::chrono::steady_clock::duration> test_timings;

    for (int i = 0; i < runs; ++i)
    {
        auto start = std::chrono::steady_clock::now();
        wb.load(file);

        auto end = std::chrono::steady_clock::now();
        wb.clear();
        test_timings.push_back(end - start);

        std::cout << milliseconds_d(test_timings.back()).count() << " ms\n";
    }
}

void run_save_test(const xyxlnt::path &file, int runs = 10)
{
    std::cout << file.string() << "\n\n";

    xyxlnt::workbook wb;
    wb.load(file);
    const xyxlnt::path save_path(file.filename());

    std::vector<std::chrono::steady_clock::duration> test_timings;

    for (int i = 0; i < runs; ++i)
    {
        auto start = std::chrono::steady_clock::now();

        wb.save(save_path);

        auto end = std::chrono::steady_clock::now();
        test_timings.push_back(end - start);
        std::cout << milliseconds_d(test_timings.back()).count() << " ms\n";
    }
}
} // namespace

int main()
{
    run_load_test(path_helper::benchmark_file("large.xlsx"));
    run_load_test(path_helper::benchmark_file("very_large.xlsx"));

    run_save_test(path_helper::benchmark_file("large.xlsx"));
    run_save_test(path_helper::benchmark_file("very_large.xlsx"));
}