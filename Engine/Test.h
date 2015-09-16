//
//  HNLuaTest.h
//  cocos2d_libs
//
//  Created by Eleven Chen on 14-8-5.
//
//

#ifndef __cocos2d_libs__HNLuaTest__
#define __cocos2d_libs__HNLuaTest__

#include <string>


class Test
{
public:

    static std::string helloMsg();

    static Test* create();

    bool init();
};

#endif /* defined(__cocos2d_libs__HNLuaTest__) */