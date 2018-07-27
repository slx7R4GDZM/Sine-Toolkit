// Copyright slx7R4GDZM
// Distributed under the terms of the MIT License.
// Refer to the License.txt file for details.

#pragma once

#include "../Other/Common-Things.h"

class Ship_Creator
{
private:
    static void add_basic_vector_to_VO(const Basic_Vector basic_vector, vector<u16>& vector_object);
    static bool higher_SVEC(const Basic_Vector basic_vector, Short_Vector& converted_SVEC);
    static Long_Vector lower_VCTR(const Basic_Vector basic_vector);
public:
    static void rotate_vector(const Basic_Vector vector_to_rotate, vector<u16> vector_object_table[]);
    static void output_lives_icon(const vector<u16> upward_ship_VO);
};
