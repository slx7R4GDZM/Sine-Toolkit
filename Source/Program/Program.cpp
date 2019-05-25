// Copyright slx7R4GDZM
// Distributed under the terms of the MIT License.
// Refer to the License.txt file for details.

#include "Program.h"

#include <thread>
#include <SFML/Graphics.hpp>
#include "Ship-Creator.h"
#include "../Graphics/Graphics-Handler.h"
#include "../Graphics/Vector-Generator.h"
#include "../Other/Text.h"
#include "../Other/Vectors.h"

static string global_scale_text(Global_Scale scale);

Program::Program()
    : game_activity(RUN_WITH_INPUT)
    , fast_timer(0)
    , mode(MAIN_MENU)
    , main_menu_choice(Main_Menu::EDIT_VECTOR_OBJECT)
    , vector_object_menu_choice(Vector_Object_Menu::ADD_INSTRUCTION)
    , instruction_menu_choice(Instruction_Menu::DRAW_LONG_VECTOR)
    , ship_menu_choice(Rotating_Vector_Menu::ADD_INSTRUCTION)
    , ship_thrust_menu_choice(Rotating_Vector_Menu::ADD_INSTRUCTION)
    , toggle_vectors_menu_choice(Toggle_Vectors_Menu::BOX)
    , vo_instructions{0xD000}
    , start_global_scale(MUL_1)
    , long_vector{VCTR_9, 7}
    , load_absolute{MUL_1, 512, 512}
    , short_vector{0, 7}
    , ship_edit_vector{12}
    , thrust_edit_vector{12}
    , vector_toggle{1, 1, 2}
{
    for (int i = 0; i < ROTATIONS; i++)
    {
        current_edit_table[i] = {0xD000};
        ship_table[i] = {0xD000};
        ship_thrust_table[i] = {0xD000};
    }

    settings.create_startup_window(window);
    settings.output_settings();

    Vector_Generator vector_generator(settings);
    make_UI(vector_generator);

    while (window.isOpen())
    {
        const steady_clock::time_point start_time = steady_clock::now();
        process_events(vector_generator, window);
        if (game_activity != PAUSE)
        {
            window.clear();

            if (game_activity == RUN_WITH_INPUT)
            {
                input.update(settings);
                if (input.on_press(TOGGLE_FULLSCREEN))
                {
                    settings.toggle_fullscreen(window);
                    vector_generator.set_resolution_scale(settings);
                }
                else if (input.on_press(EXIT))
                    window.close();
            }

            process_current_mode(vector_generator);
            draw_vector_object(vector_generator);
            draw_ship_edit_stuff(vector_generator);

            if (input.any_pressed())
                make_UI(vector_generator);

            draw_UI(vector_generator);

            fast_timer++;

            window.display();
        }
        limit_FPS(start_time);
    }
}

void Program::process_events(Vector_Generator& vector_generator, RenderWindow& window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Resized)
        {
            settings.resize_window({event.size.width, event.size.height}, window);
            vector_generator.set_resolution_scale(settings);
        }
        else if (event.type == sf::Event::Closed)
            window.close();
        else if (settings.get_inactive_mode() == PAUSE)
        {
            if (event.type == sf::Event::LostFocus)
                game_activity = PAUSE;
            else if (event.type == sf::Event::GainedFocus)
                game_activity = RUN_WITH_INPUT;
        }
        else if (settings.get_inactive_mode() == RUN_WITHOUT_INPUT)
        {
            if (event.type == sf::Event::LostFocus)
            {
                game_activity = RUN_WITHOUT_INPUT;
                input.release_all_buttons();
            }
            else if (event.type == sf::Event::GainedFocus)
                game_activity = RUN_WITH_INPUT;
        }
    }
}

void Program::limit_FPS(steady_clock::time_point start_time) const
{
    if (settings.get_frame_limiter_mode() == SLEEPING)
    {
        const nanoseconds time_elapsed = steady_clock::now() - start_time;
        if (time_elapsed < TARGET_FRAME_TIME)
            std::this_thread::sleep_for(TARGET_FRAME_TIME - time_elapsed);
    }
    else
        while (steady_clock::now() - start_time < TARGET_FRAME_TIME);
}

void Program::process_current_mode(Vector_Generator& vector_generator)
{
    switch (mode)
    {
    case VIEWING:
        viewing_mode(vector_generator);
        break;
    case MAIN_MENU:
        main_menu(vector_generator);
        break;
    case VECTOR_OBJECT_MENU:
        vector_object_menu(vector_generator);
        break;
    case INSTRUCTION_MENU:
        instruction_menu(vector_generator);
        break;
    case SET_BRIGHTNESS:
        if (instruction_menu_choice == Instruction_Menu::DRAW_LONG_VECTOR)
            choose_brightness(long_vector.brightness, vector_generator);
        else
            choose_brightness(short_vector.brightness, vector_generator);
        break;
    case SET_GLOBAL_SCALE:
        set_global_scale(vector_generator);
        break;
    case EDIT_VCTR:
        edit_VCTR(vector_generator);
        break;
    case EDIT_LABS:
        edit_LABS(vector_generator);
        break;
    case EDIT_SVEC:
        edit_SVEC(vector_generator);
        break;
    case SET_STARTING_GLOBAL_SCALE:
        set_starting_global_scale(vector_generator);
        break;
    case EDIT_TEXT:
        text_editor.edit_text(mode, fast_timer, input, vector_generator, window);
        break;
    case SHIP_MENU:
        rotating_vector_menu(ship_edit_vector, ship_table, ship_menu_choice, SHIP_MENU_CHOICES, vector_generator);
        break;
    case EDIT_SHIP:
        rotating_vector_editor(ship_edit_vector, ship_table, vector_generator);
        break;
    case THRUST_MENU:
        rotating_vector_menu(thrust_edit_vector, ship_thrust_table, ship_thrust_menu_choice, THRUST_MENU_CHOICES, vector_generator);
        break;
    case EDIT_THRUST:
        rotating_vector_editor(thrust_edit_vector, ship_thrust_table, vector_generator);
        break;
    case TOGGLE_VECTORS_MENU:
        toggle_vectors_menu(vector_generator);
        break;
    }
}

void Program::viewing_mode(Vector_Generator& vector_generator)
{
    if (input.on_press(CONFIRM))
        mode = MAIN_MENU;
}

void Program::main_menu(Vector_Generator& vector_generator)
{
    process_menu(MAIN_MENU_CHOICES, main_menu_choice, vector_generator);
    if (input.on_press(CONFIRM))
    {
        switch (main_menu_choice)
        {
        case Main_Menu::EDIT_VECTOR_OBJECT:
            mode = VECTOR_OBJECT_MENU;
            break;
        case Main_Menu::EDIT_TEXT:
            mode = EDIT_TEXT;
            break;
        case Main_Menu::EDIT_SHIP:
            mode = SHIP_MENU;
            break;
        case Main_Menu::EDIT_SHIP_THRUST:
            mode = THRUST_MENU;
            break;
        case Main_Menu::TOGGLE_VECTORS:
            mode = TOGGLE_VECTORS_MENU;
            break;
        }
    }
    else if (input.on_press(CANCEL))
        mode = VIEWING;
}

void Program::vector_object_menu(Vector_Generator& vector_generator)
{
    process_menu(VECTOR_OBJECT_MENU_CHOICES, vector_object_menu_choice, vector_generator);
    if (input.on_press(CONFIRM))
    {
        switch (vector_object_menu_choice)
        {
        case Vector_Object_Menu::ADD_INSTRUCTION:
            mode = INSTRUCTION_MENU;
            break;
        case Vector_Object_Menu::REMOVE_INSTRUCTION:
            remove_last_instruction(vo_instructions);
            break;
        case Vector_Object_Menu::SET_GLOBAL_SCALE:
            mode = SET_STARTING_GLOBAL_SCALE;
            break;
        case Vector_Object_Menu::OUTPUT_VECTOR_OBJECT:
            output_vector_object();
            break;
        case Vector_Object_Menu::CLEAR_VECTOR_OBJECT:
            clear_instructions(vo_instructions);
            break;
        }
    }
    else if (input.on_press(CANCEL))
        mode = MAIN_MENU;
}

void Program::instruction_menu(Vector_Generator& vector_generator)
{
    process_menu(INSTRUCTION_MENU_CHOICES, instruction_menu_choice, vector_generator);
    if (input.on_press(CONFIRM))
    {
        switch (instruction_menu_choice)
        {
        case Instruction_Menu::DRAW_LONG_VECTOR:
            if (vo_instructions.size() + 2 <= MAX_VO_INSTRUCTIONS)
            {
                mode = SET_BRIGHTNESS;
                add_blank_instructions(vo_instructions, 2);
                calculate_VCTR(long_vector, vo_instructions);
            }
            break;
        case Instruction_Menu::LOAD_ABSOLUTE:
            if (vo_instructions.size() + 2 <= MAX_VO_INSTRUCTIONS)
            {
                mode = SET_GLOBAL_SCALE;
                add_blank_instructions(vo_instructions, 2);
                calculate_LABS(load_absolute, vo_instructions);
            }
            break;
        case Instruction_Menu::DRAW_SHORT_VECTOR:
            if (vo_instructions.size() + 1 <= MAX_VO_INSTRUCTIONS)
            {
                mode = SET_BRIGHTNESS;
                add_blank_instructions(vo_instructions, 1);
                calculate_SVEC(short_vector, vo_instructions);
            }
            break;
        }
    }
    else if (input.on_press(CANCEL))
        mode = VECTOR_OBJECT_MENU;
}

void Program::choose_brightness(u8& brightness, Vector_Generator& vector_generator)
{
    bool brightness_changed = false;
    if (input.is_pressed(OPTIONS))
    {
        draw_options(BRIGHTNESS_OPTIONS, vector_generator, window);
        if (input.movement_on_press())
        {
            brightness_changed = true;
            if (input.on_press(UP))
                brightness = 15;
            else if (input.on_press(DOWN))
                brightness = 12;
            else if (input.on_press(LEFT))
                brightness = 7;
            else
                brightness = 0;
        }
    }
    else
    {
        draw_option_button(vector_generator);
        if (input.on_press(UP) || input.on_press(RIGHT))
        {
            brightness_changed = true;
            brightness = (brightness < 15 ? add_1(brightness, 15) : 0);
        }
        else if (input.on_press(DOWN) || input.on_press(LEFT))
        {
            brightness_changed = true;
            brightness = (brightness > 0 ? sub_1(brightness, 0) : 15);
        }
    }
    draw_string("BRIGHTNESS ", MUL_2, 99, 180, vector_generator, window);
    draw_string(to_string(brightness), MUL_2, 99, 170, vector_generator, window);

    if (instruction_menu_choice == Instruction_Menu::DRAW_LONG_VECTOR)
    {
        if (brightness_changed)
            calculate_VCTR(long_vector, vo_instructions);
        if (input.on_press(CONFIRM))
            mode = EDIT_VCTR;
        else if (input.on_press(CANCEL))
        {
            mode = INSTRUCTION_MENU;
            remove_last_instructions(vo_instructions, 2);
        }
    }
    else
    {
        if (brightness_changed)
            calculate_SVEC(short_vector, vo_instructions);
        if (input.on_press(CONFIRM))
            mode = EDIT_SVEC;
        else if (input.on_press(CANCEL))
        {
            mode = INSTRUCTION_MENU;
            remove_last_instructions(vo_instructions, 1);
        }
    }
}

void Program::set_global_scale(Vector_Generator& vector_generator)
{
    const bool global_scale_changed = modify_global_scale(load_absolute.global_scale, vector_generator);

    draw_string("GLOBAL SCALE ", MUL_2, 93, 180, vector_generator, window);
    draw_string(global_scale_text(load_absolute.global_scale), MUL_2, 93, 170, vector_generator, window);

    if (global_scale_changed)
        calculate_LABS(load_absolute, vo_instructions);
    if (input.on_press(CONFIRM))
        mode = EDIT_LABS;
    else if (input.on_press(CANCEL))
    {
        mode = INSTRUCTION_MENU;
        remove_last_instructions(vo_instructions, 2);
    }
}

void Program::edit_VCTR(Vector_Generator& vector_generator)
{
    if (input.is_pressed(OPTIONS))
    {
        draw_options(VCTR_OPTIONS, vector_generator, window);
        if (input.movement_on_press())
        {
            if (input.on_press(UP))
                long_vector.opcode = add_1(long_vector.opcode, 9);
            else if (input.on_press(DOWN))
                long_vector.opcode = sub_1(long_vector.opcode, 0);
            else if (input.on_press(LEFT))
                long_vector.opcode = 0;
            else
                long_vector.opcode = 9;

            calculate_VCTR(long_vector, vo_instructions);
        }
    }
    else
    {
        draw_option_button(vector_generator);
        if (input.movement_is_pressed())
        {
            if (input.is_pressed(UP))
                long_vector.delta_y = add_1(long_vector.delta_y, 1023);
            if (input.is_pressed(DOWN))
                long_vector.delta_y = sub_1(long_vector.delta_y, -1023);
            if (input.is_pressed(LEFT))
                long_vector.delta_x = sub_1(long_vector.delta_x, -1023);
            if (input.is_pressed(RIGHT))
                long_vector.delta_x = add_1(long_vector.delta_x, 1023);

            calculate_VCTR(long_vector, vo_instructions);
        }
        if (input.on_press(CONFIRM))
        {
            mode = VECTOR_OBJECT_MENU;
            long_vector.delta_x = 0;
            long_vector.delta_y = 0;
        }
        else if (input.on_press(CANCEL))
            mode = SET_BRIGHTNESS;
    }
}

void Program::edit_LABS(Vector_Generator& vector_generator)
{
    if (input.is_pressed(OPTIONS))
    {
        draw_options(LABS_OPTIONS, vector_generator, window);
        if (input.movement_on_press())
        {
            if (input.on_press(UP))
                load_absolute.global_scale = static_cast<Global_Scale>(add_1(load_absolute.global_scale, DIV_2));
            else if (input.on_press(DOWN))
                load_absolute.global_scale = static_cast<Global_Scale>(sub_1(load_absolute.global_scale, MUL_1));
            else if (input.on_press(LEFT))
                load_absolute.global_scale = MUL_1;
            else
                load_absolute.global_scale = DIV_2;

            calculate_LABS(load_absolute, vo_instructions);
        }
    }
    else
    {
        draw_option_button(vector_generator);
        if (input.movement_is_pressed())
        {
            if (input.is_pressed(UP))
                load_absolute.y_position = static_cast<Global_Scale>(add_1(load_absolute.y_position, 1023));
            if (input.is_pressed(DOWN))
                load_absolute.y_position = static_cast<Global_Scale>(sub_1(load_absolute.y_position, -1023));
            if (input.is_pressed(LEFT))
                load_absolute.x_position = static_cast<Global_Scale>(sub_1(load_absolute.x_position, -1023));
            if (input.is_pressed(RIGHT))
                load_absolute.x_position = static_cast<Global_Scale>(add_1(load_absolute.x_position, 1023));

            calculate_LABS(load_absolute, vo_instructions);
        }
        if (input.on_press(CONFIRM))
        {
            mode = VECTOR_OBJECT_MENU;
            load_absolute.x_position = 512;
            load_absolute.y_position = 512;
        }
        else if (input.on_press(CANCEL))
            mode = SET_GLOBAL_SCALE;
    }
}

void Program::edit_SVEC(Vector_Generator& vector_generator)
{
    if (input.is_pressed(OPTIONS))
    {
        draw_options(SVEC_OPTIONS, vector_generator, window);
        if (input.movement_on_press())
        {
            if (input.on_press(UP))
                short_vector.local_scale = add_1(short_vector.local_scale, 3);
            else if (input.on_press(DOWN))
                short_vector.local_scale = sub_1(short_vector.local_scale, 0);
            else if (input.on_press(LEFT))
                short_vector.local_scale = 0;
            else
                short_vector.local_scale = 3;

            calculate_SVEC(short_vector, vo_instructions);
        }
    }
    else
    {
        draw_option_button(vector_generator);
        if (input.movement_on_press())
        {
            if (input.on_press(UP))
                short_vector.delta_y = add_1(short_vector.delta_y, 3);
            if (input.on_press(DOWN))
                short_vector.delta_y = sub_1(short_vector.delta_y, -3);
            if (input.on_press(LEFT))
                short_vector.delta_x = sub_1(short_vector.delta_x, -3);
            if (input.on_press(RIGHT))
                short_vector.delta_x = add_1(short_vector.delta_x, 3);

            calculate_SVEC(short_vector, vo_instructions);
        }
        if (input.on_press(CONFIRM))
        {
            mode = VECTOR_OBJECT_MENU;
            short_vector.delta_x = 0;
            short_vector.delta_y = 0;
        }
        else if (input.on_press(CANCEL))
            mode = SET_BRIGHTNESS;
    }
}

void Program::set_starting_global_scale(Vector_Generator& vector_generator)
{
    modify_global_scale(start_global_scale, vector_generator);
    draw_string("STARTING GLOBAL SCALE ", MUL_2, 66, 180, vector_generator, window);
    draw_string(global_scale_text(start_global_scale), MUL_2, 66, 170, vector_generator, window);
    if (input.on_press(CONFIRM) || input.on_press(CANCEL))
        mode = VECTOR_OBJECT_MENU;
}

bool Program::modify_global_scale(Global_Scale& scale, Vector_Generator& vector_generator)
{
    bool global_scale_changed = false;
    if (input.is_pressed(OPTIONS))
    {
        draw_options(GLOBAL_SCALE_OPTIONS, vector_generator, window);
        if (input.movement_on_press())
        {
            global_scale_changed = true;
            if (input.on_press(UP))
                scale = MUL_2;
            else if (input.on_press(DOWN))
                scale = MUL_1;
            else if (input.on_press(LEFT))
                scale = DIV_2;
            else
                scale = DIV_4;
        }
    }
    else
    {
        draw_option_button(vector_generator);
        if (input.on_press(UP) || input.on_press(RIGHT))
        {
            global_scale_changed = true;
            if (scale < DIV_2)
                scale = static_cast<Global_Scale>(add_1(scale, DIV_2));
            else
                scale = MUL_1;
        }
        else if (input.on_press(DOWN) || input.on_press(LEFT))
        {
            global_scale_changed = true;
            if (scale > MUL_1)
                scale = static_cast<Global_Scale>(sub_1(scale, MUL_1));
            else
                scale = DIV_2;
        }
    }
    return global_scale_changed;
}

template <unsigned SIZE>
void Program::rotating_vector_menu(Basic_Vector vector_to_rotate, vector<u16> vector_object_table[], Rotating_Vector_Menu& menu_choice, const array<string, SIZE>& choices, Vector_Generator& vector_generator)
{
    process_menu(choices, menu_choice, vector_generator);
    if (input.on_press(CONFIRM))
    {
        switch (menu_choice)
        {
        case Rotating_Vector_Menu::ADD_INSTRUCTION:
        {
            bool add_vector = true;
            for (int i = 0; i < ROTATIONS && add_vector; i++)
            {
                if (vector_object_table[i].size() >= MAX_VO_INSTRUCTIONS)
                    add_vector = false;
            }
            if (add_vector)
            {
                Ship_Creator::rotate_vector(vector_to_rotate, current_edit_table);
                if (mode == SHIP_MENU)
                    mode = EDIT_SHIP;
                else
                    mode = EDIT_THRUST;
            }
            break;
        }
        case Rotating_Vector_Menu::REMOVE_INSTRUCTION:
            for (int i = 0; i < ROTATIONS; i++)
                remove_last_instruction(vector_object_table[i]);

            break;
        case Rotating_Vector_Menu::OUTPUT_VECTOR_OBJECT:
            output_rotating_VO_offsets(vector_object_table);
            output_rotating_VO(vector_object_table);
            if (main_menu_choice == Main_Menu::EDIT_SHIP)
                Ship_Creator::output_lives_icon(ship_table[ROTATIONS - 1]);

            break;
        case Rotating_Vector_Menu::CLEAR_VECTOR_OBJECT:
            for (int i = 0; i < ROTATIONS; i++)
                clear_instructions(vector_object_table[i]);

            break;
        }
    }
    else if (input.on_press(CANCEL))
        mode = MAIN_MENU;
}

void Program::rotating_vector_editor(Basic_Vector& vector_to_rotate, vector<u16> vector_object_table[], Vector_Generator& vector_generator)
{
    if (input.is_pressed(OPTIONS))
    {
        draw_options(SHIP_CREATOR_OPTIONS, vector_generator, window);
        if (input.movement_on_press())
        {
            if (input.on_press(UP))
                vector_to_rotate.brightness = add_1(vector_to_rotate.brightness, 15);
            else if (input.on_press(DOWN))
                vector_to_rotate.brightness = sub_1(vector_to_rotate.brightness, 0);
            else if (input.on_press(LEFT))
                vector_to_rotate.brightness = 0;
            else
                vector_to_rotate.brightness = 12;

            Ship_Creator::rotate_vector(vector_to_rotate, current_edit_table);
        }
    }
    else
    {
        draw_option_button(vector_generator);
        if (input.movement_on_press())
        {
            if (input.on_press(UP))
                vector_to_rotate.delta_y = add_1(vector_to_rotate.delta_y, DELTA_MAX);
            if (input.on_press(DOWN))
                vector_to_rotate.delta_y = sub_1(vector_to_rotate.delta_y, -DELTA_MAX);
            if (input.on_press(LEFT))
                vector_to_rotate.delta_x = sub_1(vector_to_rotate.delta_x, -DELTA_MAX);
            if (input.on_press(RIGHT))
                vector_to_rotate.delta_x = add_1(vector_to_rotate.delta_x, DELTA_MAX);

            Ship_Creator::rotate_vector(vector_to_rotate, current_edit_table);
        }

        if (input.on_press(CONFIRM))
        {
            bool add_vector = true;
            for (int i = 0; i < ROTATIONS && add_vector; i++)
            {
                if (vector_object_table[i].size() + current_edit_table[i].size() - 1 > MAX_VO_INSTRUCTIONS)
                    add_vector = false;
            }
            if (add_vector)
            {
                for (int i = 0; i < ROTATIONS; i++)
                {
                    vector_object_table[i].insert(vector_object_table[i].end() - 1, current_edit_table[i].begin(), current_edit_table[i].end() - 1);
                    clear_instructions(current_edit_table[i]);
                }
                vector_to_rotate.delta_x = 0;
                vector_to_rotate.delta_y = 0;
                if (mode == EDIT_SHIP)
                    mode = SHIP_MENU;
                else
                    mode = THRUST_MENU;
            }
        }
        else if (input.on_press(CANCEL))
        {
            for (int i = 0; i < ROTATIONS; i++)
                clear_instructions(current_edit_table[i]);

            vector_to_rotate.delta_x = 0;
            vector_to_rotate.delta_y = 0;
            if (mode == EDIT_SHIP)
                mode = SHIP_MENU;
            else
                mode = THRUST_MENU;
        }
    }
}

void Program::toggle_vectors_menu(Vector_Generator& vector_generator)
{
    process_menu(TOGGLE_VECTORS_MENU_CHOICES, toggle_vectors_menu_choice, vector_generator);

    set_position_and_size(232, 218, MUL_2, vector_generator, window);
    draw_digit(vector_toggle.box, vector_generator, window);
    set_position_and_size(232, 208, MUL_2, vector_generator, window);
    draw_digit(vector_toggle.object, vector_generator, window);
    set_position_and_size(232, 198, MUL_2, vector_generator, window);
    draw_digit(vector_toggle.ship_and_thrust, vector_generator, window);

    if (input.on_press(CONFIRM))
    {
        switch (toggle_vectors_menu_choice)
        {
        case Toggle_Vectors_Menu::BOX:
            if (++vector_toggle.box > 2)
                vector_toggle.box = 0;
            break;
        case Toggle_Vectors_Menu::VECTOR_OBJECT:
            vector_toggle.object = !vector_toggle.object;
            break;
        case Toggle_Vectors_Menu::SHIP_AND_THRUST:
            if (++vector_toggle.ship_and_thrust > 2)
                vector_toggle.ship_and_thrust = 0;
            break;
        }
    }
    else if (input.on_press(CANCEL))
        mode = MAIN_MENU;
}

string Program::get_mode_text() const
{
    switch (mode)
    {
    case VIEWING:
        return "VIEWING";
    case MAIN_MENU:
        return "MAIN MENU";
    case VECTOR_OBJECT_MENU:
        return "VECTOR OBJECT MENU";
    case INSTRUCTION_MENU:
        return "INSTRUCTION MENU";
    case SET_BRIGHTNESS:
        return "SET BRIGHTNESS";
    case SET_GLOBAL_SCALE:
        return "SET GLOBAL SCALE";
    case EDIT_VCTR:
        return "EDITING VCTR";
    case EDIT_LABS:
        return "EDITING LABS";
    case EDIT_SVEC:
        return "EDITING SVEC";
    case SET_STARTING_GLOBAL_SCALE:
        return "SET STARTING GLOBAL SCALE";
    case EDIT_TEXT:
        return "TEXT EDITOR";
    case SHIP_MENU:
        return "SHIP MENU";
    case EDIT_SHIP:
        return "SHIP EDITOR";
    case THRUST_MENU:
        return "THRUST MENU";
    case EDIT_THRUST:
        return "THRUST EDITOR";
    case TOGGLE_VECTORS_MENU:
        return "TOGGLE VECTORS MENU";
    default:
        return "UNKNOWN MODE";
    }
}

void Program::make_UI(Vector_Generator& vector_generator)
{
    interface_text.clear();
    interface_text.push_back("MODE " + get_mode_text());

    if (mode >= VECTOR_OBJECT_MENU && mode <= SET_STARTING_GLOBAL_SCALE)
        interface_text.push_back("INSTRUCTIONS " + to_string(vo_instructions.size()));
    else if (mode == SHIP_MENU || mode == EDIT_SHIP)
    {
        u16 instructions = 0;
        for (int i = 0; i < ROTATIONS; i++)
            instructions += ship_table[i].size() + current_edit_table[i].size() - 1;

        interface_text.push_back("INSTRUCTIONS " + to_string(instructions));
    }
    else if (mode == THRUST_MENU || mode == EDIT_THRUST)
    {
        u16 instructions = 0;
        for (int i = 0; i < ROTATIONS; i++)
            instructions += ship_thrust_table[i].size() + current_edit_table[i].size() - 1;

        interface_text.push_back("INSTRUCTIONS " + to_string(instructions));
    }

    interface_text.push_back("");

    if (mode == VECTOR_OBJECT_MENU || mode == INSTRUCTION_MENU || mode == SET_STARTING_GLOBAL_SCALE)
    {
        interface_text.push_back("GLOBAL SCALE START " + global_scale_text(start_global_scale));
        interface_text.push_back("GLOBAL SCALE " + global_scale_text(global_scale));
        interface_text.push_back("CURRENT X " + to_string(current_x));
        interface_text.push_back("CURRENT Y " + to_string(current_y));
    }
    else if (mode >= SET_BRIGHTNESS && mode <= EDIT_SVEC)
    {
        if (instruction_menu_choice == Instruction_Menu::DRAW_SHORT_VECTOR)
            binary_instruction(1, interface_text);
        else
            binary_instruction(2, interface_text);

        if (instruction_menu_choice == Instruction_Menu::DRAW_LONG_VECTOR)
        {
            interface_text.push_back("OPCODE" + local_scale_text(long_vector.opcode));
            interface_text.push_back("DELTA X " + to_string(long_vector.delta_x));
            interface_text.push_back("DELTA Y " + to_string(long_vector.delta_y));
            interface_text.push_back("BRIGHTNESS " + to_string(long_vector.brightness));

            interface_text.push_back("");

            interface_text.push_back("GLOBAL SCALE START " + global_scale_text(start_global_scale));
            interface_text.push_back("GLOBAL SCALE " + global_scale_text(global_scale));
            interface_text.push_back("CURRENT X " + to_string(current_x));
            interface_text.push_back("CURRENT Y " + to_string(current_y));
            interface_text.push_back("SCALED DELTA X " + to_string(get_scaled_delta(long_vector.delta_x)));
            interface_text.push_back("SCALED DELTA Y " + to_string(get_scaled_delta(long_vector.delta_y)));
        }
        else if (instruction_menu_choice == Instruction_Menu::LOAD_ABSOLUTE)
        {
            interface_text.push_back("OPCODE 10");
            interface_text.push_back("GLOBAL SCALE " + to_string(load_absolute.global_scale));
            interface_text.push_back("X POSITION " + to_string(load_absolute.x_position));
            interface_text.push_back("Y POSITION " + to_string(load_absolute.y_position));

            interface_text.push_back("");

            interface_text.push_back("GLOBAL SCALE START " + global_scale_text(start_global_scale));
            interface_text.push_back("CURRENT X " + to_string(current_x));
            interface_text.push_back("CURRENT Y " + to_string(current_y));
        }
        else
        {
            interface_text.push_back("OPCODE 15");
            interface_text.push_back("LOCAL SCALE" + local_scale_text(short_vector.local_scale));
            interface_text.push_back("DELTA X " + to_string(short_vector.delta_x));
            interface_text.push_back("DELTA Y " + to_string(short_vector.delta_y));
            interface_text.push_back("BRIGHTNESS " + to_string(short_vector.brightness));

            interface_text.push_back("");

            interface_text.push_back("GLOBAL SCALE START " + global_scale_text(start_global_scale));
            interface_text.push_back("GLOBAL SCALE " + global_scale_text(global_scale));
            interface_text.push_back("CURRENT X " + to_string(current_x));
            interface_text.push_back("CURRENT Y " + to_string(current_y));
            interface_text.push_back("SCALED DELTA X " + to_string(get_scaled_delta(short_vector.delta_x)));
            interface_text.push_back("SCALED DELTA Y " + to_string(get_scaled_delta(short_vector.delta_y)));
        }
    }
    else if (mode == EDIT_SHIP)
    {
        interface_text.push_back("DELTA X " + to_string(ship_edit_vector.delta_x));
        interface_text.push_back("DELTA Y " + to_string(ship_edit_vector.delta_y));
        interface_text.push_back("BRIGHTNESS " + to_string(ship_edit_vector.brightness));
    }
    else if (mode == EDIT_THRUST)
    {
        interface_text.push_back("DELTA X " + to_string(thrust_edit_vector.delta_x));
        interface_text.push_back("DELTA Y " + to_string(thrust_edit_vector.delta_y));
        interface_text.push_back("BRIGHTNESS " + to_string(thrust_edit_vector.brightness));
    }
}

void Program::draw_UI(Vector_Generator& vector_generator)
{
    u8 y = 221;
    for (unsigned i = 0; i < interface_text.size(); i++)
    {
        if (!interface_text[i].empty())
        {
            draw_string(interface_text[i], MUL_1, 2, y, vector_generator, window);
            y -= 5;
        }
        else
            y -= 2;
    }
    draw_copyright(vector_generator);
}

void Program::binary_instruction(u8 words, vector<string>& interface_text) const
{
    for (int i = 0; i < words; i++)
    {
        string line;
        for (int b = 15; b >= 0; b--)
            line += '0' + ((vo_instructions[vo_instructions.size() - 1 - words + i] >> b) & 1);

        interface_text.push_back(line);
    }
}

string Program::local_scale_text(u8 local_scale) const
{
    string local_scale_text = ' ' + to_string(local_scale) + '(';
    if (instruction_menu_choice == Instruction_Menu::DRAW_LONG_VECTOR)
        local_scale_text += '/' + to_string(1 << (9 - local_scale));
    else
        local_scale_text += '*' + to_string(1 << (local_scale + 1));

    return local_scale_text + ')';
}

string global_scale_text(Global_Scale scale)
{
    string global_scale_text = to_string(scale) + '(';
    if (scale <= MUL_128)
        global_scale_text += '*' + to_string(1 << scale);
    else
        global_scale_text += '/' + to_string(1 << (16 - scale));

    return global_scale_text + ')';
}

s16 Program::get_scaled_delta(s16 delta) const
{
    s16 scaled_delta = delta;
    if (instruction_menu_choice == Instruction_Menu::DRAW_LONG_VECTOR)
        scaled_delta /= std::pow(2, 9 - long_vector.opcode);
    else
        scaled_delta *= std::pow(2, short_vector.local_scale + 1);

    if (global_scale <= MUL_128)
        scaled_delta *= std::pow(2, static_cast<u8>(global_scale));
    else
        scaled_delta /= std::pow(2, 16 - global_scale);

    return scaled_delta;
}

void Program::draw_copyright(Vector_Generator& vector_generator)
{
    vector_generator.process(COPYRIGHT_SYMBOL, window);

    // 2017_
    draw_character(3, vector_generator, window);
    draw_character(1, vector_generator, window);
    draw_character(2, vector_generator, window);
    draw_character(8, vector_generator, window);
    draw_character(0, vector_generator, window);

    // slx7R4GDZM
    draw_character(29, vector_generator, window);
    draw_character(22, vector_generator, window);
    draw_character(34, vector_generator, window);
    draw_character( 8, vector_generator, window);
    draw_character(28, vector_generator, window);
    draw_character( 5, vector_generator, window);
    draw_character(17, vector_generator, window);
    draw_character(14, vector_generator, window);
    draw_character(36, vector_generator, window);
    draw_character(23, vector_generator, window);
}

template <unsigned SIZE, typename T>
void Program::process_menu(const array<string, SIZE>& menu_text, T& current_option, Vector_Generator& vector_generator)
{
    if (input.on_press(UP))
    {
        if (static_cast<u8>(current_option) == 0)
            current_option = static_cast<T>(SIZE - 1);
        else
            current_option = static_cast<T>(sub_1(static_cast<u8>(current_option), 0));
    }
    else if (input.on_press(DOWN))
    {
        if (static_cast<u8>(current_option) == SIZE - 1)
            current_option = static_cast<T>(0);
        else
            current_option = static_cast<T>(add_1(static_cast<u8>(current_option), SIZE - 1));
    }

    set_position_and_size(121, 229 - static_cast<u8>(current_option) * 10, MUL_1, vector_generator, window);
    vector_generator.process(ARROW, window, 18);

    for (unsigned i = 0; i < SIZE; i++)
        draw_string(menu_text[i], MUL_2, 135, 218 - i * 10, vector_generator, window, i == static_cast<u8>(current_option));
}

void Program::draw_option_button(Vector_Generator& vector_generator)
{
    set_position_and_size(2, 47, MUL_1, vector_generator, window);
    vector_generator.process(BOX, window);
    draw_string("OPTN", MUL_1, 4, 38, vector_generator, window);
}

void Program::draw_vector_object(Vector_Generator& vector_generator)
{
    // draw box
    set_position_and_size(128, 128, start_global_scale, vector_generator, window);
    if (vector_toggle.box == 1)
        vector_generator.process(EDIT_BOX, window);
    else if (vector_toggle.box == 2)
        vector_generator.process(LETTER_BOX, window);

    // draw instructions
    if (vector_toggle.object || (mode >= VECTOR_OBJECT_MENU && mode <= SET_STARTING_GLOBAL_SCALE))
    {
        vector_generator.process(vo_instructions.data(), window);

        // draw dot if needed
        switch (mode)
        {
        case SET_BRIGHTNESS:
        case EDIT_VCTR:
        case EDIT_SVEC:
            if (instruction_menu_choice == Instruction_Menu::DRAW_LONG_VECTOR)
            {
                if (!long_vector.brightness)
                    vector_generator.process(DOT, window);
            }
            else if (instruction_menu_choice == Instruction_Menu::DRAW_SHORT_VECTOR)
            {
                if (!short_vector.brightness)
                    vector_generator.process(DOT, window);
            }
            break;
        case SET_GLOBAL_SCALE:
        case EDIT_LABS:
            vector_generator.process(DOT, window);
            break;
        default:
            break;
        }
        current_x = vector_generator.current_x;
        current_y = vector_generator.current_y;
        global_scale = vector_generator.global_scale;
    }
}

void Program::draw_ship_edit_stuff(Vector_Generator& vector_generator)
{
    // spinning ship
    if (vector_toggle.ship_and_thrust || (mode >= SHIP_MENU && mode <= EDIT_THRUST))
    {
        set_position_and_size(168, 128, DIV_4, vector_generator, window);
        draw_rotating_vector(ship_table, fast_timer, vector_generator, window);
        if (mode == EDIT_SHIP)
            draw_rotating_vector(current_edit_table, fast_timer, vector_generator, window);
    }

    // spinning thrust
    if (vector_toggle.ship_and_thrust == 2 || (mode >= SHIP_MENU && mode <= EDIT_THRUST))
    {
        if (fast_timer % 8 >= 4)
        {
            draw_rotating_vector(ship_thrust_table, fast_timer, vector_generator, window);
            if (mode == EDIT_THRUST)
                draw_rotating_vector(current_edit_table, fast_timer, vector_generator, window);
        }
    }

    if (mode >= SHIP_MENU && mode <= EDIT_THRUST)
    {
        // edit boxes
        for (int i = 0; i < 3; i++)
        {
            set_position_and_size(140 + i * 20, 136, MUL_1, vector_generator, window);
            vector_generator.process(BOX, window);
        }

        // spinning arrow
        set_position_and_size(148, 128, DIV_4, vector_generator, window);
        draw_rotating_vector(ROTATING_ARROW_TABLE, fast_timer, vector_generator, window);

        // direction arrow
        set_position_and_size(184, 128, MUL_1, vector_generator, window);
        vector_generator.process(SHIP_EDITOR_ARROW, window);

        // ship
        set_position_and_size(188, 128, DIV_2, vector_generator, window);
        vector_generator.process(ship_table[0].data(), window);
        if (mode == SHIP_MENU || mode == EDIT_SHIP)
        {
            vector_generator.process(current_edit_table[0].data(), window);
            if (!ship_edit_vector.brightness)
                vector_generator.process(DOT, window);
        }

        // thrust
        vector_generator.process(ship_thrust_table[0].data(), window);
        if (mode == THRUST_MENU || mode == EDIT_THRUST)
        {
            vector_generator.process(current_edit_table[0].data(), window);
            if (!thrust_edit_vector.brightness)
                vector_generator.process(DOT, window);
        }
    }
}

void Program::output_vector_object() const
{
    printf("const u16 VECTOR_OBJECT[] = {");
    for (unsigned i = 0; i < vo_instructions.size(); i++)
    {
        printf("0x%04X", vo_instructions[i]);
        if (i < vo_instructions.size() - 1)
            printf(", ");
    }
    printf("};\n\n");
}

void Program::output_rotating_VO_offsets(const vector<u16> vector_object_table[]) const
{
    int total_words = 0;
    for (int i = 0; i < ROTATIONS; i++)
        total_words += vector_object_table[i].size();

    u8 offset_nibbles = 0;
    bool done = false;
    while (!done)
    {
        if (total_words >> (offset_nibbles * 4) != 0)
            offset_nibbles++;
        else
            done = true;
    }

    if (offset_nibbles <= 2)
        printf("const u8 SHIP");
    else if (offset_nibbles <= 4)
        printf("const u16 SHIP");
    else
        printf("const u32 SHIP");

    if (main_menu_choice == Main_Menu::EDIT_SHIP_THRUST)
        printf("_THRUST");
    printf("_OFFSET_TABLE[] = \n{\n");

    int total_offset = 0;
    for (int i = 0; i < ROTATIONS; i++)
    {
        printf("    0x%0*X", offset_nibbles, total_offset);
        if (i < ROTATIONS - 1)
            printf(",");

        printf("\n");
        total_offset += vector_object_table[i].size();
    }
    printf("};\n\n");
}

void Program::output_rotating_VO(const vector<u16> vector_object_table[]) const
{
    if (main_menu_choice == Main_Menu::EDIT_SHIP)
        printf("const u16 SHIP_TABLE[] = \n{\n");
    else
        printf("const u16 SHIP_THRUST_TABLE[] = \n{\n");

    for (int i = 0; i < ROTATIONS; i++)
    {
        printf("    ");
        for (unsigned x = 0; x < vector_object_table[i].size(); x++)
        {
            printf("0x%04X", vector_object_table[i][x]);
            if (x < vector_object_table[i].size() - 1)
                printf(", ");
        }
        if (i < ROTATIONS - 1)
            printf(",");

        printf("\n");
    }
    printf("};\n\n");
}
