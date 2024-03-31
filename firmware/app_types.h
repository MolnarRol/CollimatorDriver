/**
 * @file app_types.h
 * @brief Application common variable typedefs and variable limits
 *
 * =================================================================
 * @author Bc. Roland Molnar
 *
 * =================================================================
 * KEM, FEI, TUKE
 * @date 27.02.2024
 */
#include <stdint.h>
#include <limits.h>

#ifndef INC_APP_TYPES_H_
#define INC_APP_TYPES_H_

/* Unsigned integers. */
typedef uint16_t	U16;
#define U16_MAX		( UINT16_MAX )

typedef uint32_t	U32;
#define U32_MAX		( UINT32_MAX )

typedef uint64_t	U64;
#define U64_MAX		( UINT64_MAX )

/* Signed integers. */
typedef int16_t		S16;
#define S16_MIN		( INT16_MIN )
#define S16_MAX		( INT16_MAX )

typedef int32_t		S32;
#define S32_MIN		( INT32_MIN )
#define S32_MAX		( INT32_MAX )

typedef int64_t		S64;
#define S64_MIN		( INT64_MIN )
#define S64_MAX		( INT64_MAX )

/* Floating point */
typedef float		F32;
typedef double		F64;

#define True_b      ( (U16)1 )
#define False_b     ( (U16)0 )
typedef U16         boolean;


#endif /* INC_APP_TYPES_H_ */
