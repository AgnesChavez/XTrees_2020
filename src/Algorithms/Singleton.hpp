/*
 * Copyright (c) 2012-2013 Agnes Chavez and Alessandro Saccoia
 * Written by Alessandro Saccoia, <alessandro.saccoia@gmail.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Sotware.
 */

 
#ifndef _SINGLETON_HPP
#define _SINGLETON_HPP
#include "ofConstants.h"

template <class T> 
class Singleton {
public:
	static T *instance() {
		static std::unique_ptr<T> instance_ = std::make_unique<T>();
		return instance_.get();
//		return getInstance();
    }
private:
	
//	static T * getInstance()
//	{
//
//	}
	
//    static T *instance_;
};

//template <class T> T* Singleton<T>::instance_ = 0;

#endif //_SINGLETON_HPP
