//
// Created by sh1rm4n on 6/20/26.
//

#ifndef CENTURY_CCVAR_HPP
#define CENTURY_CCVAR_HPP

class CConVar;

class CCvar {
public:
    constexpr CConVar* FindCommandBase(const char* name) noexcept {
        return memory::Call<CConVar*>(this, 16, name);
    }
};

#endif //CENTURY_CCVAR_HPP
