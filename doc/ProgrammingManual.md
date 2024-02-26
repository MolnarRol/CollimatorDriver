# Software design manual

## Data types
Defined in `app_types.h`:
```
#include "app_types.h"
```
|Data type |Naming postfix 	| Description             |
|----------|----------------|-------------------------|
|`BOOL`		 |`_b` 						| boolean	                |
|`U16`		 |`_U16` 					|unsigned 16-bit integer  |
|`S16`		 |`_S16`					|signed 16-bit integer   	|
|`U32`     |`_U32`					|unsigned 32-bit integer  |
|`S32`		 |`_S32`					|signed 32-bit integer    |  
|`U64`     |`_U64`					|unsigned 64-bit integer  |
|`S64`		 |`_S64`					|signed 64-bit integer		|
|`F32`		 |`_F32`					|single precision float   |

## Variables
All variables must be self explanatory, ideally short but specific.
``` 
(s_)(MODULE)_{name_of_the_variable}(__unit_){_VariableType}
 |	   |							   							'-> If the variable is unit
 |     '---------------------------------> If the variable is global	
 '---------------------------------------> If the variable is static
```
If the variable **must not be seen outside of the module**, then use `static` keyword.
Keyword `volatile` is **recommended to use with variables with asynchronous access** (in interrupts).

### Variable names
All variables names are written in [snake case](https://en.wikipedia.org/wiki/Snake_case). Example:
```
your_variable_name
```
### Global variables
If the variable is global (static included), then they should contain prefix with module name. **Local variables are without this prefix**. For example:
```
U16 MODULE_some_global_variable_U16;						/* Global variable. */

void fun(void)
{
	U32 tmp_val_U32;															/* Local variable. */
	static U16 s_last_index_U16;									/* Static local variable. */
}
```

### Static variables
Static variables are marked with `s_` prefix. If the variable is **global**, then the module name comes after the prefix like so:
```
static U32 s_MODULE_name_of_the_variable_U32;		/* Static variable. */

void fun(void)
{
	static U16 s_last_index_U16;									/* Static local variable. */
}
```
### Variables containing units
For better code readability we should include physical dimension of a unit that we want to represent with a variable.
| Unit 										| Variable Name | Description 			|
--------------------------| --------------|-------------------|
|$\mathrm{V}$     				|`__V__`		 	  |Volt				 				|
|$\mathrm{A}$     				|`__A__`        |Ampere       			|
|$\mathrm{Nm}$		 				|`__Nm__`			  |Newton meter 			|
|$\mathrm{kg.m^2}$ 				|`__kgm2__`		  |Moment of inertia 	|
|$\mathrm{rad.s^{-1}}$		|`__rad_s1__`		|Angular velocity 	|
|%     										|`__per_cent__` |percent						|
|‰     										|`__pro_mile__` |promile						|

We can also use unit prefixes:
| Unit 									| Variable Name | Description 			|
------------------------| --------------|-------------------|
|$\mathrm{M}$						|`__M<unit>__`  | Mega							|
|$\mathrm{k}$						|`__k<unit>__`  | kilo							|
|$\mathrm{m}$     			|`__m<unit>__`	| milli		 					|
|$\mathrm{\mu}$     		|`__u<unit>__`  | micro       			|
|$\mathrm{n}$		 				|`__n<unit>__`	| nano				 			|
|$\mathrm{p}$						|`__p<unit>__`	| pico							|

**Example:**
```
F32 output_voltage__V__F32 = (F32)12.3;
F32 output_current__mA__F32 = (F32)314;
F32 output_power__W__F32 = output_voltage__V__F32 * output_current__mA__F32 
														/ (F32)1000.0;
```
## Enumeration (Enums)
When typedef-ing enum use `_enum` postfix. Each enum member has `_e` postfix. Name of each enum is in **capitals** :
```
typedef enum
{
	MODULE_STATE_RUNNING_e = 0,
	MODULE_STATE_STOPPED_e,
	MODULE_STATE_ERROR_e 
} MODULE_my_enum_name_enum;
```

## Functions
