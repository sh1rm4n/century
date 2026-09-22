//
// Created by sh1rm4n on 6/27/26.
//

#ifndef CENTURY_C_BASEVIEWMODEL_HPP
#define CENTURY_C_BASEVIEWMODEL_HPP

class C_BaseViewModel {
public:
    constexpr C_BaseEntity* GetOwner() noexcept {
        return memory::Call<C_BaseEntity*>(this, 332);
    }
};

#endif //CENTURY_C_BASEVIEWMODEL_HPP
