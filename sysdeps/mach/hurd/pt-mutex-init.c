/* pthread_mutex_init.  Hurd version.
   Copyright (C) 2016-2018 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License
   as published by the Free Software Foundation; either
   version 2 of the license, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with this program; if not, see
   <http://www.gnu.org/licenses/>.
*/

#include <pthread.h>
#include <stdlib.h>
#include <assert.h>
#include <pt-internal.h>
#include "pt-mutex.h"
#include <hurdlock.h>

static const pthread_mutexattr_t dfl_attr = {
  .__prioceiling = 0,
  .__protocol = PTHREAD_PRIO_NONE,
  .__pshared = PTHREAD_PROCESS_PRIVATE,
  .__mutex_type = __PTHREAD_MUTEX_TIMED
};

int
_pthread_mutex_init (pthread_mutex_t *mtxp, const pthread_mutexattr_t *attrp)
{
  if (attrp == NULL)
    attrp = &dfl_attr;

  mtxp->__flags = (attrp->__pshared == PTHREAD_PROCESS_SHARED ?
		   GSYNC_SHARED : 0) | ((attrp->
					 __prioceiling & PTHREAD_MUTEX_ROBUST) ?
					PTHREAD_MUTEX_ROBUST : 0);

  mtxp->__type = attrp->__mutex_type +
      (attrp->__mutex_type != __PTHREAD_MUTEX_TIMED);

  mtxp->__owner_id = 0;
  mtxp->__shpid = 0;
  mtxp->__cnt = 0;
  mtxp->__lock = 0;

  return 0;
}

strong_alias (_pthread_mutex_init, pthread_mutex_init)
