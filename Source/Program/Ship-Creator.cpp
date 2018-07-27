// Copyright slx7R4GDZM
// Distributed under the terms of the MIT License.
// Refer to the License.txt file for details.

#include "Ship-Creator.h"

#include <cstdlib>
#include "../Other/Constants.h"

void Ship_Creator::rotate_vector(const Basic_Vector vector_to_rotate, vector<u16> vector_object_table[])
{
    for (int i = 0; i < ROTATIONS; i++)
    {
        clear_instructions(vector_object_table[i]);

        const float angle_radians = (90.0f / (ROTATIONS - 1) * i) * PI / 180;
        const float angle_sin = std::sin(angle_radians);
        const float angle_cos = std::cos(angle_radians);

        const float rotated_delta_x = vector_to_rotate.delta_x * angle_cos - vector_to_rotate.delta_y * angle_sin;
        const float rotated_delta_y = vector_to_rotate.delta_y * angle_cos + vector_to_rotate.delta_x * angle_sin;

        const Basic_Vector rotated_vector =
        {
            vector_to_rotate.brightness,
            static_cast<s16>(std::round(rotated_delta_x * DELTA_LENGTH)),
            static_cast<s16>(std::round(rotated_delta_y * DELTA_LENGTH))
        };
        add_basic_vector_to_VO(rotated_vector, vector_object_table[i]);
    }
}

void Ship_Creator::add_basic_vector_to_VO(const Basic_Vector basic_vector, vector<u16>& vector_object)
{
    Short_Vector short_vector;
    const bool vector_fits_SVEC = higher_SVEC(basic_vector, short_vector);

    if (!vector_fits_SVEC)
    {
        add_blank_instructions(vector_object, 2);
        const Long_Vector long_vector = lower_VCTR(basic_vector);
        calculate_VCTR(long_vector, vector_object);
    }
    else
    {
        add_blank_instructions(vector_object, 1);
        calculate_SVEC(short_vector, vector_object);
    }
}

// tries to convert basic vector to a short vector with highest local scale
// without losing precision, returns false if it's not possible
bool Ship_Creator::higher_SVEC(const Basic_Vector basic_vector, Short_Vector& converted_SVEC)
{
    for (u8 scale = 3; scale < 4; scale--)
    {
        s8 delta_x;
        s8 delta_y;
        bool x_fits_scale = false;
        bool y_fits_scale = false;
        for (u8 delta = 0; delta < 4; delta++)
        {
            const u8 new_delta = std::pow(2, scale + 1) * delta;
            if (!x_fits_scale && new_delta == std::abs(basic_vector.delta_x))
            {
                x_fits_scale = true;
                delta_x = delta;
                if (basic_vector.delta_x < 0)
                    delta_x = -delta_x;
            }
            if (!y_fits_scale && new_delta == std::abs(basic_vector.delta_y))
            {
                y_fits_scale = true;
                delta_y = delta;
                if (basic_vector.delta_y < 0)
                    delta_y = -delta_y;
            }
        }
        if (x_fits_scale && y_fits_scale)
        {
            converted_SVEC = {scale, basic_vector.brightness, delta_x, delta_y};
            return true;
        }
    }
    return false;
}

// converts basic vector to a long vector with the
// lowest opcode possible without losing precision
Long_Vector Ship_Creator::lower_VCTR(const Basic_Vector basic_vector)
{
    Long_Vector converted_VCTR =
    {
        VCTR_9,
        basic_vector.brightness,
        basic_vector.delta_x,
        basic_vector.delta_y
    };

    bool done = false;
    for (u8 scale = VCTR_0; scale < VCTR_9 && !done; scale++)
    {
        const s16 scaled_delta_x = basic_vector.delta_x * std::pow(2, VCTR_9 - scale);
        const s16 scaled_delta_y = basic_vector.delta_y * std::pow(2, VCTR_9 - scale);

        if (scaled_delta_x > -1024 && scaled_delta_x < 1024
         && scaled_delta_y > -1024 && scaled_delta_y < 1024)
        {
            converted_VCTR = {scale, basic_vector.brightness, scaled_delta_x, scaled_delta_y};
            done = true;
        }
    }
    return converted_VCTR;
}

void Ship_Creator::output_lives_icon(const vector<u16> upward_ship_VO)
{
    s16 total_delta_x = 0;
    s16 total_delta_y = 0;
    vector<u16> icon = upward_ship_VO;

    // add the delta from every vector in the vertical ship vector object
    // to help figure out the spacing between the lives icon
    // also adjust the max brightness of the icon to 7
    for (unsigned i = 0; i < icon.size(); i++)
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

    Basic_Vector lives_spacing =
    {
        0,
        static_cast<s16>(-total_delta_x + 64),
        static_cast<s16>(-total_delta_y)
    };
    add_basic_vector_to_VO(lives_spacing, icon);

    printf("const u16 LIVES_REMAINING_SHIP[] = {");
    for (unsigned i = 0; i < icon.size(); i++)
    {
        printf("0x%04X", icon[i]);
        if (i < icon.size() - 1)
            printf(", ");
    }
    printf("};\n\n");
}
