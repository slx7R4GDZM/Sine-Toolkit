// Copyright slx7R4GDZM
// Distributed under the terms of the MIT License.
// Refer to the License.txt file for details.

#pragma once

#include "Text-Editor.h"
#include "../Input/Input-Handler.h"
#include "../Other/Constants.h"
#include "../Settings/Settings-Handler.h"

class Vector_Generator;

struct Vector_Toggle
{
    u8 box;
    bool object;
    u8 ship_and_thrust;
};

enum class Main_Menu
{
    EDIT_VECTOR_OBJECT,
    EDIT_TEXT,
    EDIT_SHIP,
    EDIT_SHIP_THRUST,
    TOGGLE_VECTORS
};

enum class Vector_Object_Menu
{
    ADD_INSTRUCTION,
    REMOVE_INSTRUCTION,
    SET_GLOBAL_SCALE,
    OUTPUT_VECTOR_OBJECT,
    CLEAR_VECTOR_OBJECT
};

enum class Instruction_Menu
{
    DRAW_LONG_VECTOR,
    LOAD_ABSOLUTE,
    DRAW_SHORT_VECTOR
};

enum class Rotating_Vector_Menu
{
    ADD_INSTRUCTION,
    REMOVE_INSTRUCTION,
    OUTPUT_VECTOR_OBJECT,
    CLEAR_VECTOR_OBJECT
};

enum class Toggle_Vectors_Menu
{
    BOX,
    VECTOR_OBJECT,
    SHIP_AND_THRUST
};

class Program
{
private:
    Settings_Handler settings;
    RenderWindow window;
    Input_Handler input;
    Inactive_Mode game_activity;

    u8 fast_timer;
    Mode mode;
    vector<string> interface_text;
    s16 current_x;
    s16 current_y;
    Global_Scale global_scale;

    Main_Menu main_menu_choice;
    Vector_Object_Menu vector_object_menu_choice;
    Instruction_Menu instruction_menu_choice;
    Rotating_Vector_Menu ship_menu_choice;
    Rotating_Vector_Menu ship_thrust_menu_choice;
    Toggle_Vectors_Menu toggle_vectors_menu_choice;

    vector<u16> vo_instructions;
    Global_Scale start_global_scale;
    Long_Vector long_vector;
    Load_Absolute load_absolute;
    Short_Vector short_vector;

    Text_Editor text_editor;

    Basic_Vector ship_edit_vector;
    Basic_Vector thrust_edit_vector;
    vector<u16> current_edit_table[ROTATIONS];
    vector<u16> ship_table[ROTATIONS];
    vector<u16> ship_thrust_table[ROTATIONS];

    Vector_Toggle vector_toggle;

    void process_events(Vector_Generator& vector_generator, RenderWindow& window);
    void limit_FPS(const steady_clock::time_point start_time) const;

    void process_current_mode(Vector_Generator& vector_generator);

    void viewing_mode(Vector_Generator& vector_generator);

    void main_menu(Vector_Generator& vector_generator);

    void vector_object_menu(Vector_Generator& vector_generator);
    void instruction_menu(Vector_Generator& vector_generator);
    void choose_brightness(u8& brightness, Vector_Generator& vector_generator);
    void set_global_scale(Vector_Generator& vector_generator);
    void edit_VCTR(Vector_Generator& vector_generator);
    void edit_LABS(Vector_Generator& vector_generator);
    void edit_SVEC(Vector_Generator& vector_generator);
    void set_starting_global_scale(Vector_Generator& vector_generator);
    bool modify_global_scale(Global_Scale& scale, Vector_Generator& vector_generator);

    template <unsigned SIZE>
    void rotating_vector_menu(const Basic_Vector vector_to_rotate, vector<u16> vector_object_table[], Rotating_Vector_Menu& menu_choice, const array<string, SIZE>& choices, Vector_Generator& vector_generator);
    void rotating_vector_editor(Basic_Vector& vector_to_rotate, vector<u16> vector_object_table[], Vector_Generator& vector_generator);

    void toggle_vectors_menu(Vector_Generator& vector_generator);

    string get_mode_text() const;
    void make_UI(Vector_Generator& vector_generator);
    void draw_UI(Vector_Generator& vector_generator);
    void binary_instruction(const u8 words, vector<string>& interface_text) const;
    string local_scale_text(const u8 local_scale) const;
    static string global_scale_text(const Global_Scale scale);
    s16 get_scaled_delta(const s16 delta) const;
    void draw_copyright(Vector_Generator& vector_generator);
    template <unsigned SIZE, typename T>
    void process_menu(const array<string, SIZE>& menu_text, T& current_option, Vector_Generator& vector_generator);
    void draw_option_button(Vector_Generator& vector_generator);
    void draw_vector_object(Vector_Generator& vector_generator);
    void draw_ship_edit_stuff(Vector_Generator& vector_generator);

    void output_vector_object() const;
    void output_rotating_VO_offsets(const vector<u16> vector_object_table[]) const;
    void output_rotating_VO(const vector<u16> vector_object_table[]) const;
public:
    Program();
};
