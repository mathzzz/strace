/*
 * Copyright (c) 2015 Dmitry V. Levin <ldv@strace.io>
 * Copyright (c) 2015-2021 The strace developers.
 * All rights reserved.
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include "defs.h"
#include <linux/memfd.h>
#include "xlat/memfd_create_flags.h"

SYS_FUNC(memfd_create)
{
	printpathn(tcp, tcp->u_arg[0], 255 - (sizeof("memfd:") - 1));
	tprints(", ");

	unsigned int flags = tcp->u_arg[1];

	if (!flags || xlat_verbose(xlat_verbosity) != XLAT_STYLE_ABBREV)
		tprintf("%#x", flags);

	if (!flags || xlat_verbose(xlat_verbosity) == XLAT_STYLE_RAW)
		return RVAL_DECODED | RVAL_FD;

	if (xlat_verbose(xlat_verbosity) == XLAT_STYLE_VERBOSE)
		tprints(" /* ");

	const unsigned int mask = MFD_HUGE_MASK << MFD_HUGE_SHIFT;
	const unsigned int hugetlb_value = flags & mask;
	flags &= ~mask;

	if (flags || !hugetlb_value)
		printflags_ex(flags, "MFD_???", XLAT_STYLE_ABBREV,
			      memfd_create_flags, NULL);

	if (hugetlb_value)
		tprintf("%s%u<<MFD_HUGE_SHIFT",
			flags ? "|" : "",
			hugetlb_value >> MFD_HUGE_SHIFT);

	if (xlat_verbose(xlat_verbosity) == XLAT_STYLE_VERBOSE)
		tprints(" */");

	return RVAL_DECODED | RVAL_FD;
}
