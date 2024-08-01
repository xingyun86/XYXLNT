#pragma once

#include <cmath>
#include <exception>
#include <xyxlnt/utils/exceptions.hpp>

#define XYXLNT_STRINGIFYX(x) #x
#define XYXLNT_STRINGIFY(x) XYXLNT_STRINGIFYX(x)

#define xyxlnt_assert(expression)                                               \
    do                                                                        \
    {                                                                         \
        try                                                                   \
        {                                                                     \
            if (expression) break;                                            \
        }                                                                     \
        catch (std::exception & ex)                                           \
        {                                                                     \
            throw ex;                                                         \
        }                                                                     \
        catch (...)                                                           \
        {                                                                     \
        }                                                                     \
        throw xyxlnt::exception(                                              \
            "assert failed at L:" XYXLNT_STRINGIFY(__LINE__) "\n" XYXLNT_STRINGIFY(expression)); \
    } while (false)

#define xyxlnt_assert_throws_nothing(expression)                                \
    do                                                                        \
    {                                                                         \
        try                                                                   \
        {                                                                     \
            expression;                                                       \
            break;                                                            \
        }                                                                     \
        catch (...)                                                           \
        {                                                                     \
        }                                                                     \
        throw xyxlnt::exception("assert throws nothing failed at L:" XYXLNT_STRINGIFY(__LINE__) "\n" XYXLNT_STRINGIFY(expression)); \
    } while (false)

#define xyxlnt_assert_throws(expression, exception_type)                        \
    do                                                                        \
    {                                                                         \
        try                                                                   \
        {                                                                     \
            expression;                                                       \
        }                                                                     \
        catch (exception_type)                                                \
        {                                                                     \
            break;                                                            \
        }                                                                     \
        catch (...)                                                           \
        {                                                                     \
        }                                                                     \
        throw xyxlnt::exception("assert throws failed at L:" XYXLNT_STRINGIFY(__LINE__) "\n" XYXLNT_STRINGIFY(expression)); \
    } while (false)

#define xyxlnt_assert_equals(left, right) xyxlnt_assert((left) == (right))
#define xyxlnt_assert_differs(left, right) xyxlnt_assert((left) != (right))
#define xyxlnt_assert_delta(left, right, delta) xyxlnt_assert(std::abs((left) - (right)) <= (delta))
