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
 *  Contains Interface to define and use test fixtures.
 */
/** @addtogroup Framework
 @{
*/

#include "CUnit/TestFixture.h"
#include "CUnit/CUnit.h"

static CU_Fixture *known_fixtures;

static void save_fixture(CU_Fixture * const fixture) {
  // look through the fixtures, if not already added, append it
  CU_Fixture *next_fixture = known_fixtures;
  CU_Fixture *prev_fixture = NULL;
  while (next_fixture != NULL) {
    if (next_fixture == fixture) {
      // already added
      return;
    }
    prev_fixture = next_fixture;
    next_fixture = next_fixture->pNextFixture;
  }

  if (prev_fixture != NULL) {
    // append
    prev_fixture->pNextFixture = fixture;
    fixture->pPrevFixture = prev_fixture;
    fixture->pNextFixture = NULL;
  } else {
    known_fixtures = fixture;
  }
}

static void enter_fixture(CU_Fixture *fixture) {
  const CU_Suite *cur_suite = CU_get_current_suite();
  CU_BOOL call_setup = CU_FALSE;
  CU_ASSERT_PTR_NOT_NULL_FATAL(cur_suite);

  switch (fixture->eScope) {
    case CU_ScopeSession:
      if (fixture->uiCallCount == 0) {
        call_setup = CU_TRUE;
      }
    break;
    case CU_ScopeSuite:
      if (cur_suite) {
        if (cur_suite->fInTestSetup || cur_suite->fInSetUp) {
          // calling from a test or suite setup func
          if ( fixture->fActive == CU_FALSE) {
            call_setup = CU_TRUE;
          }
        }
      }
      break;

    case CU_ScopeFunction:
    default:
      // test scoped fixtures can be called in test setup, or tests
      if (cur_suite && cur_suite->fInSetUp) {
        // in suite setup, not allowed!
        fprintf(stderr, "Fixture '%s' is test scoped and can only be called in "
                        "tests or test setup functions\n", fixture->pName);
        CU_FAIL_FATAL("test scoped fixture called from suite setup");
      }
      call_setup = CU_TRUE;
      break;
  }

  if (call_setup) {
    fixture->uiCallCount++;
    fixture->pSetup();
    fixture->fActive = CU_TRUE;
    save_fixture(fixture);
  }

}

void CU_UseFixture(CU_Fixture* pfixture)
{
  enter_fixture(pfixture);
}

void CU_ResetFixtures(const CU_Fixture_Scope scope) {
  CU_Fixture *cur_fixture = known_fixtures;
  CU_Fixture *last_fixture = NULL;
  // go to the last fixture
  while (cur_fixture != NULL) {
    last_fixture = cur_fixture;
    cur_fixture = cur_fixture->pNextFixture;
  }

  cur_fixture = last_fixture;

  // teardown in reverse order
  while (cur_fixture != NULL) {
    if (cur_fixture->fActive && cur_fixture->eScope == scope) {
      // shutdown
      cur_fixture->fActive = CU_FALSE;
      if (cur_fixture->pTeardown != NULL) {
        cur_fixture->pTeardown();
      }
    }
    cur_fixture = cur_fixture->pPrevFixture;
  }
}
