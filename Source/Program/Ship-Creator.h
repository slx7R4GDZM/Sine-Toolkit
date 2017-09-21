// Copyright slx7R4GDZM
// Distributed under the terms of the MIT License.
// Refer to the License.txt file for details.

#pragma once

#include "../Other/Common-Things.h"

class Ship_Creator
{
private:
    static void basic_vector_to_instructions(vector<u16>& instructions, const Basic_Vector rotated_vector);
    static bool higher_SVEC(const Basic_Vector rotated_vector, Short_Vector& short_vector);
    static Long_Vector lower_VCTR(const Basic_Vector rotated_vector);
public:
    static void rotate_vector(const Basic_Vector vector_to_rotate, vector<u16> instructions[]);
    static void output_lives_icon(const vector<u16> instructions);
};
