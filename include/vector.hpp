#include <cstddef>
#include <stdexcept>
#include <type_traits>

namespace my_containers {
template <typename T>
    requires std::is_copy_assignable_v<T> && std::is_copy_constructible_v<T>
class MyVector {
  public:
    class iterator {
      public:
        using iterator_category = std::random_access_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = T;
        using pointer           = T*;
        using reference         = T&;

      private:
        pointer m_Current;

      public:
        iterator(pointer current) noexcept
            : m_Current{ current } {
        }

        iterator(const iterator& other) noexcept = default;

        iterator& operator=(const iterator& other) noexcept = default;

        reference operator*() const {
            return *m_Current;
        }

        pointer operator->() {
            return m_Current;
        }

        iterator& operator++() {
            ++m_Current;
            return *this;
        }

        iterator operator++(int) {
            iterator tmp = *this;
            ++m_Current;
            return tmp;
        }

        iterator& operator--() {
            --m_Current;
            return *this;
        }

        iterator operator--(int) {
            iterator tmp = *this;
            --m_Current;
            return tmp;
        }

        iterator& operator+=(difference_type n) {
            m_Current += n;
            return *this;
        }

        iterator operator+(difference_type n) const {
            return iterator{ m_Current + n };
        }

        iterator& operator-=(difference_type n) {
            m_Current -= n;
            return *this;
        }

        iterator operator-(difference_type n) const {
            return iterator{ m_Current - n };
        }

        difference_type operator-(const iterator& other) const {
            return m_Current - other.m_Current;
        }

        reference operator[](difference_type n) const {
            return *(m_Current + n);
        }

        bool operator==(const iterator& other) const {
            return m_Current == other.m_Current;
        }

        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }
    };

  private:
    T* m_Data_begin;
    T* m_Data_end;
    T* m_Storage_end;

  public:
    MyVector() noexcept
        : m_Data_begin{ nullptr },
          m_Data_end{ nullptr },
          m_Storage_end{ nullptr } {
    }

    MyVector(size_t size) noexcept
        : m_Data_begin{ new T[size] },
          m_Data_end{ m_Data_begin + size },
          m_Storage_end{ m_Data_begin + size } {
    }

    MyVector(const MyVector& other) noexcept
        : MyVector(other.size()) {
        std::copy(other.begin(), other.end(), m_Data_begin);
    }

    MyVector(MyVector&& other) noexcept
        : m_Data_begin{ other.m_Data_begin },
          m_Data_end{ other.m_Data_end },
          m_Storage_end{ other.m_Storage_end } {
    }

    MyVector(std::initializer_list<T> init) noexcept
        : MyVector(init.size()) {
        std::copy(init.begin(), init.end(), m_Data_begin);
    }

    MyVector& operator=(MyVector other) noexcept {
        std::swap(*this, other);
        return *this;
    }

    ~MyVector() {
        delete[] m_Data_begin;
    };

    size_t size() const {
        return m_Data_end - m_Data_begin;
    }

    size_t capacity() const {
        return m_Storage_end - m_Data_begin;
    }

    bool empty() const {
        return size() == 0;
    }

    void reserve(size_t new_capacity) {
        if (new_capacity > capacity()) {
            T* new_storage = new T[new_capacity];
            std::copy(begin(), end(), new_storage);
            size_t old_size = size();
            delete[] m_Data_begin;
            m_Data_begin  = new_storage;
            m_Data_end    = m_Data_begin + old_size;
            m_Storage_end = m_Data_begin + new_capacity;
        }
    }

    void resize(size_t new_size) {
        if (new_size > size()) {
            reserve(new_size);
            std::fill_n(end(), new_size - size(), T{});
        }
        m_Data_end = m_Data_begin + new_size;
    }

    void resize(size_t new_size, const T& value) {
        if (new_size > size()) {
            reserve(new_size);
            std::fill_n(end(), new_size - size(), value);
        }
        m_Data_end = m_Data_begin + new_size;
    }

    void clear() {
        m_Data_end = m_Data_begin;
    }

    void push_back(const T& value) {
        if (m_Data_end == m_Storage_end) {
            reserve(capacity() == 0 ? 1 : capacity() * 2);
        }
        new (m_Data_end) T{ value };
        ++m_Data_end;
    }

    void emplace_back(T&& value) {
        if (m_Data_end == m_Storage_end) {
            reserve(capacity() == 0 ? 1 : capacity() * 2);
        }
        new (m_Data_end) T{ std::move(value) };
        ++m_Data_end;
    }

    void pop_back() {
        --m_Data_end;
        m_Data_end->~T();
    }

    T& front() {
        return *m_Data_begin;
    }

    const T& front() const {
        return *m_Data_begin;
    }

    T& back() {
        return *(m_Data_end - 1);
    }

    const T& back() const {
        return *(m_Data_end - 1);
    }

    T& operator[](size_t index) {
        return *(m_Data_begin + index);
    }

    const T& operator[](size_t index) const {
        return *(m_Data_begin + index);
    }

    T& at(size_t index) {
        if (index >= size()) {
            throw std::out_of_range("index out of range");
        }
        return *(m_Data_begin + index);
    }

    const T& at(size_t index) const {
        if (index >= size()) {
            throw std::out_of_range("index out of range");
        }
        return *(m_Data_begin + index);
    }

    iterator begin() {
        return iterator{ m_Data_begin };
    }

    const iterator begin() const {
        return iterator{ m_Data_begin };
    }

    iterator end() {
        return iterator{ m_Data_end };
    }

    const iterator end() const {
        return iterator{ m_Data_end };
    }
};
} // namespace my_containers