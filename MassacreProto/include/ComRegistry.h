#pragma once

#include <string>
#include <algorithm>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <cassert>
#include "ExternalSingleton.h"
#include "Component.h"
#include "FieldTree.h"
#include "ClassInfo.h"
#include "Debug.h" // HACK

#define MCR_REGISTER_COMPONENT(com) \
    mcr::ComRegistry::instance().addType<com>(#com)

#define MCR_COMPONENT_ORDER(predecessor, successor) \
    namespace { bool g_##predecessor##successor##Order = \
        (mcr::ComRegistry::instance().addOrderingRule(#predecessor, #successor), true); }

namespace mcr {

typedef std::function<rcptr<Component>(Object*)> ComCreator;
typedef std::function<void(FieldTree&)> ComFieldEnumerator;

struct ComDesc
{
    uint id;
    std::string name;
    ComCreator creator;
    ComFieldEnumerator fieldEnum;
};

class ComRegistry: public ExternalSingleton<ComRegistry>
{
public:
    template <typename T>
    ComDesc addType(const std::string& name = ClassInfo<T>::name())
    {
        // TODO: better logic
        m_list.push_back(&T::Desc);

        ComDesc desc = {0, name, &T::create, &T::enumFields};
        return desc;
    }

    void addOrderingRule(
        const std::string& predecessor,
        const std::string& successor)
    {
        assert(m_rules.find(successor + '+' + predecessor) == m_rules.end());
        m_rules.insert(predecessor + '+' + successor);
    }

    rcptr<Component> createComponent(uint id, Object* obj)
    {
        return id < m_list.size() ? m_list[id]->creator(obj) : nullptr;
    }

    void startup()
    {
        std::sort(
            m_list.begin(),
            m_list.end(),
            [this] (const ComDesc* a, const ComDesc* b)
            {
                return m_rules.find(a->name + '+' + b->name) != m_rules.end();
            });

        for (uint i = 0; i < m_list.size(); ++i)
            m_list[i]->id = i;

        // enum fields
        FieldTree fields("Fields");

        for (auto it = m_list.begin(); it != m_list.end(); ++it)
            (*it)->fieldEnum(fields.addNode((*it)->name.c_str()));

        // describe fields
        struct Scribe
        {
            void apply(FieldTree::BeginNode, const std::string& group)
            {
                str += indent + group + ":\n";
                indent.resize(indent.size() + 2, ' ');
            }
            void apply(const FieldList::Field& field)
            {
                str += indent + field.name + '\n';
            }
            void apply(FieldTree::EndNode)
            {
                indent.resize(indent.size() - 2, ' ');
            }

            std::string str, indent;
        };

        Scribe scribe;
        fields.visit(scribe);

        //debug("%s", scribe.str.c_str());
    }

protected:
    std::vector<ComDesc*> m_list;
    std::unordered_set<std::string> m_rules;
};

} // ns mcr
