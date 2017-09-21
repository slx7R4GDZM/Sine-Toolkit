// Copyright slx7R4GDZM
// Distributed under the terms of the MIT License.
// Refer to the License.txt file for details.

#pragma once

#include "../Other/Common-Things.h"

class Vector_Generator;

void draw_string(const string message, const Global_Scale scale, const u8 x, const u8 y, Vector_Generator& vector_generator, RenderWindow& window, const bool brighten = false);
void draw_character(const u8 character, Vector_Generator& vector_generator, RenderWindow& window);
void draw_digit(const u8 digit, Vector_Generator& vector_generator, RenderWindow& window);
void draw_number(const int number, Vector_Generator& vector_generator, RenderWindow& window);
void set_position_and_size(const u8 cur_x, const u8 cur_y, const Global_Scale scale, Vector_Generator& vector_generator, RenderWindow& window);
void draw_options(const string options_text[], Vector_Generator& vector_generator, RenderWindow& window);
void draw_rotating_vector(const vector<u16> vector_table[], const u8 fast_timer, Vector_Generator& vector_generator, RenderWindow& window);
