#pragma once

// Array
template<typename Type, u64 N>
struct Array
{
    Type elements[N];
    u64 count = 0;

    Type& operator[](u64 idx)
    {
        ASSERT(idx < N, "Index out of bounds\n");
        return elements[idx];
    }

    // Adds an element to end of array and returns the index
    u64 add(Type element)
    {
        ASSERT(count < N, "Array full\n");
        elements[count] = element;
        return count++;
    }

    void remove_and_swap(u64 idx)
    {
        ASSERT(idx < count, "Index out of bounds\n");
        elements[idx] = elements[--count];
    }

    void clear()
    {
        count = 0;
    }

    b8 is_full()
    {
        return count == N;
    }

};


template<typename Type>
static void array_insert(Type *array, u32 size, Type element)
{
    for(i32 i = 0; i < size; i++)
    {
        if (array[i] == nullptr)
        {
            array[i] = element;
            break;
        }
    }
}

template<typename Type>
static Type *array_remove(Type *array, u32 size, Type element)
{
    Type *ret;
    for(i32 i = 0; i < size; i++)
    {
        if (array[i] == element)
        {
            ret = array[i];
            array[i] = nullptr;

            return ret;
        }
    }
}

template<typename Type>
static Type *array_remove(Type *array, u32 size, u32 pos)
{
    ASSERT(pos < size);
    ASSERT(array[pos] != nullptr);

    Type *ret = array[pos];
    array[pos] = nullptr;

    return ret;
}


// Linked list
template<typename Type>
static void linked_list_insert(Type *&first, Type *&last, Type *element, u32 pos)
{
    // Empty list
    if (first == nullptr && last == nullptr)
    {
        ASSERT(pos == 0);
        first = element;
        last = element;

        element->prev = nullptr;
        element->next = nullptr;

        return;
    }

    // Loop until we find the insertion position
    Type *itr = first;
    u32 i = 1;
    while(i != pos && itr != nullptr)
    {
        itr = itr->next;
        i++;
    }


    // Insertion
    if (pos == 0)
    {
        element->next = first;
        first->prev = element;
        first = element;
        element->prev = nullptr;
        return;
    }

    ASSERT(i == pos && itr != nullptr,"linked_list_insert() failed to insert! Insertion position invalid.\n");
    if (itr == last)
    {
        itr->next = element;
        element->prev = itr;
        last = element;
        element->next = nullptr;
    }
    else
    {
        element->next = itr->next;
        itr->next->prev = element;

        itr->next = element;
        element->prev = itr;
    }

}

template<typename Type>
static void linked_list_push_back(Type *&first, Type *&last, Type *element)
{
    // Empty list
    if (first == nullptr && last == nullptr)
    {
        first = element;
        last = element;

        element->prev = nullptr;
        element->next = nullptr;

        return;
    }

    // Insertion
    last->next = element;
    element->prev = last;
    last = element;
    element->next = nullptr;

}

template<typename Type>
static void linked_list_push_front(Type *&first, Type *&last, Type *element)
{
    // Empty list
    if (first == nullptr && last == nullptr)
    {
        first = element;
        last = element;

        element->prev = nullptr;
        element->next = nullptr;
        return;
    }

    // Insertion
    element->next = first;
    first->prev = element;
    first = element;
    element->prev = nullptr;

}

template<typename Type>
static Type *linked_list_pop_back(Type *&first, Type *&last)
{
    // Empty list
    if (first == nullptr && last == nullptr)
    {
        LOG_WARN("linked_list_pop_back() called on empty list\n");
        return nullptr;
    }

    Type* ret = last;

    // Pop
    last = ret->prev;
    ret->prev->next = nullptr;
    ret->prev = nullptr;

    return ret;
}

template<typename Type>
static Type *linked_list_pop_front(Type *&first, Type *&last)
{
    // Empty list
    if (first == nullptr && last == nullptr)
    {
        LOG_WARN("linked_list_pop_front() called on empty list\n");
        return nullptr;
    }

    Type *ret = first;

    // Pop
    first = ret->next;
    ret->next->prev = nullptr;
    ret->next = nullptr;

    return ret;
}


template<typename Type>
static Type *linked_list_remove(Type *&first, Type *&last, Type *element)
{
    // Empty list
    if (first == nullptr && last == nullptr)
    {
        LOG_WARN("linked_list_remove() called on empty list\n");
        return nullptr;
    }

    // Loop until we find element
    Type *itr = first;
    while (itr != element && itr != nullptr)
    {
        itr = itr->next;
    }

    ASSERT(itr != nullptr, "linked_list_remove() failed to find element!\n");

    // Removal
    if (itr == first)
    {
        first = itr->next;
        itr->next->prev = nullptr;
        itr->next = nullptr;
        itr->prev = nullptr;
    }
    else if (itr == last)
    {
        last = itr->prev;
        itr->prev->next = nullptr;
        itr->next = nullptr;
        itr->prev = nullptr;
    }
    else
    {
        itr->prev->next = itr->next;
        itr->next->prev = itr->prev;
        itr->next = nullptr;
        itr->prev = nullptr;
    }

    return itr;
}

template<typename Type>
static Type *linked_list_remove(Type *&first, Type *&last, u32 pos)
{
    // Empty list
    if (first == nullptr && last == nullptr)
    {
        LOG_WARN("linked_list_remove() called on empty list\n");
        return nullptr;
    }

    // Loop until we find the removal position
    Type *itr = first;
    u32 i = 0;
    while(i != pos && itr != nullptr)
    {
        itr = itr->next;
        i++;
    }

    // Removal
    if (itr == first)
    {
        first = itr->next;
        itr->next->prev = nullptr;
        itr->next = nullptr;
        itr->prev = nullptr;
        return itr;
    }
    ASSERT(i == pos && itr != nullptr,"linked_list_remove() failed to remove! Removal position invalid.\n");
    if (itr == last)
    {
        last = itr->prev;
        itr->prev->next = nullptr;
        itr->next = nullptr;
        itr->prev = nullptr;
    }
    else
    {
        itr->prev->next = itr->next;
        itr->next->prev = itr->prev;
        itr->next = nullptr;
        itr->prev = nullptr;
    }

    return itr;
}

template<typename Type>
static void linked_list_log_debug(Type *&first, Type *&last)
{
    if (first == nullptr && last == nullptr)
    {
        LOG_TRACE("Linked list empty\n");
        return;
    }

    Type *itr = first;
    while(itr != nullptr)
    {
        LOG_TRACE("%d[%d, %d] ", itr->id, itr->prev ? itr->prev->id : -1, itr->next ? itr->next->id : -1);
        itr = itr->next;
    }
    LOG_TRACE("\n");
}