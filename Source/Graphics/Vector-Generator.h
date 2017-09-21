// Copyright slx7R4GDZM
// Distributed under the terms of the MIT License.
// Refer to the License.txt file for details.

#pragma once

#include "../Other/Common-Things.h"

class Settings_Handler;

class Vector_Generator
{
private:
    float res_scale;
    float x_offset;
    float y_offset;
    float line_thickness;

    sf::Vector2u resolution;
    float crop_ratio;
    u8 gamma_table[16];

    void draw_long_vector(const Opcode opcode, const vector<u16> vector_object, u16& iteration, const bool flip_x, const bool flip_y, RenderWindow& window);
    void load_absolute(const vector<u16> vector_object, u16& iteration);
    void draw_short_vector(const vector<u16> vector_object, u16& iteration, const bool flip_x, const bool flip_y, const bool brighten, RenderWindow& window);

    s16 apply_global_scale(const s16 delta) const;

    void draw_vector(const s16 raw_delta_x, const s16 raw_delta_y, const u8 brightness, const bool flip_x, const bool flip_y, RenderWindow& window);
    void draw_wide_line_segment(const float start_x, const float start_y, const s16 delta_x, const s16 delta_y, const sf::Color vector_color, RenderWindow& window) const;
    void draw_thin_line_segment(const float start_x, const float start_y, const s16 delta_x, const s16 delta_y, const sf::Color vector_color, RenderWindow& window) const;
public:
    s16 current_x;
    s16 current_y;
    Global_Scale global_scale;

    Vector_Generator(const Settings_Handler settings_handler);
    void set_resolution_scale(const sf::Vector2u resolution);
    void process(const vector<u16> vector_object, RenderWindow& window, u16 iteration = 0, const bool flip_x = false, const bool flip_y = false, const bool brighten = false);
};