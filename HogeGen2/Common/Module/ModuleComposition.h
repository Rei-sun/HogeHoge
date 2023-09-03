#pragma once

#include <map>
#include <Module.h>

namespace HogeGen2 {

class ModuleComposition {
    std::map<ModuleID, int> dict;
public:
    ModuleComposition()
    {}
    // Add
    void AddCount(ModuleID key, int count) {
        dict[key] += count;
    }

    // Set
    void SetCount(ModuleID key, int count) {
        dict[key] = count;
    }
    
    // Get
    int GetCount(ModuleID key) {
        return dict.find(key) != dict.end() ? dict[key] : 0;
    }
};

inline ModuleComposition module_composition;

}