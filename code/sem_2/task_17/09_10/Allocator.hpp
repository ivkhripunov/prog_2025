#pragma once

#include <cstddef>

#include <boost/noncopyable.hpp>

class Allocator : private boost::noncopyable
{
public :

    virtual ~Allocator() = default;

    virtual auto allocate  (std::size_t size = 0, std::size_t alignment = 0) -> void * = 0;

    virtual void deallocate(void * ptr) = 0;

    virtual void show() const {}

protected :

    template < typename T >
    auto get(void * x) const noexcept -> T *
    {
        return static_cast < T * > (x);
    }

    static inline auto s_alignment = alignof(std::max_align_t);
};