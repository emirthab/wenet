#ifndef COMMON_H
#define COMMON_H

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/main_loop.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/error_macros.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/builtin_types.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <chrono>
#include <ctime>
#include <iostream>
#include <unistd.h>

#define SCENE_TREE() Object::cast_to<SceneTree>(Engine::get_singleton()->get_main_loop())
#define SCENE_ROOT() (SCENE_TREE()->get_root())
#define NAMEOF(s) #s
#define GET_TIME() \
    ([]() { \
        auto now = std::chrono::system_clock::now(); \
        auto duration = now.time_since_epoch(); \
        long long millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count(); \
        return millis; \
    })()

#define GENERATE_UID(len) \
    ([]() { \
    srand((unsigned)time(NULL) * getpid()); \
    static const char alphanum[] = \
        "0123456789" \
        "abcdefghijklmnopqrstuvwxyz"; \
    std::string tmp_s; \
    tmp_s.reserve(len); \
    for (int i = 0; i < len; ++i) { \
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)]; \
    }\
    return tmp_s; \
    })()

#endif