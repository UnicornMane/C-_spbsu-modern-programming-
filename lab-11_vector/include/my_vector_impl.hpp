#include "my_vector.hpp"
#include <string>
#include <utility>
#include <memory>
#include <iostream>
#include <type_traits>


namespace help_functions
{
    int upper_bound_by_power_of_two(std::size_t n)
    {
        if ((n & (n - 1)) == 0)
        {
            return n;
        }
        int count = 0;
        while (n)
        {
            n >>= 1;
            ++count;
        }
        return (1 << (count + 1));
    }
} // namespace help_functions

namespace containers
{
    template<typename T>
    my_vector<T>::my_vector() : capacity_(1), size_(0),
                                array_((T*)(new char[(capacity_ * sizeof(T))]))
    {}

    template<typename T>
    my_vector<T>::my_vector(std::size_t n)
    {
        if (std::is_default_constructible<T>::value == false)
        {
            throw std::bad_alloc();
        }

        capacity_ = help_functions::upper_bound_by_power_of_two(n);
        size_ = n;
    }

    template<typename T>
    my_vector<T>::my_vector(const containers::my_vector<T> &other) : capacity_(other.capacity_), size_(other.size_)
    {
        array_ = ((T*)(new char[(other.capacity_ * sizeof(T))]));
        for (std::size_t i = 0; i < other.size_; ++i)
        {
            array_[i] = other.array_[i];
        }
    }

    template<typename T>
    my_vector<T> &my_vector<T>::operator=(containers::my_vector<T> other)
    {
        swap(other);
        return *this;
    }

    template<typename T>
    my_vector<T>::~my_vector()
    {
        clear();
        delete [](void*)array_;
    }

    template<typename T>
    std::size_t my_vector<T>::size() const
    {
        return size_;
    }

    template<typename T>
    std::size_t my_vector<T>::capacity() const
    {
        return capacity_;
    }

    template<typename T>
    bool my_vector<T>::empty() const
    {
        return size_ == 0;
    }

    template<typename T>
    void my_vector<T>::resize(std::size_t n)
    {
        if (std::is_default_constructible<T>::value == false)
        {
            throw std::bad_alloc();
        }
        size_t oldSize = size_;
        enlarge_without_default_constructor(n);
        for (std::size_t i = oldSize; i < n; ++i)
        {
            new(&array_[i]) T();
        }
    }

    template<typename T>
    void my_vector<T>::reserve(const std::size_t n)
    {
        if (n > capacity_)
        {
            my_vector<T> copiedArray(return_copy_of_my_vector_and_delete_array());
            array_ = (T*)(new char[(n * sizeof(T))]);
            capacity_ = n;
            size_ = copiedArray.size_;
            for (std::size_t i = 0; i < copiedArray.size_; ++i)
            {
                array_[i] = copiedArray.array_[i];
            }
        }
    }

    template<typename T>
    T &my_vector<T>::operator[](const std::size_t index) const
    {
        if (index >= size_)
        {
            throw std::out_of_range("Index out of range");
        }
        return array_[index];
    }

    template<typename T>
    void my_vector<T>::push_back(T t)
    {
        enlarge_without_default_constructor(size_ + 1);
        array_[size_ - 1] = t;
    }

    template<typename T>
    void my_vector<T>::pop_back()
    {
        if (size_ == 0)
        {
            throw std::out_of_range("Can't pop from empty vector");
        }
        --size_;
    }

    template<typename T>
    void my_vector<T>::clear()
    {
        for (std::size_t i = 0; i < size_; ++i)
        {
            array_[i].~T();
        }
        size_ = 0;
    }

    template<typename T>
    std::ostream &operator<<(std::ostream &os, const my_vector<T> &v)
    {
        for (std::size_t i = 0; i < v.size_; ++i)
        {
            os << v.array_[i] << ((i == v.size() - 1) ? "" : " ");
        }
        return os;
    }

    template<typename T>
    void my_vector<T>::swap(my_vector &other)
    {
        std::swap(capacity_, other.capacity_);
        std::swap(size_, other.size_);
        std::swap(array_, other.array_);
    }

    template<typename T>
    my_vector<T> my_vector<T>::return_copy_of_my_vector_and_delete_array()
    {
        my_vector<T> copiedArray(*this);
        clear();
        delete [](void*)array_;
        return copiedArray;
    }

    template<typename T>
    void my_vector<T>::enlarge_without_default_constructor(size_t n)
    {
        if (n <= capacity_)
        {
            size_ = n;
            return;
        }
        my_vector<T> copiedArray(return_copy_of_my_vector_and_delete_array());
        array_ = (T*)(new char[(n * sizeof(T))]);
        capacity_ = help_functions::upper_bound_by_power_of_two(n);
        size_ = n;
        for (std::size_t i = 0; i < copiedArray.size_; ++i)
        {
            array_[i] = copiedArray.array_[i];
        }
    }
} // namespace containers