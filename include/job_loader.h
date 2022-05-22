

#ifndef EDA_JOB_LOADER
#define EDA_JOB_LOADER


#include "utils.h"
#include "job.h"
#include "job_manager.h"


/**
 * @brief Import a json file to the jobs list. 
 * This will erase all the jobs first.
 * @param path Path to the jobs json file.
*/
void jobs_import_json(char* path);


/**
 * @brief Exports all the jobs to a json file.
 * @param path Path to the json file being exported.
*/
void jobs_export_json(char* path);


#endif