# Script API reference

> Unless stated otherwise, all function arguments are floats.
> `T` denotes any type.

## Base script

### New Constants

```dnh
M_PI        = pi                ; 3.14159265358979323846
M_PI_2      = M_PI / 2          ; 1.57079632679489661923
M_PI_4      = M_PI / 4          ; 0.78539816339744830962
M_PI_X2     = M_PI * 2          ; 6.28318530717958647693
M_PI_X4     = M_PI * 4          ; 12.5663706143591729539
M_1_PI      = 1 / M_PI          ; 0.31830988618379067154
M_2_PI      = 2 / M_PI          ; 0.63661977236758134308
M_SQRTPI    = sqrt(M_PI)        ; 1.772453850905516027298
M_1_SQRTPI  = 1 / sqrt(M_PI)    ; 0.56418958354775628695
M_2_SQRTPI  = 2 / sqrt(M_PI)    ; 1.128379167095512573896
M_SQRT2     = sqrt(2)           ; 1.4142135623730950488
M_SQRT2_2   = sqrt(2) / 2       ; 0.7071067811865475244
M_SQRT2_X2  = sqrt(2) * 2       ; 2.8284271247461900976
M_E         = e                 ; 2.71828182845904523536
M_LOG2E     = log2(e)           ; 1.44269504088896340736
M_LOG10E    = log10(e)          ; 0.43429448190325182765
M_LN2       = ln(2)             ; 0.69314718055994530942
M_LN10      = ln(10)            ; 2.30258509299404568402
M_PHI       = (1 + sqrt(5)) / 2 ; 1.61803398874989484821
M_1_PHI     = 1 / M_PHI         ; 0.6180339887498948482
INF            = infinity float
NAN            = NaN float
```

### Basic Operations

#### `__DEBUG_BREAK`

**Description**

Triggers a debug break. For developers.

#### `wait`

**Arguments**

- `int` frame

**Description**

Pauses the execution for the given amount of frames.
Much faster than loop(n){yield;}

#### `typeof`

**Arguments**

- `T` value

**Returns**

- `(int) type`

**Description**

Returns the type of the given value.

**Example**

```dnh
-60i        -> VAR_INT
920         -> VAR_FLOAT
false       -> VAR_BOOL
"str"       -> VAR_STRING
[8, 0, 2]   -> VAR_ARRAY
```

#### `ftypeof`

**Arguments**

- `T` value

**Returns**

- `(int) type`

**Description**

Returns the root type of the given value.

**Example**

```dnh
920         -> VAR_FLOAT
false       -> VAR_BOOL
"str"       -> VAR_CHAR
[8, 0, 2]   -> VAR_FLOAT
[[["a"]]]   -> VAR_CHAR
```

#### `resize`

**Arguments**

- `T[]` array
- `int` new size

**Returns**

- `(T[]) result`

**Description**

Resizes the given array.
The array must not be a null array.

#### `resize (Overload)`

**Arguments**

- `T[]` array
- `int` new size
- `T` fill value

**Returns**

- `(T[]) result`

**Description**

Overloaded with 3 arguments.
Resizes the given array.
If the new size is larger than the old size, newly inserted elements will be assigned the fill value.
Unless the array is a null array, the fill value's type must be convertible to the array's element type.

#### `insert`

**Arguments**

- `T[]` array
- `int` insert position
- `T` value

**Returns**

- `(T[]) result`

**Description**

Inserts the given value into the given position of the array.
Insertion method is "insert-before".

#### `contains`

**Arguments**

- `T[]` array
- `T` value

**Returns**

- `(bool) result`

**Description**

Checks if the given value is present as an element of the given array.

#### `replace`

**Arguments**

- `T[]` array
- `T` replace from
- `T` replace to

**Returns**

- `(T[]) result`

**Description**

Replaces all occurences of the given element with the other in the array.

#### `remove`

**Arguments**

- `T[]` array
- `T` value

**Returns**

- `(T[]) result`

**Description**

Removes all occurences of the given element in the array.

### Type Casting

#### `as_int`

**Arguments**

- `T` value

**Returns**

- `(int) result`

**Description**

Returns the given value casted to an int value.

#### `as_float`

**Arguments**

- `T` value

**Returns**

- `(float) result`

**Description**

Returns the given value casted to a float value.

#### `as_bool`

**Arguments**

- `T` value

**Returns**

- `(bool) result`

**Description**

Returns the given value casted to a bool value.

#### `as_char`

**Arguments**

- `T` value

**Returns**

- `(char) result`

**Description**

Returns the given value casted to a char value.

#### `as_int_array`

**Arguments**

- `T[]` array

**Returns**

- `(int[]) result`

**Description**

Casts array elements to ints, then returns the result.
Casting is recursive.

**Example**

```dnh
as_int_array([[true, true], [false, true]]);    //-> [[1, 1], [0, 1]]
```

#### `as_float_array`

**Arguments**

- `T[]` array

**Returns**

- `(float[]) result`

**Description**

Casts array elements to floats, then returns the result.
Casting is recursive.

#### `as_bool_array`

**Arguments**

- `T[]` array

**Returns**

- `(bool[]) result`

**Description**

Casts array elements to bools, then returns the result.
Casting is recursive.

#### `as_char_array`

**Arguments**

- `T[]` array

**Returns**

- `(char[]) result`

**Description**

Casts array elements to chars, then returns the result.
Casting is recursive.
Not an alias of ToString.

#### `as_x_array`

**Arguments**

- `T[]` array
- `const` type

**Returns**

- `(T[]) result`

**Description**

Casts array elements to the given type, then returns the result.

**Supported types:**

VAR_INT
VAR_FLOAT
VAR_BOOL
VAR_CHAR
Casting is recursive.

### Bitwise Operations

#### `bit_not`

**Arguments**

- `int` value

**Returns**

- `(int) result`

**Description**

Performs a bitwise NOT operation.

#### `bit_and`

**Arguments**

- `int` value 1
- `int` value 2

**Returns**

- `(int) result`

**Description**

Performs a bitwise AND operation.

#### `bit_or`

**Arguments**

- `int` value 1
- `int` value 2

**Returns**

- `(int) result`

**Description**

Performs a bitwise OR operation.

#### `bit_xor`

**Arguments**

- `int` value 1
- `int` value 2

**Returns**

- `(int) result`

**Description**

Performs a bitwise XOR operation.

#### `bit_left`

**Arguments**

- `int` value
- `int` shift factor

**Returns**

- `(int) result`

**Description**

Performs a bitwise left shift operation.

#### `bit_right`

**Arguments**

- `int` value
- `int` shift factor

**Returns**

- `(int) result`

**Description**

Performs a bitwise right shift operation.

### Float Utilities

#### `Float_Classify`

**Arguments**

- x

**Returns**

- `(const) result`

**Description**

Classifies the given float value.

**Possible return values:**

- FLOAT_TYPE_ZERO           ; Value is either +0 or -0.
- FLOAT_TYPE_NORMAL         ; Value is a normal float.
- FLOAT_TYPE_SUBNORMAL      ; Value is a subnormal float. (https://en.wikipedia.org/wiki/Subnormal_number)
- FLOAT_TYPE_INFINITY	    ; Value is either +infinity or -infinity.
- FLOAT_TYPE_NAN		    ; Value is not-a-number.

#### `Float_IsNan`

**Arguments**

- x

**Returns**

- `(bool) result`

**Description**

Returns true if the given float is NaN.

#### `Float_IsInf`

**Arguments**

- x

**Returns**

- `(bool) result`

**Description**

Returns true if the given float is +infinity or -infinity.

#### `Float_GetSign`

**Arguments**

- x

**Returns**

- `(float) sign`

**Description**

Returns +1.0 if the given float is positive, and -1.0 if the given float is negative.

#### `Float_CopySign`

**Arguments**

- x
- y

**Returns**

- `(float) result`

**Description**

Copies the sign of x to y, and returns the result.

### Maths

#### `clamp`

**Arguments**

- x
- lower bound
- upper bound

**Returns**

- `(float) result`

**Description**

Returns x clamped between the specified range.

#### `log2`

**Arguments**

- x

**Returns**

- `(float) result`

**Description**

Returns base-2 logarithm of x.

#### `logn`

**Arguments**

- x
- base

**Returns**

- `(float) result`

**Description**

Returns base-n logarithm of x.

#### `erf`

**Arguments**

- x

**Returns**

- `(float) result`

**Description**

Returns the error function value of x.
erf(x) = (2/sqrt(pi)) * integral(0, x, "e^(-t^2) dt")

#### `gamma`

**Arguments**

- x

**Returns**

- `(float) result`

**Description**

Returns the gamma function value of x.
gamma(z + 1) = integral(0, infinity, "(x^z)(e^(-x)) dx")

#### `exp`

**Arguments**

- value

**Returns**

- `(float) result`

**Description**

Returns e (Euler's number) raised to the power of x.

#### `sqrt`

**Arguments**

- x

**Returns**

- `(float) result`

**Description**

Returns the square root of x.
Much faster than pow(x, 0.5).

#### `cbrt`

**Arguments**

- x

**Returns**

- `(float) result`

**Description**

Returns the cube root of x.
Equivalent to (x ^ (1/3)).

#### `nroot`

**Arguments**

- x
- root

**Returns**

- `(float) result`

**Description**

Returns the nth root of x.
Equivalent to pow(x, 1 / root).

#### `hypot`

**Arguments**

- a
- b

**Returns**

- `(float) result`

**Description**

Returns the hypotenuse of the triangle formed by the two given values.
hypot(a, b) = sqrt(a * a + b * b)

#### `distance`

**Arguments**

- x1
- y1
- x2
- y2

**Returns**

- `(float) result`

**Description**

Returns distance between the given two points.
distance(x1, y1, x2, y2) = hypot(x1 - x2, y1 - y2)

#### `distancesq`

**Arguments**

- x1
- y1
- x2
- y2

**Returns**

- `(float) result`

**Description**

Returns square of the distance between the given two points.
distancesq(x1, y1, x2, y2) = (x1 - x2)^2 + (y1 - y2)^2

#### `dottheta`

**Arguments**

- x1
- y1
- x2
- y2

**Returns**

- `(float) result (degrees)`

**Description**

Returns angle (in degrees) between the given two points.
dottheta(x1, y1, x2, y2) = atan2(y2 - y1, x2 - x1)

#### `rdottheta`

**Arguments**

- x1
- y1
- x2
- y2

**Returns**

- `(float) result (radians)`

**Description**

Returns angle (in radians) between the given two points.
rdottheta(x1, y1, x2, y2) = ratan2(y2 - y1, x2 - x1)

### Trigonometry and Angular Maths

#### `sincos`

**Arguments**

- angle

**Returns**

- `(float[2]) [sin(angle), cos(angle)]`

#### `rsin`

**Arguments**

- angle

**Returns**

- `(float) result`

**Description**

sin() for radians input.

#### `rcos`

**Arguments**

- angle (radians)

**Returns**

- `(float) result`

**Description**

cos() for radians input.

#### `rtan`

**Arguments**

- angle (radians)

**Returns**

- `(float) result`

**Description**

tan() for radians input.

#### `rsincos`

**Arguments**

- angle (radians)

**Returns**

- `(float[2]) result`

**Description**

sincos() for radians input.

#### `rasin`

**Arguments**

- value

**Returns**

- `(float) result`

**Description**

asin() for radians output.

#### `racos`

**Arguments**

- value

**Returns**

- `(float) result`

**Description**

acos() for radians output.

#### `ratan`

**Arguments**

- y
- x

**Returns**

- `(float) result`

**Description**

atan() for radians output.

#### `ratan2`

**Arguments**

- value

**Returns**

- `(float) result`

**Description**

atan2() for radians output.

#### `ToDegrees`

**Arguments**

- angle

**Returns**

- `(float) result`

**Description**

Converts the given angle to degrees.

#### `ToRadians`

**Arguments**

- angle

**Returns**

- `(float) result`

**Description**

Converts the given angle to radians.

#### `NormalizeAngle`

**Arguments**

- angle (degrees)

**Returns**

- `(float) result`

**Description**

Normalizes the given angle. (Range = [0, 360))

#### `NormalizeAngleR`

**Arguments**

- angle (radians)

**Returns**

- `(float) result`

**Description**

Normalizes the given angle. (Range = [0, 2pi))

#### `AngularDistance`

**Arguments**

- angle from (degrees)
- angle to (degrees)

**Returns**

- `(float) result`

**Description**

Calculates the shortest angular distance between the given angles. (Range = [-180, 180))

#### `AngularDistanceR`

**Arguments**

- angle from (radians)
- angle to (radians)

**Returns**

- `(float) result`

**Description**

Calculates the shortest angular distance between the given angles. (Range = [-pi, pi))

#### `ReflectAngle`

**Arguments**

- ray angle (degrees)
- surface angle (degrees)

**Returns**

- `(float) result`

**Description**

Calculates the given ray's angle of reflection upon a surface of the given angle. (Range = [0, 360))

#### `ReflectAngleR`

**Arguments**

- ray angle (radians)
- surface angle (radians)

**Returns**

- `(float) result`

**Description**

Calculates the given ray's angle of reflection upon a surface of the given angle. (Range = [0, 2pi))

### Interpolation

For the functions in this section, refer to these Desmos graph plot.
"Basic" interpolations 	-> https://www.desmos.com/calculator/m8dnz348cq
Bezier interpolations	-> https://www.desmos.com/calculator/q1uptzx8ti

#### `Interpolate_Linear`

**Arguments**

- a
- b
- x

**Returns**

- `IF a, b are not arrays`
- `(float) result`
- `ELSE`
- `(float[]) result array`

**Description**

Returns the result of interpolation between a and b, using x as the interpolation value.
Interpolation method is linear.
Direct formula is (a + (b - a) * (x))
Represented by the red graph.
When interpolating between two arrays, they must be of the same size.

#### `Interpolate_Smooth`

**Arguments**

- a
- b
- x

**Returns**

- `IF a, b are not arrays`
- `(float) result`
- `ELSE`
- `(float[]) result array`

**Description**

Returns the result of interpolation between a and b, using x as the interpolation value.
Interpolation method is first-order smoothstep curve.
Direct formula is (a + (b - a) * (x^2 * (3 - 2 * x)))
Represented by the blue graph.
When interpolating between two arrays, they must be of the same size.

#### `Interpolate_Smoother`

**Arguments**

- a
- b
- x

**Returns**

- `IF a, b are not arrays`
- `(float) result`
- `ELSE`
- `(float[]) result array`

**Description**

Returns the result of interpolation between a and b, using x as the interpolation value.
Interpolation method is second-order smoothstep curve.
Direct formula is (a + (b - a) * (x^3 * (x * (x * 6 - 15) + 10)))
Represented by the green graph.
When interpolating between two arrays, they must be of the same size.

#### `Interpolate_Accelerate`

**Arguments**

- a
- b
- x

**Returns**

- `IF a, b are not arrays`
- `(float) result`
- `ELSE`
- `(float[]) result array`

**Description**

Returns the result of interpolation between a and b, using x as the interpolation value.
Interpolation method is parabolic.
Direct formula is (a + (b - a) * (x^2))
Represented by the purple graph.
When interpolating between two arrays, they must be of the same size.

#### `Interpolate_Decelerate`

**Arguments**

- a
- b
- x

**Returns**

- `IF a, b are not arrays`
- `(float) result`
- `ELSE`
- `(float[]) result array`

**Description**

Returns the result of interpolation between a and b, using x as the interpolation value.
Interpolation method is inverse parabolic.
Direct formula is (a + (b - a) * (1 - (1 - x)^2))
Represented by the black graph.
When interpolating between two arrays, they must be of the same size.

#### `Interpolate_Modulate`

**Arguments**

- a
- b
- c
- x

**Returns**

- `(float) result`

**Description**

Returns the result of interpolation between a and b, using x as the interpolation value.
Interpolation method is sine.
Direct formula is (a + (b - a) * (x + (rsin(2pi * x) / (2pi)) * c))
Represented by the dotted red graph.

#### `Interpolate_Overshoot`

**Arguments**

- a
- b
- c
- x

**Returns**

- `(float) result`

**Description**

Returns the result of interpolation between a and b, using x as the interpolation value.
Interpolation method is half sine.
Direct formula is (a + (b - a) * (x + (rsin(pi * x) / pi) * c))
Represented by the dotted blue graph.

#### `Interpolate_QuadraticBezier`

**Arguments**

- a
- b
- c
- x

**Returns**

- `(float) result`

**Description**

Calculates a coordinate component of a quadratic Bezier curve.
Direct formula is ((a * (1 - x)^2) + (2 * c * x * (1 - x)) + (b * x^2))
Represented by the red graph.

#### `Interpolate_CubicBezier`

**Arguments**

- a
- b
- c1
- c2
- x

**Returns**

- `(float) result`

**Description**

Calculates a coordinate component of a cubic Bezier curve.
Direct formula is ((a * (1 - x)^3) + (3 * c1 * x * (1 - x)^2) + (3 * c2 * x^2 * (1 - x)) + (b * x^3))
Represented by the blue graph.

#### `Interpolate_Hermite`

**Arguments**

- start x
- start y
- end x
- end y
- starting point tangent vector magnitude
- starting point tangent vector angle (degrees)
- ending point tangent vector magnitude
- ending point tangent vector angle (degrees)
- x

**Returns**

- `(float[2]) [x, y]`

**Description**

Calculates the position along the given cubic Hermite spline curve.
https://en.wikipedia.org/wiki/Cubic_Hermite_spline

#### `Interpolate_X`

**Arguments**

- a
- b
- x
- `int` interpolation type

**Returns**

- `IF a, b are not arrays`
- `(float) result`
- `ELSE`
- `(float[]) result array`

**Description**

Interpolates between a and b using x with the given interpolation type.

**Available interpolation types:**

**LERP_LINEAR:**

Interpolate_Linear

**LERP_SMOOTH:**

Interpolate_Smooth

**LERP_SMOOTHER:**

Interpolate_Smoother

**LERP_ACCELERATE:**

Interpolate_Accelerate

**LERP_DECELERATE:**

Interpolate_Decelerate

#### `Interpolate_X_PackedInt`

**Arguments**

- `int` a
- `int` b
- x
- `int` interpolation type

**Returns**

- `(int) result`

**Description**

Divides a and b (both 8-byte ints) into eight 1-byte chunks, performs interpolation on each pair, and recombines them to return the result.

**Example**

```dnh
Interpolate_X_PackedInt(0x37ff4040, 0xaa001010, 0.5, LERP_LINEAR);        //result = 0x707f2828
```

#### `Interpolate_X_Angle`

**Arguments**

- a (degrees)
- b (degrees)
- x
- `int` interpolation type

**Returns**

- `(float) angle (degrees)`

**Description**

Interpolates between angles a and b using x with the given interpolation type.

**Equivalent to:**

NormalizeAngle(a + Interpolate_X(0, AngularDistance(a, b), x, type));

#### `Interpolate_X_AngleR`

**Arguments**

- a (radians)
- b (radians)
- x
- `int` interpolation type

**Returns**

- `(float) angle (radians)`

**Description**

Interpolates between angles a and b using x with the given interpolation type.

**Equivalent to:**

NormalizeAngleR(a + Interpolate_X(0, AngularDistanceR(a, b), x, type));

#### `Interpolate_X_Array`

**Arguments**

- `T[]` values
- x
- `int` interpolation type

**Returns**

- `(float) result`

**Description**

Interpolates between multiple elements of the array.

**Examples:**

Interpolate_X_Array([a, b, c, d], 0.2, LERP_LINEAR);
= Interpolate_X(a, b, 0.2, LERP_LINEAR);
Interpolate_X_Array([a, b, c, d], 1.5, LERP_LINEAR);
= Interpolate_X(b, c, 0.5, LERP_LINEAR);
Interpolate_X_Array([a, b, c, d], 2.9, LERP_LINEAR);
= Interpolate_X(c, d, 0.9, LERP_LINEAR);

### Random

#### `rand_int`

**Arguments**

- `int` min
- `int` max

**Returns**

- `(int) random integer`

**Description**

Returns a random integer within the range [a, b].
rand_int(a, b) = as_int(rand(a, b + 0.99999))

#### `prand`

**Arguments**

- min
- max

**Returns**

- `(float) random value`

**Description**

Exactly like rand(), but the seed of which doesn't get saved to replays, and does not affect the state of rand().

#### `prand_int`

**Arguments**

- `int` min
- `int` max

**Returns**

- `(int) random integer`

**Description**

Returns a random integer within the range [a, b] using the prand() function.
prand_int(a, b) = as_int(prand(as_int(a), as_int(b) + 0.99999))

#### `psrand`

**Arguments**

- `int` seed

**Description**

Sets the seed for prand and prand_int.

#### `count_rand`

**Returns**

- `(int) count`

**Description**

Returns the amount of number rand or rand_int have been used.
Resets when restarting the game or exiting to the script select menu.

#### `count_prand`

**Returns**

- `(int) count`

**Description**

Returns the amount of number prand or prand_int have been used.
Resets when restarting the game or exiting to the script select menu.

#### `reset_count_rand`

**Description**

Sets the rand count to 0.

#### `reset_count_prand`

**Description**

Sets the prand count to 0.

### Rotation Maths

#### `Rotate2D`

**Arguments**

- x
- y
- angle

**Returns**

- `(float[2]) rotated position [x, y]`

**Description**

Rotates the given (x, y) point around the origin (0, 0).

**Direct formula is:**

x = x * cos(angle) - y * sin(angle)
y = x * sin(angle) + y * cos(angle)

#### `Rotate2D (overload)`

**Arguments**

- x
- y
- angle
- origin x
- origin y

**Returns**

- `(float[2]) rotated position [x, y]`

**Description**

Overloaded with 5 arguments.
Rotates the given (x, y) point around the given origin.

**Direct formula is:**

_x = x - ox
_y = y - oy
x = ox + (_x * cos(angle) - _y * sin(angle))
y = oy + (_x * sin(angle) + _y * cos(angle))

#### `Rotate3D`

**Arguments**

- x
- y
- z
- X angle
- Y angle
- Z angle

**Returns**

- `(float[3]) rotated position [x, y, z]`

**Description**

Rotates the given (x, y, z) point around the origin (0, 0) with the given Euler angles.

**Direct formula is:**

x = x * (cos(angY) * cos(angZ) - sin(angX) * sin(angY) * sin(angZ)) + y * (-cos(angX) * sin(angZ)) + z * (sin(angY) * cos(angZ) + sin(angX) * cos(angY) * sin(angZ))
y = x * (cos(angY) * sin(angZ) + sin(angX) * sin(angY) * cos(angZ)) + y * (cos(angX) * cos(angZ)) + z * (sin(angY) * sin(angZ) - sin(angX) * cos(angY) * cos(angZ))
z = x * (-cos(angX) * sin(angY)) + y * (sin(angX)) + z * (cos(angX) * cos(angY))

#### `Rotate3D (overload)`

**Arguments**

- x
- y
- z
- X angle
- Y angle
- Z angle
- origin x
- origin y
- origin z

**Returns**

- `(float[3]) rotated position [x, y, z]`

**Description**

Rotates the given (x, y, z) point around the given origin with the given Euler angles.

**Direct formula is:**

_x = x - ox
_y = y - oy
_z = z - oz
x = ox + (_x * (cos(angY) * cos(angZ) - sin(angX) * sin(angY) * sin(angZ)) + _y * (-cos(angX) * sin(angZ)) + _z * (sin(angY) * cos(angZ) + sin(angX) * cos(angY) * sin(angZ)))
y = oy + (_x * (cos(angY) * sin(angZ) + sin(angX) * sin(angY) * cos(angZ)) + _y * (cos(angX) * cos(angZ)) + _z * (sin(angY) * sin(angZ) - sin(angX) * cos(angY) * cos(angZ)))
z = oz + (_x * (-cos(angX) * sin(angY)) + _y * (sin(angX)) + _z * (cos(angX) * cos(angY)))

### String

#### `atoi (overload)`

**Arguments**

- `string` value
- `int` base

**Returns**

- `(int) value`

**Description**

Overloaded with 2 arguments.
Converts the string into an integer with the specified base.

**Examples:**

atoi("3511", 10);		//3511
atoi("3511", 16);		//13585
atoi("0x7f3", 16);		//2035

#### `SplitString2`

**Arguments**

- `string` source string
- `string` pattern

**Returns**

- `(string[]) split result`

**Description**

Splits the string at the given pattern.

**Examples:**

SplitString2("ab3cd3ef3g", "3");
//Result: ["ab", "cd", "ef", "g"]
SplitString2("1b23abc45a6abc789c", "abc");
//Result: ["1b23", "45a6", "789c"]
SplitString("1b23abc45a6abc789c", "abc");
//Result: ["1", "23", "", "", "45", "6", "", "", "789", ""]

#### `StringFormat`

**Arguments**

- `string` source string
- `string` argument types
- `T` format arguments...

**Returns**

- `(string) formatted string`

**Description**

Formats the string in the style of sprintf.
The number of format arguments must match the number of argument types.
Make sure that the amount of format arguments match the formatting tokens in the source string,
the game may crash if the formatting token count exceeds the format argument count.

**Available argument types:**

- d:	Argument is passed a 32-bit integer (int).
- l:	Argument is passed a 64-bit integer (long int).
- f:	Argument is passed a floating point number (double).
- s:	Argument is passed a string (wstring).

**Example**

```dnh
StringFormat("This is a %s", "s", "string.");
//Result: This is a string.

StringFormat("The value is %s", "s", 32);
//Result: The value is 32.000000

StringFormat("%f + %d = %.2f", "fdf", 1.15836, 2.12, 3.27836);
//Result: 1.158360 + 2 = 3.28

StringFormat("0x%08x is %d", "ll", 8023465, 8023465);
//Result: 0x007a6da9 is 8023465

StringFormat("0x%016llx %s %lld", "lsl", 2275101568023465, "is", 2275101568023465);
//Result: 0x000815315cca3ba9 is 2275101568023465
```

#### `RegexMatch`

**Arguments**

- `string` source string
- `string` pattern

**Returns**

- `(string[]) match groups`

**Description**

Uses regular expression to match patterns in a given string.
Returns a string array of matches.

**Example**

```dnh
RegexMatch("Voyage 1969", "[0-9]+");
//Result: ["1969"]

RegexMatch("S231 A001 S101 S000", "S([0-9]{3})");
//Result: "S231", "231"]
```

#### `RegexMatchRepeated`

**Arguments**

- `string` source string
- `string` pattern

**Returns**

- `(string[][]) array of match groups`

**Description**

Behaves like RegexMatch, but returns all matches in the string rather than only the first match.

**Example**

```dnh
RegexMatchRepeated("Voyage 1969", "[0-9]+");
//Result: [["1969"]]

RegexMatchRepeated("S231 A001 S101 S0040", "S([0-9]{3})");
//Result: [["S231", "231"], ["S101", "101"], ["S004", "004"]]
```

#### `RegexReplace`

**Arguments**

- `string` source string
- `string` pattern
- `string` replacing string

**Returns**

- `(string) replace result`

**Description**

Uses regular expression to replace patterns in a given string.
Returns the pattern-replaced string.

**Example**

```dnh
RegexReplace("Voyage 1969", "[0-9]+", "1970");
//Result: "Voyage 1970"

RegexReplace("Example String", "(a|e|i)", "*");
//Result: "Ex*mpl* Str*ng"

RegexReplace("Red, Green, and Blue", "[A-Z]+[a-zA-Z]*", "\'$&\'");
//Result: "'Red', 'Green', and 'Blue'"
```

### File Path and Archives

#### `GetArchiveFilePathList`

**Arguments**

- `string` archive path
- `bool` print full path

**Returns**

- `(string[]) file names of stored entries`

**Description**

Returns an array of all filepaths stored inside the specified archive.
The archive must have already been loaded with AddArchiveFile, otherwise an empty array will be returned.

#### `GetWorkingDirectory`

**Returns**

- `(string) working directory`

**Description**

Returns the working directory of the engine.
The working directory is the directory in which the engine is operating in,
and will always be in the same directory as the exe's if the engine was started normally.
But there are some cases where the working directory won't necessary be the same as the exe's, such as when the game is started via Steam.

#### `GetModuleName`

**Returns**

- `(string) module name`

**Description**

Returns the file name of the engine, without the extension.

#### `GetFileDirectoryFromModule`

**Arguments**

- `string` path

**Returns**

- `(string) directory name`

**Description**

Returns the directory of the given file path, without the directory of the engine.
The given path does not need to actually exist.

#### `GetFileTopDirectory`

**Arguments**

- `string` path

**Returns**

- `(string) directory name`

**Description**

Returns the topmost directory of the given file path.
The given path does not need to actually exist.

#### `GetFileName`

**Arguments**

- `string` path

**Returns**

- `(string) file name`

**Description**

Returns the file name of the given file path.
The given path does not need to actually exist.

#### `GetFileNameWithoutExtension`

**Arguments**

- `string` path

**Returns**

- `(string) file name`

**Description**

Returns the file name of the given file path, without its extension.
The given path does not need to actually exist.

#### `GetFileExtension`

**Arguments**

- `string` path

**Returns**

- `(string) file extension`

**Description**

Returns the file extension of the given file path.
The given path does not need to actually exist.

#### `IsFileExists`

**Arguments**

- `string` path

**Returns**

- `(bool) result`

**Description**

Returns true if the given file exists.

#### `IsDirectoryExists`

**Arguments**

- `string` path

**Returns**

- `(bool) result`

**Description**

Returns true if the given directory exists.

### Time

#### `GetSystemTimeMilliS`

**Returns**

- `(int) time`

**Description**

Returns the time of the system clock in milliseconds.

#### `GetSystemTimeNanoS`

**Returns**

- `(int) time`

**Description**

Returns the time of the system clock in nanoseconds.

### Debugging

#### `WriteLog`

**Arguments**

- `T` value...

**Description**

Change.
Writes the values of the given arguments to the LogWindow.

#### `RaiseMessageWindow`

**Arguments**

- `string` title text
- `string` description text
- `int` window flags

**Returns**

- `(int) window result`

**Description**

Raises a message window similar to RaiseError, but does not terminate the script.
The game will be paused until the message window is addressed. The LogWindow will not be affected.
For the list of window flags, refer to this page:
https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-messagebox

**The following flags are not allowed and will be automatically deleted:**

MB_SETFOREGROUND
MB_DEFAULT_DESKTOP_ONLY
MB_SERVICE_NOTIFICATION
MB_SYSTEMMODAL
MB_TASKMODAL

**The following flags are forced:**

MB_APPLMODAL
The return value is dependent on the user's interaction with the message window.
Refer to the same page for the list and explaination of return values.

#### `RaiseMessageWindow (Overload)`

**Arguments**

- `string` title text
- `string` description text

**Returns**

- `(int) window result`

**Description**

Overloaded with 2 arguments.
Triggers RaiseMessageWindow with MB_OK as the window flag value.

### Common Data

#### `GetCommonData (Overload)`

**Arguments**

- `string` key

**Returns**

- `(T) stored value`

**Description**

Overloaded with 1 argument.
A variant of GetCommonData(key, default) where the "default value" argument is omitted.
A null value will be returned if the requested key-value pair does not exist.

#### `GetAreaCommonData (Overload)`

**Arguments**

- `string` area
- `string` key

**Returns**

- `(T) stored value`

**Description**

Overloaded with 2 arguments.
A variant of GetAreaCommonData(area, key, default) where the "default value" argument is omitted.
A null value will be returned if the requested key-value pair does not exist.

#### `DeleteWholeAreaCommonData`

**Arguments**

- `string` name

**Description**

Deletes the whole area common data, not just the key-value list.
To use the area common data again, it must be recreated with CreateCommonDataArea.

#### `LoadCommonDataValuePointer`

**Arguments**

- `string` key
- `T` initial value

**Returns**

- `(pointer) value pointer`

**Description**

Obtains a value pointer to the common data value in the default area.
If the requested common data does not exist, it will be created and initialized with the provided value.

#### `LoadCommonDataValuePointer (Overload)`

**Arguments**

- `string` key

**Returns**

- `(pointer) value pointer`

**Description**

Overloaded with 1 argument.
Obtains a value pointer to the common data value in the default area.
If the requested common data does not exist, a null pointer will be returned.

#### `LoadAreaCommonDataValuePointer`

**Arguments**

- `string` area
- `string` key
- `T` initial value

**Returns**

- `(pointer) value pointer`

**Description**

Obtains a value pointer to the common data value in the given area.
If the requested common data does not exist, it will be created and initialized with the provided value.

#### `LoadAreaCommonDataValuePointer (Overload)`

**Arguments**

- `string` area
- `string` key

**Returns**

- `(pointer) value pointer`

**Description**

Overloaded with 1 argument.
Obtains a value pointer to the common data value in the given area.
If the requested common data does not exist, a null pointer will be returned.

#### `IsValidCommonDataValuePointer`

**Arguments**

- `pointer` value pointer

**Returns**

- `(bool) valid`

**Description**

Returns if the given value is a valid common data value pointer.

#### `SetCommonDataPtr`

**Arguments**

- `pointer` value pointer
- `T` value

**Description**

SetCommonData/SetAreaCommonData using pointers.
The use of direct pointers mitigate the need for map traversals present in regular common data accesses,
and thus would remarkably improve speed.
Using an invalid pointer may cause a memory access violation.

#### `GetCommonDataPtr`

**Arguments**

- `pointer` value pointer
- `T` default value

**Returns**

- `(T) stored value`

**Description**

GetCommonData/GetAreaCommonData using pointers.
The use of direct pointers mitigate the need for map traversals present in regular common data accesses,
and thus would remarkably improve speed.
Using an invalid pointer may cause a memory access violation.

#### `GetCommonDataPtr (Overload)`

**Arguments**

- `pointer` value pointer

**Returns**

- `(T) stored value`

**Description**

Overloaded with 1 argument.
GetCommonData/GetAreaCommonData using pointers.
The use of direct pointers mitigate the need for map traversals present in regular common data accesses,
and thus would remarkably improve speed.
Using an invalid pointer may cause a memory access violation.

### Engine Utilities

#### `IsEngineFastMode`

**Returns**

- `(bool) result`

**Description**

Returns whether the engine is in the "fast mode" state.
(Note: Normally triggered with the LCtrl key.)

#### `GetConfigWindowSizeIndex`

**Returns**

- `(int) index`

**Description**

Returns the config's selected window size index.

#### `GetConfigWindowSizeList`

**Returns**

- `(int[][]) available window sizes`

**Description**

Returns an array of the window sizes specified in th_dnh.def.

#### `GetConfigVirtualKeyMapping`

**Arguments**

- `int` virtual key

**Returns**

- `(int[2]) keys mapping`

**Description**

Returns the key mapping [keyboard, pad] as set in the config.
Mappings will not change if keys are reassigned in-game.

#### `GetConfigWindowTitle`

**Returns**

- `(string) window title`

**Description**

Returns the engine's default window title, or the one specified in th_dnh.def if it exists.

#### `SetWindowTitle`

**Arguments**

- `string` window title

**Description**

Sets the window title.
Passing in an empty string will reset the window title to the default.

#### `SetEnableUnfocusedProcessing`

**Arguments**

- `bool` enables

**Description**

Enables or disables unfocused processing.

#### `GetLastFrameUpdateSpeed`

**Returns**

- `(int) milliseconds`

**Description**

Returns the time the engine took to update scripts in the previous frame in milliseconds.

#### `GetLastFrameRenderSpeed`

**Returns**

- `(int) milliseconds`

**Description**

Returns the time the engine took to render the previous frame in milliseconds.

### Stage Information

#### `SetScore`

**Arguments**

- `int` score

**Description**

Sets the score value.

#### `SetGraze`

**Arguments**

- `int` graze

**Description**

Sets the graze count.

#### `SetPoint`

**Arguments**

- `int` point

**Description**

Sets the default point item value.

### Archive

#### `AddArchiveFile (Overload)`

**Arguments**

- `string` path
- `int` offset

**Returns**

- `(bool) load successful`

**Description**

Overloaded with 2 arguments.
Loads a .dat archive, with the added capability to specify the read offset.

#### `GetArchiveFilePathList`

**Arguments**

- `string` path
- `bool` full path

**Returns**

- `(string[]) paths`

**Description**

Lists out the stored file entries in the given .dat archive.
Archive must have already been loaded.

### Screenshot

#### `SaveSnapShotA3`

**Arguments**

- `string` path
- `int` rect left
- `int` rect top
- `int` rect right
- `int` rect bottom
- `const` format

**Returns**

- `(bool) result`

**Description**

Renders the whole screen to a texture and saves the specified rectangular area of the texture to a file with the given name.
Returns true if the save was successful.

**Available formats:**

IFF_BMP: Bitmap file. 										(.bmp) (default)
IFF_JPG: Joint Photographic Experts Group graphics file. 	(.jpeg)
IFF_TGA: TARGA bitmap file. 								(.tga)
IFF_PNG: Portable Network Graphics file. 					(.png)
IFF_DDS: DirectDraw Surface file. 							(.dds)
IFF_PPM: Portable PixelMap graphics file. 					(.ppm)

### Script

#### `UnloadScript`

**Arguments**

- `int` script ID

**Description**

Unloads a script that was loaded with LoadScript or LoadScriptInThread.

#### `UnloadScriptFromCache`

**Arguments**

- `string` script path

**Description**

Unloads the given script from the script source cache.
Does nothing if the script does not exist in the cache.

#### `StartScript (Overload)`

**Arguments**

- `int` script ID
- `bool` unload

**Description**

Overloaded with 2 arguments.
Starts the given script, and whether or not to remove the script from the "Loaded Scripts" cache. (NOT the "Script Source" cache)
A script that is often restarted would benefit from this overload by using LoadScript only once,
and then using StartScript(idScript, false) to avoid having to repeatedly call LoadScript.
*StartScript(idScript) = StartScript(idScript, true)

#### `PauseScript`

**Arguments**

- `int` script ID
- `bool` pause

**Description**

Pauses or unpauses the specified script. Not guaranteed to work flawlessly.
Causes an error if the script ID is the ID of the script from which the function was called,
aka; "a script cannot pause itself".

#### `GetScriptStatus`

**Arguments**

- `int` script ID

**Returns**

- `(const) status`

**Description**

Returns the status of the specified script.

**Possible return values:**

- STATUS_LOADING	; Script is currently loading.
- STATUS_LOADED		; Script has been loaded.
- STATUS_RUNNING	; Script is running.
- STATUS_PAUSED		; Script has been paused.
- STATUS_CLOSING	; Script is closing.
- STATUS_INVALID	; Any other cases, or invalid script ID.

### Event

#### `GetEventArgumentCount`

**Returns**

- `(int) count`

**Description**

Returns the event argument count of the active event.

#### `NotifyEvent`

**Arguments**

- `int` script ID
- `int` event type
- `T` arguments...

**Returns**

- `(T) event result`

**Description**

Change.
It is possible to pass as many event arguments as you want, or none at all.
*Scripts paused with PauseScript will still be able to run events.

#### `NotifyEventOwn`

**Arguments**

- `int` event type
- `T` arguments...

**Returns**

- `(T) event result`

**Description**

NotifyEvent's the script in which it was called.
It is possible to pass as many event arguments as you want, or none at all.
*Scripts paused with PauseScript will still be able to run events.

#### `NotifyEventAll`

**Arguments**

- `int` event type
- `T` arguments...

**Description**

Change.
NotifyEvent's all actively running scripts.
It is possible to pass as many event arguments as you want, or none at all.
*Scripts paused with PauseScript will still be able to run events.

### Matrix

A "matrix" here is a 16-member array representing a 4x4 matrix arranged row-by-row.
Ex: [1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1] (The identity matrix)
A "vector" here is a float value array of the specified size.
Ex: [3, 4, 5] 		-> VECTOR3
[5, 5, 0, 2]	-> VECTOR4

#### `MatrixIdentity`

**Returns**

- `(matrix) the identity matrix`

**Description**

Contructs an identity matrix.

#### `MatrixInverse`

**Arguments**

- `matrix` matrix

**Returns**

- `(matrix) result`

**Description**

Inverses the given matrix.

#### `MatrixAdd`

**Arguments**

- `matrix` matrix1
- `matrix` matrix2

**Returns**

- `(matrix) result`

**Description**

Adds two matrices together.
Note: Directly using the "+" operator also works.

#### `MatrixSubtract`

**Arguments**

- `matrix` matrix1
- `matrix` matrix2

**Returns**

- `(matrix) result`

**Description**

Subtracts two matrices.
Note: Directly using the "-" operator also works.

#### `MatrixMultiply`

**Arguments**

- `matrix` matrix1
- `matrix` matrix2

**Returns**

- `(matrix) result`

**Description**

Multiplies two matrices together.

#### `MatrixDivide`

**Arguments**

- `matrix` matrix1
- `matrix` matrix2

**Returns**

- `(matrix) result`

**Description**

Divides two matrices.

#### `MatrixTranspose`

**Arguments**

- `matrix` matrix

**Returns**

- `(matrix) result`

**Description**

Transposes the given matrix.

#### `MatrixDeterminant`

**Arguments**

- `matrix` matrix

**Returns**

- `(float) result`

**Description**

Calculates the determinant of the given matrix.

#### `MatrixLookAtLH`

**Arguments**

- `VECTOR3` eye vector
- `VECTOR3` destination vector
- `VECTOR3` orientation vector

**Returns**

- `(matrix) result`

**Description**

Constructs a left-handed look-at matrix.

#### `MatrixLookAtRH`

**Arguments**

- `VECTOR3` eye vector
- `VECTOR3` destination vector
- `VECTOR3` orientation vector

**Returns**

- `(matrix) result`

**Description**

Constructs a right-handed look-at matrix.

#### `MatrixTransformVector`

**Arguments**

- `VECTOR3` position
- `matrix` transformation matrix

**Returns**

- `(VECTOR4) result`

**Description**

Transforms the given (x, y, z, 1) point with the given matrix.

### Screen Status

#### `GetMonitorWidth`

**Returns**

- `(int) monitor width`

**Description**

Returns the width of the primary display monitor.

#### `GetMonitorHeight`

**Returns**

- `(int) monitor height`

**Description**

Returns the height of the primary display monitor.

#### `GetScreenWidth`

**Returns**

- `(int) screen width`

**Description**

Returns the width of the engine screen. (specified in th_dnh.def as "screen.width")

#### `GetScreenHeight`

**Returns**

- `(int) screen height`

**Description**

Returns the height of the engine screen. (specified in th_dnh.def as "screen.height")

#### `GetWindowedWidth`

**Returns**

- `(int) window width`

**Description**

Returns the width of the windowed mode.

#### `GetWindowedHeight`

**Returns**

- `(int) window width`

**Description**

Returns the height of the windowed mode.

#### `IsFullscreenMode`

**Returns**

- `(bool) result`

**Description**

Returns whether or not the game is in fullscreen mode.
Both pseudo and true fullscreen modes register as true.

### Sound

#### `SetSoundDivisionVolumeRate`

**Arguments**

- `const` division
- rate

**Description**

Sets the volume rate of the given sound division.
The volume rates of all sound objects assigned to the division are multiplied by the division's rate.

#### `GetSoundDivisionVolumeRate`

**Arguments**

- `const` division

**Returns**

- `(float) volume rate`

**Description**

Returns the volume rate of the given sound division.

### Input

#### `GetVirtualKeyMapping`

**Arguments**

- `int` virtual key

**Returns**

- `(int[2]) keys mapping`

**Description**

Returns the current key mapping [keyboard, pad].

### Resource Loading

#### `LoadTextureEx`

**Arguments**

- `string` path
- `bool` use mipmap
- `bool` use non-power-of-two

**Returns**

- `(bool) loading result`

**Description**

Returns a boolean value of whether loading was successful.
Loads a texture with more options.
Mipmap option generates a mipmap chain, causes textures to look better when scaled down when used with mipmap filtering.
Non-power-of-two option prevents blurring when the source texture's dimensions are not powers of two. (Due to DirectX's nature, may cause rendering to be slightly slower.)

#### `LoadTextureInLoadThreadEx`

**Arguments**

- `string` path
- `bool` use mipmap
- `bool` use non-power-of-two

**Returns**

- `(bool) loading result`

**Description**

Returns a boolean value of whether initial loading was successful.
LoadTextureEx in the load thread.

#### `LoadShader`

**Arguments**

- `string` path

**Returns**

- `(bool) loading result`

**Description**

Loads a shader file, and returns a boolean value of whether loading was successful.

#### `RemoveShader`

**Arguments**

- `string` path

**Description**

Removes a shader data that was previously loaded with LoadShader.

#### `LoadMesh`

**Arguments**

- `string` path

**Returns**

- `(bool) loading result`

**Description**

Loads a mesh data file, and returns a boolean value of whether loading was successful.

#### `RemoveMesh`

**Arguments**

- `string` path

**Description**

Removes a mesh data that was previously loaded with LoadMesh.

### Render Target

#### `CreateRenderTargetEx`

**Arguments**

- `string` name
- `int` width
- `int` height

**Returns**

- `(bool) creation result`

**Description**

Returns a boolean value of whether creation was successful.
Creates a render target with the specified name, width, and height.
Fails if either value is not a positive integer.

#### `ClearRenderTargetA1`

**Arguments**

- `string` name

**Returns**

- `(bool) clear result`

**Description**

Previously [SetRenderTarget].
Returns a boolean value of whether the operation was successful.
Clears the render target.

#### `ClearRenderTargetA2`

**Arguments**

- `string` name
- `int` clear color red
- `int` clear color green
- `int` clear color blue
- `int` clear color alpha

**Returns**

- `(bool) clear result`

**Description**

Returns a boolean value of whether the operation was successful.
Clears the render target and fills it with the specified color and alpha.

#### `ClearRenderTargetA3`

**Arguments**

- `string` name
- `int` clear color red
- `int` clear color green
- `int` clear color blue
- `int` clear color alpha
- `int` rect left
- `int` rect top
- `int` rect right
- `int` rect bottom

**Returns**

- `(bool) clear result`

**Description**

Returns a boolean value of whether the operation was successful.
Clears the render target in the specified rectangular area and fills that area with the specified color and alpha.

#### `SaveRenderedTextureA3`

**Arguments**

- `string` name
- `string` path
- `int` rect left
- `int` rect top
- `int` rect right
- `int` rect bottom
- `const` format

**Returns**

- `(bool) save result`

**Description**

SaveRenderedTextureA2, with the added ability of image format specification.

**Available formats are:**

IFF_BMP: Bitmap file. 										(.bmp) (default)
IFF_JPG: Joint Photographic Experts Group graphics file. 	(.jpeg)
IFF_TGA: TARGA bitmap file. 								(.tga)
IFF_PNG: Portable Network Graphics file. 					(.png)
IFF_DDS: DirectDraw Surface file. 							(.dds)
IFF_PPM: Portable PixelMap graphics file. 					(.ppm)

### Device Capabilities

#### `IsVertexShaderSupported`

**Arguments**

- `int` major
- `int` minor

**Returns**

- `(bool) result`

**Description**

Checks if the video card on the device supports the given vertex shader version.

### Fog

#### `SetFogParam (Overload)`

**Arguments**

- fog start
- fog end
- `int` fog hex color

**Description**

Overloaded with 3 arguments.
Sets the fog color as an XRGB hexadecimal color value.

**Example**

```dnh
SetFogParam(0, 1024, 0xFF7B32);        //-> SetFogParam(0, 1024, 255, 123, 50);
```

### 3D Camera

#### `SetCameraMode`

**Arguments**

- `const` mode

**Description**

Switches the 3D camera between available modes.

**Modes:**

CAMERA_NORMAL
- The normal 3D camera mode, positioned with elevation and azimuth angles.
CAMERA_LOOKAT
- The new 3D camera mode for maths enthusiasts, with the ability to directly specify camera position and view target position.
Configured with SetCameraFocusX/Y/Z or SetCameraPosEye and SetCameraPosLookAt.

#### `SetCameraPosEye`

**Arguments**

- x
- y
- z

**Description**

Literally just SetCameraFocusXYZ with a different name.

#### `SetCameraPosLookAt`

**Arguments**

- x
- y
- z

**Description**

Used with CAMERA_LOOKAT mode.
Sets the camera's "look-at" position.

#### `GetCameraViewMatrix`

**Returns**

- `(matrix) view matrix`

**Description**

Returns the 3D camera's view matrix.

#### `GetCameraProjectionMatrix`

**Returns**

- `(matrix) projection matrix`

**Description**

Returns the 3D camera's projection matrix.

#### `GetCameraViewProjectionMatrix`

**Returns**

- `(matrix) view-projection matrix`

**Description**

Returns the 3D camera's view-projection matrix.

### Intersection

#### `IsIntersected_Point_Polygon`

**Arguments**

- point - x
- point - y
- `float[2][]` polygon vertices

**Returns**

- `(bool) intersection`

**Description**

Returns true if the given point intersects with the polygon.
The polygon can be convex, concave, or complex (self-intersecting).

#### `IsIntersected_Point_Circle`

**Arguments**

- point  - x
- point  - y
- circle - x
- circle - y
- circle - radius

**Returns**

- `(bool) intersection`

**Description**

Returns true if the given point intersects with the circle.

#### `IsIntersected_Point_Ellipse`

**Arguments**

- point   - x
- point   - y
- ellipse - x
- ellipse - y
- ellipse - x radius
- ellipse - y radius

**Returns**

- `(bool) intersection`

**Description**

Returns true if the given point intersects with the ellipse.

#### `IsIntersected_Point_Line`

**Arguments**

- point - x
- point - y
- line - start x
- line - start y
- line - end x
- line - end y
- line - width

**Returns**

- `(bool) intersection`

**Description**

Returns true if the given point intersects with the line.

#### `IsIntersected_Point_RegularPolygon`

**Arguments**

- point   - x
- point   - y
- polygon - x
- polygon - y
- polygon - radius
- `int` polygon - edge count
- polygon - angle (degrees)

**Returns**

- `(bool) intersection`

**Description**

Returns true if the given point intersects with the regular polygon.

#### `IsIntersected_Circle_Polygon`

**Arguments**

- circle - x
- circle - y
- circle - radius
- `float[2][]` polygon vertices

**Returns**

- `(bool) intersection`

**Description**

Returns true if the given circle intersects with the polygon.
The polygon can be convex, concave, or complex (self-intersecting).

#### `IsIntersected_Circle_Circle`

**Arguments**

- circle 1 - x
- circle 1 - y
- circle 1 - radius
- circle 2 - x
- circle 2 - y
- circle 2 - radius

**Returns**

- `(bool) intersection`

**Description**

Returns true if the given circles intersect.

#### `IsIntersected_Circle_Ellipse`

**Arguments**

- circle  - x
- circle  - y
- circle  - radius
- ellipse - x
- ellipse - y
- ellipse - x radius
- ellipse - y radius

**Returns**

- `(bool) intersection`

**Description**

Returns true if the given circle intersects with the ellipse.

#### `IsIntersected_Circle_RegularPolygon`

**Arguments**

- circle  - x
- circle  - y
- circle  - radius
- polygon - x
- polygon - y
- polygon - radius
- `int` polygon - edge count
- polygon - angle (degrees)

**Returns**

- `(bool) intersection`

**Description**

Returns true if the given circle intersects with the regular polygon.

#### `IsIntersected_Line_Polygon`

**Arguments**

- line - start x
- line - start y
- line - end x
- line - end y
- line - width
- `float[2][]` polygon vertices

**Returns**

- `(bool) intersection`

**Description**

Returns true if the given line intersects with the polygon.
The polygon can be convex, concave, or complex (self-intersecting).

#### `IsIntersected_Line_Circle`

**Arguments**

- line   - start x
- line   - start y
- line   - end x
- line   - end y
- line   - width
- circle - x
- circle - y
- circle - radius

**Returns**

- `(bool) intersection`

**Description**

Returns true if the given line intersects with the circle.

#### `IsIntersected_Line_Ellipse`

**Arguments**

- line    - start x
- line    - start y
- line    - end x
- line    - end y
- line    - width
- ellipse - x
- ellipse - y
- ellipse - x radius
- ellipse - y radius

**Returns**

- `(bool) intersection`

**Description**

Returns true if the given line intersects with the ellipse.

#### `IsIntersected_Line_Line`

**Arguments**

- line 1 - start x
- line 1 - start y
- line 1 - end x
- line 1 - end y
- line 1 - width
- line 2 - start x
- line 2 - start y
- line 2 - end x
- line 2 - end y
- line 2 - width

**Returns**

- `(bool) intersection`

**Description**

Returns true if the given lines intersect.

#### `IsIntersected_Line_RegularPolygon`

**Arguments**

- line    - start x
- line    - start y
- line    - end x
- line    - end y
- line    - width
- polygon - x
- polygon - y
- polygon - radius
- `int` polygon - edge count
- polygon - angle (degrees)

**Returns**

- `(bool) intersection`

**Description**

Returns true if the given line intersects with the regular polygon.

#### `IsIntersected_Polygon_Polygon`

**Arguments**

- `float[2][]` polygon 1 vertices
- `float[2][]` polygon 2 vertices

**Returns**

- `(bool) intersection`

**Description**

Returns true if the given polygons intersect.
The polygons can be convex, concave, or complex (self-intersecting).

#### `IsIntersected_Polygon_Ellipse`

**Arguments**

- `float[2][]` polygon vertices
- ellipse - x
- ellipse - y
- ellipse - x radius
- ellipse - y radius

**Returns**

- `(bool) intersection`

**Description**

Returns true if the given polygon intersects with the ellipse.
The polygon can be convex, concave, or complex (self-intersecting).

#### `IsIntersected_Polygon_RegularPolygon`

**Arguments**

- `float[2][]` polygon 1 vertices
- polygon 2 - x
- polygon 2 - y
- polygon 2 - radius
- `int` polygon 2 - edge count
- polygon 2 - angle (degrees)

**Returns**

- `(bool) intersection`

**Description**

Returns true if the given polygon intersects with the regular polygon.
The polygon can be convex, concave, or complex (self-intersecting).

### Color Utility

#### `ColorARGBToHex`

**Arguments**

- `int` alpha
- `int` red
- `int` green
- `int` blue

**Returns**

- `(int) hex color`

**Description**

Returns an ARGB hexadecimal color value of the given colors.

**Example**

```dnh
ColorARGBToHex(255, 255, 255, 255);
//Result: 0xFFFFFFFF

ColorARGBToHex(67, 255, 0, 125);
//Result: 0x43FF007D
```

#### `ColorARGBToHex (Overload)`

**Arguments**

- `int[4]` color

**Returns**

- `(int) hex color`

**Description**

Overloaded with 1 argument.
Returns ColorARGBToHex(color[0], color[1], color[2], color[3]);

#### `ColorHexToARGB`

**Arguments**

- `int` hex color

**Returns**

- `(int[4]) ARGB color`

**Description**

Returns an ARGB color array of the given hexadecimal color (D3DCOLOR).

**Example**

```dnh
ColorHexToARGB(0xFFFFFFFF);
//Result: [255, 255, 255, 255]

ColorHexToARGB(0x43FF007D);
//Result: [67, 255, 0, 125]
```

#### `ColorHexToARGB (Overload)`

**Arguments**

- `int` hex color
- `int` control

**Returns**

- `(int[]) color array`

**Description**

Overloaded with 2 arguments.
Returns an ARGB color array of the given hexadecimal color (D3DCOLOR), controlled by the control argument.
The control value dictates which color channels to output, and in which order.

**The bit format is:**

MSB                         LSB   (16 bit)
000000   00      00  00  00  00
Unused   Count   1st 2nd 3rd 4th
- Bits 0-5 are unused.
- Bits 6-7 control the amount of color channels to return, subtracted by 1.
(11 is 4 channels, 10 is 3, 01 is 2, 00 is 1)
- Bits 8-15 control the order of the color channels, duplication is allowed.
00 (0) signifies the A channel.
01 (1) signifies the R channel.
10 (2) signifies the G channel.
11 (3) signifies the B channel.

**Examples:**

0b1100011011 (0x31b) -> [A, R, G, B]
0b1101101100 (0x36c) -> [R, G, B, A]
0b1100000000 (0x300) -> [A, A, A, A]
0b1001101100 (0x26c) -> [R, G, B]
0b0001000000 (0x040) -> [R]
For people whose brain lacks the required amount of sanity (or lack thereof) to understand the control argument, here are some constants:
COLOR_PERMUTE_ARGB
COLOR_PERMUTE_RGBA
COLOR_PERMUTE_BGRA
COLOR_PERMUTE_RGB
COLOR_PERMUTE_BGR
COLOR_PERMUTE_A
COLOR_PERMUTE_R
COLOR_PERMUTE_G
COLOR_PERMUTE_B

**Example**

```dnh
ColorHexToARGB(0xFFFFFFFF, 0x31b);
//Result: [255, 255, 255, 255]

ColorHexToARGB(0xFFFFFFFF, 0x000);
//Result: [255]

ColorHexToARGB(0x43FF007D, 0x31b)
//Result: [67, 255, 0, 125]

ColorHexToARGB(0x43FF007D, 0x36c);
//Result: [255, 0, 125, 67]

ColorHexToARGB(0x43FF007D, 0x2e4);
//Result: [125, 0, 255]
```

#### `ColorRGBtoHSV`

**Arguments**

- `int` red
- `int` green
- `int` blue

**Returns**

- `(int[3]) color array`

**Description**

Returns the given RGB color as a HSV color array.

#### `ColorHexRGBtoHSV`

**Arguments**

- `int` hex color

**Returns**

- `(int[3]) color array`

**Description**

Returns the given XRGB hexadecimal color as a HSV color array.

#### `ColorHSVtoRGB`

**Arguments**

- `int` hue
- `int` saturation
- `int` value

**Returns**

- `(int[3]) color array`

**Description**

Returns the given HSV color as an RGB color array.

#### `ColorHSVtoHexRGB`

**Arguments**

- `int` hue
- `int` saturation
- `int` value

**Returns**

- `(int) hex color`

**Description**

Returns the given HSV color as an XRGB hexadecimal color.

### Base Object Functions

#### `SetInvalidPositionReturn`

**Arguments**

- x
- y

**Description**

Sets the default position that position-fetching functions will return if the object does not exist.
The default value is (0, 0).

#### `GetObjectDistance`

**Arguments**

- `int` object 1 ID
- `int` object 2 ID

**Returns**

- `(float) distance`

**Description**

Change.
The Z positions will not be included in the calculation if neither objects are 3D-valid.

#### `GetObjectDistanceSq`

**Arguments**

- `int` object 1 ID
- `int` object 2 ID

**Returns**

- `(float) distance`

**Description**

Returns the square of the distance between two objects.
The Z positions will not be included in the calculation if neither objects are 3D-valid.

#### `GetObjectDeltaAngle`

**Arguments**

- `int` object 1 ID
- `int` object 2 ID

**Returns**

- `(float) angle`

**Description**

Returns the angle between two objects.
The Z positions will not be included in the calculation if neither objects are 3D-valid.

#### `Obj_GetValueI`

**Arguments**

- `int` object ID
- `int` key

**Returns**

- `(T) value`

**Description**

Obj_GetValue with int value key.

#### `Obj_GetValueDI`

**Arguments**

- `int` object ID
- `int` key
- default

**Returns**

- `(T) value`

**Description**

Obj_GetValueD with int value key.

#### `Obj_SetValueI`

**Arguments**

- `int` object ID
- `int` key
- value

**Returns**

- `(T) value`

**Description**

Obj_SetValue with int value key.

#### `Obj_DeleteValueI`

**Arguments**

- `int` object ID
- `int` key

**Returns**

- `(T) value`

**Description**

Obj_DeleteValue with int value key.

#### `Obj_IsValueExistsI`

**Arguments**

- `int` object ID
- `int` key

**Returns**

- `(T) value`

**Description**

Obj_IsValueExists with int value key.

#### `Obj_GetValueCount`

**Arguments**

- `int` object ID

**Returns**

- `(int) count`

**Description**

Returns the number of stored values in the object's string-indexed value table.

#### `Obj_GetValueCountI`

**Arguments**

- `int` object ID

**Returns**

- `(int) count`

**Description**

Returns the number of stored values in the object's int-indexed value table.

#### `Obj_CopyValueTable`

**Arguments**

- `int` destination object ID
- `int` source object ID
- `int` mode

**Returns**

- `(int) source object's object value amount`

**Description**

Copies the source's object value table to the destination's object value table.

**Available modes:**

0 - Object value table is cleared before the copy.
1 - Value will be overwritten in case of key conflicts.
2 - Value will not be overwritten in case of key conflicts.

### Shader Object Functions

#### `ObjShader_SetShaderT`

**Arguments**

- `int` object ID
- `string` shader name
- `string` shader source

**Returns**

- `(bool) success`

**Description**

Compiles a shader from the given string. Returns whether the operation was successful.
Shader name should be unique.

### Render Object Functions

### Blending

#### `ObjRender_SetBlendType`

**Description**

Addition.
BLEND_ALPHA_INV - Alpha blending with source color inversion.

### Scale

#### `ObjRender_SetScaleXYZ (Overload)`

**Arguments**

- `int` object ID
- scale

**Description**

Overloaded with 2 arguments.
Sets the object's XYZ scale.

**Example**

```dnh
ObjRender_SetScaleXYZ(obj, 2);     //-> ObjRender_SetScaleXYZ(obj, 2, 2, 2);
```

### Color & Alpha

#### `ObjRender_SetColor (Overload)`

**Arguments**

- `int` object ID
- `int` hex color

**Description**

Overloaded with 2 arguments.
Sets the object's color as an XRGB hexadecimal color value.

**Example**

```dnh
ObjRender_SetColor(obj, 0xFF7B32);     //-> ObjRender_SetColor(obj, 255, 123, 50);
```

#### `ObjRender_SetColorHSV`

**Arguments**

- `int` object ID
- `int` color hue
- `int` color saturation
- `int` color value

**Description**

Sets the object's RGB color as a HSV color.

#### `ObjRender_GetColor`

**Arguments**

- `int` object ID

**Returns**

- `(int[3]) color`

**Description**

Returns an [r, g, b] array of the object's color.

#### `ObjRender_GetColorHex`

**Arguments**

- `int` object ID

**Returns**

- `(int) hex color`

**Description**

Returns the object's color an XRGB hexadecimal color value.

#### `ObjRender_GetAlpha`

**Arguments**

- `int` object ID

**Returns**

- `(int) alpha`

**Description**

Returns the object's alpha value.

### Texture Filtering

#### `ObjRender_SetTextureFilterMin`

**Arguments**

- `int` object ID
- `const` mode

**Description**

Sets the minification filtering mode for the render object.

**Filtering modes:**

FILTER_NONE
FILTER_POINT
FILTER_LINEAR
FILTER_ANISOTROPIC
Default is FILTER_LINEAR.

#### `ObjRender_SetTextureFilterMag`

**Arguments**

- `int` object ID
- `const` mode

**Description**

Sets the magnification filtering mode for the render object.
Default is FILTER_LINEAR.

#### `ObjRender_SetTextureFilterMip`

**Arguments**

- `int` object ID
- `const` mode

**Description**

Sets the mipmap filtering mode for the render object.
Default is FILTER_NONE.

#### `ObjRender_SetTextureFilter`

**Arguments**

- `int` object ID
- `const` filter min
- `const` filter mag
- `const` filter mip

**Description**

Fused ObjRender_SetTextureFilterMin+Mag+Mip.

### Vertex Shader

#### `ObjRender_SetVertexShaderRenderingMode`

**Arguments**

- `int` object ID
- `bool` use vertex shader mode

**Description**

Sets whether the render object will be rendered with support for vertex shaders.
Default is false.
Rendering will not be performed without an attached shader object when this is set to true.
When using vertex shader rendering mode with 3D objects, fog must be manually computed with the shader.

#### `ObjRender_SetEnableDefaultTransformMatrix`

**Arguments**

- `int` object ID
- `bool` enable

**Description**

Sets whether the render object will calculate its transformation matrix.
Default is true.
The transformation matrix contains the object's scale, angle, and position data, along with the 2D camera matrix.
If set to false, the transformation matrix will only contain the 2D camera matrix.

### Lighting

#### `ObjRender_SetLightingEnable`

**Arguments**

- `int` object ID
- `bool` lighting enable
- `bool` specular lighting enable

**Description**

Enables or disables the 3D directional lighting and specular lighting for the render object.
The default values are false and false.
For mesh objects, the default values are true and false.
If it is not necessary, I recommend you not use specular lighting, as it's quite computationally expensive.

#### `ObjRender_SetLightingDiffuseColor`

**Arguments**

- `int` object ID
- `int` r
- `int` g
- `int` b

**Description**

Sets the diffuse color of the lighting.
The default color value is (128, 128, 128).

#### `ObjRender_SetLightingDiffuseColor (Overload)`

**Arguments**

- `int` object ID
- `int` hex color

**Description**

Overloaded with 2 arguments.
Sets the object's diffuse lighting color as an XRGB hexadecimal color value.

#### `ObjRender_SetLightingSpecularColor`

**Arguments**

- `int` object ID
- `int` r
- `int` g
- `int` b

**Description**

Sets the specular color of the lighting.
The default color value is (0, 0, 0).

#### `ObjRender_SetLightingSpecularColor (Overload)`

**Arguments**

- `int` object ID
- `int` hex color

**Description**

Overloaded with 2 arguments.
Sets the object's specular lighting color as an XRGB hexadecimal color value.

#### `ObjRender_SetLightingAmbientColor`

**Arguments**

- `int` object ID
- `int` r
- `int` g
- `int` b

**Description**

Sets the ambient color of the lighting.
The default color value is (128, 128, 128).

#### `ObjRender_SetLightingAmbientColor (Overload)`

**Arguments**

- `int` object ID
- `int` hex color

**Description**

Overloaded with 2 arguments.
Sets the object's ambient lighting color as an XRGB hexadecimal color value.

#### `ObjRender_SetLightingDirection`

**Arguments**

- `int` object ID
- x component
- y component
- z component

**Description**

Sets the direction vector of the lighting.
Vector does not need to be normalized, but should have a non-zero length.
The default direction vector is (-1, -1, -1).

### Primitive Object Functions

#### `ObjPrim_GetTexture`

**Arguments**

- `int` object ID

**Returns**

- `(string) texture name`

**Description**

Returns the path/name of the primitive object's texture.

#### `ObjPrim_SetVertexColor (Overload)`

**Arguments**

- `int` object ID
- `int` vertex index
- `int` hex color

**Description**

Overloaded with 3 arguments.
Sets the vertex color as an XRGB hexadecimal color value.

#### `ObjPrim_SetVertexColorHSV`

**Arguments**

- `int` object ID
- `int` vertex index
- color hue
- color saturation
- color value

**Description**

Sets the vertex color of the primitive object as a HSV color.

#### `ObjPrim_GetVertexColor`

**Arguments**

- `int` object ID
- `int` vertex index

**Returns**

- `(int[3]) color`

**Description**

Returns the vertex color as an [r, g, b] array.

#### `ObjPrim_GetVertexColorHex`

**Arguments**

- `int` object ID
- `int` vertex index

**Returns**

- `(int) hex color`

**Description**

Returns the vertex color as an XRGB hexadecimal color value.

#### `ObjPrim_GetVertexAlpha`

**Arguments**

- `int` object ID
- `int` vertex index

**Returns**

- `(int) alpha`

**Description**

Returns the vertex alpha value.

#### `ObjPrim_SetVertexIndex`

**Arguments**

- `int` object ID
- `int[]` indices

**Description**

Sets vertex indices to the primitive object.
Required for particle list objects.

**Example**

```dnh
ObjPrim_SetPrimitiveType(obj, PRIMITIVE_TRIANGLELIST);
ObjPrim_SetVertexCount(obj, 4);
ObjPrim_SetVertexPosition(obj, 0, 0, 0);
ObjPrim_SetVertexPosition(obj, 1, 32, 0);
ObjPrim_SetVertexPosition(obj, 2, 0, 32);
ObjPrim_SetVertexPosition(obj, 3, 32, 32);
ObjPrim_SetVertexIndex(obj, [0, 1, 2, 1, 2, 3]);
```

### 2D Sprite Object Functions

#### `ObjSprite2D_SetSourceRect (overload)`

**Arguments**

- `int` object ID
- `int[4]` rect

**Description**

Overloaded with 2 arguments.
ObjSprite2D_SetSourceRect(obj, rect[0], rect[1], rect[2], rect[3]);

#### `ObjSprite2D_SetDestRect (overload)`

**Arguments**

- `int` object ID
- `float[4]` rect

**Description**

Overloaded with 2 arguments.
ObjSprite2D_SetDestRect(obj, rect[0], rect[1], rect[2], rect[3]);

### 2D Sprite List Object Functions

#### `ObjSpriteList2D_SetSourceRect (overload)`

**Arguments**

- `int` object ID
- `int[4]` rect

**Description**

Overloaded with 2 arguments.
ObjSpriteList2D_SetSourceRect(obj, rect[0], rect[1], rect[2], rect[3]);

#### `ObjSpriteList2D_SetDestRect (overload)`

**Arguments**

- `int` object ID
- `float[4]` rect

**Description**

Overloaded with 2 arguments.
ObjSpriteList2D_SetDestRect(obj, rect[0], rect[1], rect[2], rect[3]);

#### `ObjSpriteList2D_SetAutoClearVertexCount`

**Arguments**

- `int` object ID
- `bool` clear

**Description**

Sets whether the 2D sprite list object will clear its vertices upon rendering.

### 3D Sprite Object Functions

#### `ObjSprite3D_SetSourceRect (overload)`

**Arguments**

- `int` object ID
- `int[4]` rect

**Description**

Overloaded with 2 arguments.
ObjSprite3D_SetSourceRect(obj, rect[0], rect[1], rect[2], rect[3]);

#### `ObjSprite3D_SetDestRect (overload)`

**Arguments**

- `int` object ID
- `float[4]` rect

**Description**

Overloaded with 2 arguments.
ObjSprite3D_SetDestRect(obj, rect[0], rect[1], rect[2], rect[3]);

#### `ObjSprite3D_SetSourceDestRect (overload)`

**Arguments**

- `int` object ID
- `float[4]` rect

**Description**

Overloaded with 2 arguments.
ObjSprite3D_SetSourceDestRect(obj, rect[0], rect[1], rect[2], rect[3]);

### Particle List Object Functions

#### `ObjParticleList_Create`

**Arguments**

- `const` type

**Description**

Creates a new particle list object and returns its ID.

**Available types are:**

OBJ_PARTICLE_LIST_2D
OBJ_PARTICLE_LIST_3D
Vertex shader rendering is always on, and rendering will fail if an improper custom shader is used.
Not using a custom shader will result in the default shader being used.

#### `ObjParticleList_SetPosition`

**Arguments**

- `int` object ID
- X position
- Y position
- Z position

**Description**

Sets the instance position data.

#### `ObjParticleList_SetScaleX`

**Arguments**

- `int` object ID
- X scale

**Description**

Sets the instance X scale data.

#### `ObjParticleList_SetScaleY`

**Arguments**

- `int` object ID
- Y scale

**Description**

Sets the instance Y scale data.

#### `ObjParticleList_SetScaleZ`

**Arguments**

- `int` object ID
- Z scale

**Description**

Sets the instance Z scale data.

#### `ObjParticleList_SetScale`

**Arguments**

- `int` object ID
- X scale
- Y scale
- Z scale

**Description**

Sets the instance scale data.

#### `ObjParticleList_SetScale (Overload)`

**Arguments**

- `int` object ID
- scale

**Description**

Overloaded with 2 arguments.
Sets the instance scale data.
ObjParticleList_SetScale(obj, 2);  //->  ObjParticleList_SetScale(obj, 2, 2, 2);

#### `ObjParticleList_SetAngleX`

**Arguments**

- `int` object ID
- X angle

**Description**

Sets the instance X angle data.

#### `ObjParticleList_SetAngleY`

**Arguments**

- `int` object ID
- Y angle

**Description**

Sets the instance Y angle data.

#### `ObjParticleList_SetAngleZ`

**Arguments**

- `int` object ID
- Z angle

**Description**

Sets the instance Z angle data.

#### `ObjParticleList_SetAngleXYZ`

**Arguments**

- `int` object ID
- X angle
- Y angle
- Z angle

**Description**

Sets the instance XYZ angle data.

#### `ObjParticleList_SetColor`

**Arguments**

- `int` object ID
- `int` red
- `int` green
- `int` blue

**Description**

Sets the instance RGB color data.

#### `ObjParticleList_SetColor (Overload)`

**Arguments**

- `int` object ID
- `int` hex color

**Description**

Overloaded with 2 arguments.
Sets the instance color data as an XRGB hexadecimal color value.

#### `ObjParticleList_SetAlpha`

**Arguments**

- `int` object ID
- `int` alpha

**Description**

Sets the instance alpha data.

#### `ObjParticleList_SetExtraData`

**Arguments**

- `int` object ID
- data 1
- data 2
- data 3

**Description**

Sets instance extra data.
These can be freely utilized by the scripters in case a custom shader is used.

#### `ObjParticleList_AddInstance`

**Arguments**

- `int` object ID

**Description**

Submits the current data to the next instance.

#### `ObjParticleList_ClearInstance`

**Arguments**

- `int` object ID

**Description**

Clears all previously submitted instance data of the current frame.

### Mesh Object Functions

#### `ObjMesh_SetColor (Overload)`

**Arguments**

- `int` object ID
- `int` hex color

**Description**

Overloaded with 2 arguments.
Sets the mesh object's color as an XRGB hexadecimal color value.

### Text Object Functions

#### `ObjText_GetText`

**Arguments**

- `int` object ID

**Returns**

- `(string) text`

**Description**

Returns the text of the text object.

#### `ObjText_SetFixedWidth`

**Arguments**

- `int` object ID
- `int` width

**Description**

Activates and sets the width of monospacing mode for the text object.
If width is <= 0, the spacing mode returns to regular dynamic spacing.

#### `ObjText_SetFontWeight`

**Arguments**

- `int` object ID
- `int` weight

**Description**

Sets the font weight for the following text object.
Weight ranges from 0 to 1000, with 0 being the lightest and 1000 being the thickest. -1 for the default weight.
Some fonts may not support all weight values.
*ObjText_SetFontBold(a, b) is equivalent to ObjText_SetFontWeight(a, b ? 700 : 400).

#### `ObjText_SetFontCharacterSet`

**Arguments**

- `int` object ID
- `int` charset

**Description**

Sets the charset for the following text object.

**The following constants are defined:**

CHARSET_ANSI
CHARSET_DEFAULT
CHARSET_SHIFTJIS
CHARSET_HANGUL
CHARSET_JOHAB
CHARSET_CHINESEBIG5
CHARSET_TURKISH
CHARSET_VIETNAMESE
CHARSET_HEBREW
CHARSET_ARABIC
CHARSET_THAI
For the full list of all charset values, refer to here:
https://docs.microsoft.com/en-us/openspecs/windows_protocols/ms-wmf/0d0b32ac-a836-4bd2-a112-b6000a1b4fc9
Avoid using CHARSET_DEFAULT, as it may be device-dependent.
To ensure that font glyphs generate correctly, match the text object's charset with the font's charset.
*The font charset is reset to either CHARSET_ANSI or CHARSET_SHIFTJIS every time ObjText_SetFontType is used.

#### `ObjText_SetFontColorTop (Overload)`

**Arguments**

- `int` object ID
- `int` hex color

**Description**

Overloaded with 2 arguments.
Sets the text object's font's top color as an XRGB hexadecimal color value.

#### `ObjText_SetFontColorBottom (Overload)`

**Arguments**

- `int` object ID
- `int` hex color

**Description**

Overloaded with 2 arguments.
Sets the text object's font's bottom color as an XRGB hexadecimal color value.

#### `ObjText_SetFontBorderColor (Overload)`

**Arguments**

- `int` object ID
- `int` hex color

**Description**

Overloaded with 2 arguments.
Sets the text object's font's border color as an XRGB hexadecimal color value.

#### `ObjText_SetVertexColor (Overload)`

**Arguments**

- `int` object ID
- `int` hex color

**Description**

Overloaded with 2 arguments.
Sets the text object's color as an ARGB hexadecimal color value.

### Sound Object Functions

#### `ObjSound_SetResumeEnable`

**Arguments**

- `int` object ID
- `bool` resume enable

**Description**

Formerly ObjSound_SetRestartEnable.
Function unchanged.
If set to true, using ObjSound_Play on the sound object that was previously stopped with ObjSound_Stop will resume playing where it last left off.

#### `ObjSound_Seek`

**Arguments**

- `int` object ID
- time

**Description**

Seeks to the specified time in the audio.

#### `ObjSound_SeekSampleCount`

**Arguments**

- `int` object ID
- `int` sample

**Description**

Seeks to the specified sample in the audio.

#### `ObjSound_SetFrequency`

**Arguments**

- `int` object ID
- `int` frequency

**Description**

Sets the frequency of the audio in sample rate.
Can be used to increase or decrease play speed.
An input of 0 will reset the value to the audio's original frequency.

#### `ObjSound_GetInfo`

**Arguments**

- `int` object ID
- `const` type

**Returns**

- `[varies]`

**Description**

Returns the value of the requested information about the sound object.

**List of available information types:**

INFO_FORMAT
- The format of the audio file.
- SOUND_UNKNOWN
- SOUND_WAVE
- SOUND_OGG
- SOUND_MP3
- SOUND_AWAVE
- SOUND_MIDI
INFO_CHANNEL
- The number of channels.
INFO_SAMPLE_RATE
- The sample rate of the audio in hertz.
- ObjSound_SetFrequency does not affect this value.
INFO_AVG_BYTE_PER_SEC
- The average byte per one second of audio.
INFO_BLOCK_ALIGN
- The block align. (channel * bytePerSample)
INFO_BIT_PER_SAMPLE
- The bits per sample of the audio.
INFO_POSITION
- The current reading position of the audio in seconds.
INFO_POSITION_SAMPLE
- The current reading position of the audio in sample count.
INFO_LENGTH
- The total length of the audio in seconds.
INFO_LENGTH_SAMPLE
- The total length of the audio in sample count.

### Text File Object Functions

#### `ObjFileT_SetLineText`

**Arguments**

- `int` object ID
- `int` line number
- `string` new text

**Description**

Sets the text of the given line of the text file object.
*Note that the actual file content is only updated upon calling ObjFile_Store.

### Binary File Object Functions

#### `ObjFileB_GetLastRead`

**Arguments**

- `int` object ID

**Returns**

- `(int) result`

**Description**

Returns the amount of bytes read at the last read operation.
A result of 0 may indicate an end-of-file.

#### `ObjFileB_ReadBoolean`

**Arguments**

- `int` object ID

**Returns**

- `(bool) result`

**Description**

Change.
No longer throws an error upon attempting to read past the end-of-file.
Reads a boolean value from the binary file object.
Advances the read pointer by 1.

#### `ObjFileB_ReadByte`

**Arguments**

- `int` object ID

**Returns**

- `(int) result`

**Description**

Change.
No longer throws an error upon attempting to read past the end-of-file.
Reads a byte value from the binary file object.
Advances the read pointer by 1.

#### `ObjFileB_ReadShort`

**Arguments**

- `int` object ID

**Returns**

- `(int) result`

**Description**

Change.
No longer throws an error upon attempting to read past the end-of-file.
Reads a short integer value from the binary file object.
Advances the read pointer by 2.

#### `ObjFileB_ReadInteger`

**Arguments**

- `int` object ID

**Returns**

- `(int) result`

**Description**

Change.
No longer throws an error upon attempting to read past the end-of-file.
Reads an integer value from the binary file object.
Advances the read pointer by 4.

#### `ObjFileB_ReadLong`

**Arguments**

- `int` object ID

**Returns**

- `(int) result`

**Description**

Change.
No longer throws an error upon attempting to read past the end-of-file.
Reads a long integer value from the binary file object.
Advances the read pointer by 8.

#### `ObjFileB_ReadFloat`

**Arguments**

- `int` object ID

**Returns**

- `(float) result`

**Description**

Change.
No longer throws an error upon attempting to read past the end-of-file.
Reads a single-precision float value from the binary file object.
Advances the read pointer by 4.

#### `ObjFileB_ReadDouble`

**Arguments**

- `int` object ID

**Returns**

- `(float) result`

**Description**

Change.
No longer throws an error upon attempting to read past the end-of-file.
Reads a double-precision float value from the binary file object.
Advances the read pointer by 8.

#### `ObjFileB_WriteBoolean`

**Arguments**

- `int` object ID
- `bool` value

**Returns**

- `(int) bytes written`

**Description**

Writes a boolean value to the binary file object at the current write pointer.
Advances the write pointer by 1.

#### `ObjFileB_WriteByte`

**Arguments**

- `int` object ID
- `int` value

**Returns**

- `(int) bytes written`

**Description**

Writes a boolean value to the binary file object at the current write pointer.
Advances the write pointer by 1.

#### `ObjFileB_WriteShort`

**Arguments**

- `int` object ID
- `int` value

**Returns**

- `(int) bytes written`

**Description**

Writes a short integer value to the binary file object at the current write pointer.
Advances the write pointer by 2.

#### `ObjFileB_WriteInteger`

**Arguments**

- `int` object ID
- `int` value

**Returns**

- `(int) bytes written`

**Description**

Writes an integer value to the binary file object at the current write pointer.
Advances the write pointer by 4.

#### `ObjFileB_WriteLong`

**Arguments**

- `int` object ID
- `int` value

**Returns**

- `(int) bytes written`

**Description**

Writes a long integer value to the binary file object at the current write pointer.
Advances the write pointer by 8.

#### `ObjFileB_WriteFloat`

**Arguments**

- `int` object ID
- value

**Returns**

- `(int) bytes written`

**Description**

Writes a single-precision float value to the binary file object at the current write pointer.
Advances the write pointer by 4.

#### `ObjFileB_WriteDouble`

**Arguments**

- `int` object ID
- value

**Returns**

- `(int) bytes written`

**Description**

Writes a double-precision float value to the binary file object at the current write pointer.
Advances the write pointer by 8.

## Stage script

### Basic Stage Functions

#### `SetStgFrame (Overload)`

**Arguments**

- `int` frame left
- `int` frame top
- `int` frame right
- `int` frame bottom
- `int` STG frame render priority - starting (inclusive)
- `int` STG frame render priority - ending (exclusive)
- `int` 2D camera render priority

**Description**

Overloaded with 7 arguments.
Additional ability to change the render priority of the 2D camera.

#### `SetIntersectionVisualization`

**Arguments**

- `bool` enable

**Description**

Enables active visualization of all intersection object hitboxes.

**Color codes:**

Green   : Player hitbox+grazebox
Blue    : Player shot
Cyan    : Player spell
Yellow  : Enemy hitbox+killbox
Red     : Enemy shot

#### `SetIntersectionVisualizationRenderPriority`

**Arguments**

- `int` render priority

**Description**

Sets the render priority of hitbox visualizations.
Default value is 68. (GetCameraFocusPermitPriorityI() - 1)

### Player Functions

#### `GetPlayerScriptID`

**Returns**

- `(int) script ID`

**Description**

Returns the script ID of the player object.

#### `GetPlayerAutoItemCollectLine`

**Returns**

- `(float) position`

**Description**

Returns the position of the player's item autocollection line.

#### `SetPlayerItemScope`

**Arguments**

- `int` value

**Description**

Sets the player's item collection scope radius.
Effect equivalent to CollectItemsInCircle.

#### `GetPlayerItemScope`

**Returns**

- `(float) scope size`

**Description**

Gets the player's item collection scope radius.

#### `SetPlayerInvincibleGraze`

**Arguments**

- `bool` enable

**Description**

Sets whether the player can graze during invincbility period.
True by default. All hail ZUN's engine.

#### `SetPlayerIntersectionEraseShot`

**Arguments**

- `bool` enable

**Description**

Sets whether shots colliding with the player would get automatically deleted.
True by default. All hail ZUN's engine. Again. LOUDER!

#### `SetPlayerStateEndEnable`

**Arguments**

- `bool` enable

**Description**

Sets whether STATE_END is permitted to activate.
If set to false, player life will continue below 0 upon each death without limit.
False by default if the main script is a package script.

#### `SetPlayerShootdownEventEnable`

**Arguments**

- `bool` enable

**Description**

Sets whether EV_PLAYER_SHOOTDOWN will be notified upon the player's death.
Setting it to false would also disable STATE_DOWN and STATE_END from ever occuring.

#### `SetPlayerRebirthPosition`

**Arguments**

- x
- y

**Description**

Sets the respawn point of the player.
Use REBIRTH_DEFAULT in either position to reset it to its default value.

#### `KillPlayer`

**Description**

Kills the player.

### Enemy Functions

#### `SetEnemyAutoDeleteClip`

**Arguments**

- `int` left
- `int` top
- `int` right
- `int` bottom

**Description**

Sets the margin that enemy objects are allowed to traverse past the edges of the STG field before getting automatically deleted.
Basically, SetShotAutoDeleteClip for enemies.

### Shot Functions

#### `CreateShotA2 (overload)`

**Arguments**

- x
- y
- speed
- angle
- acceleration
- max speed
- angular velocity
- `int` shot graphic ID
- `int` delay

**Returns**

- `(int) object ID`

**Description**

Overloaded with 9 arguments.
CreateShotA2 with an additional argument for angular velocity.

#### `CreateShotC1`

**Arguments**

- position x
- position y
- x speed
- y speed
- angular offset
- `int` shot graphic ID
- `int` delay

**Returns**

- `(int) object ID`

**Description**

Creates a shot object using the C-movement mode and returns its object ID.
C-movement pattern:
The C-movement pattern is essentially B-movement with a twist.
There is an additional "angle offset" property which acts as a rotation vector for the X and Y speeds.

**To be precise:**

realSpeedXY = Rotate2D(speedX, speedY, angleOffset);

#### `CreateShotC2`

**Arguments**

- position x
- position y
- x speed
- y speed
- x acceleration
- y acceleration
- x max speed
- y max speed
- angle offset
- angle offset acceleration
- `int` shot graphic ID
- `int` delay

**Returns**

- `(int) object ID`

**Description**

Creates a shot object using the C-movement mode and returns its object ID.

#### `CreateShotOC1`

**Arguments**

- `int` parent object
- x speed
- y speed
- angular offset
- `int` shot graphic ID
- `int` delay

**Returns**

- `(int) object ID`

**Description**

Creates a shot object using the C-movement mode on the position of the parent object and returns its object ID.

#### `GetAllShotID`

**Arguments**

- `const` type

**Returns**

- `(int[]) shot object IDs`

**Description**

Returns an array of all shot object IDs of the given type.

**Available types:**

TARGET_ALL
TARGET_PLAYER
TARGET_ENEMY

#### `GetShotDataInfoA1`

**Returns**

- `[varies]`

**Description**

Addition + Change.
No longer throws an error if the given shot graphic ID is invalid.
INFO_EXISTS         - Returns whether the given shot graphic ID exists.
INFO_PATH           - Returns the image path of the given shot graphic ID.
INFO_IS_FIXED_ANGLE - Returns whether the given shot graphic ID has had fixed_angle enabled.

#### `SetShotTextureFilter`

**Arguments**

- `const` filter min
- `const` filter mag

**Description**

Sets the min and mag texture filtering modes for rendering shot objects.
The default filtering modes are FILTER_LINEAR and FILTER_LINEAR.

### Item Functions

#### `SetItemAutoDeleteClip`

**Arguments**

- `int` left
- `int` top
- `int` right
- `int` bottom

**Description**

Sets the margin that item objects are allowed to traverse past the edges of the STG field before getting automatically deleted.
Basically, SetShotAutoDeleteClip for items.

#### `GetItemIdInCircleA1`

**Arguments**

- circle x
- circle y
- circle radius

**Returns**

- `(int[]) item object IDs`

**Description**

Returns the object ID of all item objects within the specified circle.

#### `GetItemIdInCircleA2`

**Arguments**

- circle x
- circle y
- circle radius
- `int` item type

**Returns**

- `(int[]) item object IDs`

**Description**

Returns the object ID of all item objects of the specified type within the specified circle.

#### `SetItemTextureFilter`

**Arguments**

- `const` filter min
- `const` filter mag

**Description**

Sets the min and mag texture filtering modes for rendering item objects.
The default filtering modes are FILTER_LINEAR and FILTER_LINEAR.

### Intersection Functions

#### `IsIntersected_Obj_Obj`

**Arguments**

- `int` object ID 1
- `int` object ID 2

**Returns**

- `(bool) intersection`

**Description**

Returns true if any of both object's hitboxes intersect at least once.

#### `IsIntersected_Obj_Obj_All`

**Arguments**

- `int` object ID 1
- `int` object ID 2

**Returns**

- `(bool) intersection`

**Description**

Returns true if all of both object's hitboxes intersect with any of each other's at least once.

### Move Object Functions

#### `ObjMove_SetSpeed`

**Description**

Addition.
Also works on B-pattern and C-pattern movements.

#### `ObjMove_SetAngle`

**Description**

Addition.
Also works on B-pattern and C-pattern movements.

#### `ObjMove_SetAcceleration`

**Description**

Addition.
Also works on B-pattern and C-pattern movements.

#### `ObjMove_SetMaxSpeed`

**Description**

Addition.
Also works on B-pattern and C-pattern movements.

#### `ObjMove_SetAngularVelocity`

**Description**

Addition.
Also works on C-pattern movement.

#### `ObjMove_SetAngularAcceleration`

**Arguments**

- `int` object ID
- angular acceleration

**Description**

Sets the move object's angular acceleration.
Works on A-pattern and C-pattern movements.

#### `ObjMove_SetAngularMaxVelocity`

**Arguments**

- `int` object ID
- angular max velocity

**Description**

Sets the move object's angular max velocity.
Works on A-pattern and C-pattern movements.

#### `ObjMove_SetSpeedX`

**Arguments**

- `int` object ID
- speed

**Description**

Sets the move object's X speed.
Works on A-pattern, B-pattern, and C-pattern movements.

#### `ObjMove_SetSpeedY`

**Arguments**

- `int` object ID
- speed

**Description**

Sets the move object's Y speed.
Works on A-pattern, B-pattern, and C-pattern movements.

#### `ObjMove_SetSpeedXY`

**Arguments**

- `int` object ID
- x speed
- y speed

**Description**

Sets the move object's X and Y speeds.
Works on A-pattern, B-pattern, and C-pattern movements.

#### `ObjMove_GetSpeedX`

**Arguments**

- `int` object ID

**Returns**

- `(float) x speed`

**Description**

Gets the move object's X speed.

#### `ObjMove_GetSpeedY`

**Arguments**

- `int` object ID

**Returns**

- `(float) y speed`

**Description**

Gets the move object's Y speed.

#### `ObjMove_SetDestAtFrame (Overload)`

**Arguments**

- `int` object ID
- target x
- target y
- `int` frame
- `const` interpolation mode

**Description**

Overloaded with 5 arguments.
Moves the object to the target position in the given frames using the specified interpolation mode.

**Available movement interpolation modes:**

**LERP_LINEAR:**

Linear interpolation.

**LERP_SMOOTH:**

Smooth interpolation.

**LERP_SMOOTHER:**

Smoother interpolation.

**LERP_ACCELERATE:**

Accelerating interpolation.

**LERP_DECELERATE:**

Decelerating interpolation.

#### `ObjMove_AddPatternA2 (overload)`

**Arguments**

- `int` object ID
- `int` frame
- speed
- angle
- acceleration
- max speed
- angular velocity

**Description**

Overloaded with 7 arguments.
Additional ability to specify angular velocity.

#### `ObjMove_AddPatternA3`

**Arguments**

- `int` object ID
- `int` frame
- speed
- angle
- acceleration
- max speed
- angular velocity
- `int` shot graphic ID

**Description**

Change.
Swapped max speed and angular velocity.

#### `ObjMove_AddPatternA4`

**Arguments**

- `int` object ID
- `int` frame
- speed
- angle
- acceleration
- max speed
- angular velocity
- `int` shot graphic ID
- `int` relative object ID

**Description**

Change.
Swapped max speed and angular velocity.

#### `ObjMove_AddPatternA5`

**Arguments**

- `int` object ID
- `int` frame
- speed
- angle
- acceleration
- max speed
- angular velocity
- angular acceleration
- angular max velocity
- `int` shot graphic ID
- `int` relative object ID

**Description**

Self-explanatory.

#### `ObjMove_AddPatternC1`

**Arguments**

- `int` object ID
- `int` frame
- x speed
- y speed
- angle offset

**Description**

Adds a C-movement pattern to the object that will activate after the specified frames after the function call.

#### `ObjMove_AddPatternC2`

**Arguments**

- `int` object ID
- `int` frame
- x speed
- y speed
- x acceleration
- y acceleration
- x max speed
- y max speed
- angle offset
- angle offset velocity

**Description**

Self-explanatory.

#### `ObjMove_AddPatternC3`

**Arguments**

- `int` object ID
- `int` frame
- x speed
- y speed
- x acceleration
- y acceleration
- x max speed
- y max speed
- angle offset
- angle offset velocity
- `int` shot graphic ID

**Description**

Self-explanatory.

#### `ObjMove_AddPatternC4`

**Arguments**

- `int` object ID
- `int` frame
- x speed
- y speed
- x acceleration
- y acceleration
- x max speed
- y max speed
- angle offset
- angle offset velocity
- angle offset acceleration
- angle offset max speed
- `int` shot graphic ID

**Description**

Self-explanatory.

#### `ObjMove_AddPatternD1`

**Arguments**

- `int` object ID
- `int` frame
- target x
- target y
- speed

**Description**

An AddPattern version of ObjMove_SetDestAtSpeed.

#### `ObjMove_AddPatternD2`

**Arguments**

- `int` object ID
- `int` frame
- target x
- target y
- `int` frame duration

**Description**

An AddPattern version of ObjMove_SetDestAtFrame.

#### `ObjMove_AddPatternD2 (overload)`

**Arguments**

- `int` object ID
- `int` frame
- target x
- target y
- `int` frame duration
- `const` interpolation mode

**Description**

Overloaded with 6 arguments.
An AddPattern version of ObjMove_SetDestAtFrame.

#### `ObjMove_AddPatternD3`

**Arguments**

- `int` object ID
- `int` frame
- target x
- target y
- weight
- max speed

**Description**

An AddPattern version of ObjMove_SetDestAtWeight.

#### `ObjMove_SetProcessMovement`

**Arguments**

- `int` object ID
- `bool` enable

**Description**

Sets whether the object will be allowed to execute movement-related routines.
*ObjMove_SetX/Y/Position would still alter the object's position.

#### `ObjMove_GetProcessMovement`

**Arguments**

- `int` object ID

**Returns**

- `(bool) result`

**Description**

Returns true if the object is allowed to execute movement-related routines.

#### `ObjMove_GetMovementType`

**Arguments**

- `int` object ID

**Returns**

- `(const) movement type`

**Description**

Returns the current movement type of the move object.

**Possible return values:**

MOVE_NONE		; The object doesn't have an active movement function. (May be caused by ObjMove_CancelMovement.)
MOVE_ANGLE		; A-pattern movement. (Angle)
MOVE_XY			; B-pattern movement. (XY)
MOVE_XY_ANGLE	; C-pattern movement. (XY-Angle)
MOVE_LINE		; D-pattern movement. (Line, ObjMove_SetDestAtXXX)
MOVE_OTHER		; Other movements.

#### `ObjMove_CancelMovement`

**Arguments**

- `int` object ID

**Description**

Cancels the object's current movement routine.

### Enemy Object Functions

#### `ObjEnemy_GetInfo`

**Description**

Addition.

**INFO_DAMAGE_PREVIOUS_FRAME:**

Returns the amount of damage the enemy received in the previous frame.

#### `ObjEnemy_SetAutoDelete`

**Arguments**

- `int` object ID
- `bool` auto delete

**Description**

ObjShot_SetAutoDelete, but for enemy objects.
See: SetEnemyAutoDeleteClip
Default value is false.

#### `ObjEnemy_SetDeleteFrame`

**Arguments**

- `int` object ID
- `int` frame

**Description**

ObjShot_SetDeleteFrame, but for enemy objects.

#### `ObjEnemy_AddLifeEx`

**Arguments**

- `int` object ID
- life

**Description**

Adds life to the enemy object with respect to the enemy object's maximum damage.
Negative value (damaging) will count towards the maximum damage as if getting hit by a player shot.
Positive value (healing) will reduce the damage count and allow more damage to be dealt in the frame equal to how much the healing was.

#### `ObjEnemy_SetMaximumDamage`

**Arguments**

- `int` object ID
- maximum damage

**Description**

Sets the maximum amount of damage that the enemy object can receive through normal means in a single frame.
Default value is, for all intents and purposes, infinity.

#### `ObjEnemy_GetIntersectionCircleListToShot`

**Arguments**

- `int` object ID

**Returns**

- `(float[3][]) intersection circles`

**Description**

Returns an array of arrays of the enemy object's intersection circle to player shot.
e.g. [[x1, y1, radius1], [x2, y2, radius2], ...]

#### `ObjEnemy_GetIntersectionCircleListToPlayer`

**Arguments**

- `int` object ID

**Returns**

- `(float[3][]) intersection circles`

**Description**

Returns an array of arrays of the enemy object's intersection circle to player hitbox.
e.g. [[x1, y1, radius1], [x2, y2, radius2], ...]

#### `ObjEnemy_SetEnableIntersectionPositionFetching`

**Arguments**

- `int` object ID
- `bool` enable

**Description**

If set to false, the following functions will not include the specified enemy object's hitboxes.
GetEnemyIntersectionPosition
GetEnemyIntersectionPositionByIdA1
GetEnemyIntersectionPositionByIdA2
GetAllEnemyIntersectionPosition

### Enemy Boss Scene Object Functions

#### `ObjEnemyBossScene_GetInfo`

**Description**

Addition.

**INFO_IS_REQUIRE_ALL_DOWN:**

Returns if EV_REQUEST_REQUIRE_ALL_DOWN was set to true.

#### `ObjEnemyBossScene_EndSpell`

**Arguments**

- `int` boss scene object ID

**Description**

Marks the current single as being not-a-spell.
I literally do not know why I added this.

#### `ObjEnemyBossScene_SetUnloadCache`

**Arguments**

- `int` boss scene object ID
- `bool` unload

**Description**

If set to true, will unload all added scripts from the script source cache upon boss scene end or object deletion.

### Shot Object Functions

#### `ObjShot_SetOwnerType`

**Arguments**

- `int` object ID
- `const` type

**Description**

Sets the shot object's owner type.

**Available types are:**

OWNER_PLAYER
OWNER_ENEMY
Shots created inside a player script are set to OWNER_PLAYER by default, OWNER_ENEMY otherwise.

#### `ObjShot_GetIntersectionEnable`

**Arguments**

- `int` object ID

**Returns**

- `(bool) intersection enable`

**Description**

Returns whether ObjShot_SetIntersectionEnable was set on the shot object.

#### `ObjShot_SetIntersectionScaleX`

**Arguments**

- `int` object ID
- scale

**Description**

Sets the shot object's X hitbox scale.
For normal shots, the true scale is the half-point between the X and Y scales.

#### `ObjShot_SetIntersectionScaleY`

**Arguments**

- `int` object ID
- scale

**Description**

Sets the shot object's Y hitbox scale.
For normal shots, the true scale is the half-point between the X and Y scales.

#### `ObjShot_SetIntersectionScaleXY`

**Arguments**

- `int` object ID
- X scale
- Y scale

**Description**

Sets the shot object's X and Y hitbox scales.
For normal shots, the true scale is the half-point between the X and Y scales.
Note: Sorry folks, no ellipse hitboxes for you.

#### `ObjShot_SetIntersectionScaleXY (Overload)`

**Arguments**

- `int` object ID
- XY scale

**Description**

Overloaded with 2 arguments.
Sets the shot object's XY hitbox scale.

#### `ObjShot_SetPositionRounding`

**Arguments**

- `int` object ID
- `bool` enable

**Description**

If set to true, will cause the shot object to only render in non-floating point coordinates.
See: Official Touhou games

#### `ObjShot_SetAngleRounding`

**Arguments**

- `int` object ID
- snap angle

**Description**

Causes the shot render angle to be rounded to the nearest multiple of the given angle.
If the angle is <= 0, angle rounding will be disabled.
See: PC-98 Touhou games

#### `ObjShot_SetDelayMotionEnable`

**Arguments**

- `int` object ID
- `bool` enable

**Description**

If set to true, will enable the shot object to move in its delay period.
Only works on regular shots.
Default is false.

#### `ObjShot_SetDelayGraphic`

**Arguments**

- `int` object ID
- `int` image ID

**Description**

Sets the image ID of the shot object's delay cloud.
A negative value will reset the delay cloud graphic to default.

#### `ObjShot_SetDelayScaleParameter`

**Arguments**

- `int` object ID
- parameter 1 (Designated "S0")
- parameter 2 (Designated "S1")
- parameter 3 (Designated "S2")

**Description**

Sets the properties of the shot object's delay cloud's scale.

**DELAY_DEFAULT:**

scale = min(S0 + delay / S2, S1)

**DELAY_LERP:**

scale = lerpFunction(S0, S1, delay / S2)

#### `ObjShot_SetDelayAlphaParameter`

**Arguments**

- `int` object ID
- parameter 1 (Designated "A0")
- parameter 2 (Designated "A1")
- parameter 3 (Designated "A2")

**Description**

Sets the properties of the shot object's delay cloud's alpha.
Alpha value range is 0 to 1.

**DELAY_DEFAULT:**

alpha = min(A0 + delay / A2, A1)

**DELAY_LERP:**

alpha = lerpFunction(A0, A1, delay / A2)

#### `ObjShot_SetDelayMode`

**Arguments**

- `int` object ID
- `const` delay mode
- `const` scale interpolation mode
- `const` alpha interpolation mode

**Description**

Sets the properties of the shot object's delay cloud.

**Available delay modes:**

**DELAY_DEFAULT:**

Default Danmakufu delay.

**DELAY_LERP:**

ZUN-like delay.

**Available delay interpolation modes:**

**LERP_LINEAR:**

Linear interpolation.

**LERP_SMOOTH:**

Smooth interpolation.

**LERP_SMOOTHER:**

Smoother interpolation.

**LERP_ACCELERATE:**

Accelerating interpolation.

**LERP_DECELERATE:**

Decelerating interpolation.

#### `ObjShot_SetDelayColor`

**Arguments**

- `int` object ID
- `int` hex color

**Description**

Sets the color of the shot object's delay cloud as an ARGB hexadecimal color value.
Set to 0 to revert to using the shot data's delay color.

#### `ObjShot_SetDelayColoringEnable`

**Arguments**

- `int` object ID
- `bool` enable

**Description**

When set to true, enables the object color to affect the shot object's delay cloud.

#### `ObjShot_SetGrazeInvalidFrame`

**Arguments**

- `int` object ID
- `int` cooldown frame

**Description**

Sets the shot object's graze cooldown frame.
After being grazed, the shot will be put on graze cooldown, wherein it cannot be grazed for the duration.
For lasers, the default value is 20.
For normal shots, the default value is 2147483647.

#### `ObjShot_SetGrazeFrame`

**Arguments**

- `int` object ID
- `int` frame

**Description**

Sets the shot object's graze frame.
Internally, this value is reset to the graze invalid frame every time a graze is performed.
This value decreases by 1 every frame, and the shot object is eligible for a graze if it is equal to or less than 0.

#### `ObjShot_IsValidGraze`

**Arguments**

- `int` object ID

**Returns**

- `(bool) graze validity`

**Description**

Returns whether the shot object is currently able to be grazed.

#### `ObjShot_SetPenetrateShotEnable`

**Arguments**

- `int` object ID
- `bool` enable

**Description**

If set to false, the shot will not lose penetration upon collision with another enemy shot.
Intended to be used with players shots, in tandem with ObjShot_SetEraseShot.
Default is true.

#### `ObjShot_SetEnemyIntersectionInvalidFrame`

**Arguments**

- `int` object ID
- `int` cooldown

**Description**

Sets the enemy intersection cooldown duration in frames for the shot object.
Intersection with an enemy will add the enemy to the shot's cooldown list.
The shot will not be able to intersect enemy objects still on hit cooldown.
Cooldown is maintained by the shot, not the enemy.
Shot A intersecting and putting an enemy on cooldown will not stop Shot B from registering intersection with the same enemy.

#### `ObjShot_SetFixedAngle`

**Arguments**

- `int` object ID
- `bool` fixed angle

**Description**

If set to true, the shot will not visually rotate along its movement angle.
The default value is specified in the graphic's shot data as "fixed_angle", this function allows you to override that value.

#### `ObjShot_SetSpinAngularVelocity`

**Arguments**

- `int` object ID
- spin angular velocity

**Description**

Sets the Z angle spin speed of the shot graphic.
The default spin speed is specified in the graphic's shot data as "angular_velocity", this function allows you to override that value.

#### `ObjShot_SetDelayAngularVelocity`

**Arguments**

- `int` object ID
- delay angular velocity

**Description**

Sets the Z angle spin speed of the shot's delay cloud.
The default delay spin speed is 0.

### Straight Laser Object Functions

#### `ObjLaser_SetInvalidLength`

**Description**

Change.
Both invalid lengths now scale from 0 to 1 rather than 0 to 100.

#### `ObjStLaser_SetEnd`

**Arguments**

- `int` object ID
- `bool` set

**Description**

Sets whether a delay cloud will appear at the end of the straight laser object.

#### `ObjStLaser_SetEndGraphic`

**Arguments**

- `int` object ID
- `int` graphic ID

**Description**

Sets the shot image ID of the straight laser object "end" delay cloud.
See: ObjShot_SetDelayGraphic

#### `ObjStLaser_SetDelayScale`

**Arguments**

- `int` object ID
- source scale multiplier
- end scale multiplier

**Description**

Sets the scale multipliers of the straight laser object's delay clouds.
The default values are (1, 1).

#### `ObjStLaser_SetPermitExpand`

**Arguments**

- `int` object ID
- `bool` enable

**Description**

Sets whether the straight laser will expand after the delay period.
Intended for use in delay lines.

#### `ObjStLaser_GetPermitExpand`

**Arguments**

- `int` object ID

**Returns**

- `(bool) permit expand`

**Description**

Returns whether the straight laser is permitted to expand after the delay period.

### Curvy Laser Object Functions

#### `ObjCrLaser_SetTipCapping`

**Arguments**

- `int` object ID
- `bool` enable

**Description**

Sets whether the curvy laser will attempt to keep its graphic's tips proportional to the upper and lower halves of its sprite.
The exact middle of the sprite will be stretched along the laser's body between the tips.
Can be used to emulate PC-98-styled "cheeto lasers".

#### `ObjCrLaser_GetNodePointer`

**Arguments**

- `int` object ID
- `int` node index

**Returns**

- `(pointer) node pointer`

**Description**

Returns the pointer value of the specified node index.
The pointer is used in other node-related functions.
As node traversal is relatively expensive, it is not recommended to repeatedly use this function.
At each frame, the node at the laser's end is invalidated if the laser is able to move.
Using an invalid pointer may cause a memory access violation.

#### `ObjCrLaser_GetNodePointerList`

**Arguments**

- `int` object ID

**Returns**

- `(pointer[]) node pointers`

**Description**

Returns a list of node pointers of the curvy laser object.
The pointers are used in other node-related functions.
At each frame, the node at the laser's end is invalidated if the laser is able to move.
Using an invalid pointer may cause a memory access violation.

#### `ObjCrLaser_GetNodePosition`

**Arguments**

- `int` object ID
- `pointer` node pointer

**Returns**

- `(float[2]) node position`

**Description**

Returns the [x, y] position array of the specified node.
The node pointer is obtained with ObjCrLaser_GetNodePointer[List], do NOT pass in any other values.

#### `ObjCrLaser_GetNodeAngle`

**Arguments**

- `int` object ID
- `pointer` node pointer

**Returns**

- `(float) node angle`

**Description**

Returns the facing angle of the specified node.
The node pointer is obtained with ObjCrLaser_GetNodePointer[List], do NOT pass in any other values.

#### `ObjCrLaser_GetNodeWidthScale`

**Arguments**

- `int` object ID
- `pointer` node pointer

**Returns**

- `(float) node width`

**Description**

Returns the render width scale of the specified node.
The node pointer is obtained with ObjCrLaser_GetNodePointer[List], do NOT pass in any other values.

#### `ObjCrLaser_GetNodeColor`

**Arguments**

- `int` object ID
- `pointer` node pointer

**Returns**

- `(int[4]) node color`

**Description**

Returns the [a, r, g, b] color array of the specified node.
The node pointer is obtained with ObjCrLaser_GetNodePointer[List], do NOT pass in any other values.

#### `ObjCrLaser_GetNodeColorHex`

**Arguments**

- `int` object ID
- `pointer` node pointer

**Returns**

- `(int) hex color`

**Description**

Returns the ARGB hexadecimal color value of the specified node.
The node pointer is obtained with ObjCrLaser_GetNodePointer[List], do NOT pass in any other values.

#### `ObjCrLaser_SetNode`

**Arguments**

- `int` object ID
- `pointer` node pointer
- node X
- node Y
- node angle
- `int` node hex color

**Description**

Sets the position, angle, and color of the specified node.
The node pointer is obtained with ObjCrLaser_GetNodePointer[List], do NOT pass in any other values.

#### `ObjCrLaser_SetNode (Overload)`

**Arguments**

- `int` object ID
- `pointer` node pointer
- node X
- node Y
- node angle
- `int` node hex color
- =node width scale

**Description**

Overloaded with 7 arguments.
Additional ability to set node render width render.

#### `ObjCrLaser_SetNodePosition`

**Arguments**

- `int` object ID
- `pointer` node pointer
- node X
- node Y

**Description**

Sets the position of the specified node.
The node pointer is obtained with ObjCrLaser_GetNodePointer[List], do NOT pass in any other values.

#### `ObjCrLaser_SetNodeAngle`

**Arguments**

- `int` object ID
- `pointer` node pointer
- node angle

**Description**

Sets the angle of the specified node.
The node pointer is obtained with ObjCrLaser_GetNodePointer[List], do NOT pass in any other values.

#### `ObjCrLaser_SetNodeWidthScale`

**Arguments**

- `int` object ID
- `pointer` node pointer
- node scale

**Description**

Sets the render width scale of the specified node.
The node pointer is obtained with ObjCrLaser_GetNodePointer[List], do NOT pass in any other values.

#### `ObjCrLaser_SetNodeColor`

**Arguments**

- `int` object ID
- `pointer` node pointer
- `int` node hex color

**Description**

Sets the color of the specified node as an ARGB hexadecimal color value.
The node pointer is obtained with ObjCrLaser_GetNodePointer[List], do NOT pass in any other values.

#### `ObjCrLaser_AddNode`

**Arguments**

- `int` object ID
- node X
- node Y
- node angle
- `int` node hex color

**Description**

Adds a laser node at the end of the curvy laser with the specified position, angle, and color.

#### `ObjCrLaser_AddNode (Overload)`

**Arguments**

- `int` object ID
- node X
- node Y
- node angle
- `int` node hex color
- node width scale

**Description**

Overloaded with 6 arguments.
Additional ability to specify node render width render.

### Shot Pattern Object Functions

#### `ObjPatternShot_Create`

**Description**

Creates a pattern shot object and returns its object ID.
Can be used for firing a large amount of bullets with only a few function calls, like in ZUN's ECL scripts.

#### `ObjPatternShot_Fire`

**Arguments**

- `int` object ID

**Description**

Fires the shot pattern defined by the given shot pattern object.

#### `ObjPatternShot_FireReturn`

**Arguments**

- `int` object ID

**Returns**

- `(int[]) shot IDs`

**Description**

Fires the shot pattern defined by the given shot pattern object and returns the IDs of the created shots in an array.

#### `ObjPatternShot_SetParentObject`

**Arguments**

- `int` object ID
- `int` parent object ID

**Description**

Sets the parent object of the given shot pattern object.

#### `ObjPatternShot_SetPatternType`

**Arguments**

- `int` object ID
- `const` type

**Description**

Sets the firing pattern of the given shot pattern object.

**Available pattern types:**

PATTERN_FAN
PATTERN_FAN_AIMED
PATTERN_RING
PATTERN_RING_AIMED
PATTERN_ARROW
PATTERN_ARROW_AIMED
PATTERN_POLYGON
- [way]:            Number of polygon vertices.
- [stack]:          Number of shots per sides.
- [angle argument]:	Edge skip, should normally be 1. Other values will result in polygrams.
- [speed argument]:	No effect.
PATTERN_POLYGON_AIMED
PATTERN_ELLIPSE
- [stack]:          No effect.
- [angle argument]:	Angle of the ellipse. (Not of the shots)
- [base speed]:     Max speed at the ends of the primary axis.
- [speed argument]:	Max speed at the ends of the secondary axis.
PATTERN_ELLIPSE_AIMED
PATTERN_SCATTER_ANGLE
PATTERN_SCATTER_SPEED
PATTERN_SCATTER
PATTERN_LINE
PATTERN_ROSE
- [way]:            Number of "petals".
- [stack]:          Number of shots per petals.

#### `ObjPatternShot_SetShotType`

**Arguments**

- `int` object ID
- `const` type

**Description**

Sets the shot type of the given shot pattern object.

**Supported types:**

OBJ_SHOT
OBJ_LOOSE_LASER
OBJ_CURVE_LASER

#### `ObjPatternShot_SetInitialBlendMode`

**Arguments**

- `int` object ID
- `const` blend type

**Description**

Sets the initial blend type of the shots.
See ObjRender_SetBlendType for the list of blend types.

#### `ObjPatternShot_SetShotCount`

**Arguments**

- `int` object ID
- `int` way
- `int` stack

**Description**

Sets the shot way and shot stack of the given shot pattern object.

#### `ObjPatternShot_SetSpeed`

**Arguments**

- `int` object ID
- base speed
- speed argument

**Description**

Sets the shot speed and argument of the given shot pattern object.

#### `ObjPatternShot_SetAngle`

**Arguments**

- `int` object ID
- base angle
- angle argument

**Description**

Sets the shot angle and argument of the given shot pattern object.

#### `ObjPatternShot_SetBasePoint`

**Arguments**

- `int` object ID
- X position
- Y position

**Description**

Sets the base firing position of the given shot pattern object.
Defaults to the parent object's position if not used.
Use PATTERN_BASEPOINT_RESET in either arguments to reset it to the parent object's position.

#### `ObjPatternShot_SetBasePointOffset`

**Arguments**

- `int` object ID
- X offset
- Y offset

**Description**

Sets the firing position offset of the given shot pattern object from its base point.

#### `ObjPatternShot_SetBasePointOffsetCircle`

**Arguments**

- `int` object ID
- angle
- radius

**Description**

Sets the firing position offset in a circular manner of the given shot pattern object from its base point.
Equivalent to ObjPatternShot_SetBasePointOffset(obj, radius * cos(angle), radius * sin(angle));

#### `ObjPatternShot_SetShootRadius`

**Arguments**

- `int` object ID
- radius

**Description**

Causes the shots to be fired a certain radius away from their base points.

#### `ObjPatternShot_SetDelay`

**Arguments**

- `int` object ID
- `int` delay

**Description**

Sets the shot delay of the given shot pattern object.

#### `ObjPatternShot_SetGraphic`

**Arguments**

- `int` object ID
- `int` graphic

**Description**

Sets the shot image ID of the given shot pattern object.

#### `ObjPatternShot_SetLaserParameter`

**Arguments**

- `int` object ID
- width
- length

**Description**

Sets the length and width of the given shot pattern object.
Has no effect if the pattern is firing normal shots.

#### `ObjPatternShot_CopySettings`

**Arguments**

- `int` destination object ID
- `int` source object ID

**Description**

Copies the settings of a shot pattern object to another.

#### `ObjPatternShot_AddTransform`

**Arguments**

- `int` object ID
- `const` transform type
- transform arguments...

**Description**

Adds a transformation to the shot pattern object.
Transforms arguments will be designated, in order: [arg0], [arg1], [arg2], etc...

**Available transformations are:**

TRANSFORM_WAIT 				(argc = 1)
- Delays the next transformation for [arg0] frames.
TRANSFORM_ADD_SPEED_ANGLE 	(argc = 4)
- Waits for [arg1] frames. (Does not delay the next transformation)
- For [arg0] frames, sets acceleration to [arg2].
- For [arg0] frames, sets angular velocity to [arg3].
TRANSFORM_ANGULAR_MOVE 		(argc = 3)
- For [arg0] frames, sets angular velocity to [arg1].
- Sets shot graphic "spin" to [arg2].
TRANSFORM_N_DECEL_CHANGE 	(argc = 5)
- For [arg1] times, decelerates/accelerates the shot to a complete halt in [arg0] frames. (See: Aunn's first spell)
- Movement will then change depending on [arg2].
- 0: Changes the movement angle to [arg4].
- 1: Increments the movement angle by [arg4].
- 2: Aims to the player with an added randomness factor of -[arg4] to [arg4].
- 3: Aims to the player with an increment of [arg4].
- 4: Angle becomes random.
- 5, other: No angle change.
- All modes sets the speed to [arg3].
- Delay the next transformation by ([arg0] * [arg1]) frames.
TRANSFORM_GRAPHIC_CHANGE 	(argc = 1)
- Changes the shot graphic to [arg0].
TRANSFORM_BLEND_CHANGE 		(argc = 1)
- Changes the shot blend type to [arg0].
TRANSFORM_TO_SPEED_ANGLE 	(argc = 3)
- Gradually changes the shot's speed to [arg1] in [arg0] frames.
- Gradually changes the shot's angle to [arg2] in [arg0] frames.
- Using TOPLAYER_CHANGE will aim the shot to the player object.
- NO_CHANGE can be used for [arg1] and [arg2].
TRANSFORM_ADDPATTERN_A1 	(argc = 3)
- Applies [ObjMove_AddPatternA1(obj, [arg0], [arg1], [arg2])]
TRANSFORM_ADDPATTERN_A2 	(argc = 8)
- Applies [ObjMove_AddPatternA4(obj, [arg0], [arg1], [arg2], [arg3], [arg4], [arg5], [arg6], [arg7])]
TRANSFORM_ADDPATTERN_B1 	(argc = 3)
- Applies [ObjMove_AddPatternB1(obj, [arg0], [arg1], [arg2])]
TRANSFORM_ADDPATTERN_B2 	(argc = 8)
- Applies [ObjMove_AddPatternB3(obj, [arg0], [arg1], [arg2], [arg3], [arg4], [arg5], [arg6], [arg7])]
TRANSFORM_ADDPATTERN_C1 	(argc = 3)
- Applies [ObjMove_AddPatternC1(obj, [arg0], [arg1], [arg2])]
TRANSFORM_ADDPATTERN_C2 	(argc = 10)
- Applies [ObjMove_AddPatternC3(obj, [arg0], [arg1], [arg2], [arg3], [arg4], [arg5], [arg6], [arg7], [arg8], [arg9])]

#### `ObjPatternShot_SetTransform`

**Arguments**

- `int` object ID
- `int` transform index
- `const` transform type
- transform arguments...

**Description**

Sets transformation to the specified transform index.

### Collision Object Functions

#### `ObjCol_GetListOfIntersectedShotID`

**Arguments**

- `int` object ID
- `const` shot owner type

**Returns**

- `(int[]) intersected shot IDs`

**Description**

Returns an array of all shot objects of the specified owner type currently intersecting with the object.

**Available owner types are:**

OWNER_PLAYER
OWNER_ENEMY

### Item Object Functions

#### `ObjItem_SetAutoCollectEnableFlags`

**Arguments**

- `int` object ID
- `int` flags

**Description**

Sets the autocollection enable flags of the item object.
Autocollection enable flags dictate which autocollection types will affect the item object.

**Available flags:**

ITEM_AUTOCOLLECT_PLAYER_SCOPE
ITEM_AUTOCOLLECT_COLLECT_ALL
ITEM_AUTOCOLLECT_POC_LINE
ITEM_AUTOCOLLECT_COLLECT_CIRCLE
ITEM_AUTOCOLLECT_ALL
Flags can be combined with bitwise OR.
Using 0 will cause the item object to be impervious to all autocollection types.

#### `ObjItem_GetInfo`

**Arguments**

- `int` object ID
- `const` info type

**Returns**

- `[varies]`

**Description**

Addition.

**INFO_ITEM_MOVE_TYPE:**

Returns the move type of the item object.
Returned values are the possible inputs of ObjItem_SetDefinedMovePatternA1.

**INFO_ITEM_TYPE:**

Returns the graphic ID of the item object.

#### `ObjItem_SetMoveToPlayer`

**Arguments**

- `int` object ID
- `bool` move to player

**Description**

Causes the item object to start or stop moving to the player.
If set to true, EV_COLLECT_ITEM will be notified.
If set to false, EV_CANCEL_ITEM will be notified.

#### `ObjItem_IsMoveToPlayer`

**Arguments**

- `int` object ID

**Returns**

- `(bool) result`

**Description**

Returns whether the item object is moving to the player.

#### `ObjItem_Collect`

**Arguments**

- `int` object ID

**Description**

Causes the item object to be instantly collected.
Not affected by ObjItem_SetIntersectionEnable.

#### `ObjItem_SetAutoDelete`

**Arguments**

- `int` object ID
- `bool` auto delete

**Description**

Sets whether the item will be subject to the auto-deletion clip.
The auto-deletion clip size can be altered using SetItemAutoDeleteClip.

#### `ObjItem_SetIntersectionRadius`

**Arguments**

- `int` object ID
- `int` radius

**Description**

Sets the item object's player intersection radius.

#### `ObjItem_SetIntersectionEnable`

**Arguments**

- `int` object ID
- `bool` enable

**Description**

Sets whether the item object is allowed to directly intersect with the player.

#### `ObjItem_GetIntersectionEnable`

**Arguments**

- `int` object ID

**Returns**

- `(bool) enable`

**Description**

Returns whether the item object has intersection enabled.

#### `ObjItem_SetDefaultCollectMovement`

**Arguments**

- `int` object ID
- `bool` enable

**Description**

Sets whether the item object will use the built-in autocollection behaviour.
Intended to be used in tandem with EV_COLLECT_ITEM.

#### `ObjItem_SetPositionRounding`

**Arguments**

- `int` object ID
- `bool` enable

**Description**

If set to true, will cause the item object to only render in non-floating point coordinates.

## Item script

### Events

EV_DELETE_SHOT_TO_ITEM is notified when an item is deleted and turned into an item.

**Actions that trigger this event:**

- Using ObjShot_ToItem.
- Using DeleteShotAll or DeleteShotInCircle with TYPE_ITEM.

**Event arguments:**

1) (int) shot object ID
2) (float[]) position [x, y]
3) (bool) collided with player
If true, the shot was deleted from coliision with a player hitbox.
4) (int) shot image ID
EV_COLLECT_ITEM is notified when an item is autocollected.

**Actions that trigger this event:**

- Collision with the player's item scope.
- The player moving up into the item autocollection line.
- Using CollectItemsInCircle, CollectAllItems, or ObjItem_SetMoveToPlayer.

**Event arguments:**

1) (int) item object ID
2) (int) item type
3) (int) collection type
0 - Player scope
1 - Player autocollection line
2 - Via CollectItemsInCircle
3 - Via CollectAllItems
4 - Via ObjItem_SetMoveToPlayer
4) (float) collection info

**Varies with collection type:**

Type 0 -> Player item scope radius
Type 1 -> Player autocollection line position
Type 2 -> Collection circle size
Type 3 -> 0
Type 4 -> 0
EV_CANCEL_ITEM is notified when an item's autocollection is canceled.

**Actions that trigger this event:**

- The player leaving STATE_NORMAL while the item is in the middle of being collected.
- Using CancelCollectItems or ObjItem_SetMoveToPlayer.

**Event arguments:**

1) (int) item object ID
2) (int) item type
3) (int) cancel type
0 - Player died
1 - Via CancelCollectItems
2 - Via ObjItem_SetMoveToPlayer

## Player script

### Shot Functions

#### `CreatePlayerShotA1`

**Description**

Change.
No longer returns a null value if SetForbidPlayerShot was set to true.

### Events

EV_DELETE_SHOT_PLAYER is notified to the player script when a player shot is deleted upon collision with an enemy intersection.
ObjShot_SetPenetration(obj, 0) can also be used to manually activate the event.

**Event arguments:**

1) (int) shot object ID
2) (float[2]) position
3) (int) shot graphic ID
4) (int) object ID that the shot collided with
- Will be ID_INVALID in case of manual activation.
