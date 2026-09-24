template<class T>
void DArr<T>::reallocate(size_t new_cap)
{
    if(new_cap < m_size) new_cap = m_size;
    if(new_cap == 0)new_cap = 1;
    
    T* temp = new T[new_cap];
    for(size_t i=0;i<m_size;i++)
    {
        temp[i] = std::move(m_arr[i]);
    }
    delete []m_arr;
    m_arr = temp;
    m_capacity = new_cap;
}

template<class T>
DArr<T>::DArr(): m_size(0), m_capacity(1), m_arr(new T[1])
{
}

template<class T>
DArr<T>::DArr(const size_t capacity):m_size(0), m_capacity(capacity==0?1:capacity), m_arr(new T[m_capacity])
{   
}

template<class T>
DArr<T>::DArr(const size_t capacity, const T& default_value):m_size(capacity), m_capacity(capacity==0?1:capacity), m_arr(new T[m_capacity])
{
    for(size_t i=0;i<m_size;i++)
    {
        m_arr[i] = default_value;
    }
}


template<class T>
DArr<T>::DArr(const DArr& other) noexcept: m_size(other.m_size), m_capacity(other.m_capacity), m_arr(new T[other.m_capacity])
{
    for(size_t i=0;i<m_size;i++)
        m_arr[i] = other.m_arr[i];
}

template<class T>
DArr<T>::DArr(DArr&& other): m_size(other.m_size), m_capacity(other.m_capacity), m_arr(other.m_arr)
{
    other.m_arr = nullptr;
    other.m_capacity = 0;
    other.m_size = 0;
}

template<class T>
DArr<T>& DArr<T>::operator=(DArr other)
{
    std::swap(m_size, other.m_size);
    std::swap(m_capacity, other.m_capacity);
    std::swap(m_arr, other.m_arr);
    return *this;
}

template<class T>
DArr<T>::~DArr()
{
    delete []m_arr;
}

template<class T>
void DArr<T>::push_back(const T& value)
{
    if(m_size == m_capacity)
        reallocate(m_capacity == 0? 1: m_capacity * 2);
    
    m_arr[m_size] = value;
    m_size++;
}

template<class T>
void DArr<T>::pop_back()
{
    if(m_size == 0)
    {
        throw(std::runtime_error("Array is Empty"));
    }
    m_size--;
    if(m_capacity > 1 && m_size <= m_capacity/4)
        reallocate(m_capacity / 2);
}

template<class T>
size_t DArr<T>::size() const noexcept
{
    return m_size;
}

template<class T>
T& DArr<T>::get(size_t idx) const
{
    if(idx >= m_size)
    {
        throw std::out_of_range("Index out of Range\n");
    }
    return m_arr[idx];
}

template<class T>
void DArr<T>::set(size_t idx, const T& value)
{
    if(idx >= m_size)
    {
        throw std::out_of_range("Index out of bounds");
    }
    m_arr[idx] = value;
}

