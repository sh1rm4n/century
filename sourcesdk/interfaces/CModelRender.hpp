//
// Created by sh1rm4n on 6/5/26.
//

#ifndef CENTURY_CMODELRENDER_HPP
#define CENTURY_CMODELRENDER_HPP

class CModelRender {
public:
    constexpr void SuppressEngineLighting(const bool suppress) noexcept {
        memory::Call<void>(this, 23, suppress);
    }
};

#endif //CENTURY_CMODELRENDER_HPP
