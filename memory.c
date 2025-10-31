#include "oslabs.h"

int check_null(struct MEMORY_BLOCK block)
{
	if(block.process_id == 0 
        && block.start_address == 0 
        && block.end_address == 0 
        && block.segment_size == 0)
	{
	    return 1;    
    }
	else
		return 0;
}

struct MEMORY_BLOCK set_null(struct MEMORY_BLOCK *block)
{
	(*block).process_id =0;
	(*block).start_address = 0;
	(*block).end_address = 0;
	(*block).segment_size = 0;
	return *block;
}

//Best Fit
struct MEMORY_BLOCK best_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt, int process_id)
{
    int index = -1;
    int size_diff  = 2147483647;

    for (int i = 0; i < *map_cnt; i++)
	{
        if (memory_map[i].process_id == 0)
		{
            int diff = memory_map[i].segment_size - request_size;
            if (diff >= 0)
			{
                if (diff < size_diff || (diff == size_diff && (index == -1 || memory_map[i].start_address < memory_map[index].start_address)))
				{
                    size_diff  = diff;
                    index = i;
                }
            }
        }
    }

    if (index == -1)
	{
        struct MEMORY_BLOCK NULLBLOCK;
        return set_null(&NULLBLOCK);
    }

    if (size_diff == 0)
	{
        memory_map[index].process_id = process_id;
        return memory_map[index];
    }

    struct MEMORY_BLOCK chosen = memory_map[index];
    memory_map[index].end_address = chosen.start_address + request_size - 1;
    memory_map[index].segment_size = request_size;
    memory_map[index].process_id = process_id;

    struct MEMORY_BLOCK remainder;
    remainder.process_id = 0;
    remainder.start_address = memory_map[index].end_address + 1;
    remainder.end_address = chosen.end_address;
    remainder.segment_size = remainder.end_address - remainder.start_address + 1;

    for (int j = *map_cnt; j > index + 1; j--)
	{
        memory_map[j] = memory_map[j - 1];
    }
    memory_map[index + 1] = remainder;
    (*map_cnt)++;

    return memory_map[index];
}
// First Fit
struct MEMORY_BLOCK first_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt, int process_id)
{
    int index = -1;

	for (int i = 0; i < *map_cnt; i++)
	{
        if (memory_map[i].process_id == 0 && memory_map[i].segment_size >= request_size)
		{
            index = i;
            break;
        }
    }

    if (index == -1)
	{
        struct MEMORY_BLOCK NULLBLOCK;
        return set_null(&NULLBLOCK);
    }

    if (memory_map[index].segment_size == request_size)
	{
        memory_map[index].process_id = process_id;
        return memory_map[index];
    }

    struct MEMORY_BLOCK chosen = memory_map[index];
    memory_map[index].end_address = chosen.start_address + request_size - 1;
    memory_map[index].segment_size = request_size;
    memory_map[index].process_id = process_id;

    struct MEMORY_BLOCK remainder;
    remainder.process_id = 0;
    remainder.start_address = memory_map[index].end_address + 1;
    remainder.end_address = chosen.end_address;
    remainder.segment_size = remainder.end_address - remainder.start_address + 1;

    for (int j = *map_cnt; j > index + 1; j--)
	{
        memory_map[j] = memory_map[j - 1];
    }
    memory_map[index + 1] = remainder;
    (*map_cnt)++;

    return memory_map[index];
} 

// Worst Fit
struct MEMORY_BLOCK worst_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt, int process_id)
{
    int index = -1;
    int size_diff = -1;

    for (int i = 0; i < *map_cnt; i++) {
        if (memory_map[i].process_id == 0 && memory_map[i].segment_size >= request_size)
		{
            if (memory_map[i].segment_size > size_diff)
			{
                size_diff = memory_map[i].segment_size;
                index = i;
            } else if (memory_map[i].segment_size == size_diff && index != -1 && memory_map[i].start_address < memory_map[index].start_address)
			{
                index = i;
            }
        }
    }

    if (index == -1) {
        struct MEMORY_BLOCK NULLBLOCK;
        return set_null(&NULLBLOCK);
    }

    if (memory_map[index].segment_size == request_size)
	{
        memory_map[index].process_id = process_id;
        return memory_map[index];
    }

    struct MEMORY_BLOCK chosen = memory_map[index];
    memory_map[index].end_address = chosen.start_address + request_size - 1;
    memory_map[index].segment_size = request_size;
    memory_map[index].process_id = process_id;

    struct MEMORY_BLOCK remainder;
    remainder.process_id = 0;
    remainder.start_address = memory_map[index].end_address + 1;
    remainder.end_address = chosen.end_address;
    remainder.segment_size = remainder.end_address - remainder.start_address + 1;

    for (int j = *map_cnt; j > index + 1; j--)
	{
        memory_map[j] = memory_map[j - 1];
    }
    memory_map[index + 1] = remainder;
    (*map_cnt)++;

    return memory_map[index];
}  

//Next Fit
struct MEMORY_BLOCK next_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id, int last_address)
{
    if (*map_cnt == 0)
	{
        struct MEMORY_BLOCK NULLBLOCK;
        return set_null(&NULLBLOCK);
    }

    int start_idx = 0;
    for (int i = 0; i < *map_cnt; i++)
	{
        if (memory_map[i].start_address >= last_address)
		{
			start_idx = i; 
			break;
		}
        if (i == *map_cnt - 1)
		start_idx = 0;
    }

    int index = -1;

    for (int pass = 0; pass < 2 && index == -1; pass++)
	{
        int i = (pass == 0 ? start_idx : 0);
        int end = (pass == 0 ? *map_cnt : start_idx);
        for (; i < end; i++)
		{
            if (memory_map[i].process_id == 0 && memory_map[i].segment_size >= request_size)
			{
                index = i;
                break;
            }
        }
    }

    if (index == -1)
	{
        struct MEMORY_BLOCK NULLBLOCK;
        return set_null(&NULLBLOCK);
    }

    if (memory_map[index].segment_size == request_size)
	{
        memory_map[index].process_id = process_id;
        return memory_map[index];
    }

    struct MEMORY_BLOCK chosen = memory_map[index];
    memory_map[index].end_address = chosen.start_address + request_size - 1;
    memory_map[index].segment_size = request_size;
    memory_map[index].process_id = process_id;

    struct MEMORY_BLOCK remainder;
    remainder.process_id = 0;
    remainder.start_address = memory_map[index].end_address + 1;
    remainder.end_address = chosen.end_address;
    remainder.segment_size = remainder.end_address - remainder.start_address + 1;

    for (int j = *map_cnt; j > index + 1; j--)
	{
        memory_map[j] = memory_map[j - 1];
    }
    memory_map[index + 1] = remainder;
    (*map_cnt)++;

    return memory_map[index];
} 
// Release memory
void release_memory(struct MEMORY_BLOCK freed_block, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt)
{
    int index = -1;
    for (int i = 0; i < *map_cnt; i++)
	{
        if (memory_map[i].start_address == freed_block.start_address && memory_map[i].end_address == freed_block.end_address && memory_map[i].process_id == freed_block.process_id)
		{
            index = i;
            break;
        }
    }
    if (index == -1)
	return;

    memory_map[index].process_id = 0;
    memory_map[index].segment_size = memory_map[index].end_address - memory_map[index].start_address + 1;

    int left_free  = (index > 0) && (memory_map[index - 1].process_id == 0);
    int right_free = (index + 1 < *map_cnt) && (memory_map[index + 1].process_id == 0);

    if (left_free && right_free)
	{
        memory_map[index - 1].end_address = memory_map[index + 1].end_address;
        memory_map[index - 1].segment_size = memory_map[index - 1].end_address - memory_map[index - 1].start_address + 1;

        for (int j = index; j + 2 < *map_cnt; j++)
		{
            memory_map[j] = memory_map[j + 2];
        }
        *map_cnt -= 2;
		
    } else if (left_free)
	{
        memory_map[index - 1].end_address = memory_map[index].end_address;
        memory_map[index - 1].segment_size = memory_map[index - 1].end_address - memory_map[index - 1].start_address + 1;

        for (int j = index; j + 1 < *map_cnt; j++)
		{
            memory_map[j] = memory_map[j + 1];
        }
        (*map_cnt)--;
    } else if (right_free)
	{
        memory_map[index].end_address = memory_map[index + 1].end_address;
        memory_map[index].segment_size = memory_map[index].end_address - memory_map[index].start_address + 1;

        for (int j = index + 1; j + 1 < *map_cnt; j++)
		{
            memory_map[j] = memory_map[j + 1];
        }
        (*map_cnt)--;
    } else 
	{

    }
}
