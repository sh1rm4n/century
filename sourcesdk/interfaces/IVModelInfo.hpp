//
// Created by sh1rm4n on 5/24/26.
//

#ifndef CENTURY_IVMODELINFO_HPP
#define CENTURY_IVMODELINFO_HPP

struct model_t;

class IVModelInfo {
public:
    const char* GetModelName(const model_t* model) noexcept {
        return memory::Call<const char*>(this, 3, model);
    }
};

#endif //CENTURY_IVMODELINFO_HPP
