//
// Created by sh1rm4n on 5/18/26.
//

#ifndef CENTURY_CSDLMGR_HPP
#define CENTURY_CSDLMGR_HPP

class CSDLMgr {
public:
    void* GetMainContext() noexcept {
        return memory::Call<void*>(this, 11);
    }

    void* CreateExtraContext() noexcept {
        return memory::Call<void*>(this, 29);
    }

    void MakeContextCurrent(void* ctx) noexcept {
        memory::Call<void>(this, 31, ctx);
    }
};

#endif //CENTURY_CSDLMGR_HPP
