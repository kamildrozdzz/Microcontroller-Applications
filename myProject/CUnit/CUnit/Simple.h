
/** @file
 * Funciton pointer prototypes.
 */
/** @addtogroup Framework
 * @{
 */


#ifndef CU_SIMPLE_H
#define CU_SIMPLE_H

typedef int  (*CU_InitializeFunc)(void);  /**< Signature for suite initialization function. */
typedef int  (*CU_CleanupFunc)(void);     /**< Signature for suite cleanup function. */
typedef void (*CU_TestFunc)(void);        /**< Signature for a testing function in a test case. */
typedef void (*CU_SetUpFunc)(void);       /**< Signature for a test SetUp function. */
typedef void (*CU_TearDownFunc)(void);    /**< Signature for a test TearDown function. */


#endif //CU_SIMPLE_H
