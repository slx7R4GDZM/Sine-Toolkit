// Copyright slx7R4GDZM
// Distributed under the terms of the MIT License.
// Refer to the License.txt file for details.

#pragma once

#include "../Other/Constants.h"

class fstream;

class Settings_Handler
{
private:
    Kb::Key button_key[TOTAL_BUTTONS];

    // some of these can probably be made to be temporary
    // and done with after the window is created
    Window_Mode window_mode;
    u32 window_style;
    sf::Vector2u fallback_res;
    sf::Vector2u current_res;
    sf::Vector2i fallback_win_pos;
    Inactive_Mode inactive_mode;
    float crop_ratio;
    u8 samples_MSAA;
    sf::ContextSettings context_settings;
    float gamma_correction;
    bool enable_v_sync;
    Frame_Limiter_Mode frame_limiter_mode;

    void parse_file_settings(std::ifstream& input);
    void parse_buttons(const string& setting, const string& value);
    void parse_settings(const string& setting, const string& value);
    template <typename T>
    static T clamp_string_value(const string& setting, const string& value, const T min_v, const T max_v);
public:
    Settings_Handler();
    void create_window(RenderWindow& win);
    void create_window(const bool toggle_fullscreen, const bool reuse_pos, const sf::Vector2u new_res, RenderWindow& win);
    void output_settings() const;
    Kb::Key get_button_key(const Button button) const;
    sf::Vector2u get_resolution() const;
    Inactive_Mode get_inactive_mode() const;
    Frame_Limiter_Mode get_frame_limiter_mode() const;
    void get_settings(float& crop_ratio, u8 gamma_table[]) const;
};