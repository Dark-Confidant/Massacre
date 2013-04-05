#pragma once

#include <functional>
#include "bb/ByteBuffer.h"

namespace mcr {

class FieldList
{
public:
    struct Field
    {
        std::string name;
        std::function<void(const void*, bb::ByteBuffer&)> save;
        std::function<void(void*, bb::ByteBuffer&)> load;
    };

    template <class Class, typename T>
    uint add(
        const char* fieldName,
        T(Class::* getter)() const,
        void(Class::* setter)(T) = nullptr)
    {
        typedef typename std::remove_const<typename std::remove_reference<T>::type>::type ValueType;

        Field field = {fieldName};

        field.save = [getter] (const void* inst, bb::ByteBuffer& buf)
        {
            buf << (static_cast<const Class*>(inst)->*getter)();
        };

        if (setter)
        {
            field.load = [setter] (void* inst, bb::ByteBuffer& buf)
            {
                ValueType val;
                buf >> val;

                (static_cast<Class*>(inst)->*setter)(val);
            };
        }

        m_fields.emplace_back(field);

        return uint(m_fields.size() - 1u);
    }

    template <typename T>
    void visit(T& visitor) const
    {
        for (auto it = m_fields.begin(); it != m_fields.end(); ++it)
            visitor.apply(*it);
    }

    template <typename T>
    void visit(T& visitor)
    {
        for (auto it = m_fields.begin(); it != m_fields.end(); ++it)
            visitor.apply(*it);
    }

protected:
    std::vector<Field> m_fields;
};

class FieldTree: public FieldList
{
public:
    FieldTree(const char* name):
        m_name(name) {}

    FieldTree& addNode(const char* name)
    {
        m_nodes.emplace_back(FieldTree(name));
        return m_nodes.back();
    }

    template <typename T>
    void visit(T& visitor) const
    {
        _visit_impl(visitor, this);
    }

    template <typename T>
    void visit(T& visitor)
    {
        _visit_impl(visitor, this);
    }

    enum BeginNode { beginNode };
    enum EndNode { endNode };

protected:
    template <typename T, class Class>
    static void _visit_impl(T& visitor, Class* that)
    {
        visitor.apply(beginNode, that->m_name);
        that->FieldList::visit(visitor);

        for (auto it = that->m_nodes.begin(); it != that->m_nodes.end(); ++it)
            it->visit(visitor);

        visitor.apply(endNode);
    }

    std::string m_name;
    std::list<FieldTree> m_nodes;
};

} // ns mcr
