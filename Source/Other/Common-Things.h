// Copyright slx7R4GDZM
// Distributed under the terms of the MIT License.
// Refer to the License.txt file for details.

#pragma once

#include <array>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>

using namespace std::chrono;
using namespace std::chrono_literals;

using u8 = std::uint_fast8_t;
using u16 = std::uint_fast16_t;
using u32 = std::uint_fast32_t;
using u64 = std::uint_fast64_t;

using s8 = std::int_fast8_t;
using s16 = std::int_fast16_t;
using s32 = std::int_fast32_t;
using s64 = std::int_fast64_t;

using std::array;
using std::cerr;
using std::clog;
using std::extent;
using std::printf;
using std::string;
using std::to_string;
using std::vector;

using sf::RenderWindow;
using Kb = sf::Keyboard;

enum Button
{
    CONFIRM,
    CANCEL,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    OPTIONS,
    TOGGLE_FULLSCREEN,
    EXIT
};

enum Global_Scale
{
    MUL_1,
    MUL_2,
    MUL_4,
    MUL_8,
    MUL_16,
    MUL_32,
    MUL_64,
    MUL_128,
    DIV_256,
    DIV_128,
    DIV_64,
    DIV_32,
    DIV_16,
    DIV_8,
    DIV_4,
    DIV_2
};

enum Opcode
{
    VCTR_0,
    VCTR_1,
    VCTR_2,
    VCTR_3,
    VCTR_4,
    VCTR_5,
    VCTR_6,
    VCTR_7,
    VCTR_8,
    VCTR_9,
    LABS,
    HALT,
    JSRL,
    RTSL,
    JMPL,
    SVEC
};

enum Window_Mode
{
    WIN_NORMAL,
    WIN_BORDERLESS,
    WIN_FULLSCREEN
};

enum Inactive_Mode
{
    PAUSE,
    RUN_WITHOUT_INPUT,
    RUN_WITH_INPUT
};

enum Frame_Limiter_Mode
{
    SLEEPING,
    BUSY_WAITING
};

enum Mode
{
    VIEWING,
    MAIN_MENU,
    VECTOR_OBJECT_MENU,
    INSTRUCTION_MENU,
    SET_BRIGHTNESS,
    SET_GLOBAL_SCALE,
    EDIT_VCTR,
    EDIT_LABS,
    EDIT_SVEC,
    SET_STARTING_GLOBAL_SCALE,
    EDIT_TEXT,
    SHIP_MENU,
    EDIT_SHIP,
    THRUST_MENU,
    EDIT_THRUST,
    TOGGLE_VECTORS_MENU
};

struct Long_Vector
{
    u8 opcode;
    u8 brightness;
    s16 delta_x;
    s16 delta_y;
};

struct Load_Absolute
{
    Global_Scale global_scale;
    s16 x_position;
    s16 y_position;
};

struct Short_Vector
{
    u8 local_scale;
    u8 brightness;
    s16 delta_x;
    s16 delta_y;
};

struct Basic_Vector
{
    u8 brightness;
    s16 delta_x;
    s16 delta_y;
};

int add_1(const int base_val, const int max_val);
int sub_1(const int base_val, const int min_val);

void add_blank_instructions(vector<u16>& instructions, const int amount_to_add);
void remove_last_instruction(vector<u16>& instructions);
void remove_last_instructions(vector<u16>& instructions, const int amount_to_remove);
void clear_instructions(vector<u16>& instructions);

void calculate_VCTR(const Long_Vector lv, vector<u16>& instructions);
void calculate_LABS(const Load_Absolute la, vector<u16>& instructions);
void calculate_SVEC(const Short_Vector sv, vector<u16>& instructions);
