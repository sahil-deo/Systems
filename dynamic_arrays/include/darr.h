#include <algorithm>
#include <stdexcept>
template <class T>

class DArr 
{
    private:

    T* m_arr{nullptr};
    size_t m_capacity{0};
    size_t m_size{0};

    private:
    void reallocate(size_t new_cap);
    
    public:

    DArr();                     // default
    DArr(const T& value);       // initializer
    DArr(const DArr& other);    // copy cons
    DArr(DArr&& other);         // move cons
    DArr& operator= (DArr other);
    ~DArr();                    // des

    void push_back(const T& value);
    void pop_back();

    size_t size() const noexcept;

    T& get(const int& idx) const;
    void set(const int& idx, const T& value);

};

#include <darr.tpp>