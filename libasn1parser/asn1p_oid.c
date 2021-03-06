#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "asn1parser.h"

asn1p_oid_t *
asn1p_oid_construct(asn1p_oid_arc_t *arc, int narcs) {
	asn1p_oid_t *oid;

	if(narcs <= 0)
		return NULL;

	oid = asn1p_oid_new();
	if(oid) {
		for(; narcs--; arc++) {
			if(asn1p_oid_add_arc(oid, arc)) {
				asn1p_oid_free(oid);
				return NULL;
			}
		}
	}

	return oid;
}

asn1p_oid_t *
asn1p_oid_new() {
	return calloc(1, sizeof(asn1p_oid_t));
}

int
asn1p_oid_add_arc(asn1p_oid_t *oid, asn1p_oid_arc_t *template) {
	void *p;
	p = realloc(oid->arcs, (oid->arcs_count + 1) * sizeof(oid->arcs[0]));
	if(p) {
		oid->arcs = p;
		oid->arcs[oid->arcs_count].name
				= template->name?strdup(template->name):0;
		oid->arcs[oid->arcs_count].number
				= template->number?strdup(template->number):0;
		oid->arcs_count++;
		return 0;
	} else {
		return -1;
	}
}

void
asn1p_oid_free(asn1p_oid_t *oid) {
	if(oid) {
		if(oid->arcs) {
			while(oid->arcs_count--) {
				if(oid->arcs[oid->arcs_count].name)
					free(oid->arcs[oid->arcs_count].name);
				if(oid->arcs[oid->arcs_count].number)
					free(oid->arcs[oid->arcs_count].number);
			}
		}
		free(oid);
	}
}

asn1p_oid_arc_t *
asn1p_oid_arc_new(const char *optName, const char *optNumber /* = NULL */) {
	asn1p_oid_arc_t *arc;

	arc = calloc(1, sizeof *arc);
	if(arc) {
		if(optName)
			arc->name = strdup(optName);
		if(optNumber)
			arc->number = strdup(optNumber);
	}

	return arc;
}

void
asn1p_oid_arc_free(asn1p_oid_arc_t *arc) {
	if(arc) {
		if(arc->name)
			free(arc->name);
		if(arc->number)
			free(arc->number);
		free(arc);
	}
}

int
asn1p_oid_compare(asn1p_oid_t *a, asn1p_oid_t *b) {
	int i;

	for(i = 0; ; i++) {
		asn1c_integer_t cmp;

		if(b->arcs_count > i) {
			if(a->arcs_count <= i)
				return -1;
		} else if(a->arcs_count > i) {
			if(b->arcs_count <= i)
				return 1;
		} else if(b->arcs_count <= i && a->arcs_count <= i) {
			cmp = b->arcs_count - a->arcs_count;
			if(cmp < 0)
				return -1;
			else if(cmp > 0)
				return 1;
			return 0;
		}

		if(!b->arcs[i].number) {
			if(a->arcs[i].number)
				return 1;
		}
		else if(!a->arcs[i].number)
			return -1;
		else {
			int scmp = strcmp(a->arcs[i].number, b->arcs[i].number);
			if(scmp != 0)
				return scmp;
		}
	}
}


