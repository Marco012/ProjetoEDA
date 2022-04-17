#ifndef EDA_VIEWS
#define EDA_VIEWS


#include "utils.h"
#include <gui/gui.h>

#include "job.h"
#include "job_manager.h"
#include "list.h"


#define VIEW_TITLE_JOBS "Jobs"
#define VIEW_TITLE_MIN_TIME "Min Time"
#define VIEW_TITLE_MAX_TIME "Max Time"
#define VIEW_TITLE_AVERAGE_TIME "Average Time"


void view_render_jobs(view_t* view, void* data);


#endif