#include <iostream>
//#include <initializer_list>
#include <iterator>
#include <list>

using namespace std;

template <class T>
class VECTOR {
    private:
        T* elements;
        int length;
        int place;

        void RESIZE() {
            if(length>place) {
                place *=2;
                T* newElements = new T[place];
                for(int i=0; i<length; i++)
                    newElements[i] = elements[i];
                elements = newElements;
            }
        }
    public:
        VECTOR() {
            elements = new T[1];
            place = 1;
            length = 0;
        };

        void PUSH_BACK(T newElement) {
            length++;
            RESIZE();
            elements[length-1] = newElement;
        }

        int SIZE() {
            return length;
        }

        T operator [](int index) {
            return elements[index];
        }

        /*vector& operator =(const vector& __x) {
            static_cast<_Base&>(*this) = __x;
            this->_M_invalidate_all();
            _M_update_guaranteed_capacity();
            return *this;
        }

        vector& operator =(vector&& __x) {
            clear();
            swap(__x);
            return *this;
        }

        vector& operator =(initializer_list<value_type> __l) {
            static_cast<_Base&>(*this) = __l;
            this->_M_invalidate_all();
            _M_update_guaranteed_capacity();
            return *this;
        }*/

        /*VECTOR& operator =(initializer_list<T> __l) {
            static_cast<_Base&>(*this) = __l;
            this->_M_invalidate_all();
            _M_update_guaranteed_capacity();
            return *this;
        }*/
};

void smieszek(initializer_list<int> l) {
    list<int>::iterator it = l.begin();
    list<int>::iterator itEnd = l.end();
    while(it!=itEnd) {
        it++;
        cout << it* << endl;
    }
    cout << endl;
}

int main() {

    VECTOR<int> test;

    for(int i=0; i<200; i++)
        test.PUSH_BACK(i);

    cout << "Index: " << test[5] << endl;

    return 0;
}
