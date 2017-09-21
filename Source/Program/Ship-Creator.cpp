// Copyright slx7R4GDZM
// Distributed under the terms of the MIT License.
// Refer to the License.txt file for details.

#include "Ship-Creator.h"

#include <cstdio>
#include <cstdlib>
#include "../Other/Constants.h"

void Ship_Creator::rotate_vector(const Basic_Vector vector_to_rotate, vector<u16> instructions[])
{
    for (int i = 0; i < ROTATIONS; i++)
    {
        clear_instructions(instructions[i]);

        const float angle_radians = (90.0f / (ROTATIONS - 1) * i) * PI / 180;
        const float sin_angle = std::sin(angle_radians);
        const float cos_angle = std::cos(angle_radians);

        const float rotated_delta_x = cos_angle * vector_to_rotate.delta_x - sin_angle * vector_to_rotate.delta_y;
        const float rotated_delta_y = sin_angle * vector_to_rotate.delta_x + cos_angle * vector_to_rotate.delta_y;

        const Basic_Vector rotated_vector =
        {
            vector_to_rotate.brightness,
            static_cast<s16>(std::round(rotated_delta_x * DELTA_LENGTH)),
            static_cast<s16>(std::round(rotated_delta_y * DELTA_LENGTH))
        };
        basic_vector_to_instructions(instructions[i], rotated_vector);
    }
}

void Ship_Creator::basic_vector_to_instructions(vector<u16>& instructions, const Basic_Vector rotated_vector)
{
    Short_Vector short_vector;
    const bool found_SVEC = higher_SVEC(rotated_vector, short_vector);

    if (!found_SVEC)
    {
        add_blank_instructions(instructions, 2);
        Long_Vector long_vector = lower_VCTR(rotated_vector);
        calculate_VCTR(long_vector, instructions);
    }
    else
    {
        add_blank_instructions(instructions, 1);
        calculate_SVEC(short_vector, instructions);
    }
}

// tries to convert rotated_vector to SVEC with highest local scale
// without losing precision, returns false if not possible
bool Ship_Creator::higher_SVEC(const Basic_Vector rotated_vector, Short_Vector& short_vector)
{
    for (s8 scale = 3; scale >= 0; scale--)
    {
        s8 delta_x;
        s8 delta_y;
        bool found_x = false;
        bool found_y = false;
        for (int delta = 0; delta < 4; delta++)
        {
            const u8 scaled_SVEC_delta = std::pow(2, scale + 1) * delta;
            if (!found_x && std::abs(rotated_vector.delta_x) == scaled_SVEC_delta)
            {
                found_x = true;
                delta_x = delta;
                if (rotated_vector.delta_x < 0)
                    delta_x = -delta_x;
            }
            if (!found_y && std::abs(rotated_vector.delta_y) == scaled_SVEC_delta)
            {
                found_y = true;
                delta_y = delta;
                if (rotated_vector.delta_y < 0)
                    delta_y = -delta_y;
            }
        }
        if (found_x && found_y)
        {
            short_vector = {static_cast<u8>(scale), rotated_vector.brightness, delta_x, delta_y};
            return true;
        }
    }
    return false;
}

// converts rotated_vector to a VCTR with the lowest opcode without losing precision
Long_Vector Ship_Creator::lower_VCTR(const Basic_Vector rotated_vector)
{
    Long_Vector lowered_VCTR = {VCTR_9, rotated_vector.brightness, rotated_vector.delta_x, rotated_vector.delta_y};
    bool done = false;
    for (int i = 1; i <= 9 && !done; i++)
    {
        const s16 new_delta_x = rotated_vector.delta_x * std::pow(2, i);
        const s16 new_delta_y = rotated_vector.delta_y * std::pow(2, i);
        if (new_delta_x <= -1024 || new_delta_x >= 1024 ||
            new_delta_y <= -1024 || new_delta_y >= 1024)
            done = true;
        else
        {
            lowered_VCTR.opcode = VCTR_9 - i;
            lowered_VCTR.delta_x = new_delta_x;
            lowered_VCTR.delta_y = new_delta_y;
        }
    }
    return lowered_VCTR;
}

void Ship_Creator::output_lives_icon(const vector<u16> instructions)
{
    s16 total_delta_x = 0;
    s16 total_delta_y = 0;
    vector<u16> icon = instructions;

    // calculate the total delta and adjust icon brightness to 7
    for (unsigned int i = 0; i < icon.size(); i++)
    {
        const u8 opcode = icon[i] >> 12;
        if (opcode <= VCTR_9)
        {
            s16 delta_y = icon[i] & 0x03FF;
            s16 delta_x = icon[i + 1] & 0x03FF;

            delta_y >>= (9 - opcode);
            delta_x >>= (9 - opcode);

            if (icon[i++] & 0x0400)
                delta_y = -delta_y;
            if (icon[i] & 0x0400)
                delta_x = -delta_x;

            if (icon[i] & 0xF000)
                icon[i] = (icon[i] & 0x0FFF) | 0x7000;

            total_delta_x += delta_x;
            total_delta_y += delta_y;
        }
        else if (opcode == SVEC)
        {
            s16 delta_x = (icon[i] & 0x0003) << 8;
            s16 delta_y = icon[i] & 0x0300;

            const u8 local_scale = ((icon[i] & 0x0008) >> 2)
                                 | ((icon[i] & 0x0800) >> 11);

            delta_x >>= (7 - local_scale);
            delta_y >>= (7 - local_scale);

            if (icon[i] & 0x0004)
                delta_x = -delta_x;
            if (icon[i] & 0x0400)
                delta_y = -delta_y;

            if (icon[i] & 0x00F0)
                icon[i] = (icon[i] & 0xFF0F) | 0x0070;

            total_delta_x += delta_x;
            total_delta_y += delta_y;
        }
    }

    Basic_Vector lives_spacing = {
        0,
        static_cast<s16>(-total_delta_x + 64),
        static_cast<s16>(-total_delta_y)
    };
    basic_vector_to_instructions(icon, lives_spacing);

    cout << "const u16 LIVES_REMAINING_SHIP[] = {";
    for (unsigned int i = 0; i < icon.size(); i++)
    {
        std::printf("0x%04X", icon[i]);
        if (i != icon.size() - 1)
            cout << ", ";
    }
    cout << "};\n\n";
}
