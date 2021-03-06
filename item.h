﻿#pragma once

#include <memory>
#include <string>
#include <vector>

class Item final
{
public:
    enum class Type
    {
        Root,
        Namespace,
        Struct,
        Bool,
        Double,
        Int8,
        Int16,
        Int32,
        Int64,
        UInt8,
        UInt16,
        UInt32,
        UInt64,
        String,
        Array
    };

    Item();
    Item(std::string&& name, Type type, Item* parent);

    std::unique_ptr<Item> clone(Item* parent);

    Item* addChild(std::string&& name, Item::Type type);

    Item* find(const std::string& name) const;

    std::string name_;
    Type type_;

    Item* parent_;
    std::vector<std::unique_ptr<Item>> children_;
};

const char* toString(Item::Type type);
