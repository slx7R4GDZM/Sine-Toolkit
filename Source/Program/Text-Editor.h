// Copyright slx7R4GDZM
// Distributed under the terms of the MIT License.
// Refer to the License.txt file for details.

#pragma once

#include "../Other/Common-Things.h"

class Input_Handler;
class Vector_Generator;

class Text_Editor
{
private:
    s8 key;
    string text;

    static string get_visible_string(const string& input);
    void output_packed_text() const;
    u16 pack_message(const unsigned msg_start, const bool final_message) const;
public:
    Text_Editor();
    void edit_text(Mode& mode, const u8 fast_timer, const Input_Handler input, Vector_Generator& vector_generator, RenderWindow& window);
};
