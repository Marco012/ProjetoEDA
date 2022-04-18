#ifndef EDA_VIEWS
#define EDA_VIEWS


#include "utils.h"
#include <gui/gui.h>

#include "job.h"
#include "job_utils.h"
#include "job_manager.h"
#include "list.h"


#define MARGIN_X(v) gui_margin_x(v)
#define MARGIN_X_1() MARGIN_X(20)
#define MARGIN_X_2() MARGIN_X(40)
#define MARGIN_X_3() MARGIN_X(60)
#define MARGIN_X_4() MARGIN_X(80)
#define MARGIN_X_5() MARGIN_X(100)


#define VIEW_TITLE_JOBS "Jobs"
#define VIEW_TITLE_JOB "Job"
#define VIEW_TITLE_MIN_TIME "Min Time"
#define VIEW_TITLE_MAX_TIME "Max Time"
#define VIEW_TITLE_AVERAGE_TIME "Average Time"


void view_open_jobs();
void view_register_jobs();

void view_open_job(job_t* job);
void view_register_job();

void view_open_max_time(job_t* job);
void view_register_max_time();

void view_open_min_time(job_t* job);
void view_register_min_time();

#endif