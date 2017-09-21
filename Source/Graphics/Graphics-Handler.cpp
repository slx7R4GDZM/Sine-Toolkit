// Copyright slx7R4GDZM
// Distributed under the terms of the MIT License.
// Refer to the License.txt file for details.

#include "Graphics-Handler.h"

#include <cstdlib>
#include "Vector-Generator.h"
#include "../Other/Vectors.h"

void draw_string(const string message, const Global_Scale scale, const u8 x, const u8 y, Vector_Generator& vector_generator, RenderWindow& window, const bool brighten)
{
    set_position_and_size(x, y, scale, vector_generator, window);
    for (unsigned int i = 0; i < message.length(); i++)
    {
        const u8 character = message[i];
        switch (character)
        {
        case 'a' ... 'z':
            vector_generator.process(CHARACTER_TABLE, window, CHARACTER_OFFSET_TABLE[character - 86], false, false, brighten);
            break;
        case '0' ... '9':
            vector_generator.process(CHARACTER_TABLE, window, CHARACTER_OFFSET_TABLE[character - 47], false, false, brighten);
            break;
        case ' ':
            vector_generator.process(CHARACTER_TABLE, window, CHARACTER_OFFSET_TABLE[0], false, false, brighten);
            break;
        case '(':
            vector_generator.process(LEFT_PARENTHESIS, window);
            break;
        case ')':
            vector_generator.process(RIGHT_PARENTHESIS, window);
            break;
        case '*':
            vector_generator.process(ASTERISK, window);
            break;
        case '/':
            vector_generator.process(DIVIDE, window);
            break;
        default:
            vector_generator.process(MINUS_SIGN, window);
        }
    }
}

void draw_character(const u8 character, Vector_Generator& vector_generator, RenderWindow& window)
{
    vector_generator.process(CHARACTER_TABLE, window, CHARACTER_OFFSET_TABLE[character]);
}

void draw_digit(const u8 digit, Vector_Generator& vector_generator, RenderWindow& window)
{
    vector_generator.process(CHARACTER_TABLE, window, CHARACTER_OFFSET_TABLE[digit + 1]);
}

void draw_number(const int number, Vector_Generator& vector_generator, RenderWindow& window)
{
    if (number < 0)
        vector_generator.process(MINUS_SIGN, window);

    const string num_string = to_string(std::abs(number));
    const u8 digits = num_string.length();

    for (int i = 0; i < digits; i++)
        draw_digit(num_string[i] - '0', vector_generator, window);
}

void set_position_and_size(const u8 cur_x, const u8 cur_y, const Global_Scale scale, Vector_Generator& vector_generator, RenderWindow& window)
{
    const vector<u16> vector_object =
    {
        static_cast<u16>(( LABS << 12) | (cur_y << 2)),
        static_cast<u16>((scale << 12) | (cur_x << 2)),
                         ( RTSL << 12)
    };
    vector_generator.process(vector_object, window);
}

void draw_options(const string options_text[], Vector_Generator& vector_generator, RenderWindow& window)
{
    u8 y = 101;
    for (int i = 0; i < 4; i++)
    {
        set_position_and_size(2, y, MUL_1, vector_generator, window);
        vector_generator.process(BOX, window);
        vector_generator.process(ARROW, window, i * 6);
        draw_string(options_text[i], MUL_2, 21, y - 11, vector_generator, window);
        y -= 18;
    }
}

void draw_rotating_vector(const vector<u16> vector_table[], const u8 fast_timer, Vector_Generator& vector_generator, RenderWindow& window)
{
    if (fast_timer < 64)
    {
        const u8 rotation = fast_timer / 4;
        vector_generator.process(vector_table[rotation], window);
    }
    else if (fast_timer < 128)
    {
        const u8 rotation = 31 - (fast_timer - 1) / 4;
        vector_generator.process(vector_table[rotation], window, 0, true);
    }
    else if (fast_timer < 192)
    {
        const u8 rotation = (fast_timer - 128) / 4;
        vector_generator.process(vector_table[rotation], window, 0, true, true);
    }
    else
    {
        const u8 rotation = 63 - (fast_timer - 1) / 4;
        vector_generator.process(vector_table[rotation], window, 0, false, true);
    }
}
