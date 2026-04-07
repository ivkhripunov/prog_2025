///////////////////////////////////////////
// chapter : Translation
///////////////////////////////////////////
// section : Projects
///////////////////////////////////////////
// content : Pattern Pimpl (Buffer-Based)
// content : Interfaces and Implementations
// content : Compile-Time Dependencies
///////////////////////////////////////////

#pragma once

///////////////////////////////////////////

#include <array>
#include <cstddef>

///////////////////////////////////////////

#include <boost/noncopyable.hpp>

///////////////////////////////////////////

class Entity : boost::noncopyable {
public :
    Entity();

    //  -------------------------------------

    Entity(Entity &&other) noexcept;

    //  -------------------------------------

    ~Entity();

    //  -------------------------------------

    Entity &operator=(Entity &&other) noexcept;

    //  -------------------------------------

    void test() const;

private :
    class Implementation;

    Implementation *get() noexcept;

    const Implementation *get() const noexcept;

    alignas(std::max_align_t) std::array<std::byte, 16> m_storage{};
};

///////////////////////////////////////////
