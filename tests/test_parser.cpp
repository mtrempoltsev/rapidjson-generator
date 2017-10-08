#include "../parser.h"

#include <sstream>

#include <gtest/gtest.h>

const char* text = R"(
#pragma once

namespace First
{
    namespace Second
    {
        struct Data
        {
            std::vector<double> a;

            bool b;

            double d;

            int8_t i8;
            int16_t i16;
            int32_t i32;
            int64_t i64;

            std::string s;

            struct Inner
            {
                bool ok;
            };

            uint8_t u8;
            uint16_t u16;
            uint32_t u32;
            uint64_t u64;
        };

        struct DataList
        {
            std::vector<Data> data;
        };
    }
}
)";

const std::string expected = 
R"(namespace First
  namespace Second
    struct Data
      array a
        double
      bool b
      double d
      int8 i8
      int16 i16
      int32 i32
      int64 i64
      string s
      struct Inner
        bool ok
      uint8 u8
      uint16 u16
      uint32 u32
      uint64 u64
    struct DataList
      array data
        struct Data
          array a
            double
          bool b
          double d
          int8 i8
          int16 i16
          int32 i32
          int64 i64
          string s
          struct Inner
            bool ok
          uint8 u8
          uint16 u16
          uint32 u32
          uint64 u64
)";

void toString(Item* item, std::stringstream& out, const std::string& indent)
{
    out << indent << toString(item->type_);
    if (!item->name_.empty())
        out << ' ' << item->name_;
    out << '\n';
    for (auto& child : item->children_)
        toString(child.get(), out, indent + "  ");
}

std::string toString(Item* root)
{
    std::stringstream buf;
    for (auto& child : root->children_)
        toString(child.get(), buf, std::string());
    return buf.str();
}

TEST(parser, common)
{
    auto root = Parser::parse(text);

    const auto result = toString(root.get());

    ASSERT_EQ(result, expected);
}
