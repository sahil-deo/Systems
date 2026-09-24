template<class T>
void DArr<T>::reallocate(size_t new_cap)
{
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
DArr<T>::DArr():m_size(0), m_capacity(0), m_arr(nullptr)
{
    reallocate(1);
}

template<class T>
DArr<T>::DArr(const T& value):m_size(0), m_capacity(0), m_arr(nullptr)
{
    reallocate(1);
    m_size++;
    m_arr[0] = value;

}

template<class T>
DArr<T>::DArr(const DArr& other): m_size(other.m_size), m_capacity(other.m_capacity), m_arr(new T[other.m_capacity])
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
    std::swap(m_arr, other.m_arr);
    std::swap(m_size, other.m_size);
    std::swap(m_capacity, other.m_capacity);
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
    m_size++;
    if(m_size > m_capacity)
        reallocate(m_capacity * 2);

    m_arr[m_size-1] = value;
}

template<class T>
void DArr<T>::pop_back()
{
    if(m_size == 0)
    {
        std::cerr << "Array is empty\n";
        return;
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
T& DArr<T>::get(const int& idx) const
{
    if(idx < 0)
    {
        throw std::out_of_range("Index out of bounds");
    }
    if(idx >= static_cast<int>(m_size))
    {
        throw std::out_of_range("Index out of bounds");
    }

    return m_arr[idx];
}

template<class T>
void DArr<T>::set(const int& idx, const T& value)
{
    if(idx < 0)
    {
        throw std::out_of_range("Index out of bounds");
    }
    if(idx >= static_cast<int>(m_size))
    {
        throw std::out_of_range("Index out of bounds");
    }
    m_arr[idx] = value;
}

