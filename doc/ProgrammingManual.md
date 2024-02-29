[TOC]
# General guildline
- Do not use dynamic allocation (calloc, malloc)
- Always specify data type of variables and function return values.
- Limit mixing math operations with multiple data types. Use casting if necessary.
- Good comments make the code more readable.
- Do not use magic numbers (constant literals) in code. Replace them with defines.
- Variable that must remain constant declare as such (`const`).
# Data types
Defined in `app_types.h`:
```
#include "app_types.h"
```
|Data type |Naming postfix  | Description             |
|----------|----------------|-------------------------|
|`BOOL`	   |`_b` 	    	| boolean	              |
|`U16`	   |`_U16`	    	|unsigned 16-bit integer  |
|`S16`	   |`_S16`	    	|signed 16-bit integer    |
|`U32`     |`_U32`	    	|unsigned 32-bit integer  |
|`S32`     |`_S32`	    	|signed 32-bit integer    |  
|`U64`     |`_U64`	    	|unsigned 64-bit integer  |
|`S64`	   |`_S64`	    	|signed 64-bit integer    |
|`F32`     |`_F32`	    	|single precision float   |

# Variables
All variables must be self explanatory, ideally short but specific.
``` 
(s_)(MODULE)_{name_of_the_variable}(__unit_){_VariableType}
 |     |		  	       '-> If the variable is unit
 |     '---------------------------------> If the variable is global	
 '---------------------------------------> If the variable is static
```
If the variable **must not be seen outside of the module**, then use `static` keyword.
Keyword `volatile` is **recommended to use with variables with asynchronous access** (in interrupts).

## Variable names
All variables names are written in [snake case](https://en.wikipedia.org/wiki/Snake_case). Example:
```
your_variable_name
```
## Global variables
If the variable is global (static included), then they should contain prefix with module name. **Local variables are without this prefix**. For example:
```
U16 MODULE_some_global_variable_U16;			/* Global variable. */

void fun(void)
{
	U32 tmp_val_U32;				/* Local variable. */
	static U16 s_last_index_U16;			/* Static local variable. */
}
```

## Static variables
Static variables are marked with `s_` prefix. If the variable is **global**, then the module name comes after the prefix like so:
```
static U32 s_MODULE_name_of_the_variable_U32;		/* Static variable. */

void fun(void)
{
	static U16 s_last_index_U16;			/* Static local variable. */
}
```
## Variables containing units
For better code readability we should include physical dimension of a unit that we want to represent with a variable.
| Unit 			  			| Variable Name | Description       |
----------------------------| --------------|-------------------|
|$\mathrm{V}$     	  		|`__V__`	  	|Volt		      	|
|$\mathrm{A}$     	  		|`__A__`        |Ampere             |
|$\mathrm{Nm}$		  		|`__Nm__`	  	|Newton meter       |
|$\mathrm{kg.m^2}$ 	  		|`__kgm2__`	  	|Moment of inertia  |
|$\mathrm{rad.s^{-1}}$	  	|`__rad_s1__`   |Angular velocity   |
|%     			  			|`__per_cent__` |percent	      	|
|‰     			  			|`__pro_mile__` |promile	      	|

We can also use unit prefixes:
| Unit 					| Variable Name | Description 	|
------------------------| --------------|---------------|
|$\mathrm{M}$			|`__M<unit>__`  | Mega			|
|$\mathrm{k}$			|`__k<unit>__`  | kilo	   		|	
|$\mathrm{m}$     		|`__m<unit>__`	| milli			|
|$\mathrm{\mu}$     	|`__u<unit>__`  | micro       	|
|$\mathrm{n}$			|`__n<unit>__`	| nano			|
|$\mathrm{p}$			|`__p<unit>__`	| pico			|

**Example:**
```
F32 output_voltage__V__F32 = (F32)12.3;
F32 output_current__mA__F32 = (F32)314;
F32 output_power__W__F32 = output_voltage__V__F32 * output_current__mA__F32 
					/ (F32)1000.0;
```
# Enumeration (Enums)
When typedef-ing enum use `_enum` postfix. Each enum member has `_e` postfix. Name of each enum is in **capitals** :
```
typedef enum
{
	MODULE_STATE_RUNNING_e = 0,
	MODULE_STATE_STOPPED_e,
	MODULE_STATE_ERROR_e 
} MODULE_my_enum_name_enum;
```

# Functions
Functions must do only one thing. If the control logic is complicated, then break down this logic to simpler and smaller units.
**Function naming scheme:**
```
{ReturnDataType} {MODULE_}{FunctionName}(__unit__)(_ReturnDataType)()
			                    |		'-> If the function is void, then the data type is not specified
				            '-> Only if function's return value is unit.
```
**If the function has no input parameters then:**
```
/* Do not. Calling the function fun() with parameters does not result in error - bad. */
void fun()		
{
	/* Implementation. */
}

/* Do instead. Calling the function fun() with parameters results in error - good. */
void fun(void)			
{
	/* Implementation. */
}
```
**Static functions** do not have `s_`  prefix.

When **passing a struct** to a function it is recommended to use pointer rather than copy.
## Naming the functions
Function names like variable names should be safe explanatory. It is up to you as a programmer to name the function, but it is a good practice to stick to standard naming of basic functions like: For example:
| Name  	    	|Return void     	| Description 									|
|-------------------|-------------------|-----------------------------------------------|
|`Get(void)`	    |No 	  			|Get variable value								|
|`Set()`	    	|Yes	  			|Set variable value								|
|`Reset(void)`	    |Yes	  			|Reset variable/struct to default value			|
|`Calculate()`	    |Not specified		|Calculate from provided parameters new value 	|
|`Init()`	    	|Not specified 		|Initialize module 								|
|`DeInit()`	    	| Not specified 	|De-initialize module 							|

# Pointers
Pointers are marked with `_p<DataType>` suffix:
```
U32* data_pU32 = (U32*)0;			/* Pointer to U32. */
```
|Pointer to 	|Notation 						|
|---------------|-------------------------------|
|data type  	| `_p<data>`					|
|struct			| `_ps`							|
|function		| `_pF<function return type>`	|


*__Do not use uninitialised pointers!!!__*
```
U16* data_pU16;
*data_pU16 = (U16)0;		/* Results in undefined behaviour! */
```
Limit usage of **void pointers**.

# Defines and macros
Whenever the code needs to work with constant literals use defines and macros. All defines and macros are in capital letters with underscore for word separation:

**Notation:**
|Type			|Notation(suffix)		|
|---------------|-----------------------|		
|Constant define| `_d<DataType>`		|
|Macro			| `_dM<DataType>`		|

Examples:
```
/* Defines */
#define GREEN_LED_PIN_dU16			( (U16)24 )
#define W_REG_I_dF32				( (F32)12.64 )

/* Macros */
#define CALC_POWER__W__dMF32(U, I)		( (F32)(U) * (F32)(I) )
```

# Comments
All types of comments are allowed.

## File comments
Every `.c` and `.h` file should contain basic file description at the start:
```
/**
 * @file NameOfTheFile
 * @brief BriefDescription
 * @details DetailedDescription
 *
 * =================================================================
 * @author Author1
 * @author Author2
 *
 * =================================================================
 * KEM, FEI, TUKE
 * @date CurrentDate
 */
```
