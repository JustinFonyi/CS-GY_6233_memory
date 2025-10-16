#include "oslabs.h"

//Set Null
struct MEMORY_BLOCK set_null(struct MEMORY_BLOCK *block)
{
	(*block).process_id =0;
	(*block).start_address = 0;
	(*block).end_address = 0;
	(*block).segment_size = 0;
	return *block;
}

//Null Check
int check_null(struct MEMORY_BLOCK block){
	if(block.process_id == 0 
        && block.start_address == 0 
        && block.end_address == 0 
        && block.segment_size == 0){
	    return 1;
        }
	else
		return 0;
}

//Best Fit
struct MEMORY_BLOCK best_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt, int process_id){
    int index = 0;
    int size_diff = 10000;
    for (int i = 0; i < *map_cnt; i++) {
        if (memory_map[i].process_id == 0 && memory_map[i].segment_size - request_size <= size_diff) {
                size_diff = memory_map[i].segment_size -request_size;
                index = i;
        }
    }

    if(size_diff == 100000){
        struct MEMORY_BLOCK NULLBLOCK;
        return set_null (&NULLBLOCK);
    }
	else if(size_diff == 0){
		memory_map[index].process_id = process_id;
		return memory_map[index];
    }

	else{
		struct MEMORY_BLOCK chunk;
		chunk.process_id = 0;
		chunk.start_address = memory_map[index].start_address + request_size;
		chunk.end_address = memory_map[index].end_address;
		chunk.segment_size = chunk.end_address - chunk.start_address + 1;
		memory_map[*map_cnt] = chunk;
		(*map_cnt)++;
		
		memory_map[index].end_address = memory_map[index].start_address + request_size - 1;
		memory_map[index].process_id = process_id;
		memory_map[index].segment_size = request_size;
		
		return memory_map[index];
		
	}
}

// First Fit
struct MEMORY_BLOCK first_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt, int process_id){
    int size_diff = 10000;
    int index = 0;
    for (int i = 0; i < *map_cnt; i++) {
        if (memory_map[i].process_id == 0 && memory_map[i].segment_size - request_size <= size_diff){
			size_diff = memory_map[i].segment_size - request_size;
			index = i;
		}
	}
	if(size_diff == 10000){
		struct MEMORY_BLOCK NULLBLOCK;
		return set_null(&NULLBLOCK);
	}
	else if(size_diff == 0){
		memory_map[index].process_id = process_id;
		return memory_map[index];
	}
	else{
		struct MEMORY_BLOCK chunk;
		chunk.process_id = 0;
		chunk.start_address = memory_map[index].start_address + request_size;
		chunk.end_address = memory_map[index].end_address;
		chunk.segment_size = chunk.end_address - chunk.start_address + 1;
		memory_map[*map_cnt] = chunk;
		(*map_cnt)++;
		
		memory_map[index].end_address = memory_map[index].start_address + request_size - 1;
		memory_map[index].process_id = process_id;
		memory_map[index].segment_size = request_size;
		
		return memory_map[index];
	}
}  

// Worst Fit
struct MEMORY_BLOCK worst_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt, int process_id){
    int index = 0;
    int size_diff = 10000;
    for (int i = 0; i < *map_cnt; i++) {
        if (memory_map[i].process_id == 0 && memory_map[i].segment_size - request_size <= size_diff)
        {
            size_diff = memory_map[i].segment_size - request_size;
            index = i;
        }
    }
	if(size_diff == 10000){
		struct MEMORY_BLOCK NULLBLOCK;
		return set_null(&NULLBLOCK);
	}
	else if(size_diff == 0){
		memory_map[index].process_id = process_id;
		return memory_map[index];
	}
	else{
		struct MEMORY_BLOCK chunk;
		chunk.process_id = 0;
		chunk.start_address = memory_map[index].start_address + request_size;
		chunk.end_address = memory_map[index].end_address;
		chunk.segment_size = chunk.end_address - chunk.start_address + 1;
		memory_map[*map_cnt] = chunk;
		(*map_cnt)++;
		
		memory_map[index].end_address = memory_map[index].start_address + request_size - 1;
		memory_map[index].process_id = process_id;
		memory_map[index].segment_size = request_size;
		
		return memory_map[index];
	}
}  

//Next Fit
struct MEMORY_BLOCK next_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id, int last_address){
	int size_diff = 10000;
	int index = 0;
	for (int i = 0; i < *map_cnt; i++){
		if(memory_map[i].segment_size - request_size <= sizeDiff && memory_map[i].process_id == 0){
			sizeDiff = memory_map[i].segment_size - request_size;
			index = i;
		}
	}
	if(size_diff == 10000){
		struct MEMORY_BLOCK nullblock;
		return set_null(&nullblock);
	}
	else if(size_diff == 0){
		memory_map[index].process_id = process_id;
		return memory_map[index];
	}
	else{
		struct MEMORY_BLOCK chunk;
		chunk.process_id = 0;
		chunk.start_address = memory_map[index].start_address + request_size;
		chunk.end_address = memory_map[index].end_address;
		chunk.segment_size = chunk.end_address - chunk.start_address + 1;
		memory_map[*map_cnt] = chunk;
		(*map_cnt)++;
		
		memory_map[index].end_address = memory_map[index].start_address + request_size - 1;
		memory_map[index].process_id = process_id;
		memory_map[index].segment_size = request_size;
		
		return memory_map[index];
	}

} 
// Release memory
void release_memory(struct MEMORY_BLOCK freed_block, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt) {
    int free_block;
    for (int i = 0; i < *map_cnt; i++){
        if(freed_block.process_id == memory_map[i].process_id){
            free_block = i;
            break;
        }
    }
    if (memory_map[free_block-1].process_id == 0 && memory_map[free_block + 1]. process_id == 0){
		memory_map[free_block-1].end_address = memory_map[free_block + 1].end_address;
		memory_map[free_block-1].segment_size = (memory_map[free_block-1].end_address - memory_map[free_block-1].start_address) + 1;
		*map_cnt = *map_cnt - 2;
		for(int i = free_block; i < *map_cnt; i++){
			memory_map[i] = memory_map[i+2];
        }
	}
	else if (memory_map[free_block-1].process_id ==0){
		memory_map[free_block-1].end_address = memory_map[free_block].end_address;
		memory_map[free_block-1].segment_size = (memory_map[free_block-1].end_address - memory_map[free_block-1].start_address) + 1;
		*map_cnt = *map_cnt - 1;
		for(int i = free_block; i < *map_cnt; i++){
			memory_map[i] = memory_map[i+1];
        }
	}
	else if (memory_map[free_block+1].process_id ==0){
		memory_map[free_block].end_address = memory_map[free_block+1].end_address;
		memory_map[free_block].segment_size = (memory_map[free_block].end_address - memory_map[free_block].start_address) + 1;
		memory_map[free_block].process_id = 0;
		*map_cnt = *map_cnt - 1;
		for(int i = free_block; i < *map_cnt; i++){
			memory_map[i] = memory_map[i+1];
        }
	}
	else{
		memory_map[free_block].process_id = 0;
	}
}
