#include <job_manager.h>


static list_t jobs;


list_t* jobs_get_all() {
	return &jobs;
}


void jobs_insert(job_t* job) {
	list_push(&jobs, job, sizeof(job_t));
}