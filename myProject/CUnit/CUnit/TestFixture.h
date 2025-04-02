/*
 *  CUnit - A Unit testing framework library for C.
 *  Copyright (C) 2001       Anil Kumar
 *  Copyright (C) 2004-2006  Anil Kumar, Jerry St.Clair
 *  Copyright (C) 2025       Ian Norton
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


/** @file
 *  Test Fixture support interface.
 *  
 *  Typedefs and fixture use functions.
 */
/** @addtogroup Framework
 * @{
 */

#ifndef CUNIT_TESTFIXTURE_H
#define CUNIT_TESTFIXTURE_H

#include "CUnit/CUnit.h"
#include "CUnit/Simple.h"

#ifdef __cplusplus
extern "C" {
#endif


/** When a test fixture is executed and reset */
typedef enum CU_Fixture_Scope {
 CU_ScopeFunction = 0,
 CU_ScopeSuite,
 CU_ScopeSession,
} CU_Fixture_Scope;

/** Data type for holding a fixture */
typedef struct CU_Fixture CU_Fixture;
struct CU_Fixture {
 const char* pName;
 CU_Fixture_Scope  eScope;
 const CU_SetUpFunc pSetup;
 const CU_TearDownFunc pTeardown;

 unsigned int uiCallCount;
 CU_BOOL fActive;
 CU_Fixture *pNextFixture;
 CU_Fixture *pPrevFixture;
};


CU_EXPORT void CU_UseFixture(CU_Fixture* pFixture);
/**< Request a test/setup/suite uses a fixture. */

CU_EXPORT void CU_ResetFixtures(CU_Fixture_Scope scope);
/**< Reset all active fixtures with the current scope */

#ifdef __cplusplus
}
#endif
#endif //CUNIT_TESTFIXTURE_H
