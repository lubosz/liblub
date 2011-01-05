/*
 * Copyright © 2010 Lubosz Sarnecki
 * Singleton.h
 *
 *  Created on: Jun 28, 2010
 */

#pragma once

template<typename T>
class Singleton {
 public:

    typedef T object_type;

    static object_type& Instance() {
        static object_type singleton;
        return singleton;
    }
};


