#ifndef FORWARD_LIST_PRIVATE_H
#define FORWARD_LIST_PRIVATE_H

#include "forward_list.h"

struct flist_node
{
	void *data;
	flist_node_t next;
};

struct flist
{
	flist_node_t head;
};

#endif // !FORWARD_LIST_PRIVATE_H
