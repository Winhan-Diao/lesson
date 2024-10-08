#pragma once
#include "linked_int.h"
#include "traits.h"
#include <iostream>

LinkedInt *&Create(LinkedInt *& toCreate, unsigned volume, int value = 0);

// Can handle both pointer to LinkedInt object and LinkedInt object
template <class _OfLinkedInt, typename = std::enable_if_t<std::is_same_v<remove_cv_ref_ptr_ptrref_t<_OfLinkedInt>, LinkedInt>>>  
unsigned NumNodes(_OfLinkedInt&& ofLinkedInt);

template <class _OfLinkedInt, typename = std::enable_if_t<std::is_same_v<remove_cv_ref_ptr_ptrref_t<_OfLinkedInt>, LinkedInt>>>  
void ShowList(_OfLinkedInt&& ofLinkedInt, std::ostream& os = std::cout);

void Insert(LinkedInt& linkedInt, unsigned position, int value);

void Append(LinkedInt& linkedInt, int value);

void FreeList(LinkedInt& linkedInt);

// ----------------------
//      definition
// ----------------------

template <class _OfLinkedInt, typename>  
unsigned NumNodes(_OfLinkedInt&& ofLinkedInt) {
    return toReference(ofLinkedInt).getSize();
}

template <class _OfLinkedInt, typename>  
void ShowList(_OfLinkedInt&& ofLinkedInt, std::ostream& os) {
    os << "NON-HEAD -> ";
    toReference(ofLinkedInt).consumerTraverse([&os](const LinkedInt& _linkedInt){
        os << _linkedInt.getValue();
        if (_linkedInt.getNext()) {
            os << " -> ";
        } else {
            os << " -> NULL";
        }
    });
}




