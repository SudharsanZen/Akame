#pragma once
#include<iostream>
#include<vector>
template<typename _vec>
class joined_vector
{
    _vec& m_vec1;
    _vec& m_vec2;

public:

    struct Iterator
    {
        typedef typename _vec::iterator::value_type type_value;
        typedef typename _vec::iterator::value_type* pointer;
        typedef typename _vec::iterator::value_type& reference;
        typedef std::forward_iterator_tag iterator_category;
        typedef std::ptrdiff_t difference_type;
        _vec* m_vec1;
        _vec* m_vec2;
        Iterator(pointer ptr) :m_ptr(ptr)
        {

        }
        Iterator operator++()
        {
            if (m_vec1->size() > 0 && m_ptr == &(*m_vec1)[m_vec1->size() - 1] && m_vec2->size() != 0)
                m_ptr = &(*m_vec2)[0];
            else
                ++m_ptr;
            return m_ptr;
        }
        Iterator operator++(int)
        {
            pointer curr = m_ptr;
            if (m_vec1->size() > 0 && m_ptr == &(*m_vec1)[m_vec1->size() - 1] && m_vec2->size() != 0)
                m_ptr = &(*m_vec2)[0];
            else
                ++m_ptr;
            return curr;
        }
        reference operator *()const
        {
            return *m_ptr;
        }
        pointer operator ->()const
        {
            return m_ptr;
        }

        friend bool operator == (Iterator& itr1, Iterator& itr2) 
        {
            return itr1.m_ptr == itr2.m_ptr;
        }
        friend bool operator != (Iterator& itr1, Iterator& itr2) 
        {
            return itr1.m_ptr != itr2.m_ptr;
        }
    private:
        pointer m_ptr;
    };

    joined_vector(_vec& vec1, _vec& vec2) :m_vec1(vec1), m_vec2(vec2)
    {
    }

    Iterator begin() const
    {
        //checkes if m_vec1 is empty and gets the first elemet's address,
        //if it's empty then it get's the first address of the second vector m_vec2
        //if both of them are empty then nullptr is returned as the first pointer
        Iterator itr_beg((m_vec1.size() != 0) ? &m_vec1[0] : ((m_vec2.size() != 0) ? &m_vec2[0] : nullptr));
        itr_beg.m_vec1 = &m_vec1;
        itr_beg.m_vec2 = &m_vec2;
        return itr_beg;
    }
    Iterator end() const
    {
        //check if m_vec2 is empty and get the last address of that vector
        //if the second vector is empty then the m_vec1's vector/the first vector's last element's address is taken
        //if both of them are empty then a null pointer is returned as the end pointer
        typename _vec::value_type* p = ((m_vec2.size() != 0) ? &m_vec2[m_vec2.size() - 1] : ((m_vec1.size()) != 0 ? &m_vec1[m_vec1.size() - 1] : nullptr));
        Iterator itr_beg(p != nullptr ? p + 1 : nullptr);
        itr_beg.m_vec1 = &m_vec1;
        itr_beg.m_vec2 = &m_vec2;
        return itr_beg;
    }
    typename _vec::value_type& operator [] (const size_t i)
    {
        if (i < m_vec1.size())
            return m_vec1[i];
        else
            return m_vec2[i - m_vec1.size()];
    }
    typename const _vec::value_type& operator [] (const size_t i) const
    {
        if (i < m_vec1.size())
            return m_vec1[i];
        else
            return m_vec2[i - m_vec1.size()];
    }
    size_t size() const
    {
        return m_vec1.size() + m_vec2.size();
    }

};