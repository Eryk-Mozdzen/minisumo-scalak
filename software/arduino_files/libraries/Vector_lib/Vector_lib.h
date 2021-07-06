// Vector_lib.h
// Eryk Mozdzen 2019

#ifndef Vector_lib_h
#define Vector_lib_h

#include "Arduino.h"

template <class T>
class Vector {
    private:
        T* elements;
        int length;
        int place;

        void resize() {
            if(length>place) {
                place *=2;
                T* newElements = new T[place];
                for(int i=0; i<length; i++)
                    newElements[i] = elements[i];
                elements = newElements;
            }
        }
    public:
        Vector() {
            elements = new T[1];
            place = 1;
            length = 0;
        }

        void push_back(T newElement) {
            length++;
            resize();
            elements[length-1] = newElement;
        }

        int size() {
            return length;
        }

		clear() {
			elements = new T[1];
            place = 1;
            length = 0;
		}

        T operator [](int index) {
            return elements[index];
        }
};

#endif
