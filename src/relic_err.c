/*
 * RELIC is an Efficient LIbrary for Cryptography
 * Copyright (C) 2007-2017 RELIC Authors
 *
 * This file is part of RELIC. RELIC is legal property of its developers,
 * whose names are not listed here. Please refer to the COPYRIGHT file
 * for contact information.
 *
 * RELIC is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * RELIC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with RELIC. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file
 *
 * Implementation of error-handling routines.
 *
 * @ingroup relic
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "relic_core.h"
#include "relic_conf.h"
#include "relic_err.h"

#if defined(VERBS) && OPSYS == LINUX
#include <execinfo.h>
#endif

/*============================================================================*/
/* Public definitions                                                         */
/*============================================================================*/

#ifdef CHECK

void err_simple_msg(int error) {
	if (error != ERR_CAUGHT) {
		fprintf(stderr, "\nERROR: %s.\n", err_core_get()->reason[error]);
	}
}

#ifdef VERBS

void err_full_msg(const char *function, const char *file, int line,
		int error) {
	err_ctx_t *ctx = err_core_get();

	if (error == ERR_CAUGHT) {
		fprintf(stderr, "\tCAUGHT in %s() at %s,%d.\n", function, file, line);
	} else {
		fprintf(stderr, "\nERROR in %s() at %s,%d: %s.\n", function, file, line,
				ctx->reason[error]);

#if OPSYS == LINUX
		void *trace[100];
		char **symbols;
		int n;

		/* Print the stack trace. */
		fprintf(stderr, "\tCall stack:\n");
		n = backtrace(trace, 100);
		symbols = backtrace_symbols(trace, n);
		/*
		 * Skip the first entry (err_complete_error) and the two last (this
		 * binary and a libc entry.
		 */
		for (int i = 1; i < n - 2; i++) {
			fprintf(stderr, "\t\t#%d %s\n", i - 1, symbols[i]);
		}
		free(symbols);
#endif
	}
}

#endif /* VERBS */

void err_get_msg(err_t *e, char **msg) {
	err_ctx_t *ctx = err_core_get();
	*e = *(ctx->last->error);
	*msg = ctx->reason[*e];
	ctx->last = NULL;
}

/**
 * If multi-threading is enabled, assigns each thread a local copy of the data.
 */
#if ERRMO == ERRMO_SPTHREAD
#define thread 	__thread
#else
#define thread /* */
#endif

#if ERRMO != ERRMO_NONE
/**
 * Default library context.
 */
thread err_ctx_t first_err_ctx;

/**
 * Active library context.
 */
thread err_ctx_t *err_core_ctx = NULL;

void err_core_init_default(void) {
    if (err_core_ctx == NULL) {
        err_core_ctx = &(first_err_ctx);
    }

    err_core_init(err_core_ctx);
}
#endif

void err_core_init(err_ctx_t *err_core_ctx) {
    memset(err_core_ctx, 0, sizeof(err_ctx_t));

    err_core_ctx->reason[ERR_NO_MEMORY] = MSG_NO_MEMORY;
    err_core_ctx->reason[ERR_NO_PRECI] = MSG_NO_PRECI;
    err_core_ctx->reason[ERR_NO_FILE] = MSG_NO_FILE;
    err_core_ctx->reason[ERR_NO_READ] = MSG_NO_READ;
    err_core_ctx->reason[ERR_NO_VALID] = MSG_NO_VALID;
    err_core_ctx->reason[ERR_NO_BUFFER] = MSG_NO_BUFFER;
    err_core_ctx->reason[ERR_NO_FIELD] = MSG_NO_FIELD;
    err_core_ctx->reason[ERR_NO_CURVE] = MSG_NO_CURVE;
    err_core_ctx->reason[ERR_NO_CONFIG] = MSG_NO_CONFIG;
    err_core_ctx->last = NULL;
}

err_ctx_t *err_core_get() {
#if ERRMO == ERRMO_NONE
    return &core_get()->err_ctx;
#else
    return err_core_ctx;
#endif
}

#endif /* CHECK */

int err_get_code(void) {
	ctx_t *ctx = core_get();
	int r = ctx->code;
	ctx->code = STS_OK;
	return r;
}
