#ifndef MINSC_IS_DERIVED_H_
#define MINSC_IS_DERIVED_H_

#define IS_DERIVED(T, value) \
    _Generic((&(value)->base), T * : true, const T* : true, default : false)

#endif  // MINSC_IS_DERIVED_H_
