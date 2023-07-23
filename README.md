# CXC - CX Compiler

A compiler for a variation of C.

## Examples

```
# value
value foo: i32;
value bar: i32 {32}
value baz: i32 {
    # only expressions allowed in this block
    bar == 32 => 21 =: 6
}

# the declaration of `baz` is equavalent to:
value baz: i32;
if bar == 32 {
    baz = 21;
} else {
    baz = 6;
} 

# function
function create(row: u64, column: u64)
    -> type Position = (
        row: u64,
        column: u64)
{
    return Position {
        .row = row,
        .column = column
    };
}

# tuple
type Tuple  = u64;
type Tuple2 = foo: u64, bar: u8; # tagged-tuple

# array
type Array  = 4 * u8;
type Array2 = 3 * 3 * u8; # 3x3 array of u8

# union
type Union = u8 + u64;
type Union2 = (u64, i8) + 3 * u8; # arithmetic order of operations applies

# enum
type Enum = monday | tuesday;
type Enum2 = tuple: Tuple2 | array: Array2 | union: Union2; # algebraic enum

```
