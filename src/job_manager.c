#include <job_manager.h>


static list_t jobs;


static void job_free(void* value) {
	job_t* job = (job_t*)value;
	list_clear(&job->operations);
	free(job);
}


void jobs_init() {
	jobs = list_init(job_free);
}


list_t* jobs_get_all() {
	return &jobs;
}


void jobs_insert(job_t* job) {
	list_push(&jobs, job, sizeof(job_t));
}


void jobs_remove(int id) {
	list_remove(&jobs, id);
}


void jobs_clear() {
	list_clear(&jobs);
}