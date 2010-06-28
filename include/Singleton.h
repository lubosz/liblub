/*
 * Singleton.h
 *
 *  Created on: Jun 28, 2010
 *      Author: bmonkey
 */

#pragma once

template <typename T>
class Singleton {
public:

    typedef T object_type;

	static object_type& Instance()
	 {
	    static object_type singleton;
	    return singleton;
	  }

};


