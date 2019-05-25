// Copyright slx7R4GDZM
// Distributed under the terms of the MIT License.
// Refer to the License.txt file for details.

#pragma once

#include "../Other/Common-Things.h"

class Vector_Generator;

void draw_string(const string& message, Global_Scale scale, u8 x, u8 y, Vector_Generator& vector_generator, RenderWindow& window, bool brighten = false);
void draw_character(u8 character, Vector_Generator& vector_generator, RenderWindow& window);
void draw_digit(u8 digit, Vector_Generator& vector_generator, RenderWindow& window);
void set_position_and_size(u8 cur_x, u8 cur_y, Global_Scale scale, Vector_Generator& vector_generator, RenderWindow& window);
void draw_options(const string options_text[], Vector_Generator& vector_generator, RenderWindow& window);
void draw_rotating_vector(const vector<u16> vector_table[], u8 fast_timer, Vector_Generator& vector_generator, RenderWindow& window);
