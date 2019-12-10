#include <uk/diagnostic.h>
#include <uk/list.h>
#include <uk/print.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uk/ctors.h>

static UK_LIST_HEAD(diagnostic_entries);

/**
 * Local functions
 *
 */
void add_entry(struct diagnostic_entry *entry)
{
	uk_pr_info("name: %s\n", entry->diag_name);
	uk_list_add_tail(&entry->next, &diagnostic_entries);
}

struct diagnostic_entry *find_entry(char *name)
{
	struct diagnostic_entry *iter;
	uk_list_for_each_entry(iter, &diagnostic_entries, next)
	{
		if (strcmp(iter->diag_name, name) == 0) {
			return iter;
		}
	}
	return NULL;
}

int run_diag_function(char *name, int *out)
{
	struct diagnostic_entry *entry = find_entry(name);
	if (entry == NULL) {
		return -1;
	}
	*out = entry->func();
	return 0;
}

static int test_function()
{
	printf("Running test_function\n");
	return 10;
}

DIAGNOSTIC_FUNCTION("test function", test_function);
