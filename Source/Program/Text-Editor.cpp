// Copyright slx7R4GDZM
// Distributed under the terms of the MIT License.
// Refer to the License.txt file for details.

#include "Text-Editor.h"

#include <cstdio>
#include "../Graphics/Graphics-Handler.h"
#include "../Graphics/Vector-Generator.h"
#include "../Input/Input-Handler.h"
#include "../Other/Vectors.h"

const u8 MAX_TEXT_LENGTH = 100;

const u8 CHARS_PER_MSG = 3;
const u8 BITS_PER_CHAR = 5;

Text_Editor::Text_Editor() : key(0)
{
}

void Text_Editor::edit_text(Mode& mode, const u8 fast_timer, const Input_Handler input, Vector_Generator& vector_generator, RenderWindow& window)
{
    for (int i = 0; i < 30; i++)
    {
        const u8 x = i * 14 - i / 10 * 140 + 32;
        const u8 y = 80 - i / 10 * 10 * 2;
        const u8 character = i + (i < 26 ? 11 : -26);
        set_position_and_size(x, y, MUL_4, vector_generator, window);
        draw_character(character, vector_generator, window);
    }
    draw_string("clear", MUL_2, 172, 83, vector_generator, window);
    draw_string("backspace", MUL_2, 172, 63, vector_generator, window);
    draw_string("output", MUL_2, 172, 43, vector_generator, window);

    if (input.on_press(UP))
    {
        key -= 11;
        if (key < 0)
            key += 33;
    }
    if (input.on_press(DOWN))
    {
        key += 11;
        if (key >= 33)
            key -= 33;
    }
    if (input.on_press(LEFT))
    {
        key--;
        if ((key + 1) % 11 == 0)
            key += 11;
    }
    if (input.on_press(RIGHT))
    {
        key++;
        if (key % 11 == 0)
            key -= 11;
    }

    const u8 x = key * 14 - key / 11 * 154 + 32;
    const u8 y = 77 - key / 11 * 10 * 2;
    set_position_and_size(x, y, MUL_4, vector_generator, window);
    vector_generator.process(UNDERSCORE, window);

    if (input.on_press(CONFIRM))
    {
        if (key == 10) // clear
            text.clear();
        else if (key == 21) // backspace
        {
            if (text.length() > 0)
                text.pop_back();
        }
        else if (key == 32) // output
        {
            output_packed_text();
            key = 0;
            text.clear();
            mode = MAIN_MENU;
        }
        else if (text.size() + 1 <= MAX_TEXT_LENGTH)
        {
            switch (key)
            {
            case 0 ... 9: // A-J
                text += (key + 'a');
                break;
            case 11 ... 20: // K-T
                text += (key + 'a' - 1);
                break;
            case 22 ... 27: // U-Z
                text += (key + 'a' - 2);
                break;
            case 28: // space
                text += ' ';
                break;
            case 29 ... 31: // 0-2
                text += (key + 'a' - 78);
                break;
            }
        }
    }
    else if (input.on_press(CANCEL))
        mode = MAIN_MENU;

    draw_string(get_last_n_chars(text, 41), MUL_2, 5, 103, vector_generator, window);
    if (fast_timer % 64 < 32)
        vector_generator.process(UNDERSCORE, window);
}

string Text_Editor::get_last_n_chars(const string input, const int last_chars)
{
    const int size_after_removing_n = input.length() - last_chars;
    if (size_after_removing_n < 0)
        return input;

    return input.substr(size_after_removing_n, last_chars);
}

void Text_Editor::output_packed_text() const
{
    cout << '"';
    string uppercased_input;
    for (unsigned int i = 0; i < text.length(); i++)
        uppercased_input += toupper(text[i]);

    cout << uppercased_input << "\"\n";

    if (text.empty())
        cout << '\n';

    // convert the input string to packed text and output it
    for (unsigned int i = 0; i < text.length(); i += CHARS_PER_MSG)
    {
        const bool final_message = text.length() - i <= CHARS_PER_MSG;
        const u16 message = pack_message(i, final_message);

        std::printf("0x%04X", message);
        if (!final_message)
            cout << ", ";
        else
            cout << "\n\n";
    }
}

//pack message
u16 Text_Editor::pack_message(const unsigned int msg_start, const bool final_message) const
{
    u16 message = 0;
    bool null_terminated = false;
    for (unsigned int i = msg_start; i < msg_start + CHARS_PER_MSG && !null_terminated; i++)
    {
        const u8 character = text[i];
        const u8 char_offset = 11 - (i - msg_start) * BITS_PER_CHAR;
        if (character == ' ')
            message |= (character - 31) << char_offset;
        else if (character >= '0' && character <= '2')
            message |= (character - 46) << char_offset;
        else if (character >= 'a' && character <= 'z')
            message |= (character - 92) << char_offset;
        else // invalid character input
            null_terminated = true;
    }

    // if it's the final message and it hasn't been null terminated
    // then set the terminate bit
    if (final_message && !null_terminated)
        message |= 1;

    return message;
}
