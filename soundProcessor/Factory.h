#pragma once

#include <string>
#include <map>
#include <stdexcept>

template <class AbstractProduct, class IdentifierType, class ProductCreator = AbstractProduct * (*)()>
class AbstractFactory {
public:
    bool Register(const IdentifierType& id, ProductCreator creator) {
        return map_.insert(make_pair(id, creator)).second;
    }
    bool Unregistered(const IdentifierType& id) {
        return map_.erase(id) == 1;
    }

    AbstractProduct* CreateObject(const IdentifierType& id) {
        typename AssocMap::const_iterator it = map_.find(id);
        if (it == map_.end()) {
            throw std::runtime_error("Undefine ID");
        }
        return (it->second)();
    }

private:
    typedef std::map <IdentifierType, ProductCreator> AssocMap;
    AssocMap map_;
};
