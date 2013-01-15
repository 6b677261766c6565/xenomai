/*
 * Copyright (C) 2010 Philippe Gerum <rpm@xenomai.org>.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
 */

#ifndef _COPPERPLATE_TIMEROBJ_H
#define _COPPERPLATE_TIMEROBJ_H

#include <pthread.h>
#include <time.h>
#include <copperplate/lock.h>

#ifdef CONFIG_XENO_COBALT

#include <copperplate/list.h>

struct timerobj_corespec {
	struct itimerspec spec;
	struct pvholder link;
};

#else  /* CONFIG_XENO_MERCURY */

struct timerobj_corespec {
	/* Nothing specific. */
};

#endif /* CONFIG_XENO_MERCURY */

struct timerobj {
	struct timerobj_corespec core;
	void (*handler)(struct timerobj *tmobj);
	timer_t timer;
	pthread_mutex_t lock;
	int cancel_state;
};

static inline int timerobj_lock(struct timerobj *tmobj)
{
	return write_lock_safe(&tmobj->lock, tmobj->cancel_state);
}

static inline int timerobj_unlock(struct timerobj *tmobj)
{
	return write_unlock_safe(&tmobj->lock, tmobj->cancel_state);
}

static inline int timerobj_enabled(const struct timerobj *tmobj)
{
	return tmobj->handler != NULL;
}

#ifdef __cplusplus
extern "C" {
#endif

int timerobj_init(struct timerobj *tmobj);

void timerobj_destroy(struct timerobj *tmobj);

int timerobj_start(struct timerobj *tmobj,
		   void (*handler)(struct timerobj *tmobj),
		   struct itimerspec *it);

int timerobj_stop(struct timerobj *tmobj);

int timerobj_pkg_init(void);

#ifdef __cplusplus
}
#endif

#endif /* _COPPERPLATE_TIMEROBJ_H */