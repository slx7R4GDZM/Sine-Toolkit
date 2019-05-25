// Copyright slx7R4GDZM
// Distributed under the terms of the MIT License.
// Refer to the License.txt file for details.

#pragma once

#include "../Other/Common-Things.h"

class Input_Handler;
class Vector_Generator;

class Text_Editor
{
public:
    Text_Editor();

    void edit_text(Mode& mode, u8 fast_timer, const Input_Handler& input, Vector_Generator& vector_generator, RenderWindow& window);
private:
    void output_packed_text() const;
    u16 pack_message(unsigned msg_start, bool final_message) const;

    s8 key;
    string text;
};
