# Scripting language features

## Syntax tutorial

### Number literals

#### Base literals

```dnh
WriteLog(0b1011);   //Binary literal, outputs 11
WriteLog(0o503);    //Octal literal, outputs 323
WriteLog(0x8f62);   //Hexadecimal literal, outputs 36706
```

Base literals default to integer type unless suffixed otherwise.

#### Suffixes

- `"f"` / `"F"` forces a number literal to a float type.
- `"i"` / `"I"` forces a number literal to an integer type.

```dnh
WriteLog(5);        //5.000000
WriteLog(5f);       //5.000000
WriteLog(5i);       //5
WriteLog(0x5f);     //95
WriteLog(0x5fi);    //95
WriteLog(7.76);     //7.760000
WriteLog(7.76f);    //7.760000
WriteLog(7.76i);    //7
```

#### Digit separator

The "_" character can be used in number literals to aid in visibility.
Has no effect on the resultant value.

```dnh
WriteLog(5000000000i);      //5000000000
WriteLog(5_000_000_000i);   //still 5000000000
WriteLog(0x7f23a7e2);       //2133043170
WriteLog(0x7f23_a7e2);      //still 2133043170
```

### Variables

#### Explicit types

Originally, variables were declared with `let`, `var`, or `real`; all three
keywords had the same effect.

Since version 1.30a, the scripting language has changed considerably:

- `real` was renamed to `float` in version 1.32a.
- Variables can now be declared with an explicit type.

```dnh
int a = 700;            //typeof(a) == VAR_INT
float b = 56.243;       //typeof(b) == VAR_FLOAT
char c = '0';           //typeof(c) == VAR_CHAR
bool d = false;         //typeof(d) == VAR_BOOL
string e = "aubergine"; //typeof(e) == VAR_STRING
int[] f = [4i, 8];      //typeof(f) == VAR_ARRAY, ftypeof(f) == VAR_INT
```

`let` and `var` still behave similarly to `auto` in C++:

```dnh
let a = 700i;           //typeof(a) == VAR_INT
let b = 56.243;         //typeof(b) == VAR_FLOAT
let c = '0';            //typeof(c) == VAR_CHAR
let d = false;          //typeof(d) == VAR_BOOL
let e = "aubergine";    //typeof(e) == VAR_STRING
let f = [4i, 8];        //typeof(f) == VAR_ARRAY, ftypeof(f) == VAR_INT
```

- You may not use `let[]` or `const[]`. Array type declarations require a
  non-auto type.

#### Multiple variables declaration.

```dnh
int a, b = 7, c = 100, d;       //All will be of type "int"
const bool e = true, f = true;  //All will be of type "const bool"
let g = 56, h = true, i = 6i;   //g will be "float", h will be "bool", and i will be "int"
bool[][][] j, k;                //All will be of type "bool[][][]" (3D bool array)
```

#### Using "const"

"const" is used to declare constant variables.
A standalone "const" will behave like a "let".
"const" can be prefixed or suffixed by another type.
Constant variables cannot be modified.
Attempting to do so will throw a compile-time error.

```dnh
const a = 1;
const let b = true;
const string c = "xolotl";
const int d;

a = 6;      //ERROR
d = 10;     //ERROR
```

#### Supplement for smart people

| ph3sx script type | Internal C type | Size in bytes |
| --- | --- | ---: |
| `int` | `int64_t` | 8 |
| `float` | `double` | 8 |
| `bool` | `bool` | 1 |
| `char` | `wchar_t` | 2 |

### Scoping

- `"local"` has been deprecated. Although it has not been completely removed,
  it is advisable to stop using it.

  What can replace it? Nothing: a `local { ... }` block can be written as
  `{ ... }`.

What used to be:

```dnh
local {
    //...
}
```

can now be reduced to just:

```dnh
{
    //...
}
```

Scoping rules will still apply normally, like so:

```dnh
let a;

//Only a is accessible here

{
    let b;

    //a and b are both accessible here

    {
        let c;

        //a, b, and c are all accessible here
    }
}
```

### Operators

All available script operators are listed below in order of precedence, highest to lowest.

- **Precedence 1:** `()` — Function call

```dnh
let c = SomeFunc(a, b);
```

- **Precedence 1:** `as_x()` — Type cast

```dnh
let a = as_int(x);
let b = as_bool(56);
```

- **Precedence 1:** `length()` — Array length

```dnh
let a = length(x);
let b = length([1, 2, 3]);
```

- **Precedence 1:** `[]` — Array indexing

```dnh
let a = arr[3];
let b = [10, 100, 1000][x];
```

- **Precedence 1:** `(| |)` — Absolute

```dnh
let a = (|-4|);         //a == 4
let b = (|a|);          //b == 4
```

- **Precedence 1:** `()` — Parentheses

- **Precedence 2:** `^` — Power (Right-associative)

```dnh
let a = 2 ^ 4;          //a == 16
let b = 2 ^ 2 ^ 3;      //b == 256
```

- **Precedence 2:** `[..]` — Array slice

```dnh
let x = [9, 10, 11, 12, 13];
let a = x[0..2];        //a == [9, 10]
let b = x[2..999];      //b == [11, 12, 13]
let c = x[3..0];        //c == [11, 10, 9]
```

- **Precedence 3:** `+` — Unary plus

```dnh
let a = +6;
```

- **Precedence 3:** `-` — Unary minus

```dnh
let a = -10;
```

- **Precedence 3:** `!` — Unary logical not

```dnh
let a = !b;
```

- **Precedence 3:** `~` — Unary bitwise not

```dnh
let a = ~312;           //a == -313
let b = ~(0b1011001);   //b == -90
```

- **Precedence 4:** `*` — Multiply

```dnh
let a = 4 * 10;         //a == 40
```

- **Precedence 4:** `/` — Divide

```dnh
let a = 10 / 3;         //a == 3.333333
```

- **Precedence 4:** `~/` — Floored divide

```dnh
let a = 10 ~/ 3;        //a == 3
```

- **Precedence 4:** `%` — Remainder (Modulo)

```dnh
let a = 7 % 4;          //a == 3
```

- **Precedence 5:** `+` — Add

```dnh
let a = 3 + 32;         //a == 35
```

- **Precedence 5:** `-` — Subtract

```dnh
let a = 9 - 12;         //a == -3
```

- **Precedence 5:** `~` — Array concatenate

```dnh
let a = [8, 3] ~ [10];  //a == [8, 3, 10]
```

- **Precedence 6:** `<<` — Bitwise shift left

```dnh
let a = 7 << 3;         //a == 56
```

- **Precedence 6:** `>>` — Bitwise shift right

```dnh
let a = 198 >> 2;       //a == 49
```

- **Precedence 7:** `==`, `!=`, `<`, `<=`, `>`, `>=` — Comparison

```dnh
let a = 5 < 6;
let b = a != false;
let c = 10 >= 10;
```

- **Precedence 8:** `^^` — Bitwise XOR

```dnh
let a = 63 ^^ 234;      //a == 213
```

- **Precedence 9:** `|` — Bitwise OR

```dnh
let a = 63 | 234;       //a == 255
```

- **Precedence 10:** `&` — Bitwise AND

```dnh
let a = 63 & 234;       //a == 42
```

- **Precedence 11:** `||` — Logical OR

```dnh
let a = true || false;  //a == true
```

- **Precedence 11:** `&&` — Logical AND

```dnh
let a = true || false;  //a == false
```

- **Precedence 12:** `?:` — Ternary expression

```dnh
let a = x > y ? 10 : 20;
let b = z == 0 ? x * 10 : y + 4;
let c = k ? (y ? 8 : 0) : p ? 5 : 2 + x;
```

### Boolean expressions

#### Short-circuiting

This feature was present in vanilla ph3 as well, but I have yet to see a tutorial mention it, so here it is.
Short-circuiting (short-circuit evaluation) is an optimization for boolean expressions.
Take these statements, for example:

```dnh
bool c = a && b;
bool z = x || y;
```

This !RUN-TIME! optimization will occur when expression (a) evaluates to false,
where the evaluation of expression (b) will be completely skipped over.
Conversely, in the second statement, the evaluation of expression (y) will also be skipped over if

```dnh
expression (x) evaluates to true.
```

Short-circuiting will take place left-to-right, so it is beneficial to format your logical expressions
in a way that more lightweight expressions get evaluated sooner rather than later.

```dnh
if (bCheckHit && GetShotIdInCircleA2(...)) { /*...*/ }
```

would benefit more from short-circuit evaluation than

```dnh
if (GetShotIdInCircleA2(...) && bCheckHit) { /*...*/ }
```

Short-circuiting only applies to logical expressions, not bitwise expressions.

```dnh
bool c = a & b;
bool d = a | b;
```

Here, both expressions (a) and (b) will be evaluated regardless of the result of expression (a).

### Loops

- Ascent/Descent
Ascent and descent loops remain unchanged, but you may now assign an explicit type to the counter variable.

```dnh
ascent (i in 0..3)
    WriteLog(i);
```

Will result in the following output:

```text
0.000000
1.000000
2.000000
```

```dnh
//-----------------------------------------------

ascent (i in 0i..3)
    WriteLog(i);
```

Will result in the following output:

```text
0
1
2
```

```dnh
//-----------------------------------------------

ascent (int i in 0..3)
    WriteLog(i);
```

Will result in the following output:

```text
0
1
2
```

#### New loop forms

- **For loop:** A generalized version of ascent/descent loops, using the
  standard loop format found in many programming languages.
- **For-each loop:** A concise way to iterate through arrays.

```dnh
int[] arr = [10, 9, 8, 7];
for each (int i in arr)
    WriteLog(i);
```

Will result in the following output:

```text
10
9
8
7
```

```dnh
//-----------------------------------------------

for each (float i in arr)
    WriteLog(i);
```

Will result in the following output:

```text
10.000000
9.000000
8.000000
7.000000
```

##### The `ref` keyword

Normally, a for-each loop iterates over a copy of the array. The `ref` keyword
makes it read the original array instead, avoiding the copy and allowing the
loop to respond to modifications made during iteration.

```dnh
int[] arr = [1, 2, 3, 4];

for each (i in arr) {
    if (i % 2 == 0)
        arr ~= [5];
    WriteLog(i);
}
WriteLog(arr);
```

Will result in the following output:

```text
1
2
3
4
[1, 2, 3, 4, 5, 5]
```

```dnh
//-----------------------------------------------

for each (i in ref arr) {
    if (i % 2 == 0)
        arr ~= [5];
    WriteLog(i);
}
WriteLog(arr);
```

Will result in the following output:

```text
1
2
3
4
5
5
[1, 2, 3, 4, 5, 5]
```

##### Additional for-each syntax

The index can be declared separately and supplied as a second iterator:

```dnh
{
    int i = 0;
    for each (itr in array) {
        //...
        i++;
    }
}

//is equivalent to:

for each (i, itr in array) {
    //...
}
```

These forms are also allowed:

```dnh
for each ((i, itr) in array) { /*...*/ }
for each ((int i, itr) in array) { /*...*/ }
for each (i, float itr in array) { /*...*/ }
for each ((float i, string itr) in array) { /*...*/ }
```

The colon (`:`) may be used in place of the keyword `in`:

```dnh
for each (itr : array) { /*...*/ }
for each ((i, itr) : array) { /*...*/ }
```

#### In addition, there is a new flow control statement "continue".

"continue" is used in the same manner as "break", but rather than simply ending the loop,
it will merely skip the current iteration of the loop.

```dnh
ascent (int i in 0..8) {
    if (i % 2 == 0) continue;
    WriteLog(i);
}
```

Will result in the following output:

```text
1
3
5
7
```

Unlike in vanilla ph3, `break` and `continue` cannot be used outside a loop.
Attempting to do so produces a compile-time error.

### Functions, Tasks, Subs ("Callables")

#### Parameters

Parameters, like variables, can have explicit types.

```dnh
function Func(int a, float b, const string[] c) {}
```

#### Function return type

You may explicitly specify a function's return type:

```dnh
function<int> Func1(a, b) {
    return a + b;
}

WriteLog(Func1(10, 4.2));       //Output: 14
WriteLog(Func1(10, "sdfs"));    //ERROR: Invalid implicit casting
```

Marking a function with type `void` forbids it from returning a value:

```dnh
function<void> Func1(a, b) {
    return;         //OK: no return value
}

function<void> Func2(a, b) {
    return a + b;   //ERROR: void function can't return a value
}
```

#### Overloading

Two callables with the same name, but different argument counts, will be treated as two different, unique callables.

```dnh
function<int> Func() {
    return 0;
}
function<int> Func(a) {
    return 100;
}
function<int> Func(a, b, c) {
    return a + b + c;
}

Func();              //Returns 0
Func("asdf");        //Returns 100
Func(1, 2, 3);       //Returns 6
```

#### Variadic argument count (varargs)

Some built-in functions now accept a variable number of arguments:

```dnh
NotifyEventAll(EV_USER, 0);
NotifyEventAll(EV_USER, 0, 1, 2, 3);
NotifyEventAll(EV_USER, 0, 1, 2, 3, 4, 5, 6, 7, 8);

//All of the above are valid calls to NotifyEventAll.
```

The engine can accept up to approximately 357,900,000 arguments, but this is
not a practical limit to target. Scripters cannot define their own variadic
callables.

### Async

Async blocks can be placed anywhere in the code, they behave like inlined tasks.

```dnh
function Func() {
    WriteLog(0);

    async {
        WriteLog(1);
        wait(10);
        WriteLog(2);
    }

    return true;
}

//is equivalent to:

function Func() {
    WriteLog(0);

    task AsyncTask() {
        WriteLog(1);
        wait(10);
        WriteLog(2);
    }
    AsyncTask();

    return true;
}
```

### Script

#### Character and string literals

Backslash escapes (`\\`) are recognized in character and string literals.
Hexadecimal character literals (`\x[hex]`) can also be used:

```dnh
"\x74"          -> "t"
'\x3042'        -> 'あ'
"\x042\x5f"     -> "B_"
```

#### One-lined statements

```dnh
ascent (i in 0..10) WriteLog(i);

if (true) a += 10;
else a -= 4;
```

Can be used everywhere except in function/task/sub declarations, async blocks,
@-blocks, and `local{}` blocks.

### Optimizations

#### The script compiler will try to perform basic optimizations on maths expressions.

Examples:

```dnh
a = 5 + 5 - 1;          -> optimize ->  a = 9;
a = 5 * 8 / 7;          -> optimize ->  a = 5.714286;
a = func(b) + 5 % 10;   -> optimize ->  a = func(b) + 5;
```

#### Empty loops and blocks will be optimized away during script compiling.

Examples:

```dnh
while (true) {}

for (let i = 0; i < 10; i++) {}

ascent (i in 0..100000000) {}

{
}

loop (1000) {
}

for each (i in "aaaaaaaaaa") {}

async {}

//All of the above examples will be optimized away
```

#### Loops containing a single yield will be automatically transformed into a wait.

Examples:

```dnh
loop (60) yield;            -> optimize ->  wait(60);

loop (a * 2 + 60 - 20) {    -> optimize ->  wait(a * 2 + 60 - 20);
    yield;
}
```

## Text Object Tags

Text object tags are special formatting patterns that can be used to dynamically alter rendering of text objects.
### New line tag (`r`)

Inserts a new line. Also resets formatting from other tags.

### Font tags (`font` / `f`)

Modifies the font.
Available tag properties:

- `reset` / `rs` / `r` / `clear` / `clr` / `c`: Resets the text to its
  original settings. Present in vanilla ph3 as `clear`.
- `size` / `sz`: Changes the font size. Unchanged from vanilla ph3.
- `ox` / `oy`: Changes the position offset.
- `it`: Toggles italic (`true`/`false` or `1`/`0`).
- `wg`: Changes the font weight.
- `br` / `bg` / `bb`: Changes the font's bottom color.
- `tr` / `tg` / `tb`: Changes the font's top color.
- `or` / `og` / `ob`: Changes the font's border color.
- `bc`: Changes the font's bottom color as an `(r, g, b)` list.
- `tc`: Changes the font's top color as an `(r, g, b)` list.
- `oc`: Changes the font's border color as an `(r, g, b)` list.

```dnh
ObjText_SetText(obj, "Lorem ipsum [font size=48 it=1 bc=(255, 0, 0)]dolor sit[font clr] amet");
```

### Ruby tag

Creates a ruby(furigana) text.
Available tag properties:

- `rb`: Sets the main (bottom) text. Unchanged from vanilla ph3.
- `rt`: Sets the furigana (top) text. Unchanged from vanilla ph3.
- `sz`: Changes the furigana text's font size.
- `wg`: Changes the furigana text's font weight.
- `ox`: Changes the furigana text's left margin.
- `op`: Changes the furigana text's side pitch.
