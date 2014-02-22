#include <stdio.h>
#include <iostream>


using namespace std; 

namespace cs540 {
    
    struct OutOfRange {
        OutOfRange() {}
    };

    template <typename T, size_t Dim1, size_t... Dims> 
    class Array
    {
        static_assert(Dim1 != 0, "Dimension is not greater than 0.");
    public:
        Array<T, Dims...> children[Dim1];
        size_t size;
        static T ValueType;
        // FirstDimensionMajorIterator
        // LastDimensionMajorIterator

        Array() : size(Dim1) 
        { 
            for(int i = 0; i < Dim1; i++)
                children[i] = Array<T, Dims...>();
        }

        Array(const Array &a)
        { 
            size = Dim1;
            for (int i = 0; i < Dim1; i++)
                children[i] = a.children[i];
        }

        template <typename U> 
        Array(const Array<U, Dim1, Dims...> &a)
        {
            size = Dim1;
            for (int i = 0; i < Dim1; i++)
                children[i] = a.children[i];
        }

        Array &operator=(const Array &a)
        {
            if(this == &a)
                return *this;

            if(size != a.size)
                throw OutOfRange();

            for (int i = 0; i < Dim1; i++)
                children[i] = a.children[i];

            return *this;
        }

        template <typename U> 
        Array &operator=(const Array<U, Dim1, Dims...> &a)
        {
            if(this == (Array<T, Dim1, Dims...>*)&a)
                return *this;

            if(size != a.size)
                throw OutOfRange();

            for (int i = 0; i < a.size; i++)
                children[i] = a.children[i];

            return *this;
        }

        Array<T, Dims...>& operator[](size_t i)
        {
            if(i >= Dim1 || i < 0)
                throw OutOfRange();
            return children[i];
        }

        const Array<T, Dims...>& operator[](size_t i) const
        {
            if(i >= Dim1 || i < 0)
                throw OutOfRange();
            return children[i];
        }

        class FirstDimensionMajorIterator
        {
        public:
            size_t index;
            Array<T, Dims...>* array;

            FirstDimensionMajorIterator() : index(0) { }

            FirstDimensionMajorIterator(size_t i, Array<T, Dims...>* array) : index(i), array(array) { }

            FirstDimensionMajorIterator(const FirstDimensionMajorIterator &f) : index(f.index), array(f.array) { }

            FirstDimensionMajorIterator &operator=(const FirstDimensionMajorIterator &f) 
            {
                index = f.index; 
                array = f.array;
            }

            bool operator==(const FirstDimensionMajorIterator &f)
            {
                return (index == f.index) && (array == f.array);
            }

            bool operator!=(const FirstDimensionMajorIterator &f)
            {
                return (index != f.index) || (array != f.array);
            }

            FirstDimensionMajorIterator &operator++()
            {
                (*array).fmbegin()++;
                return *this;
            }

            FirstDimensionMajorIterator operator++(int)
            {
                FirstDimensionMajorIterator temp = *this;
                (*array).fmbegin()++;
                return temp;
            }

            T &operator*() const
            {
                return (*array)[index];
            }
        };

        class LastDimensionMajorIterator
        {
        public:
            size_t index;
            Array<T, Dim1>* array;

            LastDimensionMajorIterator() : index(0) { }

            LastDimensionMajorIterator(size_t i, Array<T, Dim1>* array) : index(i), array(array) { }

            LastDimensionMajorIterator(const LastDimensionMajorIterator &f) { }

            LastDimensionMajorIterator &operator=(const LastDimensionMajorIterator &f) 
            {
                index = f.index; 
                array = f.array;
            }

            bool operator==(const LastDimensionMajorIterator &f)
            {
                if (index != f.index) 
                    return false;
                for (size_t i=0; i < Dim1; i++) 
                    if ((*array)[i] != (*f.array)[i])
                        return false;
                return true;
            }

            bool operator!=(const LastDimensionMajorIterator &f)
            {
                return (index != f.index) || (array != f.array);
            }

            LastDimensionMajorIterator &operator++()
            {
                index = (index + 1) % Dim1;
                    if (index == 0)
                        for (int i = 0; i < Dim1; i++)
                            (*array)[i]++;               
                return *this;
            }

            LastDimensionMajorIterator operator++(int)
            {
                LastDimensionMajorIterator temp = *this;
                index++;
                return temp;
            }

            T &operator*() const
            {
                return array[index].operator*();
            }
        };

        FirstDimensionMajorIterator fmbegin()
        {
            return FirstDimensionMajorIterator(0, children);
        }   

        FirstDimensionMajorIterator fmend()
        {
            return FirstDimensionMajorIterator(Dim1, children);
        }

        LastDimensionMajorIterator lmbegin()
        {
            return LastDimensionMajorIterator(0, &children);
        }

        LastDimensionMajorIterator lmend()
        {
            return LastDimensionMajorIterator(Dim1, &children);
        }
    };

    // "Base Case"
    template <typename T, size_t Dim> 
    class Array <T, Dim>
    {
        static_assert(Dim != 0, "Dimension is not greater than 0.");
    private:
        T data[Dim];

    public:
        size_t size;
        static T ValueType;
        // FirstDimensionMajorIterator
        // LastDimensionMajorIterator

        Array()
        {
            size = Dim;
        }

        Array(const Array &a)
        {
            size = Dim;

            for(int i = 0; i < Dim; i++)
                data[i] = a[i];
        }

        template <typename U> 
        Array(const Array<U, Dim> &a)
        {
            size = Dim;

            for(int i = 0; i < Dim; i++)
                data[i] = a[i];
        }

        Array &operator=(const Array &a)
        {
            if(size != a.size)
                throw OutOfRange();
            
            if(this == &a)
                return *this;

            for(int i = 0; i < size; i++)
                data[i] = a[i];

            return *this;
        }

        template <typename U> 
        Array &operator=(const Array<U, Dim> &a)
        {
            if(size != a.size)
                throw OutOfRange();
            
            if(this == (Array<T, Dim>*)&a)
                return *this;
            
            for(int i = 0; i < size; i++)
                data[i] = a[i];

            return *this;
        }

        T& operator[](size_t i) {
            if (i >= Dim || i < 0)
                throw OutOfRange();
            return data[i];
        }
        
        const T& operator[](size_t i) const 
        {
            if (i >= Dim || i < 0)
                throw OutOfRange();
            return data[i];
        }

        class FirstDimensionMajorIterator
        {
        public:
            size_t index;
            T* array;

            FirstDimensionMajorIterator() : index(0), array(nullptr) { }

            FirstDimensionMajorIterator(size_t i, T* array) : index(i), array(array) { }

            FirstDimensionMajorIterator(const FirstDimensionMajorIterator &f) : index(f.index), array(f.array) { }

            FirstDimensionMajorIterator &operator=(const FirstDimensionMajorIterator &f) 
            {
                index = f.index; 
                array = f.array;
            }

            bool operator==(const FirstDimensionMajorIterator &f)
            {
                return (index == f.index) && (array == f.array);
            }

            bool operator!=(const FirstDimensionMajorIterator &f)
            {
                return (index != f.index) || (array != f.array);
            }

            FirstDimensionMajorIterator &operator++()
            {
                index++;
                return *this;
            }

            FirstDimensionMajorIterator operator++(int)
            {
                FirstDimensionMajorIterator temp = *this;
                index++;
                return temp;
            }

            T &operator*() const
            {
                return array[index];
            }
        };

        class LastDimensionMajorIterator
        {
        public:
            size_t index;
            T* array;

            LastDimensionMajorIterator() : index(0), array(nullptr) { }

            LastDimensionMajorIterator(size_t i, T* array) : index(i), array(array) { }

            LastDimensionMajorIterator(const LastDimensionMajorIterator &f) : index(f.index), array(f.array) { }

            LastDimensionMajorIterator &operator=(const LastDimensionMajorIterator &f) 
            {
                index = f.index;
                array = f.array;
            }

            bool operator==(const LastDimensionMajorIterator &f)
            {
                return (index == f.index) && (array == f.array);
            }

            bool operator!=(const LastDimensionMajorIterator &f)
            {
                return (index != f.index) || (array != f.array);
            }

            LastDimensionMajorIterator &operator++()
            {
                index++;
                return *this;
            }

            LastDimensionMajorIterator operator++(int)
            {
                LastDimensionMajorIterator temp = *this;
                index++;
                return temp;
            }

            T &operator*() const
            {
                return array[index];
            }
        };

        FirstDimensionMajorIterator fmbegin()
        {
            return FirstDimensionMajorIterator(0, data);
        }   

        FirstDimensionMajorIterator fmend()
        {
            return FirstDimensionMajorIterator(size, data);
        }

        LastDimensionMajorIterator lmbegin()
        {
            return LastDimensionMajorIterator(0, data);
        }

        LastDimensionMajorIterator lmend()
        {
            return LastDimensionMajorIterator(size, data);
        }
    };
    
}