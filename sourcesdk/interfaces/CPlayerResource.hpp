//
// Created by sh1rm4n on 8/2/26.
//

#ifndef CENTURY_CPLAYERRESOURCE_HPP
#define CENTURY_CPLAYERRESOURCE_HPP

class CPlayerResource {
public:
    constexpr int GetArmor(const int index) noexcept {
#if defined(__linux__)
        return memory::Call<int>(this, 282, index);
#else
        return memory::Call<int>(this, 206, index);
#endif
    }
};

#endif //CENTURY_CPLAYERRESOURCE_HPP
