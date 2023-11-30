
#pragma once
#include <iostream>
#include <cassert>
#include <initializer_list>
#include <algorithm>
#include <stdexcept>
#include <iterator>
#include "array_ptr.h"

struct ReserveProxyObject {
    size_t reserve;
    explicit ReserveProxyObject(size_t capacity) : reserve(capacity) {}
};


template <typename Type>
class SimpleVector {
public:
    using Iterator = Type*;
    using ConstIterator = const Type*;

    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///
    ///
    /// =======================================>>КОНСТРУКТОРЫ ПРИСВАИВАНИЕ ДЕСТРУКТОР
    ///
    /// 
    /// ////////////////////////////////////////////////////////////////////////////////////////////////////////

    SimpleVector() noexcept;

    // Создаёт вектор из size элементов, инициализированных значением по умолчанию
    explicit SimpleVector(size_t size);

    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type& value);

    // Создаёт вектор из std::initializer_list
    SimpleVector(std::initializer_list<Type> init);

    //Копирующий
    SimpleVector(const SimpleVector& other);
    //Перемещающий
    SimpleVector(SimpleVector&& other);

    //Присваивание
    SimpleVector& operator=(const SimpleVector& rhs);
    SimpleVector& operator=(SimpleVector&& rhs);

    //Резервирующий
    SimpleVector(ReserveProxyObject obj);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///
    ///
    /// =======================================>>СЛУЖЕБНЫЕ ПУБЛИЧНЫЕ ===>ИНФОРМАТОРЫ 
    ///
    /// 
    /// ////////////////////////////////////////////////////////////////////////////////////////////////////////

   // Возвращает количество элементов в массиве
    size_t GetSize() const noexcept {
        return now_;
    }

    // Возвращает вместимость массива
    size_t GetCapacity() const noexcept {
        return cap_;
    }

    // Сообщает, пустой ли массив
    bool IsEmpty() const noexcept {
        return (now_ == 0);
    }

    // Возвращает ссылку на элемент с индексом index
    Type& operator[](size_t index) noexcept {
        assert(index < now_);
        return main_vector_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    const Type& operator[](size_t index) const noexcept {
        assert(index < now_);
        return main_vector_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    Type& At(size_t index) {
        if (index >= now_) throw std::out_of_range("out of range");
        return main_vector_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    const Type& At(size_t index) const {
        if (index >= now_) throw std::out_of_range("out of range");
        return main_vector_[index];
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///
    ///
    /// =======================================>>СЛУЖЕБНЫЕ ПУБЛИЧНЫЕ ===>МАНИПУЛЯТОРЫ 
    ///
    /// 
    /// ////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept;
    // Изменяет размер массива.
    // При увеличении размера новые элементы получают значение по умолчанию для типа Type
    void Resize(size_t new_size);

    // Обменивает значение с другим вектором
    void swap(SimpleVector& other) noexcept;

    // Добавляет элемент в конец вектора
    // При нехватке места увеличивает вдвое вместимость вектора
    void PushBack(const Type& item);
    void PushBack(Type&& item);

    // Вставляет значение value в позицию pos.
    // Возвращает итератор на вставленное значение
    // Если перед вставкой значения вектор был заполнен полностью,
    // вместимость вектора должна увеличиться вдвое, а для вектора вместимостью 0 стать равной 1
    Iterator Insert(ConstIterator pos, const Type& value);
    Type* Insert(ConstIterator pos, Type&& value);

    // "Удаляет" последний элемент вектора. Вектор не должен быть пустым
    void PopBack() noexcept;

    // Удаляет элемент вектора в указанной позиции
    Iterator Erase(ConstIterator pos);

    //Резервирует место под n_c элементов
    void Reserve(size_t new_capacity);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///
    ///
    /// =======================================>>ИТЕРАТОРЫ
    ///
    /// 
    /// ////////////////////////////////////////////////////////////////////////////////////////////////////////

   // Возвращает итератор на начало массива
   // Для пустого массива может быть равен (или не равен) nullptr
    Iterator begin() noexcept {
        return main_vector_.Get();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator end() noexcept {
        return  main_vector_.Get() + now_;
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator begin() const noexcept {
        return  main_vector_.Get();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator end() const noexcept {
        return main_vector_.Get() + now_;
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cbegin() const noexcept {
        return  main_vector_.Get();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cend() const noexcept {
        return main_vector_.Get() + now_;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///
    ///
    /// =======================================>>ОПЕРАТОРЫ СРАВНЕНИЯ
    ///
    /// 
    /// ////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool operator==(const SimpleVector<Type>& rhs) {
        return  (std::equal(this->begin(), this->end(), rhs.begin()) && this->now_ == rhs.now_);
    }

    bool operator!=(const SimpleVector<Type>& rhs) {
        return  !(std::equal(this->begin(), this->end(), rhs.begin()) && this->now_ == rhs.now_);
    }

    bool operator<(const SimpleVector<Type>& rhs) {
        return (std::lexicographical_compare(this->cbegin(), this->cend(), rhs.cbegin(), rhs.cend()));
    }

    bool operator<=(const SimpleVector<Type>& rhs) {
        if (*this < rhs || *this == rhs)return true;
        return false;
    }

    bool operator>(const SimpleVector<Type>& rhs) {
        return (std::lexicographical_compare(this->cbegin(), this->cend(), rhs.cbegin(), rhs.cend()));
    }

    bool operator>=(const SimpleVector<Type>& rhs) {
        if (*this > rhs || *this == rhs)return true;
        return false;
    }


private:

    size_t now_ = 0;
    size_t cap_ = 0;
    ArrayPtr<Type> main_vector_;

    //Перевыделяет размер и копирует старый вектор в новый в случае нехватки cap_
    void RepeatPatternPushback();

    //Перевыделяет размер и копирует старый вектор в новый в случае нехватки cap_ и вставляет элемент
    template<typename Value>
    void RepeatPatternInsert(size_t elem_num, Iterator constcasted, Value& value);


};

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                      //
/////////////////////////////////КОНЕЦ КЛАССА/////////////////////////////////////////////////////////////
//                                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////////////////////

ReserveProxyObject Reserve(size_t capacity_to_reserve) {
    return ReserveProxyObject(capacity_to_reserve);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/// =======================================>>КОНСТРУКТОРЫ ПРИСВАИВАНИЕ ДЕСТРУКТОР
///
/// 
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename Type>
SimpleVector<Type>::SimpleVector() noexcept = default;

template <typename Type>
SimpleVector<Type>::SimpleVector(size_t size) : now_(size), cap_(size), main_vector_(size)  {
    std::fill(begin(), end(), 0);
}

template <typename Type>
SimpleVector<Type>::SimpleVector(size_t size, const Type& value) : now_(size), cap_(size), main_vector_(size) {
    std::fill(begin(), end(), value);
}

template <typename Type>
SimpleVector<Type>::SimpleVector(std::initializer_list<Type> init) : now_(init.size()), cap_(init.size()), main_vector_(init.size()) {
    std::move(init.begin(), init.end(), begin());
}



template <typename Type>
SimpleVector<Type>::SimpleVector(const SimpleVector& other) : now_(other.now_), cap_(other.cap_), main_vector_(now_) {
    std::copy(other.begin(), other.end(), begin());
}

template <typename Type>
SimpleVector<Type>& SimpleVector<Type>::operator=(const SimpleVector<Type>& rhs) {
    if (*this == rhs) return *this;
    SimpleVector<Type> temp(rhs);
    this->swap(temp);
    return *this;
}

template <typename Type>
SimpleVector<Type>::SimpleVector(ReserveProxyObject obj) {
    Reserve(obj.reserve);
}

template <typename Type>
SimpleVector<Type>::SimpleVector(SimpleVector&& other) : main_vector_(other.GetSize()) {
    if (this == &other) return;
    swap(other);
}

template <typename Type>
SimpleVector<Type>& SimpleVector<Type>::operator=(SimpleVector<Type>&& rhs) {
    if (*this == rhs) return *this;
    SimpleVector<Type> temp(rhs);
    this->swap(temp);
    return *this;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/// =======================================>>СЛУЖЕБНЫЕ ПУБЛИЧНЫЕ ===>МАНИПУЛЯТОРЫ 
///                                               РЕАЛИЗАЦИЯ
///
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////


template <typename Type>
void SimpleVector<Type>::Clear() noexcept {
    now_ = 0;
}

template <typename Type>
void SimpleVector<Type>::swap(SimpleVector& other) noexcept {
    main_vector_.swap(other.main_vector_);
    std::swap(this->cap_, other.cap_);
    std::swap(this->now_, other.now_);
}

template <typename Type>
void SimpleVector<Type>::PushBack(const Type& item) {

    if (now_ >= cap_) {
        RepeatPatternPushback();
    }
    main_vector_[now_] = item;
    ++now_;
}

template <typename Type>
void SimpleVector<Type>::PushBack(Type&& item) {
    if (now_ >= cap_) {
        RepeatPatternPushback();
    }
    main_vector_[now_] = std::move(item);
    ++now_;
}

template <typename Type>
Type* SimpleVector<Type>::Insert(ConstIterator pos, const Type& value) {
    assert(pos >= begin() && pos <= end());
    Iterator constcasted = const_cast<Iterator>(&*pos);
    size_t elem_num = std::distance(cbegin(), pos);
    if (now_ >= cap_) {
        RepeatPatternInsert(elem_num, constcasted, Type(value));
    }
    else {
        std::move(pos, cend(), constcasted + 1);
        main_vector_[elem_num] = value;
    }
    ++now_;
    return Iterator(main_vector_.Get() + elem_num);
}

template <typename Type>
Type* SimpleVector<Type>::Insert(ConstIterator pos, Type&& value) {
    assert(pos >= begin() && pos <= end());
    Iterator constcasted = const_cast<Iterator>(&*pos);
    size_t elem_num = std::distance(cbegin(), pos);

    if (now_ >= cap_) {
        RepeatPatternInsert(elem_num, constcasted, value);
    }
    else {
        std::move(constcasted, end(), constcasted + 1);
        main_vector_[elem_num] = std::move(value);
    }
    ++now_;
    return Iterator(main_vector_.Get() + elem_num);
}

template <typename Type>
void SimpleVector<Type>::PopBack() noexcept {
    if (now_ > 0) --now_;
}

template <typename Type>
Type* SimpleVector<Type>::Erase(const Type* pos) {
    assert(pos >= begin() && pos <= end());
    if (now_ > 0) {
        Iterator constcasted = const_cast<Iterator>(&*pos);
        std::move(constcasted + 1, end(), constcasted);
        --now_;
        return constcasted;
    }
    return nullptr;
}

template <typename Type>
void SimpleVector<Type>::Reserve(size_t new_capacity) {
    if (new_capacity > this->cap_) {
        ArrayPtr<Type> ptr(new_capacity);
        std::move(begin(), end(), ptr.Get());
        main_vector_.swap(ptr);
        this->cap_ = new_capacity;
    }
}

template <typename Type>
void SimpleVector<Type>::Resize(size_t new_size) {
    if (new_size < now_) {
        now_ = new_size;
    }

    else {
        //Type* dynamic_arr = new Type[new_size];
        ArrayPtr<Type> ptr(new_size);
        for (size_t i = 0; i < new_size; ++i) {
            ptr[i] = std::move(Type());
        }
        std::move(begin(), end(), ptr.Get());
        main_vector_.swap(ptr);
        now_ = new_size;
        cap_ = new_size;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
///
///
/// =======================================>>СЛУЖЕБНЫЕ ПРИВАТНЫЕ ===>МАНИПУЛЯТОРЫ 
///                                               РЕАЛИЗАЦИЯ
///
/// 
/// ////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename Type>
template<typename Value>
void SimpleVector<Type>::RepeatPatternInsert(size_t elem_num, Type* constcasted, Value& value) {

    if (cap_ > 0)cap_ *= 2;
    else cap_ = 1;
    ArrayPtr<Type> ptr(cap_);
    std::move(begin(), constcasted, ptr.Get());
    ptr[elem_num] = std::move(value);
    std::move(constcasted, end(), ptr.Get() + elem_num + 1);
    main_vector_.swap(ptr);
}


template<typename Type>
void SimpleVector<Type>::RepeatPatternPushback() {
    if (cap_ > 0)cap_ *= 2;
    else cap_ = 1;
    ArrayPtr<Type> ptr(cap_);
    std::move(begin(), end(), ptr.Get());
    main_vector_.swap(ptr);
}

