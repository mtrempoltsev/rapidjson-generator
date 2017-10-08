#include "item.h"

const char* toString(Item::Type type)
{
    switch (type)
    {
    case Item::Type::Root:
        return "{root}";
    case Item::Type::Namespace:
        return "namespace";
    case Item::Type::Struct:
        return "struct";
    case Item::Type::Bool:
        return "bool";
    case Item::Type::Double:
        return "double";
    case Item::Type::Int8:
        return "int8";
    case Item::Type::Int16:
        return "int16";
    case Item::Type::Int32:
        return "int32";
    case Item::Type::Int64:
        return "int64";
    case Item::Type::UInt8:
        return "uint8";
    case Item::Type::UInt16:
        return "uint16";
    case Item::Type::UInt32:
        return "uint32";
    case Item::Type::UInt64:
        return "uint64";
    case Item::Type::String:
        return "string";
    case Item::Type::Array:
        return "array";
    }
    return "{invalid}";
}

Item::Item()
    : type_(Type::Root)
    , parent_(nullptr)
{
}

Item::Item(std::string&& name, Type type, Item* parent)
    : name_(std::move(name))
    , type_(type)
    , parent_(parent)
{
}

std::unique_ptr<Item> Item::clone(Item* parent)
{
    auto root = std::make_unique<Item>(std::string(name_), type_, parent);
    for (auto& child : children_)
    {
        root->children_.push_back(child->clone(root.get()));
    }
    return root;
}

Item* Item::addChild(std::string&& name, Item::Type type)
{
    auto item = std::make_unique<Item>(std::move(name), type, this);
    auto result = item.get();
    children_.push_back(std::move(item));
    return result;
}

Item* Item::find(const std::string& name) const
{
    for (auto& item : children_)
    {
        if (item->name_ == name)
            return item.get();

        auto result = item->find(name);
        if (result != nullptr)
            return result;
    }

    return nullptr;
}
