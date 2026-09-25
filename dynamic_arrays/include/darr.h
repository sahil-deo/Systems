#include <algorithm>
#include <stdexcept>
template <class T>

class DArr 
{
    private:

    size_t m_size;
    size_t m_capacity;
    T* m_arr;

    private:
    void reallocate(size_t new_cap);
    
    public:

    DArr();                                                 // default
    DArr(const size_t capacity);                            // initializer
    DArr(const size_t capacity, const T& default_value);    // initializer with default value
    DArr(const DArr& other) noexcept;                       // copy cons
    DArr(DArr&& other);                                     // move cons
    DArr& operator= (DArr other);                           // assigment operator
    ~DArr();                                                // des

    void push_back(const T& value);
    void pop_back();

    size_t size() const noexcept;

    T& get(size_t idx) const;
    T& operator[](size_t idx) const;
    void set(size_t idx, const T& value);

};

#include <darr.tpp>